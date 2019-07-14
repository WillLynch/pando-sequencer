#pragma once

struct note {
	int pitch;
	int velocity;

	bool operator==(const note& otherNote)
	{
		return pitch == otherNote.pitch;
	}
};
