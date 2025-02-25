#pragma once

#include "../Widgets/SimpleLabel.h"
#include "Dialog.h"

class AreYouSureDialog : public Dialog
{
public:
    explicit AreYouSureDialog(const juce::String& text = "Are you sure?");

    void resized() override;

private:
    SimpleLabel label;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AreYouSureDialog)
};

