#include "FModHandler.h"
#include <iostream>

std::string GetFormatInString(FMOD_SOUND_FORMAT format)
{
	std::string formatInString = "";
	switch (format)
	{
	case FMOD_SOUND_FORMAT_NONE:
		formatInString = "FMOD_SOUND_FORMAT_NONE";
		break;
	case FMOD_SOUND_FORMAT_PCM8:
		formatInString = "FMOD_SOUND_FORMAT_PCM8";
		break;
	case FMOD_SOUND_FORMAT_PCM16:
		formatInString = "FMOD_SOUND_FORMAT_PCM16";
		break;
	case FMOD_SOUND_FORMAT_PCM24:
		formatInString = "FMOD_SOUND_FORMAT_PCM24";
		break;
	case FMOD_SOUND_FORMAT_PCM32:
		formatInString = "FMOD_SOUND_FORMAT_PCM32";
		break;
	case FMOD_SOUND_FORMAT_PCMFLOAT:
		formatInString = "FMOD_SOUND_FORMAT_PCMFLOAT";
		break;
	case FMOD_SOUND_FORMAT_BITSTREAM:
		formatInString = "FMOD_SOUND_FORMAT_BITSTREAM";
		break;
	case FMOD_SOUND_FORMAT_MAX:
		formatInString = "FMOD_SOUND_FORMAT_MAX";
		break;
	case FMOD_SOUND_FORMAT_FORCEINT:
		formatInString = "FMOD_SOUND_FORMAT_FORCEINT";
		break;
	default:
		formatInString = "NULL";
		break;
	}
	return formatInString;
}

std::string GetTypeInString(FMOD_SOUND_TYPE type)
{
	std::string typeInString = "";
	switch (type)
	{
	case FMOD_SOUND_TYPE_UNKNOWN:
		typeInString = "FMOD_SOUND_TYPE_UNKNOWN";
		break;
	case FMOD_SOUND_TYPE_AIFF:
		typeInString = "FMOD_SOUND_TYPE_AIFF";
		break;
	case FMOD_SOUND_TYPE_ASF:
		typeInString = "FMOD_SOUND_TYPE_ASF";
		break;
	case FMOD_SOUND_TYPE_DLS:
		typeInString = "FMOD_SOUND_TYPE_DLS";
		break;
	case FMOD_SOUND_TYPE_FLAC:
		typeInString = "FMOD_SOUND_TYPE_FLAC";
		break;
	case FMOD_SOUND_TYPE_FSB:
		typeInString = "FMOD_SOUND_TYPE_FSB";
		break;
	case FMOD_SOUND_TYPE_IT:
		typeInString = "FMOD_SOUND_TYPE_IT";
		break;
	case FMOD_SOUND_TYPE_MIDI:
		typeInString = "FMOD_SOUND_TYPE_MIDI";
		break;
	case FMOD_SOUND_TYPE_MOD:
		typeInString = "FMOD_SOUND_TYPE_MOD";
		break;
	case FMOD_SOUND_TYPE_MPEG:
		typeInString = "FMOD_SOUND_TYPE_MPEG";
		break;
	case FMOD_SOUND_TYPE_OGGVORBIS:
		typeInString = "FMOD_SOUND_TYPE_OGGVORBIS";
		break;
	case FMOD_SOUND_TYPE_PLAYLIST:
		typeInString = "FMOD_SOUND_TYPE_PLAYLIST";
		break;
	case FMOD_SOUND_TYPE_RAW:
		typeInString = "FMOD_SOUND_TYPE_RAW";
		break;
	case FMOD_SOUND_TYPE_S3M:
		typeInString = "FMOD_SOUND_TYPE_S3M";
		break;
	case FMOD_SOUND_TYPE_USER:
		typeInString = "FMOD_SOUND_TYPE_USER";
		break;
	case FMOD_SOUND_TYPE_WAV:
		typeInString = "FMOD_SOUND_TYPE_WAV";
		break;
	case FMOD_SOUND_TYPE_XM:
		typeInString = "FMOD_SOUND_TYPE_XM";
		break;
	case FMOD_SOUND_TYPE_XMA:
		typeInString = "FMOD_SOUND_TYPE_XMA";
		break;
	case FMOD_SOUND_TYPE_AUDIOQUEUE:
		typeInString = "FMOD_SOUND_TYPE_AUDIOQUEUE";
		break;
	case FMOD_SOUND_TYPE_AT9:
		typeInString = "FMOD_SOUND_TYPE_AT9";
		break;
	case FMOD_SOUND_TYPE_VORBIS:
		typeInString = "FMOD_SOUND_TYPE_VORBIS";
		break;
	case FMOD_SOUND_TYPE_MEDIA_FOUNDATION:
		typeInString = "FMOD_SOUND_TYPE_MEDIA_FOUNDATION";
		break;
	case FMOD_SOUND_TYPE_MEDIACODEC:
		typeInString = "FMOD_SOUND_TYPE_MEDIACODEC";
		break;
	case FMOD_SOUND_TYPE_FADPCM:
		typeInString = "FMOD_SOUND_TYPE_FADPCM";
		break;
	case FMOD_SOUND_TYPE_OPUS:
		typeInString = "FMOD_SOUND_TYPE_OPUS";
		break;
	case FMOD_SOUND_TYPE_MAX:
		typeInString = "FMOD_SOUND_TYPE_MAX";
		break;
	case FMOD_SOUND_TYPE_FORCEINT:
		typeInString = "FMOD_SOUND_TYPE_FORCEINT";
		break;
	default:
		break;
	}
	return typeInString;
}

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

