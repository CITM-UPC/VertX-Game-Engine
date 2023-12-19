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
#include "..\Engine\Mesh.h"
#include "SDL2/SDL_cpuinfo.h"
#include <filesystem>
#include <functional>
#include <Windows.h>
#include <commdlg.h>

namespace fs = std::filesystem;

using DoubleClickCallback = std::function<void(const std::string&)>;

std::string currentFolderPath = "Assets";

struct Asset {
	std::string name;
	bool isFolder;
	bool isSelected;
	bool showDeletePopup;
};

void GoBack() {
	if (currentFolderPath != "Assets") {
		currentFolderPath = std::filesystem::path(currentFolderPath).parent_path().string();
	}
}

void DoubleClickHandler(const std::string& folderName) {
	currentFolderPath = "Assets" + folderName;
}

void ShowFolderContents(const std::string& folderName, std::vector<Asset>& assets) {
	currentFolderPath = currentFolderPath + "/" + folderName;
	assets.clear(); // Clear current asset list
	for (const auto& entry : std::filesystem::directory_iterator(currentFolderPath)) {
		assets.push_back({ entry.path().filename().string(), entry.is_directory() });
	}
}

vec3 ModuleImGUI::GetSelectedObjectPos()
{
	if (gameObjSelected != nullptr)
		return gameObjSelected->GetComponent<Transform>()->position();

	return vec3(0, 0, 0);
}


void ModuleImGUI::SetSelectedObjectTexture(string filePath)
{
	if (gameObjSelected != nullptr) {
		if (gameObjSelected->GetComponent<Texture2D>() != nullptr)
		{
			gameObjSelected->RemoveComponent(Component::Type::TEXTURE2D);
		}

		Texture2D textureToPush(gameObjSelected, filePath);
		gameObjSelected->AddComponent(textureToPush);

		if (gameObjSelected->GetComponent<Mesh>()->texture)
		{
			gameObjSelected->GetComponent<Mesh>()->texture = gameObjSelected->GetComponent<Texture2D>();
		}
	}
}

// Function to open a file dialog and return the selected file path
std::string openFileDialog() {
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = sizeof(szFileName);
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileName(&ofn))
		return ofn.lpstrFile;

	return "";
}

// Function to copy a file to the "Assets/" folder
bool copyFileToAssetsFolder(const std::string& sourcePath) {
	// Adjust this path according to your project structure
	const std::string assetsFolderPath = "Assets/Library/Imports/";

	try {
		// Extract the file name from the source path
		std::string fileName = fs::path(sourcePath).filename().string();

		// Create the destination path
		std::string destinationPath = assetsFolderPath + fileName;

		// Copy the file
		fs::copy_file(sourcePath, destinationPath, fs::copy_options::overwrite_existing);

		std::cout << "File copied to Imports folder: " << destinationPath << std::endl;
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Error copying file: " << e.what() << std::endl;
		return false;
	}
}


