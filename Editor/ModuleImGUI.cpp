
#include "Application.h"
#include "ModuleImGUI.h"
#include "GL/glew.h"
#include "SDL2/SDL_opengl.h"
#include "Globals.h"
#include "ModuleAudio.h"

#include "ModuleInput.h" //All of this is testing
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include <functional>
#include <Windows.h>
#include <commdlg.h>

namespace fs = std::filesystem;

using DoubleClickCallback = std::function<void(const std::string&)>;

std::string currentFolderPath = "VertX";

struct Asset {
	std::string name;
	bool isFolder;
	bool isSelected;
	bool showDeletePopup;
};

void GoBack() {
	if (currentFolderPath != "VertX") {
		currentFolderPath = std::filesystem::path(currentFolderPath).parent_path().string();
	}
}

void DoubleClickHandler(const std::string& folderName) {
	if (currentFolderPath == "VertX/Assets") {
		currentFolderPath = "VertX/Assets/" + folderName;
	}
	else if (currentFolderPath == "VertX") {
		currentFolderPath = "VertX/" + folderName;
	}
	else if (currentFolderPath == "VertX/Library") {
		currentFolderPath = "VertX/Library/" + folderName;
	}
}

void ShowFolderContents(const std::string& folderName, std::vector<Asset>& assets) {
	currentFolderPath = currentFolderPath + "/" + folderName;
	assets.clear(); // Clear current asset list
	for (const auto& entry : std::filesystem::directory_iterator(currentFolderPath)) {
		assets.push_back({ entry.path().filename().string(), entry.is_directory() });
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

// Function to copy a file to the "Assets/" folder
bool copyFileToAssetsFolder(const std::string& sourcePath) {
	// Adjust this path according to your project structure
	const std::string assetsFolderPath = "VertX/Assets/";

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

ModuleImGUI::ModuleImGUI(Application* app, bool start_enabled) : Module(app, start_enabled) {}

// Destructor
ModuleImGUI::~ModuleImGUI() {}

void OsOpenInShell(const char* path)
{
#ifdef _WIN32
	// Note: executable path must use backslashes!
	::ShellExecuteA(NULL, "open", path, NULL, NULL, SW_SHOWDEFAULT);
#else
#if __APPLE__
	const char* open_executable = "open";
#else
	const char* open_executable = "xdg-open";
#endif
	char command[256];
	snprintf(command, 256, "%s \"%s\"", open_executable, path);
	system(command);
#endif
}

bool ModuleImGUI::Init()
{
	//LOG("Creating UI");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		// Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;		// Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;			// IF using Docking Branch

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->game_engine->renderer3D_engine->context);
	ImGui_ImplOpenGL3_Init();

	Uint32 engineFX = App->audio->LoadFx("VertX/Assets/Audio/FX/engineSFX.wav");

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

	std::ifstream file("../LICENSE");
	std::string line;
	if (file.is_open()) {
		while (std::getline(file, line)) {
			aboutContent += line + "\n";
		}
		file.close();
	}
	else {
		//LOG("Unable to open LICENSE file.");
	}

	effectMusicPlayed = false;

	return true;
}

update_status ModuleImGUI::PreUpdate()
{
	//This here does not work. Currently in Input.cpp
	//ImGui_ImplSDL2_ProcessEvent(&pollevent);
	GetInfrastructureInfo();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	//STATIC
	// Get the constant camera position vector
	const glm::vec3& cameraPosition = App->game_engine->cameraGO.GetComponent<Transform>()->position();

	// Set a threshold distance, beyond which the sound is not heard
	double thresholdDistance = 20;

	// Get the position of the moving object (replace "movingObject" with the actual reference to your moving object)
	
		if (gameObjSelected != nullptr) {
			const glm::vec3& movingObjectPosition = gameObjSelected->GetComponent<Transform>()->position();

			// Calculate Euclidean distance between the camera and the moving object
			double distanceToMovingObject = glm::length(cameraPosition - movingObjectPosition);

			// Map the distance to a volume level using an exponential function
			double volumeLevel2 = 1.0 - std::pow(distanceToMovingObject / thresholdDistance, 2.0);
			volumeLevel2 = std::max(0.0, volumeLevel2); // Ensure volumeLevel is not negative

			// Scale down the volume by half
			volumeLevel2 *= 0.5;

			// Adjust the volume level to the desired range for SDL_mixer (0-128)
			int volumemove = int(volumeLevel2 * 128.0);

			if (fxplaying == true) {
				App->audio->PlayEffectMusic("VertX/Assets/Audio/FX/engineSFX.wav", volumemove, -1);
				effectMusicPlayed = false;
			}
		}

	if (musicplaying) {

		Uint32 currentTicks = SDL_GetTicks();
		Uint32 elapsedTicks = currentTicks - musicCycleStart;

		Uint32 trackDuration = 30000;

		if (elapsedTicks < trackDuration) {
			if (!musicPlayedThisCycle) {

				// Introduce a small delay before starting music
				SDL_Delay(10);

				if (alternateTracks) {
					App->audio->PlayMusic("VertX/Assets/Audio/Music/Mario.ogg", 2.0f, 2);
				}
				else {
					App->audio->PlayMusic("VertX/Assets/Audio/Music/Zelda.ogg", 2.0f, 3);
				}

				musicPlayedThisCycle = true;
				alternateTracks = !alternateTracks;
			}
		}
		else {
			musicCycleStart = currentTicks;
			musicPlayedThisCycle = false;
		}
	}
	else {
		Mix_HaltMusic();
		Mix_HaltChannel(-1);

		musicPlayedThisCycle = false;
		alternateTracks = false;
	}

	if (dockSpaceEnabled)
	{
		ImGuiDockNodeFlags dock_flags = 0;
		dock_flags |= ImGuiDockNodeFlags_PassthruCentralNode;
		ImGui::DockSpaceOverViewport(0, dock_flags);
	}


		if (FPSgraph)		FPSGraphWindow();
		if (logWindow)		LogConsoleWindow();

		if (controller) {
			InGameController();
		}

		if (options)		OptionsWindow();
		if (camDebug)		CamDebugWindow();
		if (about)      	AboutWindow();
		if (inspector)		InspectorWindow();
		if (hierarchy)		HierarchyWindow();
		if (fileExplorer)	FileExplorerWindow();
		if (demo)       	ImGui::ShowDemoWindow(&demo);

		if (saveasMenu) 	SaveAsMenu();
		if (loadMenu)		LoadSceneMenu();
		if (reparentMenu) 	ReparentMenu();

		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureMouse && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) return MainMenuBar();

			gameObjSelected = App->renderer->DoClickRayCast();
		}

		return MainMenuBar();
}

