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
#include <filesystem>

namespace fs = std::filesystem;

// Helper function to sanitize a filename.
std::string SanitizeFilename(const std::string& filename) {
    std::string sanitizedName = filename;

    // Replace problematic characters, e.g., remove spaces or non-alphanumeric characters.
    for (char& c : sanitizedName) {
        if (!isalnum(c)) {
            c = '_'; // Replace with an underscore or another valid character.
        }
    }

    return sanitizedName;
}

ModuleImGUI::ModuleImGUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleImGUI::~ModuleImGUI()
{}

bool ModuleImGUI::Init()
{
	LOG("EDITOR: Initializing ImGui Module -------", NULL);

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

	// Set ImGui style colors
	style.Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 0.9f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.92f, 0.18f, 0.29f, 0.50f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.12f, 0.17f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.92f, 0.18f, 0.29f, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.3922f, 0.3922f, 0.3922f, 1.00f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.92f, 0.18f, 0.29f, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.92f, 0.18f, 0.29f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.92f, 0.18f, 0.29f, 0.43f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.9f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.09f, 0.15f, 0.16f, 0.7f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.09f, 0.15f, 0.16f, 0.7f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.09f, 0.15f, 0.16f, 0.7f);

	style.WindowMinSize = ImVec2(160, 20);
	style.FramePadding = ImVec2(5, 5);
	style.ItemSpacing = ImVec2(15, 10);
	style.ItemInnerSpacing = ImVec2(6, 4);
	style.Alpha = 1.0f;
	style.WindowRounding = 12.0f;
	style.WindowPadding = ImVec2(12, 10);
	style.FrameRounding = 5.0f;
	style.IndentSpacing = 6.0f;
	style.ItemInnerSpacing = ImVec2(2, 4);
	style.ItemSpacing = ImVec2(8, 10);
	style.ColumnsMinSpacing = 50.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 16.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 16.0f;

	// Set ImGui custom font
	io.Fonts->AddFontFromFileTTF("LatinModernMono_bold.ttf", 18);
	//io.Fonts->AddFontFromFileTTF("Roboto-Black.ttf", 14);

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
			if (ImGui::MenuItem("New", "(Ctrl + N)", false, false)) {}
			if (ImGui::MenuItem("Open", "(Ctrl + O)", false, false)) {}

			ImGui::Separator();
			
			if (ImGui::MenuItem("Save", "(Ctrl + S)", false, false)) {}
			if (ImGui::MenuItem("Save As", "(Ctrl + LShift + S)", false, false)) {}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "(Ctrl + Z)", false, false)) {}
			if (ImGui::MenuItem("Redo", "(Ctrl + Y)", false, false)) {}

			ImGui::Separator();
			
			if (ImGui::MenuItem("Copy", "(Ctrl + C)", false, false)) {}
			if (ImGui::MenuItem("Paste", "(Ctrl + V)", false, false)) {}
			if (ImGui::MenuItem("Cut", "(Ctrl + X)", false, false)) {}
			
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Configuration")) 
			{
				LOG("EDITOR: Opening 'Configuration' window", NULL);
				configWindow = !configWindow;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Assets")) 
			{
				LOG("EDITOR: Opening 'Assets' window", NULL);
				assetsWindow = !assetsWindow;
			}
			if (ImGui::MenuItem("Camera Inspector"))
			{
				LOG("EDITOR: Opening 'Camera Inspector' window", NULL);
				cameraInspectorWindow = !cameraInspectorWindow;
			}
			if (ImGui::MenuItem("Hierarchy"))
			{
				LOG("EDITOR: Opening 'Hierarchy' window", NULL);
				hierarchyWindow = !hierarchyWindow;
			}
			if (ImGui::MenuItem("ImGui Console Log"))
			{
				LOG("EDITOR: Opening 'ImGui Console Log' window", NULL);
				showDebugLogWindow = !showDebugLogWindow;
			}
			if (ImGui::MenuItem("Inspector"))
			{
				LOG("EDITOR: Opening 'Inspector' window", NULL);
				inspectorWindow = !inspectorWindow;
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Game Objects"))
		{
			GeneratePrimitives();

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About")) 
			{
				LOG("EDITOR: Opening 'About' window", NULL);
				aboutWindow = !aboutWindow;
			}
			if (ImGui::MenuItem("Visit our GitHub Page!"))
			{
				App->OpenWebLink("https://github.com/CITM-UPC/VertX-Game-Engine");
			}
			if (ImGui::MenuItem("Report Bug / Suggestion"))
			{
				App->OpenWebLink("https://github.com/CITM-UPC/VertX-Game-Engine/issues");
			}
			if (ImGui::MenuItem("Download Latest Version"))
			{
				App->OpenWebLink("https://github.com/CITM-UPC/VertX-Game-Engine/releases");
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Quit"))
		{
			if (ImGui::MenuItem("Exit", "(Alt + F4)"))
				return UPDATE_STOP;

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	
	// Render the Config window
	RenderImGUIConfigWindow();

	// Render the About window
	RenderImGUIAboutWindow();

	// Render the Camera Inspector window
	RenderImGUICameraInspectorWindow();

	// Render the Inspector window
	RenderImGUIInspectorWindow();

	// Render the ImGui Debug Log window
	RenderImGUIDebugLogWindow();
	
	// Render the Assets window
	RenderImGUIAssetsWindow();

	// Render Hierarchy window
	RenderImGUIHierarchyWindow();

	// Render Console window
	RenderImGUIConsoleWindow();

	/*ImGui::ShowDemoWindow();*/


	return UPDATE_CONTINUE;
}

bool ModuleImGUI::CleanUp()
{
	LOG("EDITOR: Destroying ImGUI----", NULL);

	App->ClearConsoleLogs();

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
	/* FPS GRAPH */
	static float fps_values[100] = {}; // Store FPS values
	static int fps_index = 0;
	
	if(App->game_engine->renderer3D_engine->vsync)
		fps_values[fps_index] = ImGui::GetIO().Framerate; 
	else
		fps_values[fps_index] = (ImGui::GetIO().Framerate / 2);	 // Idk why but this function detects the frame rate x2, while the delta time is correct 

	fps_index = (fps_index + 1) % 100;
	// Plot FPS graph
	ImGui::PlotHistogram("", fps_values, 100, fps_index, "FPS", 0.0f, 175.0f, ImVec2(300, 100));
	
	/* DT GRAPH */
	static float dt_values[100] = {}; // Store dt values
	static int dt_index = 0;
	if(App->game_engine->renderer3D_engine->vsync)
		dt_values[dt_index] = (1000/ ImGui::GetIO().Framerate);
	else
		dt_values[dt_index] = App->frameDuration;

	dt_index = (dt_index + 1) % 100;
	// Plot Delta Time graph
	ImGui::PlotHistogram("", dt_values, 100, dt_index, "Delta Time (ms)", 0.0f, 100.0f, ImVec2(300, 100));
}


void ModuleImGUI::RenderImGUIAboutWindow()
{
	if (aboutWindow) {
		ImGui::SetNextWindowSize(ImVec2(600, 545));
		ImGui::Begin("About", &aboutWindow);

		ImGui::Text("VertX Game Engine v0.1\n");
		ImGui::Text("Made by Rylan Graham & Adria Pons\n");

		ImGui::Text("\nThese are the 3rd party libraries used to make this project:");
		ImGui::BulletText("SDL 2.28.4");
		ImGui::BulletText("OpenGL 3.3.0");
		ImGui::BulletText("Glew 2.2.0");
		ImGui::BulletText("GLM 0.9.8");
		ImGui::BulletText("Dear ImGui 1.89.9");
		ImGui::BulletText("Assimp 5.3.1");
		ImGui::BulletText("DevIL 1.8.0");
		ImGui::BulletText("JsonCPP 1.9.5");

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
		ImGui::Begin("Configuration", &configWindow);

		if (ImGui::CollapsingHeader("Application"))
		{
			ImGui::SeparatorText("Application name:");
			ImGui::TextColored(ImVec4(1, 1, 0, 1), TITLE);
			
			ImGui::SeparatorText("Organization:");
			ImGui::TextColored(ImVec4(0, 1, 1, 1), "UPC CITM");

			ImGui::SeparatorText("Developers:");
			ImGui::TextColored(ImVec4(1, 0, 1, 1), "Adria Pons & Rylan Graham");

		}

		if (ImGui::CollapsingHeader("Window"))
		{
			// Window parameters sliders
			if (ImGui::Checkbox("Fullscreen", &App->window->fullscreenEnabled))
				LOG("EDITOR: Fullscreen option is [ %s ]", App->window->fullscreenEnabled ? "ON" : "OFF");
			ImGui::SameLine();
			if(ImGui::Checkbox("Fullcreen Desktop", &App->window->fullcreenDesktopEnabled))
				LOG("EDITOR: Fullscreen Desktop option is [ %s ]", App->window->fullcreenDesktopEnabled ? "ON" : "OFF");
			
			if(ImGui::Checkbox("Borderless", &App->window->borderlessEnabled))
				LOG("EDITOR: Borderless option is [ %s ]", App->window->borderlessEnabled ? "ON" : "OFF");
			ImGui::SameLine();
			if(ImGui::Checkbox("Resizeable", &App->window->resizableEnabled))
				LOG("EDITOR: Resizeable option is [ %s ]", App->window->resizableEnabled ? "ON" : "OFF");

			// Vsync toggle checkbox
			if (ImGui::Checkbox("Vertical Syncronization", &App->game_engine->renderer3D_engine->vsync))
				LOG("EDITOR: VSync is [ %s ]", App->game_engine->renderer3D_engine->vsync ? "ON" : "OFF");

			ImGui::Separator();

			// Window Size Sliders (width)
			if (ImGui::SliderInt("Window width", &App->game_engine->renderer3D_engine->screen_width, 1, 1600))
			{
				SDL_SetWindowSize(App->window->window, App->game_engine->renderer3D_engine->screen_width, App->game_engine->renderer3D_engine->screen_height);
			}
			ToolTipMessage("CTRL+Click to input a value");

			// Window Size Sliders (height)
			if (ImGui::SliderInt("Window height", &App->game_engine->renderer3D_engine->screen_height, 1, 900))
			{
				SDL_SetWindowSize(App->window->window, App->game_engine->renderer3D_engine->screen_width, App->game_engine->renderer3D_engine->screen_height);
			}
			ToolTipMessage("CTRL+Click to input a value");

			// Brightness slider
			if (ImGui::SliderFloat("Window Brightness", &App->window->windowBrightness, 0.0f, 1.0f))
			{
				SDL_SetWindowBrightness(App->window->window, App->window->windowBrightness);
			}
			ToolTipMessage("CTRL+Click to input a value");

			ImGui::Separator();

			// FPS slider
			if (!App->game_engine->renderer3D_engine->vsync)
			{
				ImGui::SliderInt("Frame Rate cap", &App->fps, 1, 144);
				ToolTipMessage("CTRL+Click to input a value");
			}

			// FPS graph
			RenderFPSGraph();
		}
		
		if (ImGui::CollapsingHeader("Renderer"))
		{
			ImGui::BulletText("OpenGL Renderer Parameters:");

			if (ImGui::Checkbox("Depth Test", &App->game_engine->renderer3D_engine->glDepthTestIsEnabled))
			{
				LOG("ENGINE: 'Depth Test' render option is [ %s ]", App->game_engine->renderer3D_engine->glDepthTestIsEnabled ? "ON" : "OFF");
			}
			if (ImGui::Checkbox("Cull Face", &App->game_engine->renderer3D_engine->glCullFaceIsEnabled))
			{
				LOG("ENGINE: 'Cull Face' render option is [ %s ]", App->game_engine->renderer3D_engine->glCullFaceIsEnabled ? "ON" : "OFF");
			}
			if (ImGui::Checkbox("Color Material", &App->game_engine->renderer3D_engine->glColorMaterialIsEnabled))
			{
				LOG("ENGINE: 'Color Material' render option is [ %s ]", App->game_engine->renderer3D_engine->glColorMaterialIsEnabled ? "ON" : "OFF");
			}
			if (ImGui::Checkbox("Lighting", &App->game_engine->renderer3D_engine->glLightingIsEnabled)) 
			{
				LOG("ENGINE: 'Lighting' render option is [ %s ]", App->game_engine->renderer3D_engine->glLightingIsEnabled ? "ON" : "OFF");
			}
		}

		if (ImGui::CollapsingHeader("Hardware"))
		{
			/* CPU Info*/
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

			/* GPU Info */
			const GLubyte* gpuName = glGetString(GL_RENDERER);
			ImGui::Text("GPU Name: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 0.75), "%s", gpuName);
			
			const GLubyte* gpuVendor = glGetString(GL_VENDOR);
			ImGui::Text("GPU Vendor: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 0.75), "%s", gpuVendor);

		}

		ImGui::End();
	}
}

void ModuleImGUI::RenderImGUICameraInspectorWindow()
{
	if (cameraInspectorWindow)
	{
		// Create Camera Inspector window
		ImGui::Begin("Camera Inspector", &cameraInspectorWindow);

		// Camera Speed slider
		ImGui::Text("Camera Speed: ");
		float cameraSpeedChanger = App->game_engine->camera.cameraSpeed;
		if (ImGui::SliderFloat("", &cameraSpeedChanger, 0.01, 2.0f, "%.2f"))
		{
			App->game_engine->camera.cameraSpeed = cameraSpeedChanger;
		}
		ToolTipMessage("CTRL+Click to input a value");

		// Camera Speed Multiplier slider
		ImGui::Text("Camera Speed Multiplier: ");
		ImGui::SliderFloat("\n", &App->game_engine->camera.cameraSpeedMultiplier, 1.0f, 5.0f, "%.2f");
		ToolTipMessage("CTRL+Click to input a value");

		//--------------------------------------------

		// Camera Mouse Sensitivity slider
		ImGui::Separator();
		ImGui::Text("Camera Mouse Sensitivity: ");
		ImGui::SliderFloat(" \n", &App->game_engine->camera.mouseSensitivity, 0.01f, 1.0f, "%.2f");
		ToolTipMessage("CTRL+Click to input a value");

		ImGui::SeparatorText("Mouse Parameters");
		ImGui::BulletText("X Position: %d", App->input->GetMouseX());	ImGui::SameLine();	ImGui::BulletText("X Motion: %d", App->input->GetMouseXMotion());
		ImGui::BulletText("Y Position: %d", App->input->GetMouseY());	ImGui::SameLine();	ImGui::BulletText("Y Motion: %d", App->input->GetMouseYMotion());
		
		//--------------------------------------------

		ImGui::Separator();
		ImGui::SeparatorText("Camera Parameters");
		ImGui::PushItemWidth(60.0f);	// Make components text width shorter

		// Camera World Position X 
		ImGui::BulletText("Position vector:");
		float p1 = App->game_engine->camera.worldPosVec.x;
		ImGui::InputFloat("##CamPosX", &p1);
		App->game_engine->camera.worldPosVec.x = p1;
		ToolTipMessage("Click to input a value");

		ImGui::SameLine();

		// Camera World Position Y
		float p2 = App->game_engine->camera.worldPosVec.y;
		ImGui::InputFloat("##CamPosY", &p2);
		App->game_engine->camera.worldPosVec.y = p2;
		ToolTipMessage("Click to input a value");

		ImGui::SameLine();

		// Camera World Position Z
		float p3 = App->game_engine->camera.worldPosVec.z;
		ImGui::InputFloat("##CamPosZ", &p3);
		App->game_engine->camera.worldPosVec.z = p3;
		ToolTipMessage("Click to input a value");

		//--------------------------------------------

		// Camera Focus Point X 
		ImGui::BulletText("Reference vector:");
		float r1 = App->game_engine->camera.focusPosVec.x;
		ImGui::InputFloat("##CamFocusX", &r1);
		App->game_engine->camera.focusPosVec.x = r1;
		ToolTipMessage("Click to input a value");

		ImGui::SameLine();

		// Camera Focus Point Y 
		float r2 = App->game_engine->camera.focusPosVec.y;
		ImGui::InputFloat("##CamFocusY", &r2);
		App->game_engine->camera.focusPosVec.y = r2;
		ToolTipMessage("Click to input a value");

		ImGui::SameLine();

		// Camera Focus Point Z
		float r3 = App->game_engine->camera.focusPosVec.z;
		ImGui::InputFloat("##CamFocusZ", &r3);
		App->game_engine->camera.focusPosVec.z = r3;
		ToolTipMessage("Click to input a value");

		//--------------------------------------------

		// Camera Up Vector X
		ImGui::BulletText("Up vector :");
		float u1 = App->game_engine->camera.upVec.x;
		ImGui::InputFloat("##CamUpX", &u1);
		App->game_engine->camera.upVec.x = u1;
		ToolTipMessage("Click to input a value");

		ImGui::SameLine();

		// Camera Focus Point Y 
		float u2 = App->game_engine->camera.upVec.y;
		ImGui::InputFloat("##CamUpY", &u2);
		App->game_engine->camera.upVec.y = u2;
		ToolTipMessage("Click to input a value");

		ImGui::SameLine();

		// Camera Focus Point Z
		float u3 = App->game_engine->camera.upVec.z;
		ImGui::InputFloat("##CamUpZ", &u3);
		App->game_engine->camera.upVec.z = u3;
		ToolTipMessage("Click to input a value");

		//--------------------------------------------

		ImGui::PopItemWidth();


		// Button to reset camera to initial position
		if (ImGui::Button("RESET CAMERA PARAMETERS", ImVec2(175, 25)))
			App->game_engine->camera.ResetCameraParameters();
	}
}


void ModuleImGUI::RenderImGUIAssetsWindow()
{
	if (assetsWindow)
	{
		// Create Assets window
		ImGui::Begin("Assets", &assetsWindow);

		int assetsPerRow = 6.0f;
		float assetWidth = 100.0f;
		float assetHeight = 10.0f;
		std::string projectFolderPath = "Assets";
		float buttonPadding = 20.0f;

		// Use std::filesystem (C++17 and later) or std::experimental::filesystem (C++14) to list files.
		std::vector<std::string> assetNames;

		for (const auto& entry : std::filesystem::directory_iterator(projectFolderPath)) {
			// Check if the entry is a regular file (you can add more filters as needed).
			if (entry.is_regular_file()) {
				assetNames.push_back(entry.path().filename().string());
			}
		}

		for (size_t i = 0; i < assetNames.size(); i++) {
			if (i % assetsPerRow != 0) {
				ImGui::SameLine();
			}

			ImGui::BeginGroup();

			if (ImGui::BeginPopupContextItem(assetNames[i].c_str())) {
				if (ImGui::MenuItem("Render")) {
					// Handle rendering the asset.
					// You can implement your rendering logic here.
				}
				ImGui::EndPopup();
			}

			if (ImGui::Selectable(assetNames[i].c_str(), false, 0, ImVec2(assetWidth, assetHeight))) {
				// Handle the single-click event for the asset.
				// You can implement your logic here.
			}

			ImGui::EndGroup();
		}

		ImGui::End();
	}
}

void ModuleImGUI::RenderImGUIInspectorWindow()
{
	if (ImGui::Begin("Inspector", &inspectorWindow)) {
		if (gameObjSelected.name != "") {
			ImGui::Checkbox("Active", &gameObjSelected.isActive);
			ImGui::SameLine(); ImGui::Text("Game Object: ");
			ImGui::SameLine(); ImGui::Text(gameObjSelected.name.c_str());

			ImGui::InputText("GO Name", Title, IM_ARRAYSIZE(Title), ImGuiInputTextFlags_EnterReturnsTrue);
			if (ImGui::IsKeyDown(ImGuiKey_Enter)) {
				gameObjSelected.name = Title;
				/*gameObject.name.push_back(Title);*/
			}

			//Grab Components and set in for allows for constant polling 
			//Mesh Menu - Creation of Mesh pointer to component to call Texture methods etc
			for (auto& comp : gameObjSelected.GetComponents()) {
				bool deleteButtonPressed = false;
				if (comp.get()->getType() == Component::Type::TRANSFORM) {
					Transform* transform = dynamic_cast<Transform*>(comp.get());

					ImGui::SetNextItemOpen(true, ImGuiCond_Once);
					if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_None))
					{
						ImGui::PushItemWidth(60.0f);
						ImGui::BulletText("Position");
						ImGui::InputDouble("##PositionX", &transform->position.x, 0.0, 0.0, "%.3f");
						ImGui::SameLine();
						ImGui::InputDouble("##PositionY", &transform->position.y, 0.0, 0.0, "%.3f");
						ImGui::SameLine();
						ImGui::InputDouble("##PositionZ", &transform->position.z, 0.0, 0.0, "%.3f");

						ImGui::BulletText("Rotation");
						ImGui::InputDouble("##RotationX", &transform->rotation.x, 0.0, 0.0, "%.3f");
						ImGui::SameLine();
						ImGui::InputDouble("##RotationY", &transform->rotation.y, 0.0, 0.0, "%.3f");
						ImGui::SameLine();
						ImGui::InputDouble("##RotationZ", &transform->rotation.z, 0.0, 0.0, "%.3f");

						ImGui::BulletText("Scale");
						ImGui::InputDouble("##ScaleX", &transform->scale.x, 0.0, 0.0, "%.3f");
						ImGui::SameLine();
						ImGui::InputDouble("##ScaleY", &transform->scale.y, 0.0, 0.0, "%.3f");
						ImGui::SameLine();
						ImGui::InputDouble("##ScaleZ", &transform->scale.z, 0.0, 0.0, "%.3f");

						ImGui::PopItemWidth();
					}
				}
				//Mesh Menu - Creation of  pointer to component to call Mesh methods etc
				if (comp.get()->getType() == Component::Type::MESH) {
					Mesh* mesh = dynamic_cast<Mesh*>(comp.get());

					ImGui::SetNextItemOpen(true, ImGuiCond_Once);
					if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_None))
					{
						ImGui::GetStyle();
						if(ImGui::Checkbox("Active", &mesh->isActive))
							LOG("ENGINE: '%s' mesh is [ %s ]", mesh->getName().c_str(), mesh->isActive ? "ACTIVE" : "INACTIVE");
						ImGui::SameLine();  
						ImGui::Text("Filename: ");
						ImGui::SameLine();  
						ImGui::Text(mesh->getName().c_str());
						ImGui::Separator();
						ImGui::Text("Indexes: ");
						ImGui::SameLine();
						ImGui::Text(std::to_string(mesh->getNumIndexs()).c_str());
						ImGui::Text("Normals: ");
						ImGui::SameLine();  
						ImGui::Text(std::to_string(mesh->getNumNormals()).c_str());
						ImGui::Text("Vertexs: ");
						ImGui::SameLine();  
						ImGui::Text(std::to_string(mesh->getNumVerts()).c_str());
						ImGui::Text("Faces: ");
						ImGui::SameLine();  
						ImGui::Text(std::to_string(mesh->getNumFaces()).c_str());
						ImGui::Separator();
						if(ImGui::CollapsingHeader("View Options", ImGuiTreeNodeFlags_None)){
							ImGui::Checkbox("View Vertex Normals", &mesh->VertexNormDraw);
							ImGui::Checkbox("View Face Normals", &mesh->FaceNormDraw);
						}
						ImGui::Separator();
						if (ImGui::Checkbox("Use Texture", &mesh->usingTexture))
						{
							LOG("ENGINE: '%s' texture is [ %s ]", mesh->getName().c_str(), mesh->usingTexture ? "ACTIVE" : "INACTIVE");
							(mesh->usingTexture) ? mesh->texture = gameObjSelected.GetComponent<Texture2D>() : mesh->texture = nullptr;
						}
					}
				}
				//Texture Menu - Creation of Texture pointer to component to call Texture methods etc
				if (comp.get()->getType() == Component::Type::TEXTURE) {
					Texture2D* texture2D = dynamic_cast<Texture2D*>(comp.get());

					ImGui::SetNextItemOpen(true, ImGuiCond_Once);
					if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_None))
					{
						ImGui::Text("Texture File: ");
						ImGui::SameLine();
						ImGui::Text(texture2D->getName().c_str());
						ImGui::Text("Texture Height: ");
						ImGui::SameLine();
						ImGui::Text(std::to_string(texture2D->getHeight()).c_str());
						ImGui::Text("Texture Width: ");
						ImGui::SameLine();
						ImGui::Text(std::to_string(texture2D->getWidth()).c_str());
					}
				}
				/*if (ImGui::Button("Delete GameObject")) {
					deleteButtonPressed = true;
				}
				if (deleteButtonPressed) {
					gameObjSelected.~GameObject();
					deleteButtonPressed = false;
				}*/

			}

		}
	}

	ImGui::End();
}

