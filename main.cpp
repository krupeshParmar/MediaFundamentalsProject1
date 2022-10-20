#include <FMOD/fmod.h>
#include "FModHandler.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

GLFWwindow* window;

int main()
{
	glfwInit();
	window = glfwCreateWindow(800, 600, "Media", nullptr, nullptr);

	if (!window)
	{
		return 1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	FModHandler fmodHandler;

	if (!fmodHandler.Initialize(255, FMOD_INIT_NORMAL))
	{
		return -1;
	}

	printf("Initialize successful");

	if (!fmodHandler.CreateChannelGroup("master"))
	{
		return -2;
	}

	if (!fmodHandler.CreateSound("bg", "./sounds/music/bg.wav", FMOD_DEFAULT))
	{
		return -3;
	}

	if (!fmodHandler.PlaySound("bg", "master"))
	{
		return -4;
	}
	ImGui::CreateContext();
	ImGui::Begin("Audio Settings");
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		ImGui::NewFrame();

		glfwSwapBuffers(window);
	}

	fmodHandler.Shutdown();

	glfwTerminate();


	return 0;
}