bool ModuleImGUI::CleanUp()
{
	//LOG("Destroying UI");

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

void ModuleImGUI::RenderUI()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

update_status ModuleImGUI::MainMenuBar()
{

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene", "Not implemented"))
			{
				App->game_engine->scene->NewScene();
			}
			if (ImGui::MenuItem("Open Scene", "Not implemented")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Save"))
			{
				if (App->game_engine->scene->currentScene.fileName != "")
				{
					/*App->game_engine->scene->SaveScene();*/
				}
				else saveasMenu = true;
			}
			if (ImGui::MenuItem("Save As...", "", &saveasMenu)) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Load Scene", "", &loadMenu)) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "Alt+F4")) { return UPDATE_STOP; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Not implemented")) {}
			if (ImGui::MenuItem("Redo", "Not implemented")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Select All", "Not implemented")) {}
			if (ImGui::MenuItem("Deselect All", "Not implemented")) {}
			if (ImGui::MenuItem("Select Children", "Not implemented")) {}
			if (ImGui::MenuItem("Invert Children", "Not implemented")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Duplicate", "Not implemented")) {}
			if (ImGui::MenuItem("Delete", "Not implemented")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Play", "Play Scene")) {
				fxplaying = true;
				effectMusicPlayed = true;
				canMoveGO = true;
				musicplaying = true;
				controller = true;
				
				musicCycleStart = 0;
				// Handle initialization error
				App->game_engine->scene->paused = false;


				//IMGUI
				hierarchy = false;
				inspector = false; 
				assetsWindow = false;

			}
			if (ImGui::MenuItem("Pause", "Pause Scene")) {
				//App->logHistory.push_back("[Editor] 'Pause' Scene");
				musicplaying = false;
				fxplaying = false;
				effectMusicPlayed = false;
				App->game_engine->scene->paused = true;
				canMoveGO = false;
				ingame = true;
				//IMGUI
				hierarchy = true;
				inspector = true;
				assetsWindow = true;
			}
			if (ImGui::MenuItem("Step", "Step Scene")) {
				//App->logHistory.push_back("[Editor] 'Step' Scene");
				App->game_engine->scene->step = true;
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Settings", "Display, Controls, Renderer, System")) { options = true; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Debug"))
		{
			if (ImGui::MenuItem("Camera Debug")) camDebug = true;
			if (ImGui::MenuItem("RayCast Debug")) App->game_engine->renderer3D_engine->debugRayCast = !App->game_engine->renderer3D_engine->debugRayCast;
			if (ImGui::MenuItem("Bounding Boxes Debug")) App->game_engine->scene->bboxDebug = !App->game_engine->scene->bboxDebug;
			if (ImGui::MenuItem("Demo window")) demo = true;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window")) {
			if (ImGui::BeginMenu("Menus")) {
				ImGui::MenuItem("Assets", "", &assetsWindow);
				ImGui::MenuItem("Hierarchy", "", &hierarchy);
				ImGui::MenuItem("Inspector", "", &inspector);
				ImGui::MenuItem("File Explorer", "", &fileExplorer);
				ImGui::MenuItem("Console Log", "", &logWindow);
				ImGui::MenuItem("FPS Graph", "", &FPSgraph);
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Toggle DockSpace", "", &dockSpaceEnabled)) dockSpaceEnabled = !dockSpaceEnabled;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GameObjects")) {
			if (ImGui::BeginMenu("Create...")) {
				if (ImGui::MenuItem("Create Empty")) { App->game_engine->scene->addGameObject(); }
				if (ImGui::MenuItem("Plane", "Not implemented")) {}
				if (ImGui::MenuItem("Cube")) { Cube cube; App->game_engine->scene->addGameObject(&cube); }
				if (ImGui::MenuItem("Pyramid")) { Pyramid pyramid; App->game_engine->scene->addGameObject(&pyramid); }
				if (ImGui::MenuItem("Sphere", "Not implemented")) {}
				if (ImGui::MenuItem("Cylinder", "Not implemented")) {}
				if (ImGui::MenuItem("Cone")) { Cone cone(16); App->game_engine->scene->addGameObject(&cone); }
				if (ImGui::MenuItem("Torus", "Not implemented")) {}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Draw Mode")) {}
			/*if (ImGui::MenuItem("Add Component Camera"))
			{
				if (gameObjSelected)
				{
					Camera newCam(gameObjSelected);
					gameObjSelected->AddComponent<Camera>(newCam);
					gameObjSelected->GetComponent<Camera>()->clippingPlaneViewFar = 10;
				}
			}*/
			GameObjectOptions();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About...")) about = true;
			ImGui::Separator();
			if (ImGui::MenuItem("Check releases...")) { OsOpenInShell("https://github.com/CITM-UPC/VertX-Game-Engine/releases"); }
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	RenderImGUIAssetsWindow();

	return UPDATE_CONTINUE;
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

		int assetsPerRow = 5.0f;
		float assetWidth = 100.0f;
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

void ModuleImGUI::InGameController() {
	ImGui::Begin("Controller", &controller, ImGuiWindowFlags_NoNavInputs);
	ImGui::Text("Select the GO to move around scene. Parent = all, Child = 1");
	ImGui::Separator;
	for (const auto& gOparentPtr : App->game_engine->scene->currentScene.gameObjectList)
	{
		HierarchyRecursive(gOparentPtr.get());
	}
	if (gameObjSelected != nullptr) {
		if (gameObjSelected->name != "") {
			char Title[256];
			strncpy_s(Title, gameObjSelected->name.c_str(), sizeof(Title));
			Title[sizeof(Title) - 1] = '\0';
			for (auto& component : *gameObjSelected->GetComponents()) {		
				if (component.get()->getType() == Component::Type::TRANSFORM) {
					Transform* transform = dynamic_cast<Transform*>(component.get());
					if (ImGui::TreeNodeEx("Controller Tree Toggle", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_NoTreePushOnOpen))
					{
						// ------------------------------------ //
						// MOVE GAME OBJECT IF SCENE IS PLAYING //
						// ------------------------------------ //
						if (canMoveGO == true) {

							if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
							{
								// MOVE FORWARD
								transform->Move(vec3(0, 0, -0.1), Transform::Space::LOCAL);

								// ROTATE THE OBJECT
								if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
									transform->_rotation.y += 0.7;
									transform->RotateTo(transform->_rotation);
								}
								if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {

									transform->_rotation.y -= 0.7;
									transform->RotateTo(transform->_rotation);
								}
							}
							if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
								// MOVE BACKWARD
								transform->Move(vec3(0, 0, 0.05), Transform::Space::LOCAL);

								// ROTATE THE OBJECT
								if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
									transform->_rotation.y -= 0.7;
									transform->RotateTo(transform->_rotation);
								}
								if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {

									transform->_rotation.y += 0.7;
									transform->RotateTo(transform->_rotation);
								}
							}
						}
						ImGui::TreePop();
						// ------------------------------------ //
					}
				}
			}
		}
	}
	ImGui::EndMenu();
}
	

void ModuleImGUI::SaveAsMenu()
{
	ImGui::Begin("Save As", &saveasMenu);

	static char nameRecipient[32];

	ImGui::InputText("File Name", nameRecipient, IM_ARRAYSIZE(nameRecipient));

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && nameRecipient != "")
	{
		/*App->game_engine->scene->SaveAsScene(nameRecipient);
		saveasMenu = false;*/
	}

	ImGui::End();
}

