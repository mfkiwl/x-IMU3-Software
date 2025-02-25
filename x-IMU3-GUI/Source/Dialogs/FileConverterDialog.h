#pragma once

#include "Dialog.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include "Widgets/CustomTextEditor.h"
#include "Widgets/IconButton.h"
#include "Widgets/SimpleLabel.h"

class FileConverterDialog : public Dialog
{
public:
    FileConverterDialog();

    void resized() override;

    juce::String getSource() const;

    juce::String getDestination() const;

private:
    SimpleLabel sourceLabel { "Source:" };
    CustomTextEditor sourceValue;
    IconButton sourceButton { IconButton::Style::menuStrip, BinaryData::open_svg, 0.8f, "Select Source File" };
    SimpleLabel destinationLabel { "Destination:" };
    CustomTextEditor destinationValue;
    IconButton destinationButton { IconButton::Style::menuStrip, BinaryData::open_svg, 0.8f, "Select Destination Directory" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileConverterDialog)
};
