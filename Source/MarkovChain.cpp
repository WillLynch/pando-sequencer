#include "MarkovChain.h"
#include "Note.h"

MarkovChain::MarkovChain()
{
	entries = 0;
	lastNoteValue = -1;
	sequenceMarker = sequence.end();
	seeded = false;
	steps = 8;
	regenerations = 0;
}

int MarkovChain::get_size()
{
	return markovChain.size();
}

int MarkovChain::get_entries()
{
	return entries;
}

void MarkovChain::add_entry(note newNote, int lastNote)
{
	if (lastNote == NULL) {
		lastNote = newNote.pitch;
	}
	else {
		markovChain[lastNote].push_back(newNote);
		entries++;
	}
}

note MarkovChain::pick_initial_note()
{
	if (get_size() < 2) {
		return pick_new_note(-1);
	}
	int bucket = get_random_bucket(get_size());
	auto it = markovChain.begin();
	std::advance(it, bucket);
	note initialNote;
	initialNote.pitch = it->first;
	initialNote.velocity = 127;
	return initialNote;
}

note MarkovChain::pick_new_note(int lastNoteValue)
{
	//srand(time(NULL));
	int bucket_size = markovChain[lastNoteValue].size();

	if (bucket_size == 0)
		return pick_initial_note();

	int dice_roll = get_random_bucket(bucket_size);
	// Create iterator pointing to first element
	std::list<note>::iterator it = markovChain[lastNoteValue].begin();
	// Advance the iterator by 2 positions,
	std::advance(it, (dice_roll));
	return *it;
}

void MarkovChain::play_note(MidiBuffer& midiMessages, int offset)
{
	//if (get_entries() <= steps) return;

	if (sequence.size() == 0) {
		seed_sequence();
		if (sequence.size() == 0)
			return;
	}

	if (lastNoteValue > 0)
		midiMessages.addEvent(MidiMessage::noteOff(1, lastNoteValue), offset);

	note newNoteValue = *sequenceMarker;
	if (newNoteValue.pitch > -1)
		midiMessages.addEvent(MidiMessage::noteOn(1, newNoteValue.pitch, (uint8)newNoteValue.velocity), offset);

	lastNoteValue = newNoteValue.pitch;
	sequenceMarker++;

	if (sequenceMarker == sequence.end()) {
		evolve_sequence();
		sequenceMarker = sequence.begin();
	}
}

void MarkovChain::handleMessage(const Message& message)
{
}

int MarkovChain::get_random_bucket(int bucket_size)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, bucket_size-1); // distribution in range
	return dist6(rng);
}

void MarkovChain::seed_sequence()
{
	if (get_entries() == 0) {
		sequence.clear();
		return;
	}
	sequence.clear();
	note sequenceNote = pick_initial_note();
	sequence.push_back(sequenceNote);
	for (int i = 1; i < steps; i++) {
		note newNote = pick_new_note(sequenceNote.pitch);
		sequence.push_back(newNote);
		sequenceNote = newNote;
	}

	sequenceMarker = sequence.begin();
}

void MarkovChain::evolve_sequence()
{
	if (entries == 0) return;

	for (int i = 0; i < regenerations; i++) {

		if (sequence.size() == 1) {
			note newNote = pick_new_note(sequence.begin()->pitch);
			*sequence.begin() = newNote;
			return;
		}

		int step = get_random_bucket(steps-1);

		std::list<note>::iterator it;
		it = sequence.begin();
		std::advance(it, step);
		note firstNote = *it;
		note newNote = pick_new_note(firstNote.pitch);
		it++;
		*it = newNote;

		it++;
		while (it != sequence.end()) {
			int newNoteValue = newNote.pitch;
			bool found = (std::find(markovChain[newNoteValue].begin(), markovChain[newNoteValue].end(), *it) != markovChain[newNoteValue].end());
			if (found) break;

			*it = pick_new_note(newNote.pitch);
			newNote = *it;
			it++;
		}
	}
}

void MarkovChain::clear_entries()
{
	markovChain.clear();
	//sequence.clear();
	entries = 0;
}
