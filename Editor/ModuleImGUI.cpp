#include "Globals.h"
#include "Application.h"
#include "ModuleImGUI.h"
#include "GL/glew.h"
#include "SDL2/SDL_opengl.h"
#include "ModuleInput.h" 
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "ModuleWindow.h"
#include "SDL2/SDL_cpuinfo.h"

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

	io.DeltaTime = 16;

	// Setup Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->game_engine->renderer3D_engine->context);
	ImGui_ImplOpenGL3_Init();

	// Setup ImGui colors style
	ImGui::StyleColorsClassic();
	//ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	// Set ImGui windows rounding
	style.WindowRounding = 5.0f;
	// Swt ImGui custom font
	io.Fonts->AddFontFromFileTTF("Roboto-Black.ttf", 14);

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
			if (ImGui::MenuItem("Configuration")) 
			{
				configWindow = !configWindow;
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
				App->OpenWebLink("https://github.com/AdriaPm/VertX-Game-Engine");
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


	// Render the Config window from ImGui
	RenderImGUIConfigWindow();

	// Render the About window from ImGui
	RenderImGUIAboutWindow();

	// Render the Inspector window from ImGui
	RenderImGUIInspectorWindow();

	
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

void ModuleImGUI::ToolTipMessage(const char* tip)
{
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip(tip);
	}
}

void ModuleImGUI::RenderFPSGraph()
{
	static float fps_values[100] = {}; // Store FPS values
	static int fps_index = 0;
	fps_values[fps_index] = ImGui::GetIO().Framerate;
	fps_index = (fps_index + 1) % 100;
	ImGui::PlotHistogram("", fps_values, 100, fps_index, "FPS", 0.0f, 100.0f, ImVec2(300, 100));
	//ImGui::PlotHistogram("", fps_values, 100, fps_index, "FPS", 0.0f, 4200.0f, ImVec2(300, 100));
}


void ModuleImGUI::RenderImGUIAboutWindow()
{
	if (aboutWindow) {
		ImGui::SetNextWindowSize(ImVec2(600, 545));
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

void ModuleImGUI::RenderImGUIConfigWindow()
{
	if (configWindow)
	{
		ImGui::Begin("Configuration Window");

		if (ImGui::CollapsingHeader("Application"))
		{
			ImGui::BulletText("Application name:");
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "VertX Game Engine\n");
			
			ImGui::BulletText("Organization:");
			ImGui::TextColored(ImVec4(0, 1, 1, 1), "UPC CITM");

		}

		if (ImGui::CollapsingHeader("Window Settings"))
		{
			// Window parameters sliders
			ImGui::Checkbox("Fullscreen", &App->window->fullscreenEnabled);
			ImGui::SameLine();
			ImGui::Checkbox("Fullcreen Desktop", &App->window->fullcreenDesktopEnabled);
			
			ImGui::Checkbox("Borderless", &App->window->borderlessEnabled);
			ImGui::SameLine();
			ImGui::Checkbox("Resizeable", &App->window->resizableEnabled);

			// Brightness slider
			if (ImGui::SliderFloat("Brightness", &App->window->windowBrightness, 0.0f, 1.0f))
			{
				SDL_SetWindowBrightness(App->window->window, App->window->windowBrightness);
			}
			ToolTipMessage("CTRL+Click to input a value");

			// FPS graph
			RenderFPSGraph();
		}
		
		if (ImGui::CollapsingHeader("Hardware"))
		{
			// Display System's CPU cores
			ImGui::Text("CPUs: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 0.75), "%d cores", SDL_GetCPUCount());

			ImGui::Text("CPU Cache Line Size: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 0.75), "%d bytes", SDL_GetCPUCacheLineSize());

			// Display System's RAM
			ImGui::Text("System RAM: ");
			ImGui::SameLine();
			float systemRAM = (SDL_GetSystemRAM() / 1000);	// Get system RAM (in gb)
			ImGui::TextColored(ImVec4(1, 1, 0, 0.75), "%.2f GB", systemRAM);
			
			ImGui::Separator();

			

		}

		ImGui::End();
	}
}

void ModuleImGUI::RenderImGUIInspectorWindow()
{
	// Create Inspector window
	ImGui::Begin("Inspector");

	// Camera Speed slider
	ImGui::Text("Camera Speed: ");
	float cameraSpeedChanger = App->renderer->cameraSpeed;
	if (ImGui::SliderFloat("", &cameraSpeedChanger, 0.01, 2.0f, "%.2f"))
	{
		App->renderer->cameraSpeed = cameraSpeedChanger;
	}
	ToolTipMessage("CTRL+Click to input a value");

	// Camera Speed Multiplier slider
	ImGui::Text("Camera Speed Multiplier: ");
	ImGui::SliderFloat("\n", &App->renderer->cameraSpeedMultiplier, 1.0f, 5.0f, "%.2f");
	ToolTipMessage("CTRL+Click to input a value");

	ImGui::End();
}