#include <FMOD/fmod.h>
#include "FModHandler.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include "UI/SoundUI/SoundUI.h"
#include <vector>
#include <fstream>
#include "S_Audio.h"
#include <iostream>
#define SOUND_LIST_FILE "soundslist.txt"
#define COMP_SOUND_LIST_FILE "comp_soundslist.txt"

std::vector<S_Audio*> soundsList, sfxList;
GLFWwindow* window;
FModHandler fmodHandler;
int selectedSound = 0;
S_Audio* sAudio;

void loadSounds(std::string startingPath, int choice)
{
	std::ifstream soundListFile;
	if(choice == 1)
		soundListFile = std::ifstream(SOUND_LIST_FILE);
	else
		soundListFile = std::ifstream(COMP_SOUND_LIST_FILE);
	if (!soundListFile.is_open())
		return;

	std::string wordToken;
	while (soundListFile >> wordToken)
	{
		if (wordToken == "mus")
		{
			S_Audio* sAudio = new S_Audio();
			sAudio->soundFileType = MUSIC;
			soundListFile >> wordToken;
			std::cout << "Name: " << wordToken << std::endl;
			sAudio->name = wordToken;
			soundListFile >> wordToken;
			std::cout << "Channel: " << wordToken << std::endl;
			sAudio->channelName = wordToken;
			sAudio->enabled = true;
			sAudio->playing = false;
			soundsList.push_back(sAudio);
		}

		if (wordToken == "sfx")
		{
			S_Audio* sAudio = new S_Audio();
			sAudio->soundFileType = SFX;
			soundListFile >> wordToken;
			std::cout << "Name: " << wordToken << std::endl;
			sAudio->name = wordToken;
			soundListFile >> wordToken;
			std::cout << "Channel: " << wordToken << std::endl;
			sAudio->channelName = wordToken;
			sAudio->enabled = true;
			sAudio->playing = false;
			sAudio->speed = 1.f;
			soundsList.push_back(sAudio);
		}
	}
}

