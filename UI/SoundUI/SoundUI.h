#pragma once
#include "../../FModHandler.h"
#include <vector>
#include <string>
#include "../../S_Audio.h"

class SoundUI
{
	FModHandler* fmodHandler;
public:
	SoundUI(FModHandler* fmodHandler);
	void Render(std::vector<S_Audio*>* soundsList, int selectedSound);
	bool chorusEnabled = false;
	bool echoEnabled = false;
	bool distortionEnabled = false;
	bool faderEnabled = false;
	bool flangeEnabled = false;
	bool highPassEnabled = false;
	bool limiterEnabled = false;
	bool lowPassEnabled = false;
	bool oscillatorEnabled = false;
	bool normalizeEnabled = false;

	int FLAG_ECHO = 0;
	int FLAG_CHORUS = 0;
	int FLAG_FADER = 0;
	int FLAG_FLANGE = 0;
	int FLAG_HIGH_PASS = 0;
	int FLAG_LIMITER = 0;
	int FLAG_LOW_PASS = 0;
	int FLAG_OSCILLATOR = 0;
	int FLAG_NORMALIZE = 0;
};