#pragma once

#include <string> 
const std::string MUSIC = "MUSIC";
const std::string SFX = "SFX";

struct S_Audio
{
	std::string name;
	std::string channelName;
	float volume;
	float pan;
	float speed;
	float pitch;
	bool enabled;
	bool playing;
	std::string soundFileType;

};