void ModuleImGUI::LoadSceneMenu()
{
	ImGui::Begin("Load", &loadMenu);

	static char nameRecipient[32];

	ImGui::InputText("File Name", nameRecipient, IM_ARRAYSIZE(nameRecipient));

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && nameRecipient != "")
	{
		gameObjSelected = nullptr;

		string path = "VertX/Assets/";
		path += nameRecipient;
		path += ".mdng";

		/*App->game_engine->scene->LoadScene(path);*/
		loadMenu = false;
	}
	ImGui::End();
}

void ModuleImGUI::GameObjectOptions()
{
	bool goIsSelected;
	gameObjSelected != nullptr ? goIsSelected = true : goIsSelected = false;
	bool a = false;
	if (ImGui::MenuItem("Move", "Reparent GameObject", &reparentMenu, goIsSelected))
	{
		reparentThis = true;
		reparentTo = false;
	}
	if (ImGui::MenuItem("Delete", "Remove GameObject", a, goIsSelected))
	{
		if (gameObjSelected->parent == nullptr)
		{
			App->game_engine->scene->removeGameObject(gameObjSelected);
		}
		else
		{
			auto parent = gameObjSelected->parent;
			parent->removeChild(gameObjSelected);
		}

		gameObjSelected = nullptr;
	}
}

void ModuleImGUI::ReparentMenu()
{
	static ImGuiWindowFlags menuFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize;
	ImGui::Begin("Reparent GameObject", &reparentMenu, menuFlags);

#pragma region Reparent This
	ImGui::MenuItem("Reparent: ", "", false, false);

	orphan == nullptr ? ImGui::Selectable("...", &reparentThis) : ImGui::Selectable(orphan->name.c_str(), &reparentThis);

	if (reparentThis) {
		reparentTo = !reparentThis;
		orphan = gameObjSelected;
	}
#pragma endregion

	ImGui::Separator();

#pragma region Reparent To
	ImGui::MenuItem("To: ", "", false, false);

	adopter == nullptr ? ImGui::Selectable("...", &reparentTo) : ImGui::Selectable(adopter->name.c_str(), &reparentTo);

	if (reparentTo) {
		reparentThis = !reparentTo;
		adopter = gameObjSelected;
	}
#pragma endregion

	if (ImGui::MenuItem("Confirm"))
	{
		if (orphan->parent) {
			if (adopter != nullptr && orphan != nullptr)
			{
				orphan->Move(adopter, orphan->parent->childs);
				//App->logHistory.push_back("Moved " + orphan->name + " to " + adopter->name);
			}
			else
			{
				//App->logHistory.push_back("ERROR: Select both GameObjects in order to Reparent");
			}
		}
		else
		{
			if (adopter != nullptr && orphan != nullptr)
			{
				orphan->Move(adopter, App->game_engine->scene->currentScene.gameObjectList);
				//App->logHistory.push_back("Moved " + orphan->name + " to " + adopter->name);
			}
			else
			{
				//App->logHistory.push_back("ERROR: Select both GameObjects in order to Reparent");
			}
		}
	}
	ImGui::End();
}

