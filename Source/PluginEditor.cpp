#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PandoAudioProcessorEditor::PandoAudioProcessorEditor (PandoAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	setSize(400, 245);

	addAndMakeVisible(play);
	addAndMakeVisible(listen);
	addAndMakeVisible(reseed);
	addAndMakeVisible(forget);

	play.onClick = [this] { updatePlayToggleState(&play);   };
	listen.onClick = [this] { updateListenToggleState(&listen); };
	reseed.onClick = [this] { updateReseedButton(&reseed); };
	forget.onClick = [this] { updateForgetButton(&forget); };

	stepsSlider.setRange(1.0, 16.0, 1.0);
	stepsSlider.setSliderStyle(Slider::Rotary);
	stepsSlider.setPopupDisplayEnabled(true, false, this);
	stepsSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
	//steps.setTextValueSuffix(" Tempo");
	stepsSlider.setValue(1.0);
	// this function adds the slider to the editor
	addAndMakeVisible(stepsSlider);

	addAndMakeVisible(stepsLabel);
	stepsLabel.setText("Steps", dontSendNotification);
	stepsLabel.attachToComponent(&stepsSlider, false);

	stepsSlider.onValueChange = [this] { updateSteps(&stepsSlider); };
	stepsSlider.setValue(8);

	regenerationsSlider.setRange(0.0, 4.0, 1.0);
	regenerationsSlider.setSliderStyle(Slider::Rotary);
	regenerationsSlider.setPopupDisplayEnabled(true, false, this);
	regenerationsSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
	//steps.setTextValueSuffix(" Tempo");
	regenerationsSlider.setValue(1.0);
	// this function adds the slider to the editor
	addAndMakeVisible(regenerationsSlider);

	addAndMakeVisible(regenerationsLabel);
	regenerationsLabel.setText("Regenerations", dontSendNotification);
	regenerationsLabel.attachToComponent(&regenerationsSlider, false);

	regenerationsSlider.onValueChange = [this] { updateRegenerations(&regenerationsSlider); };
	regenerationsSlider.setValue(0);

	tempoSlider.setRange(0.0, 1.0, 0.01);
	tempoSlider.setSliderStyle(Slider::Rotary);
	tempoSlider.setPopupDisplayEnabled(true, false, this);
	tempoSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
	//steps.setTextValueSuffix(" Tempo");
	tempoSlider.setValue(1.0);
	// this function adds the slider to the editor
	addAndMakeVisible(tempoSlider);

	addAndMakeVisible(tempoLabel);
	tempoLabel.setText("Tempo", dontSendNotification);
	tempoLabel.attachToComponent(&tempoSlider, false);

	tempoSlider.onValueChange = [this] { updateTempo(&tempoSlider); };
	tempoSlider.setValue(0.5);
}

PandoAudioProcessorEditor::~PandoAudioProcessorEditor()
{
}

void PandoAudioProcessorEditor::updatePlayToggleState (Button* button)
{
	auto state = button->getToggleState();
	processor.play = state;
}

void PandoAudioProcessorEditor::updateListenToggleState(Button* button)
{
	auto state = button->getToggleState();
	processor.listen = state;
}

void PandoAudioProcessorEditor::updateReseedButton(Button* button)
{
	processor.markovChain.seed_sequence();
}

void PandoAudioProcessorEditor::updateForgetButton(Button* button)
{
	processor.markovChain.clear_entries();
}

void PandoAudioProcessorEditor::updateSteps(Slider* slider)
{
	auto stepsValue = slider->getValue();
	processor.markovChain.steps = stepsValue;
	processor.markovChain.seed_sequence();
}

void PandoAudioProcessorEditor::updateRegenerations(Slider* slider)
{
	auto regensValue = slider->getValue();
	processor.markovChain.regenerations = regensValue;
}

void PandoAudioProcessorEditor::updateTempo(Slider* slider)
{
	auto tempoValue = slider->getValue();
	processor.tempo = tempoValue;
}

//==============================================================================
void PandoAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
	g.drawFittedText("Reactive Sequencer", 0, 10, getWidth(), 30, Justification::centred, 1);
	g.setFont(12.0f);
	g.drawFittedText(ProjectInfo::companyName, 40, 205, getWidth(), 30, Justification::bottomLeft, 1);
	g.drawFittedText("Powered by probability", -40, 205, getWidth(), 30, Justification::bottomRight, 1);
}

void PandoAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	play.setBounds(40, 40, 60, 40);
	listen.setBounds(40, 70, 60, 40);
	reseed.setBounds(160, 60, 60, 40);
	forget.setBounds(300, 60, 60, 40);
	stepsSlider.setBounds(40, 150, 50, 50);
	regenerationsSlider.setBounds(160, 150, 80, 50);
	tempoSlider.setBounds(300, 150, 80, 50);
}
