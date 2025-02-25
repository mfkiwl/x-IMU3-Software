#pragma once

#include "../../CustomLookAndFeel.h"
#include "../../Widgets/Icon.h"
#include "../../Widgets/SimpleLabel.h"
#include "../DeviceSettingsIDs.h"
#include <juce_gui_basics/juce_gui_basics.h>

class DeviceSettingsItem;

class Setting : public juce::Component,
                private juce::ValueTree::Listener
{
public:
    enum class Status
    {
        normal,
        modified,
        readFailed,
        writeFailed,
    };

    static constexpr int rowMargin = 4;

    explicit Setting(const juce::ValueTree& tree_, DeviceSettingsItem* const parentIfExpandable_ = nullptr);

    ~Setting() override;

    void resized() override;

protected:
    bool isReadOnly() const;

    const juce::Rectangle<int>& getValueBounds() const;

    juce::var getValue() const;

    void setValue(const juce::var& value);

    virtual void valueChanged()
    {
    }

private:
    juce::ValueTree tree;
    SimpleLabel name;
    Icon modifiedIcon { BinaryData::upload_svg, "Value Modified but Not Written to Device" };
    Icon readFailedIcon { BinaryData::warning_orange_svg, "Read from Device Failed" };
    Icon writeFailedIcon { BinaryData::warning_orange_svg, "Write to Device Failed" };
    juce::Rectangle<int> valueBounds;
    juce::WeakReference<DeviceSettingsItem> parentIfExpandable;

    void updateIcon();

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Setting)
};
