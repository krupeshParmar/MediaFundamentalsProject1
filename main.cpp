#include <FMOD/fmod.h>
#include "FModHandler.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include "UI/SoundUI/SoundUI.h"

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


	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& imGuiIO = ImGui::GetIO();

	if (!ImGui_ImplGlfw_InitForOpenGL(window, true) || !ImGui_ImplOpenGL3_Init("#version 460"))
	{
		return -5;
	}

	ImGui::StyleColorsDark();

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

	SoundUI soundUI(&fmodHandler);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		soundUI.Render();

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