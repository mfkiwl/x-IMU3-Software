#include "../ApplicationSettings.h"
#include "SendingCommandDialog.h"

SendingCommandDialog::SendingCommandDialog(const CommandMessage& command, const std::vector<DevicePanel*>& devicePanels) : Dialog(BinaryData::progress_svg, "Sending Command " + command.json, "Close", "", &closeWhenCompleteButton, std::numeric_limits<int>::max(), true)
{
    for (auto* const devicePanel : devicePanels)
    {
        rows.push_back({ devicePanel->getColourTag(), devicePanel->getDeviceNameAndSerialNumber(), devicePanel->getConnection().getInfo()->toString() });

        devicePanel->getConnection().sendCommandsAsync({ command }, ApplicationSettings::getSingleton().retries, ApplicationSettings::getSingleton().timeout, [&, rowIndex = rows.size() - 1, self = SafePointer<juce::Component>(this)](const std::vector<std::string>& responses)
        {
            juce::MessageManager::callAsync([&, rowIndex, self, responses = responses]
                                            {
                                                if (self == nullptr)
                                                {
                                                    return;
                                                }

                                                rows[rowIndex].state = responses.empty() ? Row::State::failed : Row::State::complete;
                                                table.updateContent();

                                                if (ApplicationSettings::getSingleton().closeSendingCommandDialogWhenComplete)
                                                {
                                                    for (const auto& row : rows)
                                                    {
                                                        if (row.state != Row::State::complete)
                                                        {
                                                            return;
                                                        }
                                                    }

                                                    DialogLauncher::launchDialog(nullptr);
                                                }
                                            });
        });
    }

    addAndMakeVisible(closeWhenCompleteButton);
    closeWhenCompleteButton.setClickingTogglesState(true);
    closeWhenCompleteButton.setToggleState(ApplicationSettings::getSingleton().closeSendingCommandDialogWhenComplete, juce::dontSendNotification);
    closeWhenCompleteButton.onClick = [&]
    {
        ApplicationSettings::getSingleton().closeSendingCommandDialogWhenComplete = closeWhenCompleteButton.getToggleState();
    };

    addAndMakeVisible(deviceLabel);
    addAndMakeVisible(connectionLabel);
    addAndMakeVisible(completeLabel);

    addAndMakeVisible(table);
    const int colourTagColumnWidth = DevicePanelHeader::colourTagWidth + 5;
    table.getHeader().addColumn("", (int) ColumnIDs::colourTag, colourTagColumnWidth, colourTagColumnWidth, colourTagColumnWidth);
    table.getHeader().addColumn("", (int) ColumnIDs::device, 1);
    table.getHeader().addColumn("", (int) ColumnIDs::connection, 1);
    table.getHeader().addColumn("", (int) ColumnIDs::complete, 70, 70, 70);
    table.getHeader().setStretchToFitActive(true);
    table.setHeaderHeight(0);
    table.getViewport()->setScrollBarsShown(true, false);
    table.setColour(juce::TableListBox::backgroundColourId, UIColours::background);
    table.updateContent();

    setSize(600, calculateHeight(6));
}

void SendingCommandDialog::resized()
{
    Dialog::resized();

    auto bounds = getContentBounds(true);

    static constexpr int headerHeight = 30;
    bounds.removeFromTop(headerHeight);
    deviceLabel.setBounds(table.getHeader().getColumnPosition((int) ColumnIDs::device - 1).withHeight(headerHeight));
    connectionLabel.setBounds(table.getHeader().getColumnPosition((int) ColumnIDs::connection - 1).withHeight(headerHeight));
    completeLabel.setBounds(table.getHeader().getColumnPosition((int) ColumnIDs::complete - 1).withHeight(headerHeight));

    table.setBounds(bounds);
}

int SendingCommandDialog::getNumRows()
{
    return (int) rows.size();
}

void SendingCommandDialog::paintRowBackground(juce::Graphics& g, int rowNumber, int height, int, bool)
{
    g.setColour(rows[(size_t) rowNumber].colourTag);
    g.fillRect(0, 0, DevicePanelHeader::colourTagWidth, height);
}

juce::Component* SendingCommandDialog::refreshComponentForCell(int rowNumber, int columnID, bool, juce::Component* existingComponentToUpdate)
{
    delete existingComponentToUpdate;

    switch ((ColumnIDs) columnID)
    {
        case ColumnIDs::colourTag:
            return nullptr;

        case ColumnIDs::device:
            return new SimpleLabel(rows[(size_t) rowNumber].device);

        case ColumnIDs::connection:
            return new SimpleLabel(rows[(size_t) rowNumber].connection);

        case ColumnIDs::complete:
            struct CustomIcon : juce::Component
            {
                CustomIcon(const juce::String& svg, const juce::String& tooltip) : icon(svg, tooltip)
                {
                    addAndMakeVisible(icon);
                }

                void resized() override
                {
                    icon.setBounds(getLocalBounds().reduced(0, 3));
                }

                Icon icon;
            };

            switch (rows[(size_t) rowNumber].state)
            {
                case Row::State::inProgress:
                    return new CustomIcon(BinaryData::progress_svg, "In Progress");

                case Row::State::failed:
                    return new CustomIcon(BinaryData::warning_orange_svg, "Failed");

                case Row::State::complete:
                    return new CustomIcon(BinaryData::tick_green_svg, "Complete");

                default:
                    return nullptr;
            }

        default:
            return nullptr;
    }
}