void ModuleImGUI::FPSGraphWindow()
{
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize;
	ImGui::Begin("FPS Graph", &FPSgraph, flags);

	std::stringstream sStream;
	sStream << "Average FPS: " << App->fpsHistory[App->fpsHistory.size() - 1];
	string title = sStream.str();

	ImGui::Text("FPS lines graph");
	ImGui::PlotLines("", &App->fpsHistory[0], App->fpsHistory.size(), 0, title.c_str(), 1.0f, 100.0f, { 325, 100 });
	ImGui::Separator();
	ImGui::Text("FPS histogram");
	ImGui::PlotHistogram("", &App->fpsHistory[0], App->fpsHistory.size(), 0, title.c_str(), 1.0f, 100.0f, { 325, 100 });
	ImGui::End();
}

void ModuleImGUI::HierarchyRecursive(GameObject* gO)
{
	ImGuiTreeNodeFlags TreeNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (gO->childs.empty())		TreeNodeFlags |= ImGuiTreeNodeFlags_Leaf;
	if (gameObjSelected == gO)	TreeNodeFlags |= ImGuiTreeNodeFlags_Selected;

	bool isOpen = ImGui::TreeNodeEx(gO->name.c_str(), TreeNodeFlags);

	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())	gameObjSelected = gO;

	if (isOpen)
	{
		for (auto& child : gO->childs)
		{
			HierarchyRecursive(child.get());
		}
		ImGui::TreePop();
	}
}

void ModuleImGUI::HierarchyWindow()
{
	ImGui::Begin("Hierarchy", &hierarchy);
	for (const auto& gOparentPtr : App->game_engine->scene->currentScene.gameObjectList)
	{
		HierarchyRecursive(gOparentPtr.get());
	}
	ImGui::EndMenu();
}

