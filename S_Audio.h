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
	float echo_delay = 500.f;
	float echo_feedbackLevel = 50.f;
	float echo_dryLevel = 0.f;
	float echo_wetLevel = 0.f;
	float fader_gain = 0.f;
	float chorus_mix = 50.f;
	float chorus_rate = 0.8f;
	float chorus_depth = 3;
	float flange_mix = 50.f;
	float flange_depth = 1;
	float flange_rate = 0.1f;
	float highpass_cutoff = 5000.f;
	float highpass_resonance = 1.f;
	float limiter_releasetime = 10.f;
	float limiter_ceiling = 0.f;
	float limiter_maximizergain = 0.f;
	bool limiter_mode = false;
	float lowpass_cutoff = 5000.f;
	float lowpass_resonance = 1.f;
	int oscillator_type = 0;
	float oscillator_rate = 220.f;
	float normalize_fadetime = 5000;
	float normalize_threshold = 0.1f;
	float normalize_maxamp = 20;
};