void ModuleImGUI::RenderImGUIDebugLogWindow()
{
	if (showDebugLogWindow)
		ImGui::ShowDebugLogWindow(&showDebugLogWindow);
}

void ModuleImGUI::RenderImGUIHierarchyWindow()
{
	ImGui::Begin("Hierarchy", &hierarchy);

	for (auto& gameObject : App->game_engine->renderer3D_engine->gameObjectList) {

		if (ImGui::MenuItem(gameObject.name.c_str())) {
			gameObjSelected = gameObject;
		}
	}

	ImGui::EndMenu();
}


void ModuleImGUI::GeneratePrimitives()
{
	if (ImGui::BeginMenu("GameObject"))
	{

		if (ImGui::Button("Generate Cube")) {
			LOG("EDITOR: Adding CUBE Primitive", NULL);
			App->game_engine->renderer3D_engine->addGameObject("Assets/Cube.fbx");
		}

		if (ImGui::Button("Generate Plane")) {
			LOG("EDITOR: Adding PLANE Primitive", NULL);
			App->game_engine->renderer3D_engine->addGameObject("Assets/Plane.fbx");
		}

		if (ImGui::Button("Generate Pyramid")) {
			LOG("EDITOR: Adding PYRAMID Primitive", NULL);
			App->game_engine->renderer3D_engine->addGameObject("Assets/Pyramid.fbx");
		}

		if (ImGui::Button("Generate Sphere")) {
			LOG("EDITOR: Adding SPHERE Primitive", NULL);
			App->game_engine->renderer3D_engine->addGameObject("Assets/Sphere.fbx");
		}

		if (ImGui::Button("Generate Cylinder")) {
			LOG("EDITOR: Adding CYLINDER Primitive", NULL);
			App->game_engine->renderer3D_engine->addGameObject("Assets/Cylinder.fbx");
		}

		if (ImGui::Button("Baker House")) {
			LOG("EDITOR: Adding BAKER HOUSE", NULL);
			App->game_engine->renderer3D_engine->addGameObject("Assets/BakerHouse.fbx");
		}

		if (ImGui::Button("Medieval Bed")) {
			LOG("EDITOR: Adding MEDIEVAL BED", NULL);
			App->game_engine->renderer3D_engine->addGameObject("Assets/Medieval_Bed.fbx");
		}

		if (ImGui::Button("Obelisque")) {
			LOG("EDITOR: Adding OBELISQUE", NULL);
			App->game_engine->renderer3D_engine->addGameObject("Assets/Obelisque.fbx");
		}

		ImGui::EndMenu();
	}
}