void key_callback(GLFWwindow* window, const int key, int scancode, const int action, const int mods)
{
	if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
	{
		if (selectedSound == soundsList.size() - 1)
			selectedSound = 0;
		else selectedSound += 1;
	}
	sAudio = soundsList[selectedSound];

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		sAudio->volume = sAudio->volume < 0.99f ? sAudio->volume + 0.01f : 1.f;
		fmodHandler.SetChannelGroupVolume(sAudio->channelName, sAudio->volume);
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		sAudio->volume = sAudio->volume > 0.01f ? sAudio->volume - 0.01f : 0.f;
		fmodHandler.SetChannelGroupVolume(sAudio->channelName, sAudio->volume);
	}
	
	if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS)
	{
		ChannelGroup* channelGroup;
		if (!fmodHandler.FindChannelGroup(sAudio->channelName, &channelGroup))
		{
			std::cout << "Channel not found" << std::endl;
		}

		if (channelGroup->current_pan <= 0.99f)
		{
			channelGroup->current_pan += 0.1f;
			fmodHandler.SetChannelGroupPan(sAudio->channelName, channelGroup->current_pan);
		}
	}
	if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS)
	{
		ChannelGroup* channelGroup;
		if (!fmodHandler.FindChannelGroup(sAudio->channelName, &channelGroup))
		{
			std::cout << "Channel not found" << std::endl;
		}

		if (channelGroup->current_pan >= -0.99f)
		{
			channelGroup->current_pan -= 0.1f;
			fmodHandler.SetChannelGroupPan(sAudio->channelName, channelGroup->current_pan);
		}
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		if (!fmodHandler.PlayPauseSound(soundsList[selectedSound]->channelName, soundsList[selectedSound]))
		{
			fmodHandler.ErrorCheck(true);
			//std::cout << "\nPlay Pause Sound Failed" << std::endl;
		}
	}
	
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		if (!fmodHandler.GetChannelGroupPitch(sAudio->channelName, &(sAudio->pitch)))
		{
			std::cout << "Channel not found" << std::endl;
		}

		if (sAudio->pitch >= 0.51f)
		{
			sAudio->pitch -= 0.01f;
		}
		fmodHandler.SetChannelGroupPitch(sAudio->channelName, sAudio->pitch);
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		if (!fmodHandler.GetChannelGroupPitch(sAudio->channelName, &(sAudio->pitch)))
		{
			std::cout << "Channel not found" << std::endl;
		}

		if (sAudio->pitch <= 1.99f)
		{
			sAudio->pitch += 0.01f;
		}

		fmodHandler.SetChannelGroupPitch(sAudio->channelName, sAudio->pitch);
	}
	
	if (key == GLFW_KEY_HOME && action == GLFW_PRESS)
	{
		if (!fmodHandler.GetChannelGroupFrequency(sAudio->channelName, &(sAudio->speed)))
		{
			std::cout << "Channel not found" << std::endl;
		}

		if (sAudio->speed >= 30.f)
		{
			sAudio->speed -= 10.f;
		}
		fmodHandler.SetChannelGroupFrequency(sAudio->channelName, sAudio->speed);
	}

	if (key == GLFW_KEY_END && action == GLFW_PRESS)
	{
		if (!fmodHandler.GetChannelGroupFrequency(sAudio->channelName, &(sAudio->speed)))
		{
			std::cout << "Channel not found" << std::endl;
		}

		if (sAudio->speed <= 200000.f)
		{
			sAudio->speed += 10.f;
		}

		fmodHandler.SetChannelGroupFrequency(sAudio->channelName, sAudio->speed);
	}

	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		if(sfxList.size() > 0)
			fmodHandler.PlaySound((sfxList[0]->name), (sfxList[0]->channelName));
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		if (sfxList.size() > 1)
			fmodHandler.PlaySound((sfxList[1]->name), (sfxList[1]->channelName));
	}

	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		if (sfxList.size() > 2)
			fmodHandler.PlaySound((sfxList[2]->name), (sfxList[2]->channelName));
	}

	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		if (sfxList.size() > 3)
			fmodHandler.PlaySound((sfxList[3]->name), (sfxList[3]->channelName));
	}
	
	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		if (sfxList.size() > 4)
			fmodHandler.PlaySound((sfxList[4]->name), (sfxList[4]->channelName));
	}
	
	if (key == GLFW_KEY_6 && action == GLFW_PRESS)
	{
		if (sfxList.size() > 5)
			fmodHandler.PlaySound((sfxList[5]->name), (sfxList[5]->channelName));
	}
	
	if (key == GLFW_KEY_7 && action == GLFW_PRESS)
	{
		if (sfxList.size() > 6)
			fmodHandler.PlaySound((sfxList[6]->name), (sfxList[6]->channelName));
	}
	
	if (key == GLFW_KEY_8 && action == GLFW_PRESS)
	{
		if (sfxList.size() > 7)
			fmodHandler.PlaySound((sfxList[7]->name), (sfxList[7]->channelName));
	}
	
	if (key == GLFW_KEY_9 && action == GLFW_PRESS)
	{
		if (sfxList.size() > 8)
			fmodHandler.PlaySound((sfxList[8]->name), (sfxList[8]->channelName));
	}
}

