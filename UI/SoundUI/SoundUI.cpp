#include "SoundUI.h"
#include <imgui/imgui.h>
#include <iostream>
#include <vector>
#include "../../S_Audio.h"

SoundUI::SoundUI(FModHandler* fmodHandler) : fmodHandler(fmodHandler) {
}


void SoundUI::Render(std::vector<S_Audio*>* soundsList,int selectedSound)
{
	// Setting up IMGUI UI
	ImGui::Begin("Audio Settings");
	std::string inputs = "Press left ctrl to switch songs";
	inputs += "\nUse space key to play / pause sound";
	inputs += "\nUse Up and Down arrow to control volume";
	inputs += "\nUse left and right arrow to control pitch";
	inputs += "\nUse Page up and Page down keys to control balance (pan)";
	inputs += "\nUse Home and End keys to control speed";
	inputs += "\nUse Dsp panel to control dsp effects";
	ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), inputs.c_str());
	{
			ChannelGroup* channel_group;
		if (!fmodHandler->FindChannelGroup("master", &channel_group))
		{
			std::cout << "Master not found" << std::endl;
		}

		float currentVolume;
		if (!fmodHandler->GetChannelGroupVolume("master", &currentVolume))
		{
			std::cout << "Channel group value not found" << std::endl;
		}

		currentVolume *= 100;
		ImGui::SliderFloat("#master volume", &currentVolume, 0.0f, 100.0f, "%.0f");
		currentVolume /= 100;

		if (!fmodHandler->SetChannelGroupVolume("master", currentVolume))
		{
			std::cout << "Failed to set channel volume" << std::endl;
		}

		bool volumeEnabled;
		if (!fmodHandler->GetChannelGroupEnabled("master", &volumeEnabled))
		{
			std::cout << "Failed to get group enabled value" << std::endl;
		}

		ImGui::SameLine();
		ImGui::Checkbox("##master volume", &volumeEnabled);

		if (!fmodHandler->SetChannelGroupEnabled("master", volumeEnabled))
		{
			std::cout << "Failed to set group enabled value" << std::endl;
		}
	}
	
	{
		int sfxCount = 0;
		S_Audio* selectedAudio = NULL;
		for (int i = 0; i < soundsList->size(); i++)
		{
			ImGui::BeginChild("Music", ImVec2(800, 0),true);
			ImGui::RadioButton("Selected", i== selectedSound);
			S_Audio* sAudio = soundsList->at(i);

			if (i == selectedSound)
				selectedAudio = sAudio;

			if (sAudio->soundFileType == SFX)
			{
				ImGui::SameLine();
				sfxCount++;
				std::string press = "Press: " + std::to_string(sfxCount);
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.f), press.c_str());
			}

			ImGui::Separator();

			ChannelGroup* channelGroup;
			if (!fmodHandler->FindChannelGroup(sAudio->channelName, &channelGroup))
			{
				std::cout << "Channel not found" << std::endl;
			}


			/*VOLUME*/
			if (!fmodHandler->GetChannelGroupVolume(sAudio->channelName, &(sAudio->volume)))
			{
				std::cout << "Channel group value not found" << std::endl;
			}
			sAudio->volume *= 100;
			std::string volume = sAudio->channelName + " volume";
			ImGui::SliderFloat(volume.c_str(), &(sAudio->volume), 0.0f, 100.0f, "%.0f");
			sAudio->volume /= 100;
		
			if (!fmodHandler->SetChannelGroupVolume(sAudio->channelName, sAudio->volume))
			{
				std::cout << "Failed to set channel volume" << std::endl;
			}


			if (!fmodHandler->GetChannelGroupEnabled(sAudio->channelName, &(sAudio->enabled)))
			{
				std::cout << "Failed to get group enabled value" << std::endl;
			}

			ImGui::SameLine();
			volume = "##" + sAudio->channelName + " volume";
			ImGui::Checkbox(volume.c_str(), &(sAudio->enabled));

			if (!fmodHandler->SetChannelGroupEnabled(sAudio->channelName, sAudio->enabled))
			{
				std::cout << "Failed to set group enabled value" << std::endl;
			}

			/*Balance (PAN)*/
			if (!fmodHandler->GetChannelGroupPan(sAudio->channelName, &(channelGroup->current_pan)))
			{
				std::cout << "Channel group value not found" << std::endl;
			}

			std::string pan = "##" + sAudio->channelName + " pan";
			ImGui::SliderFloat(pan.c_str(), &channelGroup->current_pan, -1.0f, 1.0f, "%.2f");

			if (!fmodHandler->SetChannelGroupPan(sAudio->channelName, channelGroup->current_pan))
			{
				//error
				//do something?
			}

			ImGui::SameLine();
			ImGui::Text("Balance (Pan)");
			{
				/*Speed*/
				if (!fmodHandler->GetChannelGroupFrequency(sAudio->channelName, &(sAudio->speed)))
				{
					std::cout << "Speed not found" << std::endl;
				}

				ImGui::SliderFloat("Speed", &(sAudio->speed), 20.f, 200000.0f, "%.0f");
				if (!fmodHandler->SetChannelGroupFrequency(sAudio->channelName, sAudio->speed))
				{
					std::cout << "Failed to set channel speed" << std::endl;
				}
			}

			/*Pitch*/
			if (!fmodHandler->GetChannelGroupPitch(sAudio->channelName, &(sAudio->pitch)))
			{
				std::cout << "Pitch not found" << std::endl;
			}

			float pitch = sAudio->pitch;
			pitch *= 50;
			ImGui::SliderFloat("Pitch", &pitch, 0.f, 100.0f, "%.0f");
			sAudio->pitch = pitch / 50.f;
			if (!fmodHandler->SetChannelGroupPitch(sAudio->channelName, sAudio->pitch))
			{
				std::cout << "Failed to set channel pitch" << std::endl;
			}
			
			/*if (!fmodHandler->GetChannelGroupFrequency(sAudio->channelName, &(sAudio->speed)))
			{
				std::cout << "Speed not found" << std::endl;
			}

			ImGui::SliderFloat("Frequency", &(sAudio->speed), 0.5f, 2.0f, "%.0f");

			if (!fmodHandler->SetChannelGroupFrequency(sAudio->channelName, sAudio->speed))
			{
				std::cout << "Failed to set channel speed" << std::endl;
			}*/

			ImGui::EndChild();
		}

		std::string* data = new std::string("");
		if (selectedAudio != NULL)
		{
			ImGui::BeginGroup();
			ImGui::SameLine();
			ImGui::BeginChild("Music details & Dsps", ImVec2(0, 500));
			if (!fmodHandler->GetSoundData(selectedAudio->name, selectedAudio->channelName, data))
			{
				std::cout << "Error" << std::endl;
			}
			ImGui::Text(data->c_str());


			if (selectedAudio->channelName == "surface")
			{
				{
					ImGui::Separator();
					ImGui::BeginGroup();
					ImGui::Checkbox("Echo", &echoEnabled);

					if (!echoEnabled && FLAG_ECHO != 0)
					{
						FLAG_ECHO = 0;
						fmodHandler->RemoveDspEffect(selectedAudio->channelName, "echo");
					}
					else if (echoEnabled && FLAG_ECHO != 1) {

						FLAG_ECHO = 1;
						fmodHandler->AddDspEffect(selectedAudio->channelName, "echo");
					}
					FMOD::DSP* dsp;
					fmodHandler->GetDsp("echo", &dsp);
					dsp->setParameterFloat(0, selectedAudio->echo_delay);

					float echoLevlPr = (selectedAudio->echo_delay);
					ImGui::SliderFloat("Echo Delay", &echoLevlPr, 10.0f, 5000.0f, "%.f");
					selectedAudio->echo_delay = (echoLevlPr);

					dsp->setParameterFloat(1, selectedAudio->echo_feedbackLevel);

					float echo_feedback = (selectedAudio->echo_feedbackLevel);
					ImGui::SliderFloat("Echo Feedback", &echo_feedback, 0.0f, 100.0f, "%.f");
					selectedAudio->echo_feedbackLevel = (echo_feedback);

					dsp->setParameterFloat(2, selectedAudio->echo_dryLevel);

					float echo_dryLe = (selectedAudio->echo_dryLevel);
					ImGui::SliderFloat("Echo Dry Level", &echo_dryLe, -80.0f, 10.0f, "%.f");
					selectedAudio->echo_dryLevel = (echo_dryLe);

					dsp->setParameterFloat(3, selectedAudio->echo_wetLevel);

					float echo_wetLe = (selectedAudio->echo_wetLevel);
					ImGui::SliderFloat("Echo Wet Level", &echo_wetLe, -80.0f, 10.0f, "%.f");
					selectedAudio->echo_wetLevel = (echo_wetLe);
					ImGui::EndGroup();
				}
				
				{
					ImGui::Separator();
					ImGui::BeginGroup();
					ImGui::Checkbox("Fader", &faderEnabled);

					if (!faderEnabled && FLAG_FADER != 0)
					{
						FLAG_FADER = 0;
						fmodHandler->RemoveDspEffect(selectedAudio->channelName, "fader");
					}
					else if (faderEnabled && FLAG_FADER != 1) {

						FLAG_FADER = 1;
						fmodHandler->AddDspEffect(selectedAudio->channelName, "fader");
					}
					FMOD::DSP* dsp;
					fmodHandler->GetDsp("fader", &dsp);
					dsp->setParameterFloat(0, selectedAudio->fader_gain);

					float faderGain = (selectedAudio->fader_gain);
					ImGui::SliderFloat("Fader Gain", &faderGain, -80.0f, 10.0f, "%.f");
					selectedAudio->fader_gain = (faderGain);

					ImGui::EndGroup();
				}
				{
					ImGui::Separator();
					ImGui::BeginGroup();
					ImGui::Checkbox("Chorus", &chorusEnabled);

					if (!chorusEnabled && FLAG_CHORUS != 0)
					{
						FLAG_CHORUS = 0;
						fmodHandler->RemoveDspEffect(selectedAudio->channelName, "chorus");
					}
					else if (chorusEnabled && FLAG_CHORUS != 1) {

						FLAG_CHORUS = 1;
						fmodHandler->AddDspEffect(selectedAudio->channelName, "chorus");
					}
					FMOD::DSP* dsp;
					fmodHandler->GetDsp("chorus", &dsp);
					dsp->setParameterFloat(0, selectedAudio->chorus_mix);

					float chorusMix = (selectedAudio->chorus_mix);
					ImGui::SliderFloat("Chorus Mix", &chorusMix, 0.0f, 100.0f, "%.f");
					selectedAudio->chorus_mix = (chorusMix);

					dsp->setParameterFloat(1, selectedAudio->chorus_rate);

					float chorusRate = (selectedAudio->chorus_rate);
					ImGui::SliderFloat("Chorus Rate", &chorusRate, 0.0f, 20.0f, "%.0f");
					selectedAudio->chorus_rate = (chorusRate);

					dsp->setParameterFloat(2, selectedAudio->chorus_depth);

					float chorusDepth = (selectedAudio->chorus_depth);
					ImGui::SliderFloat("Chorus Depth", &chorusDepth, 0.0f, 100.0f, "%.f");
					selectedAudio->chorus_depth = (chorusDepth);

					ImGui::EndGroup();
				}
			}

			if (selectedAudio->channelName == "underground")
			{
				{
					ImGui::Separator();
					ImGui::BeginGroup();
					ImGui::Checkbox("flange", &flangeEnabled);

					if (!flangeEnabled && FLAG_FLANGE != 0)
					{
						FLAG_FLANGE = 0;
						fmodHandler->RemoveDspEffect(selectedAudio->channelName, "flange");
					}
					else if (flangeEnabled && FLAG_FLANGE != 1) {

						FLAG_FLANGE = 1;
						fmodHandler->AddDspEffect(selectedAudio->channelName, "flange");
					}
					FMOD::DSP* dsp;
					fmodHandler->GetDsp("flange", &dsp);
					dsp->setParameterFloat(0, selectedAudio->flange_mix);

					float flangeMix = (selectedAudio->flange_mix);
					ImGui::SliderFloat("Flange Mix", &flangeMix, 0.0f, 100.0f, "%.f");
					selectedAudio->flange_mix = (flangeMix);

					dsp->setParameterFloat(1, selectedAudio->flange_depth);

					float flangeDepth = (selectedAudio->flange_depth) * 100.f;
					ImGui::SliderFloat("Flange Depth", &flangeDepth, 1.f, 100.0f, "%.f");
					selectedAudio->flange_depth = (flangeDepth) / 100.f;

					dsp->setParameterFloat(2, selectedAudio->flange_rate);

					float flangeRate = (selectedAudio->flange_rate);
					ImGui::SliderFloat("Flange Rate", &flangeRate, 0.0f, 20.0f, "%.0f");
					selectedAudio->flange_rate = (flangeRate);

					ImGui::EndGroup();
				}
				{
					ImGui::Separator();
					ImGui::BeginGroup();
					ImGui::Checkbox("High Pass", &highPassEnabled);

					if (!highPassEnabled && FLAG_HIGH_PASS != 0)
					{
						FLAG_HIGH_PASS = 0;
						fmodHandler->RemoveDspEffect(selectedAudio->channelName, "highpass");
					}
					else if (highPassEnabled && FLAG_HIGH_PASS != 1) {

						FLAG_HIGH_PASS = 1;
						fmodHandler->AddDspEffect(selectedAudio->channelName, "highpass");
					}
					FMOD::DSP* dsp;
					fmodHandler->GetDsp("highpass", &dsp);
					dsp->setParameterFloat(0, selectedAudio->highpass_cutoff);

					float highpassCutoff = (selectedAudio->highpass_cutoff);
					ImGui::SliderFloat("High Pass Cut Off", &highpassCutoff, 1, 22000, "%.f");
					selectedAudio->highpass_cutoff = (highpassCutoff);

					dsp->setParameterFloat(1, selectedAudio->highpass_resonance);

					float highpassResonance = (selectedAudio->highpass_resonance) * 10;
					ImGui::SliderFloat("High Pass Resonance", &highpassResonance, 1.f, 100.0f, "%.f");
					selectedAudio->highpass_resonance = (highpassResonance) / 10;
					ImGui::EndGroup();
				}
				{
					ImGui::Separator();
					ImGui::BeginGroup();
					ImGui::Checkbox("Limiter", &limiterEnabled);

					if (!limiterEnabled && FLAG_LIMITER != 0)
					{
						FLAG_LIMITER = 0;
						fmodHandler->RemoveDspEffect(selectedAudio->channelName, "limiter");
					}
					else if (limiterEnabled && FLAG_HIGH_PASS != 1) {

						FLAG_HIGH_PASS = 1;
						fmodHandler->AddDspEffect(selectedAudio->channelName, "limiter");
					}
					FMOD::DSP* dsp;
					fmodHandler->GetDsp("limiter", &dsp);
					dsp->setParameterFloat(0, selectedAudio->limiter_releasetime);

					float limiterReleasetime = (selectedAudio->limiter_releasetime);
					ImGui::SliderFloat("Limiter Release Time", &limiterReleasetime, 1, 1000, "%.f");
					selectedAudio->limiter_releasetime = (limiterReleasetime);

					dsp->setParameterFloat(1, selectedAudio->limiter_ceiling);

					float limiterCeiling = (selectedAudio->limiter_ceiling);
					ImGui::SliderFloat("Limiter Ceiling", &limiterCeiling, -12.f, 0.f, "%f");
					selectedAudio->limiter_ceiling = (limiterCeiling);

					dsp->setParameterFloat(2, selectedAudio->limiter_maximizergain);

					float limiterMaximizergain = (selectedAudio->limiter_maximizergain);
					ImGui::SliderFloat("Limiter Maximizer Gain", &limiterMaximizergain, 0.f, 12.f, "%f");
					selectedAudio->limiter_maximizergain = (limiterMaximizergain);

					dsp->setParameterBool(3, selectedAudio->limiter_mode);

					bool limiterMode = (selectedAudio->limiter_mode);
					ImGui::Checkbox("Limiter Mode", &limiterMode);
					selectedAudio->limiter_mode = (limiterMode);
					ImGui::EndGroup(); 
				}
			}

			if (selectedAudio->channelName == "water")
			{
				{
					ImGui::Separator();
					ImGui::BeginGroup();
					ImGui::Checkbox("Low Pass", &lowPassEnabled);

					if (!lowPassEnabled && FLAG_LOW_PASS != 0)
					{
						FLAG_LOW_PASS = 0;
						fmodHandler->RemoveDspEffect(selectedAudio->channelName, "lowpass");
					}
					else if (lowPassEnabled && FLAG_LOW_PASS != 1) {

						FLAG_LOW_PASS = 1;
						fmodHandler->AddDspEffect(selectedAudio->channelName, "lowpass");
					}
					FMOD::DSP* dsp;
					fmodHandler->GetDsp("lowpass", &dsp);
					dsp->setParameterFloat(0, selectedAudio->lowpass_cutoff);

					float lowpassCutoff = (selectedAudio->lowpass_cutoff);
					ImGui::SliderFloat("Low Pass Cut Off", &lowpassCutoff, 1, 22000, "%.f");
					selectedAudio->lowpass_cutoff = (lowpassCutoff);

					dsp->setParameterFloat(1, selectedAudio->lowpass_resonance);

					float lowpassResonance = (selectedAudio->lowpass_resonance) * 10;
					ImGui::SliderFloat("Low Pass Resonance", &lowpassResonance, 1.f, 100.0f, "%.f");
					selectedAudio->lowpass_resonance = (lowpassResonance) / 10;
					ImGui::EndGroup();
				}
				
				{
					ImGui::Separator();
					ImGui::BeginGroup();
					ImGui::Checkbox("Oscillator (Danger)", &oscillatorEnabled);

					if (!oscillatorEnabled && FLAG_OSCILLATOR != 0)
					{
						FLAG_OSCILLATOR = 0;
						fmodHandler->RemoveDspEffect(selectedAudio->channelName, "oscillator");
					}
					else if (oscillatorEnabled && FLAG_OSCILLATOR != 1) {

						FLAG_OSCILLATOR = 1;
						fmodHandler->AddDspEffect(selectedAudio->channelName, "oscillator");
					}
					FMOD::DSP* dsp;
					fmodHandler->GetDsp("oscillator", &dsp);
					dsp->setParameterFloat(0, selectedAudio->oscillator_type);

					int oscillatorType = (selectedAudio->oscillator_type);
					ImGui::SliderInt("Oscillator Type", &oscillatorType, 0, 5, "%d");
					selectedAudio->oscillator_type = (oscillatorType);

					dsp->setParameterFloat(1, selectedAudio->oscillator_rate);

					float oscillatorRate = (selectedAudio->oscillator_rate);
					ImGui::SliderFloat("Oscillator Rate", &oscillatorRate, 0.f, 22000.0f, "%.f");
					selectedAudio->oscillator_rate = (oscillatorRate);
					ImGui::EndGroup();
				}
				
				{
					ImGui::Separator();
					ImGui::BeginGroup();
					ImGui::Checkbox("Normalize (Danger)", &normalizeEnabled);

					if (!normalizeEnabled && FLAG_NORMALIZE != 0)
					{
						FLAG_NORMALIZE = 0;
						fmodHandler->RemoveDspEffect(selectedAudio->channelName, "normalize");
					}
					else if (normalizeEnabled && FLAG_NORMALIZE != 1) {

						FLAG_NORMALIZE = 1;
						fmodHandler->AddDspEffect(selectedAudio->channelName, "normalize");
					}
					FMOD::DSP* dsp;
					fmodHandler->GetDsp("normalize", &dsp);
					dsp->setParameterFloat(0, selectedAudio->normalize_fadetime);

					float normalizeFadetime = (selectedAudio->normalize_fadetime);
					ImGui::SliderFloat("Normalize Fade Time", &normalizeFadetime, 0, 20000, "%.f");
					selectedAudio->normalize_fadetime = (normalizeFadetime);

					dsp->setParameterFloat(1, selectedAudio->normalize_threshold);

					float normalizeThreshold = (selectedAudio->normalize_threshold) * 10.f;
					ImGui::SliderFloat("Normalize Threshold", &normalizeThreshold, 0.f, 10.f, "%.f");
					selectedAudio->normalize_threshold = (normalizeThreshold) / 10.f;

					dsp->setParameterFloat(2, selectedAudio->normalize_maxamp);

					float normalizMaxamp = (selectedAudio->normalize_maxamp);
					ImGui::SliderFloat("Normalize Max Amp", &normalizMaxamp, 1, 100000.f, "%.f");
					selectedAudio->normalize_maxamp = (normalizMaxamp);
					ImGui::EndGroup();
				}
			}

			ImGui::EndChild();

			ImGui::EndGroup();
		}

	}
	

	ImGui::End();
}