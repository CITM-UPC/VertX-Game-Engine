#include "Globals.h"
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
	LOG("Initializing ImGui Module");

	// Setup ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); 

	// Enable keyboard
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// Enable gamepad
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	// Enable docking
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Setup Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->game_engine->renderer3D_engine->context);
	ImGui_ImplOpenGL3_Init();

	// Setup ImGui colors style
	ImGui::StyleColorsClassic();
	//ImGui::StyleColorsDark();

	return true;
}

update_status ModuleImGUI::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	
	// Enabling dockspace of ImGui windows
	ImGuiDockNodeFlags flag = 0;
	flag |= ImGuiDockNodeFlags_PassthruCentralNode;
	ImGui::DockSpaceOverViewport(0, flag);

	
	
		
	

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New (Ctrl + N)", "Not implemented")) {}
			if (ImGui::MenuItem("Open (Ctrl + O)", "Not implemented")) {}

			ImGui::Separator();
			
			if (ImGui::MenuItem("Save (Ctrl + S)", "Not implemented")) {}
			if (ImGui::MenuItem("Save As (Ctrl + Alt + S)", "Not implemented")) {}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo (Ctrl + Z)", "Not implemented")) {}
			if (ImGui::MenuItem("Redo (Ctrl + Y)", "Not implemented")) {}

			ImGui::Separator();
			
			if (ImGui::MenuItem("Copy (Ctrl + C)", "Not implemented")) {}
			if (ImGui::MenuItem("Paste (Ctrl + V)", "Not implemented")) {}
			if (ImGui::MenuItem("Cut (Ctrl + X)", "Not implemented")) {}
			
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Configure Window")) {
				optionsWindow = !optionsWindow;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About")) 
			{
				aboutWindow = !aboutWindow;
			}
			if (ImGui::MenuItem("Visit GitHub Page"))
			{
				ShellExecute(NULL, "open", "https://github.com/AdriaPm/VertX-Game-Engine", 0, 0, SW_SHOWDEFAULT);
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Quit"))
		{
			if (ImGui::MenuItem("Close the VertX Game Engine"))
				return UPDATE_STOP;

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	

	if (optionsWindow)
	{
		ImGui::Begin("Options Window");

		ImGui::Text("This window is a placeholder.\nFunctionality is WIP");
		static bool testBool = false;
		if (ImGui::Checkbox("VSYNC", &testBool)) { LOG("Checkbox Pressed"); };
		
		ImGui::End();
	}
	
	/*if (demoWindow)
		ImGui::ShowDemoWindow();*/

	
	RenderImGUIAboutWindow();

	// Create Inspector window
	ImGui::Begin("Inspector");
	

	ImGui::End();

	
	// Create Assets window
	ImGui::Begin("Assets");

	ImGui::End();


	// Create Hierarchy window

	ImGui::Begin("Hierarchy");

	ImGui::End();



	return UPDATE_CONTINUE;
}

bool ModuleImGUI::CleanUp()
{
	LOG("Destroying ImGUI");

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

void ModuleImGUI::RenderImGUIAboutWindow()
{
	if (aboutWindow) {
		ImGui::SetNextWindowSize(ImVec2(600, 525));
		ImGui::Begin("About");

		ImGui::Text("VertX Game Engine v0.1\n");
		ImGui::Text("Made by Rylan Graham & Adria Pons\n");

		ImGui::Text("\nThese are the 3rd party libraries used to make this project:");
		ImGui::BulletText("SDL 2.x");
		ImGui::BulletText("OpenGL 3.x");
		ImGui::BulletText("Glew X.x");
		ImGui::BulletText("GLM X.x");
		ImGui::BulletText("Dear ImGui X.x");
		ImGui::BulletText("Assimp X.x");
		ImGui::BulletText("DevIL X.x");
		ImGui::BulletText("JsonCPP X.x");

		ImGui::Text("\nLICENCE:\n");

		ImGui::Text("MIT Licence\n");

		ImGui::Text("Copyright (c) 2023 Adria Pons & Rylan Graham - VertX Game Engine\n");
		ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy\nof this software and associated documentation files (the 'Software'), to deal\nin the Software without restriction, including without limitation the rights\nto use, copy, modify, merge, publish, distribute, sublicense, and/or sell\ncopies of the Software, and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions:\n\n");
		ImGui::Text("The above copyright notice and this permission notice shall be included in all\ncopies or substantial portions of the Software.");
		ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE.\n");

		ImGui::End();
	}
}