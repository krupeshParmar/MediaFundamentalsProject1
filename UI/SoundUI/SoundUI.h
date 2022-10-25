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
};