int main()
{
	std::cout << "\nLoad uncompressed sounds: Press 1";
	std::cout << "\nLoad compressed sounds: Press 2";
	int choice;
	std::cout << "\nChoice: ";
	std::cin >> choice;
	loadSounds("./sounds/", choice);
	glfwInit();
	window = glfwCreateWindow(1920, 1080, "Media", nullptr, nullptr);

	if (!window)
	{
		return 1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);


	glfwSetKeyCallback(window, key_callback);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& imGuiIO = ImGui::GetIO();

	if (!ImGui_ImplGlfw_InitForOpenGL(window, true) || !ImGui_ImplOpenGL3_Init("#version 460"))
	{
		return -5;
	}

	ImGui::StyleColorsDark();

	if (!fmodHandler.Initialize(255, FMOD_INIT_NORMAL))
	{
		return -1;
	}

	printf("Initialize successful");

	if (!fmodHandler.CreateChannelGroup("master"))
	{
		return -2;
	}
	if (fmodHandler.CreateChannelGroup("surface"))
	{
		fmodHandler.SetChannelGroupParent("surface", "master");
	}
	
	if (fmodHandler.CreateChannelGroup("underground"))
	{
		fmodHandler.SetChannelGroupParent("underground", "master");
	}
	
	if (fmodHandler.CreateChannelGroup("water"))
	{
		fmodHandler.SetChannelGroupParent("water", "master");
	}
	
	if (fmodHandler.CreateChannelGroup("sfx"))
	{
		fmodHandler.SetChannelGroupParent("sfx", "master");
	}

	for (int i = 0; i < soundsList.size(); i++)
	{
		std::string path = "";
		if(soundsList[i]->soundFileType == MUSIC)
			path = "./sounds/music/";
		else
		{
			path = "./sounds/sfx/";
			sfxList.push_back(soundsList[i]);
		}

		if (!fmodHandler.CreateSound(soundsList[i]->name, path + soundsList[i]->name, FMOD_DEFAULT))
		{
			return -3;
		}
		if (soundsList[i]->soundFileType == MUSIC)
		{
			fmodHandler.LoopSound(soundsList[i]->name);
			fmodHandler.PlaySound(soundsList[i]->name, soundsList[i]->channelName);
			fmodHandler.PlayPauseSound(soundsList[i]->channelName, soundsList[i]);
		}
	}
	
	/*for (int i = 0; i < sfxList.size(); i++)
	{
		std::string path = "./sounds/sfx/";

		if (!fmodHandler.CreateSound(sfxList[i]->name, path + sfxList[i]->name, FMOD_DEFAULT))
		{
			return -3;
		}
	}*/

	if (!fmodHandler.CreateDsp("echo", FMOD_DSP_TYPE_ECHO, 500.f)
		|| !fmodHandler.CreateDsp("fader", FMOD_DSP_TYPE_FADER, 0.0f)
		|| !fmodHandler.CreateDsp("chorus", FMOD_DSP_TYPE_CHORUS, 50.f)
		|| !fmodHandler.CreateDsp("flange", FMOD_DSP_TYPE_FLANGE, 50.f)
		|| !fmodHandler.CreateDsp("highpass", FMOD_DSP_TYPE_HIGHPASS, 5000)
		|| !fmodHandler.CreateDsp("limiter", FMOD_DSP_TYPE_LIMITER, 10.f)
		|| !fmodHandler.CreateDsp("lowpass", FMOD_DSP_TYPE_LOWPASS, 5000)
		|| !fmodHandler.CreateDsp("oscillator", FMOD_DSP_TYPE_OSCILLATOR, 0)
		|| !fmodHandler.CreateDsp("normalize", FMOD_DSP_TYPE_NORMALIZE, 5000))
	{
		std::cout << "\nFailed to create dsps";
	}

	SoundUI soundUI(&fmodHandler);

	std::string inputs = "Press left ctrl to switch songs";
	inputs += "\nUse space key to play / pause sound";
	inputs += "\nUse Up and Down arrow to control volume";
	inputs += "\nUse left and right arrow to control pitch";
	inputs += "\nUse Page up and Page down keys to control balance (pan)";
	inputs += "\nUse Home and End keys to control speed";
	inputs += "\nUse Dsp panel to control dsp effects";
	std::cout << inputs << std::endl;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//ImGui::ShowDemoWindow();
		soundUI.Render(&soundsList, selectedSound);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	fmodHandler.Shutdown();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();


	return 0;
}