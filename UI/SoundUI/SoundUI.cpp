#include "SoundUI.h"
#include <imgui/imgui.h>
#include <iostream>

void SoundUI::Render()
{
	// Setting up IMGUI UI
	ImGui::Begin("Audio Settings");

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
	ImGui::SliderFloat("master volume", &currentVolume, 0.0f, 100.0f, "%.0f");
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

	ImGui::End();
}