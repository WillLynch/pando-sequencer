/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PandoAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    PandoAudioProcessorEditor (PandoAudioProcessor&);
    ~PandoAudioProcessorEditor();

	void updatePlayToggleState(Button* button);

	void updateListenToggleState(Button* button);

	void updateReseedButton(Button* button);

	void updateForgetButton(Button* button);

	void updateSteps(Slider* steps);

	void updateRegenerations(Slider* regenerationsSlider);

	void updateTempo(Slider* tempoSlider);

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PandoAudioProcessor& processor;

	Slider stepsSlider;
	Slider regenerationsSlider;
	Slider tempoSlider;
	Label regenerationsLabel;
	Label stepsLabel;
	Label tempoLabel;
	ToggleButton play { "Play" };
	ToggleButton listen { "Listen" };
	TextButton reseed { "Reseed" };
	TextButton forget { "Forget" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PandoAudioProcessorEditor)
};