bool FModHandler::GetChannelGroupPitch(const std::string& name, float* pitch)
{
	const auto iterator = channelGroups.find(name);
	if (iterator == channelGroups.end())
		return false;

	*pitch = iterator->second->pitch;

	return true;
}

bool FModHandler::SetChannelGroupPitch(const std::string& name, float pitch)
{
	const auto iterator = channelGroups.find(name);
	if (iterator == channelGroups.end())
		return false;

	iterator->second->pitch = pitch;
	return ErrorCheck(iterator->second->grp_ptr->setPitch(pitch));
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

bool FModHandler::GetSoundData(const std::string& soundName, std::string* data)
{
	const auto sound_iterator = sounds.find(soundName);

	if (sound_iterator == sounds.end())
		return false;

	char* name = new char[100];
	fResult = sound_iterator->second->getName(name, 100);
	if (!ErrorCheck())
	{
		std::cout << "\nCouldn't retrieve Name";
		return false;
	}
	*data += "\nName: " + std::string(name);
	delete [] name;

	FMOD_SOUND_FORMAT format;
	FMOD_SOUND_TYPE type;

	fResult = sound_iterator->second->getFormat(&type, &format, NULL, NULL);
	if (!ErrorCheck())
	{
		std::cout << "\nCouldn't retrieve Format";
		return false;
	}
	*data += "\nFormat: " + GetFormatInString(format);
	*data += "\nType: " + GetTypeInString(type);
	float frequency;
	fResult = sound_iterator->second->getDefaults(&frequency,NULL);
	*data += "\nFrequency: " + std::to_string(frequency);
	unsigned int length;
	fResult = sound_iterator->second->getLength(&length, FMOD_TIMEUNIT_MS);
	*data += "\nLength: " + std::to_string(length);
	float volume;
	float balance;
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


bool FModHandler::LoopSound(const std::string& soundName)
{
	const auto sound_iterator = sounds.find(soundName);

	if (sound_iterator == sounds.end())
		return false;
	sound_iterator->second->setMode(FMOD_LOOP_NORMAL);
	return sound_iterator->second->setLoopCount(10000);
}

FMOD::Sound* FModHandler::GetSound(const std::string& soundName)
{
	const auto sound_iterator = sounds.find(soundName);

	if (sound_iterator == sounds.end())
		return nullptr;
	return sound_iterator->second;
}

void FModHandler::Shutdown()
{
	if (system)
	{
		system->release();
		system = nullptr;
	}
}

