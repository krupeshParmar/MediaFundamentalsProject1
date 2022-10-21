#pragma once
#include "../../FModHandler.h"

class SoundUI
{
	FModHandler* fmodHandler;
public:
	SoundUI(FModHandler* fmodHandler) : fmodHandler(fmodHandler) {}
	void Render();
};