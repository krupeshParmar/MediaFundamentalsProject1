#pragma once

#include <cstdio>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>

#include <map>
#include <string>

struct ChannelGroup
{
	FMOD::ChannelGroup* grp_ptr;
	float current_pan;
	float dsp_pitch;
};

class FModHandler
{
public:

	FMOD_RESULT fResult;
	FMOD::System* system;

	std::map<std::string, ChannelGroup*> channelGroups;
	std::map<std::string, FMOD::Sound*> sounds;
	std::map<std::string, FMOD::DSP*> dsps;

	bool ErrorCheck(const bool show_error = true) const;

	FModHandler();
	bool Initialize(const int numberOfChannels, const int systemFlags);
	void Shutdown();

	bool CreateChannelGroup(const std::string& name);
	bool FindChannelGroup(const std::string& name, ChannelGroup** channelGroup);
	bool RemoveChannelGroup(const std::string& name);
	bool SetChannelGroupParent(const std::string& childName, const std::string& parentName);
	bool GetChannelGroupVolume(const std::string& name, float* volume);
	bool SetChannelGroupVolume(const std::string& name, float volume);
	bool GetChannelGroupPan(const std::string& name, float* pan);
	bool SetChannelGroupPan(const std::string& name, float pan);

	bool GetChannelGroupEnabled(const std::string& name, bool* enabled);
	bool SetChannelGroupEnabled(const std::string& name, bool enabled);
	bool AddDspEffect(const std::string& channelGroupName, const std::string& effectName);
	bool RemoveDspEffect(const std::string& channelGroupname, const std::string& effectName);

	bool CreateSound(const std::string& name, const std::string& path, const int mode);
	bool PlaySound(const std::string& soundName, const std::string& channelGroupName);

	bool CreateDsp(const std::string& name, FMOD_DSP_TYPE dspType, const float value);
	bool GetDsp(const std::string& name, FMOD::DSP** dsp);

};