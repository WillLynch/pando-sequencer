/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <list>
#include "MarkovChain.h"
#include "Note.h"

//==============================================================================
/**
*/
class PandoAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    PandoAudioProcessor();
    ~PandoAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

	void regenerateSequence();

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	MarkovChain markovChain;

	bool play;
	bool listen;
	int steps;
	float tempo;

private:
    //==============================================================================
	AudioParameterFloat* speed;
	int currentNote, lastNoteValue, lastNotePlayedValue;
	int time;
	float rate;
	SortedSet<int> notes;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PandoAudioProcessor)
};
