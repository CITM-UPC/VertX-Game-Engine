﻿#include "Globals.h"
#include "Application.h"
#include "ModuleImGUI.h"
#include "GL/glew.h"
#include "SDL2/SDL_opengl.h"
#include "ModuleInput.h" 
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

ModuleImGUI::ModuleImGUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleImGUI::~ModuleImGUI()
{}

bool ModuleImGUI::Init()
{
	LOG("Creating UI");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->gEngine->renderer3D_engine->context);
	ImGui_ImplOpenGL3_Init();

	return true;
}

update_status ModuleImGUI::PreUpdate()
{
	//This here does not work. Currently in Input.cpp
	//ImGui_ImplSDL2_ProcessEvent(&pollevent); 

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	#pragma region	ImGui_MenuBar_Test

	if (dockSpaceEnabled)
	{
		ImGuiDockNodeFlags dock_flags = 0;
		dock_flags |= ImGuiDockNodeFlags_PassthruCentralNode;
		ImGui::DockSpaceOverViewport(0, dock_flags);
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			//ImGui::PlotHistogram();
			if (ImGui::MenuItem("New", "Not implemented")) {}
			if (ImGui::MenuItem("Open", "Not implemented")) {}
			if (ImGui::BeginMenu("Open Recent", "Not implemented"))
			{
				ImGui::MenuItem("Example1.example");
				ImGui::MenuItem("Example1.example");
				ImGui::MenuItem("Example1.example");
				if (ImGui::BeginMenu("More.."))
				{
					ImGui::MenuItem("Example2.example");
					ImGui::MenuItem("Example2.example");
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Save", "Not implemented")) {}
			if (ImGui::MenuItem("Save As..", "Not implemented")) {}
			if (ImGui::MenuItem("Exit")) { return UPDATE_STOP; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Not implemented")) {}
			if (ImGui::MenuItem("Redo", "Not implemented", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "Not implemented")) {}
			if (ImGui::MenuItem("Copy", "Not implemented")) {}
			if (ImGui::MenuItem("Paste", "Not implemented"))
			{
				LOG("Pressed Paste Button");
			}
			if (ImGui::MenuItem("Toggle DockSpace"))
			{
				dockSpaceEnabled = !dockSpaceEnabled;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Config Window")) {
				options = !options;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Show Demo Window", " ")) {
				demoWindow = !demoWindow;
			}
			if (ImGui::MenuItem("About...", " ")) {
				about = !about;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	#pragma endregion

	if (options)
	{
		ImGui::Begin("Options Window");

		ImGui::Text("This window is a placeholder.\nFunctionality is WIP");
		static bool testBool = false;
		if (ImGui::Checkbox("VSYNC", &testBool)) { LOG("Checkbox Pressed"); };
		
		ImGui::End();
	}
	if (about) {
		ImGui::Begin("About");
		
		ImGui::Text("This Engine of Mine v0.1\nA 3D Game Engine for the Game Engines subject");
		ImGui::Text("By Jonathan Cacay & Ethan Martin\n3rd Party Libraries used :");
		ImGui::Text("LIBRARIES HERE");
		ImGui::Text("License:\nMIT License\nCopyright(c) 2023 Jonathan Cacay & Ethan Martin");
		ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the Software), \nto deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, \nand /or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions : ");
		ImGui::Text("The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.");
		ImGui::Text("THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, \nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER \nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS \nIN THE SOFTWARE.");
		
		ImGui::End();
	}
	if (demoWindow)
		ImGui::ShowDemoWindow(); // Show demo window! :)

	#pragma region ImGui_Windows_Test

	ImGui::Begin("Window A");
	ImGui::Text("This is window A");
	ImGui::Button("Button on window A");
	ImGui::End();

	ImGui::Begin("Window B");
	ImGui::Text("This is window B");
	ImGui::End();

	ImGui::Begin("Window B");
	ImGui::Button("Button on window B");
	ImGui::End();

	ImGui::Begin("Window C");
	ImGui::Text("This is window C");
	if (ImGui::Button("Button on window C")) { LOG("Button on window C pressed"); }
	if (ImGui::Button("Close editor")) { return UPDATE_STOP; }
	ImGui::End();

	#pragma endregion

	return UPDATE_CONTINUE;
}

bool ModuleImGUI::CleanUp()
{
	LOG("Destroying UI");

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleImGUI::RenderImGUI()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}