void ModuleImGUI::InspectorWindow()
{
	ImGui::Begin("Inspector", &inspector);
	if (gameObjSelected != nullptr) {
		if (gameObjSelected->name != "") {
			char Title[256];
			strncpy_s(Title, gameObjSelected->name.c_str(), sizeof(Title));
			Title[sizeof(Title) - 1] = '\0';
				ImGui::Checkbox("Active", &gameObjSelected->isActive);
				ImGui::SameLine(); ImGui::InputText("Name", Title, IM_ARRAYSIZE(Title), ImGuiInputTextFlags_EnterReturnsTrue);
				ImGui::SameLine(); ImGui::TextColored({ 0.144f, 0.422f, 0.720f, 1.0f }, gameObjSelected->name.c_str());

				ImGui::SetNextItemWidth(100.0f);
				if (ImGui::BeginCombo("Tag", "Untagged", ImGuiComboFlags_HeightSmall)) { ImGui::EndCombo(); }

				ImGui::SetNextItemWidth(100.0f);
				if (ImGui::BeginCombo("Layer", "Default", ImGuiComboFlags_HeightSmall)) { ImGui::EndCombo(); }

				if (ImGui::IsKeyDown(ImGuiKey_Enter)) {
					gameObjSelected->name = Title;
				}

				for (auto& component : *gameObjSelected->GetComponents()) {
					if (component.get()->getType() == Component::Type::TRANSFORM) {
						Transform* transform = dynamic_cast<Transform*>(component.get());
						if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_None))
						{
							ImGui::PushItemWidth(60.0f);
							ImGui::BulletText("Position");
							ImGui::DragScalar("X-p", ImGuiDataType_Double, &transform->_position.x, 0.5, nullptr, nullptr, "%.3f");
							ImGui::SameLine();
							ImGui::DragScalar("Y-p", ImGuiDataType_Double, &transform->_position.y, 0.5, nullptr, nullptr, "%.3f");
							ImGui::SameLine();
							ImGui::DragScalar("Z-p", ImGuiDataType_Double, &transform->_position.z, 0.5, nullptr, nullptr, "%.3f");

							ImGui::BulletText("Rotation");
							if (ImGui::DragScalar("X-r", ImGuiDataType_Double, &transform->_rotation.x, 0.5, nullptr, nullptr, "%.3f")) transform->RotateTo(transform->_rotation);
							ImGui::SameLine();
							if (ImGui::DragScalar("Y-r", ImGuiDataType_Double, &transform->_rotation.y, 0.5, nullptr, nullptr, "%.3f")) transform->RotateTo(transform->_rotation);
							ImGui::SameLine();
							if (ImGui::DragScalar("Z-r", ImGuiDataType_Double, &transform->_rotation.z, 0.5, nullptr, nullptr, "%.3f")) transform->RotateTo(transform->_rotation);

							ImGui::BulletText("Scale");
							if (ImGui::DragScalar("X-s", ImGuiDataType_Double, &transform->_scale.x, 0.2, nullptr, nullptr, "%.3f")) transform->Scale(transform->_scale);
							ImGui::SameLine();
							if (ImGui::DragScalar("Y-s", ImGuiDataType_Double, &transform->_scale.y, 0.2, nullptr, nullptr, "%.3f")) transform->Scale(transform->_scale);
							ImGui::SameLine();
							if (ImGui::DragScalar("Z-s", ImGuiDataType_Double, &transform->_scale.z, 0.2, nullptr, nullptr, "%.3f")) transform->Scale(transform->_scale);

							if (ImGui::Button("Reset Transforms"))
							{
								transform->_position = vec3(0.0, 0.0, 0.0);

								transform->_rotation = vec3(0.0, 0.0, 0.0);
								transform->RotateTo(transform->_rotation);

								transform->_scale = vec3(1.0, 1.0, 1.0);
								transform->Scale(transform->_scale);
							}
							ImGui::PopItemWidth();
						}

					}

					if (component.get()->getType() == Component::Type::MESH) {
						Mesh* mesh = dynamic_cast<Mesh*>(component.get());
						if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_None))
						{
							ImGui::Checkbox("Active", &mesh->isActive);
							ImGui::SameLine();  ImGui::Text("Filename: ");
							ImGui::SameLine();  ImGui::TextColored({ 0.920f, 0.845f, 0.0184f, 1.0f }, mesh->getName().c_str());
							ImGui::Separator();
							ImGui::Text("File path: ");
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(0.5, 0.5, 0.5, 1), "Library/Meshes/%s", mesh->getName().c_str());
							ImGui::Separator();
							ImGui::Text("Indexes: ");
							ImGui::SameLine();  ImGui::Text(std::to_string(mesh->getNumIndexs()).c_str());
							ImGui::Text("Normals: ");
							ImGui::SameLine();  ImGui::Text(std::to_string(mesh->getNumNormals()).c_str());
							ImGui::Text("Vertexs: ");
							ImGui::SameLine();  ImGui::Text(std::to_string(mesh->getNumVerts()).c_str());
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
							if (mesh->getName().find("Cube") == std::string::npos &&
								mesh->getName().find("Pyramid") == std::string::npos &&
								mesh->getName().find("Cone") == std::string::npos)
							{
								ImGui::Checkbox("Draw vertex normals", &mesh->drawVertexNormals);
								ImGui::Checkbox("Draw face normals", &mesh->drawFaceNormals);
							}
						}
					}
					if (component.get()->getType() == Component::Type::TEXTURE2D) {
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
					if (component.get()->getType() == Component::Type::CAMERA) {
						if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_None))
						{
							Camera* cam = dynamic_cast<Camera*>(component.get());
							if (ImGui::BeginTable("", 4))
							{
								ImGui::TableNextRow();
								ImGui::TableSetColumnIndex(0);
								ImGui::Text("");
								ImGui::Text("Look at Position");

								ImGui::TableSetColumnIndex(1);
								ImGui::Text("X");
								ImGui::Text(std::to_string(cam->lookAtPos.x).c_str());

								ImGui::TableSetColumnIndex(2);
								ImGui::Text("Y");
								ImGui::Text(std::to_string(cam->lookAtPos.y).c_str());

								ImGui::TableSetColumnIndex(3);
								ImGui::Text("Z");
								ImGui::Text(std::to_string(cam->lookAtPos.z).c_str());

								ImGui::EndTable();
							}
							ImGui::Text("");
							ImGui::Text("Fov: "); ImGui::SameLine(); ImGui::Text(std::to_string(cam->fov).c_str());
							ImGui::Text("Aspect Ratio: "); ImGui::SameLine(); ImGui::Text(std::to_string(cam->aspectRatio).c_str());
							ImGui::Text("Clipping Plane View Near: "); ImGui::SameLine(); ImGui::Text(std::to_string(cam->clippingPlaneViewNear).c_str());
							ImGui::Text("Clipping Plane View Far: "); ImGui::SameLine(); ImGui::Text(std::to_string(cam->clippingPlaneViewFar).c_str());
							ImGui::Text("Camera Offset: "); ImGui::SameLine(); ImGui::Text(std::to_string(cam->camOffset).c_str());
						}

					}
				}
			}
			
		}
		ImGui::EndMenu();
}

void ModuleImGUI::LogConsoleWindow()
{
	ImGuiTextFilter filter;
	ImGui::Begin("Log Console", &logWindow);
	if (ImGui::Button("Clear")) {
		// App->logHistory.clear(); 
	}
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine(); filter.Draw("Filter", -100.0f);
	ImGui::Separator();
	if (ImGui::BeginPopup("Options"))
	{
		ImGui::Checkbox("Auto-scroll", &autoScrollLog);
		ImGui::EndPopup();
	}
	if (ImGui::Button("Options")) ImGui::OpenPopup("Options");
	ImGui::Separator();

	if (ImGui::BeginChild("", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar)) {
		if (copy) ImGui::LogToClipboard();

		if (autoScrollLog && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);
		ImGui::GetClipboardText();
	}
	ImGui::EndChild();
	ImGui::End();
}

