#include "FModHandler.h"

bool FModHandler::ErrorCheck(const bool show_error) const
{
	if (fResult != FMOD_OK)
	{
		printf("fmod error: #%d-%s\n", fResult, FMOD_ErrorString(fResult));
	}

	return fResult == FMOD_OK;
}

FModHandler::FModHandler() : fResult(FMOD_OK), system(nullptr)
{

}

bool FModHandler::Initialize(const int numberOfChannels, const int systemFlags)
{
	fResult = FMOD::System_Create(&system);
	if (!ErrorCheck())
	{
		return false;
	}
	fResult = system->init(numberOfChannels, systemFlags, nullptr);
	if (!ErrorCheck())
	{
		return false;
	}

	return true;
}

bool FModHandler::CreateChannelGroup(const std::string& name)
{
	FMOD::ChannelGroup* channel_group;
	fResult = system->createChannelGroup(name.c_str(), &channel_group);
	if (!ErrorCheck())
	{
		return false;
	}

	auto* our_channel_group_with_extra_params = new ChannelGroup();
	our_channel_group_with_extra_params->grp_ptr = channel_group;

	channelGroups.try_emplace(name, our_channel_group_with_extra_params);

	return true;
}


bool FModHandler::CreateSound(const std::string& name, const std::string& path, const int mode)
{
	FMOD::Sound* sound;
	fResult = system->createSound(path.c_str(), mode, nullptr, &sound);
	if (!ErrorCheck())
	{
		return false;
	}

	sounds.try_emplace(name, sound);

	return true;
}

bool FModHandler::PlaySound(const std::string& sound_name, const std::string& channel_group_name)
{
	const auto sound_iterator = sounds.find(sound_name);
	const auto channel_group_iterator = channelGroups.find(channel_group_name);

	if (sound_iterator == sounds.end() || channel_group_iterator == channelGroups.end())
	{
		return false;
	}

	FMOD::Channel* channel;
	fResult = system->playSound(sound_iterator->second, channel_group_iterator->second->grp_ptr, true, &channel);
	if (!ErrorCheck())
	{
		return false;
	}

	//more stuff to come here next class

	fResult = (*channel).setPaused(false);
	if (!ErrorCheck())
	{
		return false;
	}

	return true;
}

void FModHandler::Shutdown()
{
	if (system)
	{
		system->release();
		system = nullptr;
	}
}
