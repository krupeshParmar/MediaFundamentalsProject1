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
		for (int i = 0; i < soundsList->size(); i++)
		{
			ImGui::RadioButton("Selected", i== selectedSound);
			S_Audio* sAudio = soundsList->at(i);
			FMOD::Sound* soundTest = fmodHandler->GetSound(sAudio->name);
			soundTest->setMusicSpeed(2.f);
			std::string* data = new std::string("");
			if (!fmodHandler->GetSoundData(sAudio->name, data))
			{
				std::cout << "Error" << std::endl;
			}
			ImGui::Text(data->c_str());

			ChannelGroup* channelGroup;
			if (!fmodHandler->FindChannelGroup(sAudio->channelName, &channelGroup))
			{
				std::cout << "Channel not found" << std::endl;
			}

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

			FMOD::Sound* sound = fmodHandler->GetSound(sAudio->name);

			sound->getMusicSpeed(&(sAudio->speed));

			ImGui::SliderFloat("Speed", &(sAudio->speed), 0.5f, 2.0f, "%.0f");

			if (sound->setMusicSpeed(sAudio->speed) == FMOD_OK)
			{
				std::cout << "\n GAYYYYYYYY!";
			}

			ImGui::SameLine();
			ImGui::Text("Speed");

			if (!fmodHandler->GetChannelGroupPitch(sAudio->channelName, &(sAudio->pitch)))
			{
				std::cout << "Pitch not found" << std::endl;
			}


			ImGui::SliderFloat("Pitch", &(sAudio->pitch), 0.5f, 2.0f, "%.0f");

			if (!fmodHandler->SetChannelGroupPitch(sAudio->channelName, sAudio->pitch))
			{
				std::cout << "Failed to set channel pitch" << std::endl;
			}
		}
	}
	

	ImGui::End();
}