void ModuleImGUI::OptionsWindow()
{
	ImGui::Begin("Options Window", &options, ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::BeginTabBar("", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Display"))
		{
			if (ImGui::CollapsingHeader("Window")) {
				ImGui::Text("Window size");
				ImGui::SliderInt("Width", &App->window->width, 640, 4096, "%d");
				ImGui::SliderInt("Height", &App->window->height, 360, 2160, "%d");
				ImGui::Separator();
				SDL_SetWindowSize(App->window->window, App->window->width, App->window->height);
				ImGui::Text("Window flags");
				ImGui::Checkbox("Fullscreen", &App->window->fullscreen);
				ImGui::Checkbox("Borderless", &App->window->borderless);
				ImGui::Checkbox("Resizable", &App->window->resizable);
				SDL_SetWindowFullscreen(App->window->window, App->window->fullscreen);
				(App->window->borderless) ? SDL_SetWindowBordered(App->window->window, SDL_FALSE) : SDL_SetWindowBordered(App->window->window, SDL_TRUE);
				(App->window->resizable) ? SDL_SetWindowResizable(App->window->window, SDL_TRUE) : SDL_SetWindowResizable(App->window->window, SDL_FALSE);
			}
			if (ImGui::CollapsingHeader("Framerate")) {
				ImGui::Checkbox("VSync", &App->renderer->vsync);
				SDL_GL_SetSwapInterval(App->renderer->vsync);
				string textToShow = "Resfresh rate: " + to_string(App->fpsHistory[0]);
				ImGui::Text(textToShow.c_str());
				ImGui::SliderInt("Target FPS", &App->targetFPS, 30, 240, "%d");
				App->frameDurationTime = 1.0s / App->targetFPS;
			}

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Controls"))
		{
			if (ImGui::CollapsingHeader("Mouse information")) {
				ImGui::Text("Mouse X position: ");
				ImGui::SameLine(); ImGui::TextColored({ 0.720f, 0.691f, 0.144f, 1.0f }, to_string(App->input->GetMouseX()).c_str());

				ImGui::Text("Mouse Y position: ");
				ImGui::SameLine(); ImGui::TextColored({ 0.720f, 0.691f, 0.144f, 1.0f }, to_string(App->input->GetMouseY()).c_str());

				ImGui::Text("Mouse X motion: ");
				ImGui::SameLine(); ImGui::TextColored({ 0.720f, 0.691f, 0.144f, 1.0f }, to_string(App->input->GetMouseXMotion()).c_str());

				ImGui::Text("Mouse Y motion: ");
				ImGui::SameLine(); ImGui::TextColored({ 0.720f, 0.691f, 0.144f, 1.0f }, to_string(App->input->GetMouseYMotion()).c_str());
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Renderer"))
		{
			ImGui::Text("OpenGL flags:");

			ImGui::Bullet(); ImGui::Text("General:");
			if (ImGui::Checkbox("Depth Test", &App->game_engine->renderer3D_engine->depth_test)) {
				App->game_engine->renderer3D_engine->SwapDepthTest();
			};
			ImGui::SameLine(); if (ImGui::Checkbox("Cull Face", &App->game_engine->renderer3D_engine->cull_face)) {
				App->game_engine->renderer3D_engine->SwapCullFace();
			};
			ImGui::SameLine(); if (ImGui::Checkbox("Lighting", &App->game_engine->renderer3D_engine->lighting)) {
				App->game_engine->renderer3D_engine->SwapLighting();
			};

			ImGui::Bullet(); ImGui::Text("Points & lines:");
			if (ImGui::Checkbox("Line Smooth", &App->game_engine->renderer3D_engine->line_smooth)) {
				App->game_engine->renderer3D_engine->SwapLineSmooth();
			};

			ImGui::Bullet(); ImGui::Text("Polygon:");
			if (ImGui::Checkbox("Polygon Smooth", &App->game_engine->renderer3D_engine->polygon_smooth)) {
				App->game_engine->renderer3D_engine->SwapPolygonSmooth();
			};

			ImGui::Bullet(); ImGui::Text("Color:");
			if (ImGui::Checkbox("Color Material", &App->game_engine->renderer3D_engine->color_material)) {
				App->game_engine->renderer3D_engine->SwapColorMaterial();
			};
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("System"))
		{
			if (ImGui::CollapsingHeader("Software")) {
				ImGui::Text("SDL:");
				ImGui::Bullet(); ImGui::Text("Compiled: ");
				ImGui::SameLine(); ImGui::TextColored({ 0.352f, 0.386f, 0.750f, 1.0f }, info.sdl_version_compiled.c_str());
				ImGui::Bullet(); ImGui::Text("Linked: ");
				ImGui::SameLine(); ImGui::TextColored({ 0.352f, 0.386f, 0.750f, 1.0f }, info.sdl_version_linked.c_str());
				ImGui::Text("OpenGL:");
				ImGui::Bullet(); ImGui::TextColored({ 0.719f, 0.735f, 0.910f, 1.0f }, info.gl_version.c_str());
				ImGui::Text("DevIL:");
				ImGui::Bullet(); ImGui::TextColored({ 0.610f, 0.0488f, 0.142f, 1.0f }, info.devil_version.c_str());
			}
			if (ImGui::CollapsingHeader("Hardware")) {
				ImGui::Text("GPU Information:");
				std::string textToShow = "GPU: " + info.Gpu;
				ImGui::Bullet(); ImGui::TextColored({ 0.2f, 1.0f, 0.0f, 1.0f }, textToShow.c_str());

				textToShow = "Vendor: " + info.GpuVendor;
				ImGui::Bullet(); ImGui::TextColored({ 0.2f, 1.0f, 0.0f, 1.0f }, textToShow.c_str());

				textToShow = "Driver: " + info.GpuDriver;
				ImGui::Bullet(); ImGui::TextColored({ 0.2f, 1.0f, 0.0f, 1.0f }, textToShow.c_str());

				ImGui::Separator();

				ImGui::Text("VRAM Information:");
				textToShow = "Budget: " + std::to_string(info.vram_mb_budget) + " mb";
				ImGui::Bullet(); ImGui::TextColored({ 0.0504f, 0.720f, 0.642f, 1.0f }, textToShow.c_str());

				textToShow = "Usage: " + std::to_string(info.vram_mb_usage) + " mb";
				ImGui::Bullet(); ImGui::TextColored({ 0.0504f, 0.720f, 0.642f, 1.0f }, textToShow.c_str());

				textToShow = "Available: " + std::to_string(info.vram_mb_available) + " mb";
				ImGui::Bullet(); ImGui::TextColored({ 0.0504f, 0.720f, 0.642f, 1.0f }, textToShow.c_str());

				ImGui::Separator();

				ImGui::Text("CPU Information:");

				textToShow = "CPU Cores: " + std::to_string(info.cpu_count);
				ImGui::Bullet(); ImGui::TextColored({ 0.890f, 0.876f, 0.0356f, 1.0f }, textToShow.c_str());

				textToShow = "CPU cache line size: " + std::to_string(info.l1_cachekb);
				ImGui::Bullet(); ImGui::TextColored({ 0.890f, 0.876f, 0.0356f, 1.0f }, textToShow.c_str());
			}
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
}

void ModuleImGUI::CamDebugWindow()
{
	ImGui::Begin("Cam Debug", &camDebug);
	ImGui::Text("Camera Position x: %f", App->game_engine->cameraGO.GetComponent<Transform>()->position().x);
	ImGui::Text("Camera Position y: %f", App->game_engine->cameraGO.GetComponent<Transform>()->position().y);
	ImGui::Text("Camera Position z: %f", App->game_engine->cameraGO.GetComponent<Transform>()->position().z);
	ImGui::Separator();
	ImGui::Text("LookAt Pos x: %f", App->game_engine->cameraGO.GetComponent<Camera>()->lookAtPos.x);
	ImGui::Text("LookAt Pos y: %f", App->game_engine->cameraGO.GetComponent<Camera>()->lookAtPos.y);
	ImGui::Text("LookAt Pos z: %f", App->game_engine->cameraGO.GetComponent<Camera>()->lookAtPos.z);
	ImGui::Separator();
	ImGui::Text("Forward: %f, %f, %f", App->game_engine->cameraGO.GetComponent<Transform>()->forward().x, App->game_engine->cameraGO.GetComponent<Transform>()->forward().y, App->game_engine->cameraGO.GetComponent<Transform>()->forward().z);
	ImGui::Text("Right: %f, %f, %f", App->game_engine->cameraGO.GetComponent<Transform>()->right().x, App->game_engine->cameraGO.GetComponent<Transform>()->right().y, App->game_engine->cameraGO.GetComponent<Transform>()->right().z);
	ImGui::Text("Up: %f, %f, %f", App->game_engine->cameraGO.GetComponent<Transform>()->up().x, App->game_engine->cameraGO.GetComponent<Transform>()->up().y, App->game_engine->cameraGO.GetComponent<Transform>()->up().z);
	ImGui::Separator();
	ImGui::Text("RotMat: %f, %f, %f, %f", App->game_engine->cameraGO.GetComponent<Transform>()->_transformationMatrix[0][0], App->game_engine->cameraGO.GetComponent<Transform>()->_transformationMatrix[0][1], App->game_engine->cameraGO.GetComponent<Transform>()->_transformationMatrix[0][2], App->game_engine->cameraGO.GetComponent<Transform>()->_transformationMatrix[0][3]);
	ImGui::Text("RotMat: %f, %f, %f, %f", App->game_engine->cameraGO.GetComponent<Transform>()->_transformationMatrix[1][0], App->game_engine->cameraGO.GetComponent<Transform>()->_transformationMatrix[1][1], App->game_engine->cameraGO.GetComponent<Transform>()->_transformationMatrix[1][2], App->game_engine->cameraGO.GetComponent<Transform>()->_transformationMatrix[1][3]);
	ImGui::Text("RotMat: %f, %f, %f, %f", App->game_engine->cameraGO.GetComponent<Transform>()->_transformationMatrix[2][0], App->game_engine->cameraGO.GetComponent<Transform>()->_transformationMatrix[2][1], App->game_engine->cameraGO.GetComponent<Transform>()->_transformationMatrix[2][2], App->game_engine->cameraGO.GetComponent<Transform>()->_transformationMatrix[2][3]);
	ImGui::Text("RotMat: %f, %f, %f, %f", App->game_engine->cameraGO.GetComponent<Transform>()->_transformationMatrix[3][0], App->game_engine->cameraGO.GetComponent<Transform>()->_transformationMatrix[3][1], App->game_engine->cameraGO.GetComponent<Transform>()->_transformationMatrix[3][2], App->game_engine->cameraGO.GetComponent<Transform>()->_transformationMatrix[3][3]);
	ImGui::End();
}

void ModuleImGUI::AboutWindow()
{
	ImGui::Begin("About...", &about, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Vertx Engine v0.3\nA 3D Game Engine for the Game Engines subject.\nBy Rylan Graham, Adria Pons, & Joel Chaves");
	if (ImGui::Button("Repository Link")) { OsOpenInShell("https://github.com/CITM-UPC/VertX-Game-Engine/"); }
	ImGui::Separator();
	ImGui::Text("3rd Party Libraries used :");
	ImGui::Bullet(); if (ImGui::Button("Assimp 5.2.5")) { OsOpenInShell("https://assimp-docs.readthedocs.io/"); }
	ImGui::Bullet(); if (ImGui::Button("DevIL 1.8.0#11")) { OsOpenInShell("https://openil.sourceforge.net/"); }
	ImGui::Bullet(); if (ImGui::Button("GLEW 2.2.0#3")) { OsOpenInShell("https://glew.sourceforge.net/"); }
	ImGui::Bullet(); if (ImGui::Button("GLM 2023-06-08")) { OsOpenInShell("https://glm.g-truc.net/0.9.5/index.html"); }
	ImGui::Bullet(); if (ImGui::Button("ImGUI 1.89.9")) { OsOpenInShell("https://imgui-test.readthedocs.io/"); }
	ImGui::Bullet(); if (ImGui::Button("nlohmann-json")) { OsOpenInShell("https://github.com/nlohmann/json"); }
	ImGui::Bullet(); if (ImGui::Button("OpenGL 2022-12-04#3")) { OsOpenInShell("https://www.opengl.org/"); }
	ImGui::Bullet(); if (ImGui::Button("SDL2 2.28.3")) { OsOpenInShell("https://wiki.libsdl.org/"); }
	ImGui::Bullet(); if (ImGui::Button("std:c++20")) { OsOpenInShell("https://en.cppreference.com/w/cpp/20"); }
	ImGui::Separator();
	ImGui::Text("MIT Licence\n");
	ImGui::Text("Copyright (c) 2023 Adria Pons, Joel Chaves & Rylan Graham - VertX Game Engine\n");
	ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy\nof this software and associated documentation files (the 'Software'), to deal\nin the Software without restriction, including without limitation the rights\nto use, copy, modify, merge, publish, distribute, sublicense, and/or sell\ncopies of the Software, and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions:\n\n");
	ImGui::Text("The above copyright notice and this permission notice shall be included in all\ncopies or substantial portions of the Software.");
	ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE.\n");
	if (ImGui::CollapsingHeader("License")) { ImGui::Text(aboutContent.c_str()); }
	ImGui::End();
}

std::string loadTextFile(const std::string& filePath) {
	std::ifstream file(filePath);
	if (file.is_open()) {
		std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
		file.close();
		return content;
	}
	return "";
}

void saveTextFile(const std::string& filePath, const std::string& content) {
	std::ofstream file(filePath);
	if (file.is_open()) {
		file << content;
		file.close();
	}
}

void ModuleImGUI::ShowFolderContents(const fs::path& folderPath) {
	if (ImGui::CollapsingHeader(folderPath.filename().string().c_str())) {
		for (const auto& entry : fs::directory_iterator(folderPath)) {
			if (fs::is_directory(entry.path())) {
				ShowFolderContents(entry.path());
			}
			else if (fs::is_regular_file(entry.path())) {
				ImGui::Text("%s", entry.path().filename().string().c_str());
				ImGui::SameLine();
				if (entry.path().filename().string().substr(entry.path().filename().string().find_last_of(".") + 1) == "cs") {
					if (ImGui::Button("Edit")) {
						filePath = "VertX/Library/" + folderPath.filename().string() + "/" + entry.path().filename().string();
						fileContent = loadTextFile(entry.path().string());
						editor.SetText(fileContent);
					}
					ImGui::SameLine();
				}

				if (ImGui::Button(("Delete##" + entry.path().string()).c_str())) fs::remove(entry.path());

			}
		}
	}
}

void ModuleImGUI::FileExplorerWindow()
{
	ImGui::Begin("File Explorer", &fileExplorer);

	const fs::path assetsPath = "VertX/Assets";
	ShowFolderContents(assetsPath);
	ImGui::Separator();

	const fs::path libraryPath = "VertX/Library";
	ShowFolderContents(libraryPath);
	ImGui::Separator();

	ImGui::End();
}

void ModuleImGUI::GetInfrastructureInfo()
{
	SDL_version compiled;
	SDL_version linked;

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);

	info.sdl_version_compiled = std::to_string(compiled.major) + "." + std::to_string(compiled.minor) + "." + std::to_string(compiled.patch);
	info.sdl_version_linked = std::to_string(linked.major) + "." + std::to_string(linked.minor) + "." + std::to_string(linked.patch);

	//info.gl_version = App->game_engine->getOpenGLVersion();
	//info.devil_version = App->game_engine->getDevILVersion();

	info.GpuVendor.assign((const char*)glGetString(GL_VENDOR));
	info.Gpu.assign((const char*)glGetString(GL_RENDERER));
	info.GpuDriver.assign((const char*)glGetString(GL_VERSION));

	GLint vmem_budget = 0;
	GLint vmem_available = 0;
	GLint vmem_usage = 0;

	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &vmem_budget);
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &vmem_available);

	vmem_usage = vmem_budget - vmem_available;

	info.vram_mb_budget = float(vmem_budget) / 1024.0f;
	info.vram_mb_usage = float(vmem_usage) / 1024.f;
	info.vram_mb_available = float(vmem_available) / 1024.f;

	info.cpu_count = SDL_GetCPUCount();
	info.l1_cachekb = SDL_GetCPUCacheLineSize();
}

