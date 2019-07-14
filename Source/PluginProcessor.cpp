/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Note.h"

//==============================================================================
PandoAudioProcessor::PandoAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

PandoAudioProcessor::~PandoAudioProcessor()
{
	steps = 8;
	rate = 0.5;
}

//==============================================================================
const String PandoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PandoAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PandoAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PandoAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PandoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PandoAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PandoAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PandoAudioProcessor::setCurrentProgram (int index)
{
}

const String PandoAudioProcessor::getProgramName (int index)
{
    return {};
}

void PandoAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void PandoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	ignoreUnused(samplesPerBlock);
	notes.clear();                          // [1]
	currentNote = 0;                        // [2]
	lastNotePlayedValue = -1;
	lastNoteValue = -1;                     // [3]
	time = 0.0;                             // [4]
	rate = static_cast<float> (sampleRate); // [5]
	play = false;
	listen = false;
}

void PandoAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PandoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PandoAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

   // however we use the buffer to get timing information
	auto numSamples = buffer.getNumSamples();                                                     // [7]
	// get note duration
	auto noteDuration = static_cast<int> (std::ceil(rate * 0.25f * (0.1f + (1.0f - (tempo))))); // [8]
	MidiMessage msg;
	int ignore;

	if (listen) {
		for (MidiBuffer::Iterator it(midiMessages); it.getNextEvent(msg, ignore);)                          // [9]
		{
			if (msg.isNoteOn()) {
				note currentNote;
				currentNote.pitch = msg.getNoteNumber();
				currentNote.velocity = msg.getVelocity();
				markovChain.add_entry(currentNote, lastNotePlayedValue); 
			}
			lastNotePlayedValue = msg.getNoteNumber();
		}
	}

	midiMessages.clear();

	if ((time + numSamples) >= noteDuration && play)                                                      // [11]
	{
		auto offset = jmax(0, jmin((int)(noteDuration - time), numSamples - 1));
		markovChain.play_note(midiMessages, offset);
	}
	time = (time + numSamples) % noteDuration;
}

//==============================================================================
bool PandoAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PandoAudioProcessor::createEditor()
{
    return new PandoAudioProcessorEditor (*this);
}

//==============================================================================
void PandoAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PandoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PandoAudioProcessor();
}
