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

std::vector<S_Audio*> soundsList;
GLFWwindow* window;
FModHandler fmodHandler;
int selectedSound = 0;
S_Audio* sAudio;

void loadSounds(std::string startingPath, int choice)
{
	std::ifstream soundListFile;
	soundListFile = std::ifstream(SOUND_LIST_FILE);
	/*if(choice == 1)
	else
		soundListFile = std::ifstream(COMP_SOUND_LIST_FILE);*/
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
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
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
			fmodHandler.SetChannelGroupPan(sAudio->channelName, sAudio->volume);
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
			fmodHandler.SetChannelGroupPan(sAudio->channelName, sAudio->volume);
		}
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		if (!soundsList[selectedSound]->playing)
		{
			fmodHandler.PlaySound((soundsList[selectedSound]->name), (soundsList[selectedSound]->channelName));
			soundsList[selectedSound]->playing = true;
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

	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		fmodHandler.AddDspEffect("surface", "echo");
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		fmodHandler.AddDspEffect("surface", "delay");
	}

	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		fmodHandler.AddDspEffect("surface", "distortion");
	}

	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		fmodHandler.AddDspEffect("underground", "chorus");
	}
	
	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		fmodHandler.AddDspEffect("underground", "fader");
	}
	
	if (key == GLFW_KEY_6 && action == GLFW_PRESS)
	{
		fmodHandler.AddDspEffect("underground", "normalize");
	}
	
	if (key == GLFW_KEY_7 && action == GLFW_PRESS)
	{
		fmodHandler.AddDspEffect("water", "oscillator");
	}
	
	if (key == GLFW_KEY_8 && action == GLFW_PRESS)
	{
		fmodHandler.AddDspEffect("water", "flange");
	}
	
	if (key == GLFW_KEY_9 && action == GLFW_PRESS)
	{
		fmodHandler.AddDspEffect("water", "limiter");
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		fmodHandler.RemoveDspEffect("surface", "echo");
		fmodHandler.RemoveDspEffect("surface", "delay");
		fmodHandler.RemoveDspEffect("surface", "distortion");
		fmodHandler.RemoveDspEffect("underground", "chorus");
		fmodHandler.RemoveDspEffect("underground", "fader");
		fmodHandler.RemoveDspEffect("underground", "normalize");
		fmodHandler.RemoveDspEffect("water", "oscillator");
		fmodHandler.RemoveDspEffect("water", "flange");
		fmodHandler.RemoveDspEffect("water", "limiter");
	}
}

int main()
{
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

	std::cout << "\nLoad uncompressed sounds: Press 1";
	std::cout << "\nLoad compressed sounds: Press 2";
	int choice;
	std::cout << "\nChoice: ";
	std::cin >> choice;
	loadSounds("./sounds/", choice);

	if (!fmodHandler.CreateChannelGroup("master"))
	{
		return -2;
	}

	for (int i = 0; i < soundsList.size(); i++)
	{
		if (!fmodHandler.CreateChannelGroup(soundsList[i]->channelName))
		{
			std::cout << "Channel group already exists!" << std::endl;
		}
		else {
			bool b = fmodHandler.SetChannelGroupParent(soundsList[i]->channelName, "master");
		}

		std::string path = "";
		if (soundsList[i]->soundFileType == MUSIC)
			path = "./sounds/music/";
		else path = "./sounds/sfx/";

		if (choice == 1)
		{
			if (!fmodHandler.CreateSound(soundsList[i]->name, path + soundsList[i]->name, FMOD_DEFAULT))
			{
				return -3;
			}
		}
		else 
		{
			if(!fmodHandler.CreateSound(soundsList[i]->name, path + soundsList[i]->name, FMOD_CREATECOMPRESSEDSAMPLE))
				return -3;
		}

		if (soundsList[i]->soundFileType == MUSIC)
			fmodHandler.LoopSound(soundsList[i]->name);
	}

	if (!fmodHandler.CreateDsp("echo", FMOD_DSP_TYPE_ECHO, 500.f)
		|| !fmodHandler.CreateDsp("delay", FMOD_DSP_TYPE_DELAY, FMOD_DSP_DELAY_MAXDELAY)
		|| !fmodHandler.CreateDsp("distortion", FMOD_DSP_TYPE_DISTORTION, FMOD_DSP_DISTORTION_LEVEL)
		|| !fmodHandler.CreateDsp("chorus", FMOD_DSP_TYPE_CHORUS, 90.f)
		|| !fmodHandler.CreateDsp("fader", FMOD_DSP_TYPE_FADER, -50.0f)
		|| !fmodHandler.CreateDsp("normalize", FMOD_DSP_TYPE_NORMALIZE, 0.55f)
		|| !fmodHandler.CreateDsp("oscillator", FMOD_DSP_TYPE_OSCILLATOR, FMOD_DSP_OSCILLATOR_TYPE)
		|| !fmodHandler.CreateDsp("flange", FMOD_DSP_TYPE_FLANGE, 0.28f)
		|| !fmodHandler.CreateDsp("limiter", FMOD_DSP_TYPE_LIMITER, -5.f))
	{
		std::cout << "\nFailed to create dsps";
	}

	SoundUI soundUI(&fmodHandler);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

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