// Function to display a popup for deleting an asset
void ShowDeletePopup(bool& deleteAsset, const std::string& assetName) {
	if (ImGui::BeginPopup("DeletePopup")) {
		ImGui::Text("Delete asset: %s", assetName.c_str());
		ImGui::Separator();

		if (ImGui::Button("Delete")) {
			deleteAsset = true; // Set the flag to delete the asset
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

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
			if (ImGui::MenuItem("Configuration", NULL, configWindow, !configWindow))
			{
				LOG("EDITOR: Opening 'Configuration' window...", NULL);
				configWindow = !configWindow;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Assets", NULL, assetsWindow, !assetsWindow)) 
			{
				LOG("EDITOR: Opening 'Assets' window...", NULL);
				assetsWindow = !assetsWindow;
			}
			if (ImGui::MenuItem("Camera Inspector", NULL, cameraInspectorWindow, !cameraInspectorWindow))
			{
				LOG("EDITOR: Opening 'Camera Inspector' window...", NULL);
				cameraInspectorWindow = !cameraInspectorWindow;
			}
			if (ImGui::MenuItem("Console", NULL, consoleWindow, !consoleWindow))
			{
				LOG("EDITOR: Opening 'Console' window...", NULL);
				consoleWindow = !consoleWindow;
			}
			if (ImGui::MenuItem("Hierarchy", NULL, hierarchyWindow, !hierarchyWindow))
			{
				LOG("EDITOR: Opening 'Hierarchy' window...", NULL);
				hierarchyWindow = !hierarchyWindow;
			}
			if (ImGui::MenuItem("ImGui Console Log", NULL, imGuiDebugLogWindow, !imGuiDebugLogWindow))
			{
				LOG("EDITOR: Opening 'ImGui Console Log' window...", NULL);
				imGuiDebugLogWindow = !imGuiDebugLogWindow;
			}
			if (ImGui::MenuItem("Inspector", NULL, inspectorWindow, !inspectorWindow))
			{
				LOG("EDITOR: Opening 'Inspector' window...", NULL);
				inspectorWindow = !inspectorWindow;
			}
			if (ImGui::MenuItem("Simulation Buttons", NULL, simulationButtonsWindow, !simulationButtonsWindow))
			{
				LOG("EDITOR: Opening 'Simulation Buttons' window...", NULL);
				simulationButtonsWindow = !simulationButtonsWindow;
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
			if (ImGui::MenuItem("About", NULL, aboutWindow, !aboutWindow))
			{
				LOG("EDITOR: Opening 'About' window...", NULL);
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
			{
				LOG("--- EXITING VERTX GAME ENGINE ---", NULL);
				return UPDATE_STOP;
			}
				
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

	// Render Simulation Controls Window
	RenderImGUISimulationControlsWindow();

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
	if (aboutWindow) 
	{
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
		float cameraSpeedChanger = App->game_engine->cameraGO.GetComponent<Camera>()->cameraSpeed;
		if (ImGui::SliderFloat("##CamSpeed", &cameraSpeedChanger, 0.01, 2.0f, "%.2f"))
		{
			App->game_engine->cameraGO.GetComponent<Camera>()->cameraSpeed = cameraSpeedChanger;
		}
		ToolTipMessage("CTRL+Click to input a value");

		// Camera Speed Multiplier slider
		ImGui::Text("Camera Speed Multiplier: ");
		ImGui::SliderFloat("##CamSpeedMult", &App->game_engine->cameraGO.GetComponent<Camera>()->cameraSpeedMultiplier, 1.0f, 5.0f, "%.2f");
		ToolTipMessage("CTRL+Click to input a value");

		//--------------------------------------------

		// Camera Mouse Sensitivity slider
		ImGui::Separator();
		ImGui::Text("Camera Mouse Sensitivity: ");
		ImGui::SliderFloat("##CamMouseSens", &App->game_engine->cameraGO.GetComponent<Camera>()->mouseSensitivity, 0.01f, 1.0f, "%.2f");
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
		float p1 = App->game_engine->cameraGO.GetComponent<Camera>()->worldPosVec.x;
		ImGui::InputFloat("##CamPosX", &p1);
		App->game_engine->cameraGO.GetComponent<Camera>()->worldPosVec.x = p1;
		ToolTipMessage("Click to input a value");

		ImGui::SameLine();

		// Camera World Position Y
		float p2 = App->game_engine->cameraGO.GetComponent<Camera>()->worldPosVec.y;
		ImGui::InputFloat("##CamPosY", &p2);
		App->game_engine->cameraGO.GetComponent<Camera>()->worldPosVec.y = p2;
		ToolTipMessage("Click to input a value");

		ImGui::SameLine();

		// Camera World Position Z
		float p3 = App->game_engine->cameraGO.GetComponent<Camera>()->worldPosVec.z;
		ImGui::InputFloat("##CamPosZ", &p3);
		App->game_engine->cameraGO.GetComponent<Camera>()->worldPosVec.z = p3;
		ToolTipMessage("Click to input a value");

		//--------------------------------------------

		// Camera Focus Point X 
		ImGui::BulletText("Reference vector:");
		float r1 = App->game_engine->cameraGO.GetComponent<Camera>()->focusPosVec.x;
		ImGui::InputFloat("##CamFocusX", &r1);
		App->game_engine->cameraGO.GetComponent<Camera>()->focusPosVec.x = r1;
		ToolTipMessage("Click to input a value");

		ImGui::SameLine();

		// Camera Focus Point Y 
		float r2 = App->game_engine->cameraGO.GetComponent<Camera>()->focusPosVec.y;
		ImGui::InputFloat("##CamFocusY", &r2);
		App->game_engine->cameraGO.GetComponent<Camera>()->focusPosVec.y = r2;
		ToolTipMessage("Click to input a value");

		ImGui::SameLine();

		// Camera Focus Point Z
		float r3 = App->game_engine->cameraGO.GetComponent<Camera>()->focusPosVec.z;
		ImGui::InputFloat("##CamFocusZ", &r3);
		App->game_engine->cameraGO.GetComponent<Camera>()->focusPosVec.z = r3;
		ToolTipMessage("Click to input a value");

		//--------------------------------------------

		// Camera Up Vector X
		ImGui::BulletText("Up vector :");
		float u1 = App->game_engine->cameraGO.GetComponent<Camera>()->upVec.x;
		ImGui::InputFloat("##CamUpX", &u1);
		App->game_engine->cameraGO.GetComponent<Camera>()->upVec.x = u1;
		ToolTipMessage("Click to input a value");

		ImGui::SameLine();

		// Camera Focus Point Y 
		float u2 = App->game_engine->cameraGO.GetComponent<Camera>()->upVec.y;
		ImGui::InputFloat("##CamUpY", &u2);
		App->game_engine->cameraGO.GetComponent<Camera>()->upVec.y = u2;
		ToolTipMessage("Click to input a value");

		ImGui::SameLine();

		// Camera Focus Point Z
		float u3 = App->game_engine->cameraGO.GetComponent<Camera>()->upVec.z;
		ImGui::InputFloat("##CamUpZ", &u3);
		App->game_engine->cameraGO.GetComponent<Camera>()->upVec.z = u3;
		ToolTipMessage("Click to input a value");

		//--------------------------------------------

		ImGui::PopItemWidth();

		/* --- Camera Frustum Culling parameters --- */
		ImGui::SeparatorText("Camera Frustum Culling");

		// FOV slider
		ImGui::Text("Field Of View (FOV): ");
		float fov = App->game_engine->cameraGO.GetComponent<Camera>()->fov;
		if (ImGui::SliderFloat("##fov", &fov, 30.0f, 120.0f, "%.0f"))
		{
			App->game_engine->cameraGO.GetComponent<Camera>()->fov = fov;
		}
		ToolTipMessage("CTRL+Click to input a value");

		// zNear Clipping View Plane slider
		ImGui::Text("Z-Near Clipping Plane distance: ");
		float zNear = App->game_engine->cameraGO.GetComponent<Camera>()->clippingPlaneViewNear;
		if (ImGui::SliderFloat("##zNear", &zNear, 0.1f, 1000.0f, "%.1f"))
		{
			App->game_engine->cameraGO.GetComponent<Camera>()->clippingPlaneViewNear = zNear;
		}
		ToolTipMessage("CTRL+Click to input a value");

		// zFar Clipping View Plane slider
		ImGui::Text("Z-Far Clipping Plane distance: ");
		float zFar = App->game_engine->cameraGO.GetComponent<Camera>()->clippingPlaneViewFar;
		if (ImGui::SliderFloat("##zFar", &zFar, 0.1f, 1000.0f, "%.1f"))
		{
			App->game_engine->cameraGO.GetComponent<Camera>()->clippingPlaneViewFar = zFar;
		}
		ToolTipMessage("CTRL+Click to input a value");

		// Display Camera Yaw value
		ImGui::BulletText("Camera Yaw:");
		ImGui::SameLine();
		ImGui::TextWrapped("%.1f", App->game_engine->cameraGO.GetComponent<Camera>()->cameraYaw);

		ImGui::SameLine();

		// Display Camera Pitch value
		ImGui::BulletText("Camera Pitch:");
		ImGui::SameLine();
		ImGui::TextWrapped("%.1f", App->game_engine->cameraGO.GetComponent<Camera>()->cameraPitch);

		// ------------------------------

		// Button to reset camera to initial position
		if (ImGui::Button("RESET CAMERA PARAMETERS", ImVec2(175, 25)))
			App->game_engine->cameraGO.GetComponent<Camera>()->ResetCameraParameters();
	}
}


void ModuleImGUI::RenderImGUIAssetsWindow()
{
	if (assetsWindow)
	{
		// Create Assets window
		ImGui::Begin("Assets", &assetsWindow);
		
		if (ImGui::Button("Import Local File")) {
			std::string selectedFilePath = openFileDialog();

			if (!selectedFilePath.empty()) {
				if (copyFileToAssetsFolder(selectedFilePath)) {
					// File import successful
					LOG("EDITOR: File '%s' imported successfully!", selectedFilePath.c_str());
				}
				else {
					// Handle the case where file import failed
					LOG("EDITOR: ERROR while importing file '%s'", selectedFilePath);
				}
			}
		}


			int assetsPerRow = 6.0f;
			float assetWidth = 150.0f;
			float assetHeight = 20.0f;
			//std::string currentfolderPath = "Assets";
			float buttonPadding = 20.0f;
			std::vector<Asset> assets;
			DoubleClickCallback callback = DoubleClickHandler;

			// Use std::filesystem (C++17 and later) or std::experimental::filesystem (C++14) to list files.
			std::vector<std::string> assetNames;

			for (const auto& entry : std::filesystem::directory_iterator(currentFolderPath)) {
				assets.push_back({ entry.path().filename().string(), entry.is_directory(), false, false });
			}

			if (currentFolderPath != "Assets") {
				if (ImGui::Button("Back")) {
					GoBack();
					assets.clear();
					for (const auto& entry : std::filesystem::directory_iterator(currentFolderPath)) {
						assets.push_back({ entry.path().filename().string(), entry.is_directory() });
					}
				}
			}


			for (size_t i = 0; i < assets.size(); i++) {
            if (i % assetsPerRow != 0) {
                ImGui::SameLine();
            }

            ImGui::BeginGroup();

            if (ImGui::Selectable(assets[i].name.c_str(), &assets[i].isSelected, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(assetWidth, assetHeight))) {
				if (assets[i].isFolder && ImGui::IsMouseDoubleClicked(0)) {
					callback(assets[i].name); // Trigger the callback function
				}
                if (!assets[i].isFolder && ImGui::IsMouseReleased(1)) {
                    assets[i].showDeletePopup = true;
                }
            }

            if (assets[i].showDeletePopup) {
                ImGui::OpenPopup(("DeletePopup##" + assets[i].name).c_str());
            }

            if (ImGui::BeginPopupContextItem(("DeletePopup##" + assets[i].name).c_str())) {
                if (ImGui::MenuItem("Delete")) 
				{
					LOG("EDITOR: Deleting '%s' asset...", assets[i].name.c_str());
                    std::filesystem::remove(currentFolderPath + "/" + assets[i].name);
                    assets.erase(assets.begin() + i);
                    ImGui::CloseCurrentPopup();
                    break;
                }
                ImGui::EndPopup();
            }

            ImGui::EndGroup();
        }

        ImGui::End();
	}
}

void ModuleImGUI::RenderImGUIInspectorWindow()
{
	if (inspectorWindow)
	{
		if (ImGui::Begin("Inspector", &inspectorWindow)) {
			if (gameObjSelected != nullptr) {
				if (gameObjSelected->name != "") {
					ImGui::Checkbox("Active", &gameObjSelected->isActive);
					ImGui::SameLine(); ImGui::Text("Game Object: ");
					ImGui::SameLine(); ImGui::Text(gameObjSelected->name.c_str());

					if (ImGui::BeginCombo("Tag", "Untagged", ImGuiComboFlags_HeightSmall)) { ImGui::EndCombo(); }

					ImGui::SetNextItemWidth(100.0f);
					if (ImGui::BeginCombo("Layer", "Default", ImGuiComboFlags_HeightSmall)) { ImGui::EndCombo(); }

					ImGui::InputText("GO Name", Title, IM_ARRAYSIZE(Title), ImGuiInputTextFlags_EnterReturnsTrue);
					if (ImGui::IsKeyDown(ImGuiKey_Enter)) {
						gameObjSelected->name = Title;
						/*gameObject.name.push_back(Title);*/
					}

					//Grab Components and set in for allows for constant polling 
					//Mesh Menu - Creation of Mesh pointer to component to call Texture methods etc
					for (auto& component : *gameObjSelected->GetComponents()) {
						bool deleteButtonPressed = false;
						if (component.get()->getType() == Component::Type::TRANSFORM) {
							Transform* transform = dynamic_cast<Transform*>(component.get());
							ImGui::SetNextItemOpen(true, ImGuiCond_Once);
							if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_None))
							{
								ImGui::PushItemWidth(60.0f);
								ImGui::BulletText("Position");
								/*ImGui::InputDouble("##PositionX", transform->position().x, 0.0, 0.0, "%.3f");*/
								ImGui::Text(std::to_string(transform->position().x).c_str());
								ImGui::SameLine();
								/*ImGui::InputDouble("##PositionY", &transform->position.y, 0.0, 0.0, "%.3f");*/
								ImGui::Text(std::to_string(transform->position().y).c_str());
								ImGui::SameLine();
								/*ImGui::InputDouble("##PositionZ", &transform->position.z, 0.0, 0.0, "%.3f");*/
								ImGui::Text(std::to_string(transform->position().z).c_str());

								//ImGui::BulletText("Rotation");
								///*ImGui::InputDouble("##PositionX", transform->position().x, 0.0, 0.0, "%.3f");*/
								//ImGui::Text(std::to_string(transform->position().x).c_str());
								//ImGui::SameLine();
								///*ImGui::InputDouble("##PositionY", &transform->position.y, 0.0, 0.0, "%.3f");*/
								//ImGui::Text(std::to_string(transform->Rotate().y).c_str());
								//ImGui::SameLine();
								///*ImGui::InputDouble("##PositionZ", &transform->position.z, 0.0, 0.0, "%.3f");*/
								//ImGui::Text(std::to_string(transform->position().z).c_str());

								//ImGui::BulletText("Scale");
								//ImGui::InputDouble("##ScaleX", &transform->scale.x, 0.0, 0.0, "%.3f");
								//ImGui::SameLine();
								//ImGui::InputDouble("##ScaleY", &transform->scale.y, 0.0, 0.0, "%.3f");
								//ImGui::SameLine();
								//ImGui::InputDouble("##ScaleZ", &transform->scale.z, 0.0, 0.0, "%.3f");

								ImGui::PopItemWidth();
							}
						}
						//Mesh Menu - Creation of  pointer to component to call Mesh methods etc
						if (component.get()->getType() == Component::Type::MESH) {
							Mesh* mesh = dynamic_cast<Mesh*>(component.get());
							ImGui::SetNextItemOpen(true, ImGuiCond_Once);
							if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_None))
							{
								if (ImGui::Checkbox("Active", &mesh->isActive))
									/*LOG("ENGINE: '%s' mesh is [ %s ]", mesh->getName().c_str(), mesh->meshActive ? "ACTIVE" : "INACTIVE");*/
									ImGui::SameLine();
								ImGui::Text("	Mesh Filename: ");
								ImGui::SameLine();
								ImGui::TextColored(ImVec4(1, 1, 0, 1), mesh->getName().c_str());
								ImGui::Separator();

								ImGui::Text("File path: ");
								ImGui::SameLine();
								ImGui::TextColored(ImVec4(0.5, 0.5, 0.5, 1), "Assets/%s", mesh->getName().c_str());

								ImGui::Separator();
								ImGui::Text("Indexes: ");
								ImGui::SameLine();  ImGui::Text(std::to_string(mesh->getNumIndexs()).c_str());
								ImGui::SameLine();
								ImGui::Text("Vertexs: ");
								ImGui::SameLine();  ImGui::Text(std::to_string(mesh->getNumNormals()).c_str());

								ImGui::Text("Normals: ");
								ImGui::SameLine();  ImGui::Text(std::to_string(mesh->getNumNormals()).c_str());
								ImGui::SameLine();
								ImGui::Text("Faces: ");
								ImGui::SameLine();  ImGui::Text(std::to_string(mesh->getNumFaces()).c_str());
								ImGui::Text("Tex coords: ");
								ImGui::SameLine();  ImGui::Text(std::to_string(mesh->getNumTexCoords()).c_str());
								ImGui::Separator();
								if (ImGui::Checkbox("Use Texture", &mesh->usingTexture)) {
									(mesh->usingTexture) ? mesh->texture = gameObjSelected->GetComponent<Texture2D>() : mesh->texture = nullptr;
								}
								ImGui::Checkbox("Use Checker Texture", &mesh->useChecker);
								ImGui::SameLine(); ImGui::TextColored({ 0.5f, 0.5f, 0.5f, 1.0f }, "(?)");
								if (ImGui::IsItemHovered()) {
									ImGui::SetTooltip("Use Texture must be checked in order to see the checker texture.");
								}
								ImGui::SetNextItemOpen(true, ImGuiCond_Once);
								if (ImGui::CollapsingHeader("View Options", ImGuiTreeNodeFlags_None)) {
									if (ImGui::Checkbox("View Vertex Normals", &mesh->drawVertexNormals))
										/*LOG("ENGINE: '%s' vertex normals are [ %s ]", mesh->getName().c_str(), mesh->VertexNormDraw ? "ON" : "OFF");*/
										ImGui::SameLine();
									if (ImGui::Checkbox("View Face Normals", &mesh->drawFaceNormals))
										/*LOG("ENGINE: '%s' face normals are [ %s ]", mesh->getName().c_str(), mesh->FaceNormDraw ? "ON" : "OFF");*/

										ImGui::SliderFloat("Normals Lenghts", &mesh->normalsLength, 0.1f, 20.0f);
								}
								ImGui::Separator();
								if (ImGui::Checkbox("Use Texture", &mesh->usingTexture))
								{
									LOG("ENGINE: '%s' texture is [ %s ]", mesh->getName().c_str(), mesh->usingTexture ? "ACTIVE" : "INACTIVE");
									(mesh->usingTexture) ? mesh->texture = gameObjSelected->GetComponent<Texture2D>() : mesh->texture = nullptr;
								}
							}
						}
						//Texture Menu - Creation of Texture pointer to component to call Texture methods etc
						if (component.get()->getType() == Component::Type::TEXTURE2D) {
							ImGui::SetNextItemOpen(true, ImGuiCond_Once);
							Texture2D* texture2D = dynamic_cast<Texture2D*>(component.get());
							if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_None))
							{
								ImGui::Checkbox("Active", &texture2D->isActive);
								if (ImGui::BeginCombo("Shader", "Standard", ImGuiComboFlags_HeightSmall)) { ImGui::EndCombo(); }
								if (ImGui::BeginCombo("Rendering mode", "Opaque", ImGuiComboFlags_HeightSmall)) { ImGui::EndCombo(); }
								ImGui::Separator();
								ImGui::Text("File path: ");
								ImGui::SameLine(); ImGui::TextColored({ 0.920f, 0.845f, 0.0184f, 1.0f }, texture2D->path.c_str());
								ImGui::Text("Texture size");
								ImGui::Text("Height: ");
								ImGui::SameLine(); ImGui::TextColored({ 0.920f, 0.845f, 0.0184f, 1.0f }, to_string(texture2D->height).c_str());
								ImGui::Text("Width: ");
								ImGui::SameLine(); ImGui::TextColored({ 0.920f, 0.845f, 0.0184f, 1.0f }, to_string(texture2D->width).c_str());
							}
						}

						//WORK IN PROGRESS
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
	}
}

void ModuleImGUI::RenderImGUIDebugLogWindow()
{
	if (imGuiDebugLogWindow)
	{
		ImGui::ShowDebugLogWindow(&imGuiDebugLogWindow);
	}
}

void ModuleImGUI::HierarchyRecursive(GameObject* gO)
{
	ImGuiTreeNodeFlags TreeNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (gO->childs.empty())		TreeNodeFlags |= ImGuiTreeNodeFlags_Leaf;
	if (gameObjSelected == gO)	TreeNodeFlags |= ImGuiTreeNodeFlags_Selected;

	bool isOpen = ImGui::TreeNodeEx(gO->name.c_str(), TreeNodeFlags);

	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
		gameObjSelected = gO;
	}


	if (isOpen)
	{
		for (auto& child : gO->childs)
		{
			HierarchyRecursive(child.get());
		}
		ImGui::TreePop();
	}
}

void ModuleImGUI::RenderImGUIHierarchyWindow()
{
	if (hierarchyWindow)
	{
		ImGui::Begin("Hierarchy", &hierarchyWindow);
		for (const auto& gOparentPtr : App->game_engine->scene->gameObjectList)
		{
			HierarchyRecursive(gOparentPtr.get());
		}

		//// Use iterator to keep track of the selected game object
		//auto selectedGameObjectIter = App->game_engine->renderer3D_engine->gameObjectList.end();

		//for (auto iter = App->game_engine->renderer3D_engine->gameObjectList.begin();
		//	iter != App->game_engine->renderer3D_engine->gameObjectList.end(); ++iter) {
		//	auto& gameObject = *iter;

		//	ImGui::PushID(&gameObject);

		//	// Display button to delete the game object
		//	if (ImGui::Button("Delete")) {
		//		selectedGameObjectIter = iter;
		//	}

		//	// Display selectable for the game object
		//	ImGui::SameLine();
		//	if (ImGui::Selectable(gameObject.name.c_str())) {
		//		gameObjSelected = gameObject;
		//	}

		//	ImGui::PopID();
		//}

		//// Check if a game object is selected and delete it
		//if (selectedGameObjectIter != App->game_engine->renderer3D_engine->gameObjectList.end()) {
		//	App->game_engine->renderer3D_engine->gameObjectList.erase(selectedGameObjectIter);
		//	
		//	LOG("EDITOR: Deleting the selected game object from the scene...", NULL);

		//	// Reset the selected game object to "null", so it isn't displayed anymore in the inspector.
		//	gameObjSelected = GameObject();
		//}

		ImGui::End();
	}
}

void ModuleImGUI::GeneratePrimitives()
{
	if (ImGui::BeginMenu("GameObject"))
	{
		if (ImGui::Button("Import Local File")) {
			std::string selectedFilePath = openFileDialog();

			if (!selectedFilePath.empty()) {
				if (copyFileToAssetsFolder(selectedFilePath)) {
					// File import successful
					LOG("EDITOR: File '%s' imported successfully!", selectedFilePath.c_str());
				}
				else {
					// Handle the case where file import failed
					LOG("EDITOR: ERROR while importing file '%s'", selectedFilePath);
				}
			}
		}

		ImGui::SeparatorText("Primitives:");

		if (ImGui::Button("Generate Cube")) {
			LOG("EDITOR: Adding CUBE Primitive...", NULL);
			App->game_engine->scene->addGameObject("Assets/Cube.fbx");
		}

		if (ImGui::Button("Generate Plane")) {
			LOG("EDITOR: Adding PLANE Primitive...", NULL);
			App->game_engine->scene->addGameObject("Assets/Plane.fbx");
		}

		if (ImGui::Button("Generate Pyramid")) {
			LOG("EDITOR: Adding PYRAMID Primitive...", NULL);
			App->game_engine->scene->addGameObject("Assets/Pyramid.fbx");
		}

		if (ImGui::Button("Generate Sphere")) {
			LOG("EDITOR: Adding SPHERE Primitive...", NULL);
			App->game_engine->scene->addGameObject("Assets/Sphere.fbx");
		}

		if (ImGui::Button("Generate Cylinder")) {
			LOG("EDITOR: Adding CYLINDER Primitive...", NULL);
			App->game_engine->scene->addGameObject("Assets/Cylinder.fbx");
		}

		if (ImGui::Button("Generate Torus")) {
			LOG("EDITOR: Adding TORUS Primitive...", NULL);
			App->game_engine->scene->addGameObject("Assets/Torus.fbx");
		}

		ImGui::Separator();

		ImGui::SeparatorText("Prefabs:");

		if (ImGui::Button("Baker House")) {
			LOG("EDITOR: Adding BAKER HOUSE prefab...", NULL);
			App->game_engine->scene->addGameObject("Assets/BakerHouse.fbx");
		}

		if (ImGui::Button("Medieval Bed")) {
			LOG("EDITOR: Adding MEDIEVAL BED prefab...", NULL);
			App->game_engine->scene->addGameObject("Assets/Medieval_Bed.fbx");
		}

		if (ImGui::Button("Obelisque")) {
			LOG("EDITOR: Adding OBELISQUE prefab...", NULL);
			App->game_engine->scene->addGameObject("Assets/Obelisque.fbx");
		}

		if (ImGui::Button("Street Environment")) {
			LOG("EDITOR: Adding STREET ENVIRONMENT prefab...", NULL);
			App->game_engine->scene->addGameObject("Assets/Street_Environment.fbx");
		}

		ImGui::EndMenu();
	}
}

void ModuleImGUI::RenderImGUIConsoleWindow()
{
	if (consoleWindow)
	{
		// Buffer to store the search query
		static char filterInput[256] = "";

		ImGui::Begin("Console", &consoleWindow);

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
}

void ModuleImGUI::RenderImGUISimulationControlsWindow()
{
	if (simulationButtonsWindow)
	{
		ImGui::Begin("Simulation Controls", &simulationButtonsWindow);

		// Play button
		if (!App->isPlaying) {
			if (ImGui::Button("Play"))
			{
				LOG("EDITOR: Starting the simulation...", NULL);
				App->isPlaying = true;
				App->isPaused = false;
				App->startTime = SDL_GetTicks() / 1000.0;

				App->game_engine->cameraGO.GetComponent<Camera>()->ResetCameraParameters();
			}
		}
		else
		{
			ImGui::TextWrapped("Play");
		}
		ImGui::SameLine();
		// Pause button
		if (App->isPlaying) {
			if (ImGui::Button("Pause"))
			{
				App->isPaused = !App->isPaused;
				if (App->isPaused) {
					// Paused, calculate elapsed time so far
					LOG("EDITOR: Pausing the simulation...", NULL);
					App->elapsedTime += SDL_GetTicks() / 1000.0 - App->startTime;
				}
				else {
					// Resumed, update start time
					LOG("EDITOR: Resuming the simulation...", NULL);
					App->startTime = SDL_GetTicks() / 1000.0;
				}
			}
		}
		else
		{
			ImGui::TextWrapped("Pause");
		}
		ImGui::SameLine();
		// Stop button
		if (App->isPlaying) {
			if (ImGui::Button("Stop"))
			{
				LOG("EDITOR: Stopping the simulation...", NULL);
				App->isPlaying = false;
				App->isPaused = false;
				App->elapsedTime += SDL_GetTicks() / 1000.0 - App->startTime;

				LOG("TOTAL SIMULATION TIME: %.3f seconds", App->elapsedTime);
				App->elapsedTime = 0.0;
			}
		}
		else
		{
			ImGui::TextWrapped("Stop");
		}

		ImGui::End();
	}
}