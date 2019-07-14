#pragma once
#include <unordered_map>
#include <list>
#include <random>
#include "../JuceLibraryCode/JuceHeader.h"
#include "Note.h"

class MarkovChain : MessageListener
{
private:
	std::unordered_map<int, std::list<note>> markovChain;
	int entries;
public:
	MarkovChain();
	int get_size();
	int get_entries();
	void add_entry(note newNote, int lastNote);
	note pick_initial_note();
	int get_random_bucket(int bucket_size);
	void seed_sequence();
	void evolve_sequence();
	void clear_entries();
	note pick_new_note(int lastNoteValue);
	std::list<note> sequence;
	int lastNoteValue;
	bool seeded;
	std::list<note>::iterator sequenceMarker;
	void play_note(MidiBuffer& midiMessages, int offset);
	void handleMessage(const Message& message) override;
	int steps;
	int regenerations;
};
