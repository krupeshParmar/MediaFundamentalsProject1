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

	auto* newChannelGroup = new ChannelGroup();
	newChannelGroup->grp_ptr = channel_group;

	channelGroups.try_emplace(name, newChannelGroup);

	return true;
}

bool FModHandler::FindChannelGroup(const std::string& name, ChannelGroup** channelGroup)
{
	const auto iterator = channelGroups.find(name);
	if (iterator == channelGroups.end())
		return false;

	*channelGroup = iterator->second;
	
	return true;
}

bool FModHandler::RemoveChannelGroup(const std::string& name)
{
	const auto iterator = channelGroups.find(name);
	if (iterator == channelGroups.end())
		return false;

	iterator->second->grp_ptr->release();
	channelGroups.erase(iterator);
}

bool FModHandler::SetChannelGroupParent(const std::string& childName, const std::string& parentName)
{
	const auto childGroup = channelGroups.find(childName);
	const auto parentGroup = channelGroups.find(parentName);

	if (childGroup == channelGroups.end() || parentGroup == channelGroups.end())
		return false;

	fResult = parentGroup->second->grp_ptr->addGroup(childGroup->second->grp_ptr);

	return true;
}

bool FModHandler::GetChannelGroupVolume(const std::string& name, float* volume)
{
	const auto iterator = channelGroups.find(name);
	if (iterator == channelGroups.end())
		return false;

	return ErrorCheck(iterator->second->grp_ptr->getVolume(volume));
}

bool FModHandler::SetChannelGroupVolume(const std::string& name, float volume)
{
	const auto iterator = channelGroups.find(name);
	if (iterator == channelGroups.end())
		return false;

	return ErrorCheck(iterator->second->grp_ptr->setVolume(volume));
}

bool FModHandler::GetChannelGroupPan(const std::string& name, float* pan)
{
	const auto iterator = channelGroups.find(name);
	if (iterator == channelGroups.end())
		return false;

	*pan = iterator->second->current_pan;

	return true;
}

bool FModHandler::SetChannelGroupPan(const std::string& name, float pan)
{
	const auto iterator = channelGroups.find(name);
	if (iterator == channelGroups.end())
		return false;

	return ErrorCheck(iterator->second->grp_ptr->setPan(pan));
}

bool FModHandler::GetChannelGroupEnabled(const std::string& name, bool* enabled)
{
	const auto iterator = channelGroups.find(name);
	if (iterator == channelGroups.end())
		return false;

	if (!ErrorCheck(iterator->second->grp_ptr->getMute(enabled)))
		return false;

	*enabled = !(*enabled);

	return true;
}

bool FModHandler::SetChannelGroupEnabled(const std::string& name, bool enabled)
{
	const auto iterator = channelGroups.find(name);
	if (iterator == channelGroups.end())
		return false;

	if (!ErrorCheck(iterator->second->grp_ptr->setMute(!enabled)))
		return false;

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

	fResult = (*channel).setPaused(false);
	if (!ErrorCheck())
	{
		return false;
	}

	return true;
}

bool FModHandler::CreateDsp(const std::string& name, FMOD_DSP_TYPE dspType, const float value) 
{
	FMOD::DSP* dsp;

	if (!ErrorCheck(system->createDSPByType(dspType, &dsp)))
		return false;

	if (!ErrorCheck(dsp->setParameterFloat(0, value)))
		return false;

	dsps.try_emplace(name, dsp);
	return true;
}

bool FModHandler::GetDsp(const std::string& name, FMOD::DSP** dsp)
{
	const auto dspEffectIterator = dsps.find(name);
	if (dspEffectIterator == dsps.end())
		return false;

	*dsp = dspEffectIterator->second;
	return true;
}

bool FModHandler::AddDspEffect(const std::string& channelGroupName, const std::string& effectName)
{
	const auto channelGroupIterator = channelGroups.find(channelGroupName);
	const auto dspEffectIterator = dsps.find(effectName);

	if (channelGroupIterator == channelGroups.end() || dspEffectIterator == dsps.end())
		return false;

	int numDsp;
	if (!ErrorCheck(channelGroupIterator->second->grp_ptr->getNumDSPs(&numDsp)))
		return false;

	if (!ErrorCheck(channelGroupIterator->second->grp_ptr->addDSP(numDsp, dspEffectIterator->second)))
		return false;

	return true;
}

bool FModHandler::RemoveDspEffect(const std::string& channelGroupName, const std::string& effectName)
{
	const auto channelGroupIterator = channelGroups.find(channelGroupName);
	const auto dspEffectIterator = dsps.find(effectName);

	if (channelGroupIterator == channelGroups.end() || dspEffectIterator == dsps.end())
		return false;

	if (!ErrorCheck(channelGroupIterator->second->grp_ptr->removeDSP(dspEffectIterator->second)))
		return false;
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