void ModuleImGUI::RenderImGUIConsoleWindow()
{
	// Buffer to store the search query
	static char filterInput[256] = ""; 

	ImGui::Begin("Console");

	// Display the Clear Logs button
	if (ImGui::Button("Clear Console Logs"))
	{
		App->ClearConsoleLogs();
	}

	ImGui::SameLine();

	// Search bar for filtering logs
	ImGui::InputText("Search", filterInput, IM_ARRAYSIZE(filterInput));

	ImGui::Separator();

	ImGui::BeginChild("Logs Region", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

	std::vector<std::string> logs = App->GetConsoleLogs();

	// Convert the search query to lowercase for case-insensitive comparison
	std::string filterQuery = filterInput;
	std::transform(filterQuery.begin(), filterQuery.end(), filterQuery.begin(), ::tolower);

	for (auto i = logs.begin(); i != logs.end(); ++i)
	{
		// Convert the log text to lowercase for case-insensitive comparison
		std::string logText = *i;
		std::transform(logText.begin(), logText.end(), logText.begin(), ::tolower);

		// If the search string is empty or the log contains the search string, display it
		if (filterQuery.empty() || logText.find(filterQuery) != std::string::npos)
		{
			ImGui::TextUnformatted((*i).c_str());
		}
	}

	// Scroll to the bottom to display the latest logs
	ImGui::SetScrollHereY(1.0f);

	ImGui::EndChild();
	ImGui::End();
}