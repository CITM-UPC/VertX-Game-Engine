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

	RootGO = new GameObject();

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
			if (ImGui::MenuItem("Configuration Window")) 
			{
				configWindow = !configWindow;
			}
			if (ImGui::MenuItem("ImGui Debug Log Window"))
			{
				showDebugLogWindow = !showDebugLogWindow;
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
				App->OpenWebLink("https://github.com/CITM-UPC/VertX-Game-Engine");
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

	


	// Render the Config window
	RenderImGUIConfigWindow();

	// Render the About window
	RenderImGUIAboutWindow();

	// Render the Inspector window
	RenderImGUIInspectorWindow();

	RenderImGUICameraInspectorWindow();

	// Render the ImGui Debug Log window
	RenderImGUIDebugLogWindow();
	
	// Create Assets window
	ImGui::Begin("Assets");

	ImGui::End();


	// Create Hierarchy window

	ImGui::Begin("Hierarchy");

	ImGui::End();

	//GameObjects hieracy
	if (ImGui::Begin("GameObjects")) {
		Hierarchy(RootGO);
	}
	ImGui::End();

	if (!App->input->GetMouseButton(SDL_BUTTON_LEFT))
	{
		CreatedOnce = true;
	}

	if (Selected != nullptr)
	{
		Selected->CreateInspector();
	}


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
			ImGui::SeparatorText("Application name:");
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "VertX Game Engine\n");
			
			ImGui::SeparatorText("Organization:");
			ImGui::TextColored(ImVec4(0, 1, 1, 1), "UPC CITM");

			ImGui::SeparatorText("Developers:");
			ImGui::TextColored(ImVec4(1, 0, 1, 1), "Adria Pons & Rylan Graham");

		}

		if (ImGui::CollapsingHeader("Settings"))
		{
			// Window parameters sliders
			ImGui::Checkbox("Fullscreen", &App->window->fullscreenEnabled);
			ImGui::SameLine();
			ImGui::Checkbox("Fullcreen Desktop", &App->window->fullcreenDesktopEnabled);
			
			ImGui::Checkbox("Borderless", &App->window->borderlessEnabled);
			ImGui::SameLine();
			ImGui::Checkbox("Resizeable", &App->window->resizableEnabled);

			// Vsync toggle checkbox
			if (ImGui::Checkbox("Vertical Sincronization", &App->game_engine->renderer3D_engine->vsync))
				LOG("VSYNC Toggled");

			// Brightness slider
			if (ImGui::SliderFloat("Brightness", &App->window->windowBrightness, 0.0f, 1.0f))
			{
				SDL_SetWindowBrightness(App->window->window, App->window->windowBrightness);
			}
			ToolTipMessage("CTRL+Click to input a value");

			// FPS slider
			if (!App->game_engine->renderer3D_engine->vsync)
			{
				ImGui::SliderInt("Frame Rate cap", &App->fps, 1, 144);
				ToolTipMessage("CTRL+Click to input a value");
			}

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

void ModuleImGUI::RenderImGUICameraInspectorWindow()
{
	// Create Camera Inspector window
	ImGui::Begin("Camera Inspector");

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

	ImGui::Separator();
	ImGui::SeparatorText("Camera Parameters");
	ImGui::PushItemWidth(60.0f);	// Make components text width shorter

	// Camera World Position X 
	ImGui::BulletText("Position vector:");
	float p1 = App->game_engine->camera.worldPosVec.x;
	ImGui::InputFloat("x1", &p1);
	App->game_engine->camera.worldPosVec.x = p1;

	ImGui::SameLine();

	// Camera World Position Y
	float p2 = App->game_engine->camera.worldPosVec.y;
	ImGui::InputFloat("y1", &p2);
	App->game_engine->camera.worldPosVec.y = p2;

	ImGui::SameLine();
	
	// Camera World Position Z
	float p3 = App->game_engine->camera.worldPosVec.z;
	ImGui::InputFloat("z1", &p3);
	App->game_engine->camera.worldPosVec.z = p3;

	//--------------------------------------------

	// Camera Focus Point X 
	ImGui::BulletText("Reference vector:");
	float r1 = App->game_engine->camera.focusPosVec.x;
	ImGui::InputFloat("x2", &r1);
	App->game_engine->camera.focusPosVec.x = r1;

	ImGui::SameLine();

	// Camera Focus Point Y 
	float r2 = App->game_engine->camera.focusPosVec.y;
	ImGui::InputFloat("y2", &r2);
	App->game_engine->camera.focusPosVec.y = r2;

	ImGui::SameLine();

	// Camera Focus Point Z
	float r3 = App->game_engine->camera.focusPosVec.z;
	ImGui::InputFloat("z2", &r3);
	App->game_engine->camera.focusPosVec.z = r3;

	//--------------------------------------------

	// Camera Up Vector X
	ImGui::BulletText("Up vector :");
	float u1 = App->game_engine->camera.upVec.x;
	ImGui::InputFloat("x3", &u1);
	App->game_engine->camera.upVec.x = u1;

	ImGui::SameLine();

	// Camera Focus Point Y 
	float u2 = App->game_engine->camera.upVec.y;
	ImGui::InputFloat("y3", &u2);
	App->game_engine->camera.upVec.y = u2;

	ImGui::SameLine();

	// Camera Focus Point Z
	float u3 = App->game_engine->camera.upVec.z;
	ImGui::InputFloat("z3", &u3);
	App->game_engine->camera.upVec.z = u3;
	
	//--------------------------------------------

	ImGui::PopItemWidth();

	// Button to reset camera to initial position
	if (ImGui::Button("RESET CAMERA PARAMETERS", ImVec2(175, 25)))
		App->game_engine->camera.ResetCameraParameters();
}

void ModuleImGUI::RenderImGUIInspectorWindow()
{
	

	if (ImGui::Begin("Inspector")) {
		//Configuration options
		if (ImGui::CollapsingHeader("Configuration"))
		{
			GeneratePrimitives();

			/*ImGui::Checkbox("WireFrame", &Wireframe);
			ImGui::Checkbox("Depth Test", &DepthTest);
			ImGui::Checkbox("Cull Face", &CullFace);
			ImGui::Checkbox("Lighting", &Lighting);*/
			/*ImGui::Checkbox("Color Material", &ColorMaterial);*/

		}
	}

	ImGui::End();
}

void ModuleImGUI::RenderImGUIDebugLogWindow()
{
	if (showDebugLogWindow)
		ImGui::ShowDebugLogWindow(&showDebugLogWindow);
}

void ModuleImGUI::GeneratePrimitives()
{
	int primitive;

	if (ImGui::BeginMenu("GameObject"))
	{
		if (ImGui::Button("Generate Empty GameObject")) {
			primitive = 1;
			/*App->LoadFbx->PrimitivesObjects(primitive);*/
		}

		if (ImGui::Button("Generate Cube")) {
			primitive = 2;
			/*App->LoadFbx->PrimitivesObjects(primitive);*/
		}

		if (ImGui::Button("Generate Plane")) {
			primitive = 3;
			/*App->LoadFbx->PrimitivesObjects(primitive);*/
		}

		if (ImGui::Button("Generate Pyramid")) {
			primitive = 4;
			/*App->LoadFbx->PrimitivesObjects(primitive);*/
		}

		if (ImGui::Button("Generate Sphere")) {
			primitive = 5;
			/*App->LoadFbx->PrimitivesObjects(primitive);*/
		}
		if (ImGui::Button("Generate Cylinder")) {
			primitive = 6;
			/*App->LoadFbx->PrimitivesObjects(primitive);*/
		}

		if (ImGui::Button("Delete GameObject")) {
			Selected->~GameObject();
		}

		ImGui::EndMenu();
	}

}

void ModuleImGUI::Hierarchy(GameObject* parent) {
	ImGuiTreeNodeFlags treeF = ImGuiTreeNodeFlags_DefaultOpen;


	if (parent->child.size() == 0) {
		treeF |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	if (parent == Selected)
	{
		treeF |= ImGuiTreeNodeFlags_Selected;
	}
	bool openTree = ImGui::TreeNodeEx(parent, treeF, parent->name.c_str());


	if (openTree)
	{
		if (!parent->child.empty())
		{
			for (int i = 0; i < parent->child.size(); i++)
			{
				Hierarchy(parent->child[i]);
			}
			ImGui::TreePop();
		}

		else
		{
			treeF |= ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf;
		}

	}

	if (parent != RootGO)
	{
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("GameObject", parent, sizeof(GameObject*));

			Selected = parent;

			ImGui::Text("Moving Object");
			ImGui::EndDragDropSource();
		}
		if (ImGui::IsItemHovered() && App->input->GetMouseButton(SDL_BUTTON_LEFT) && CreatedOnce == true)
		{
			CreatedOnce = false;
			Selected = parent;

		}

	}

	if (ImGui::BeginDragDropTarget() && Selected != nullptr)
	{
		int SGo;
		if (ImGui::AcceptDragDropPayload("GameObject"))
		{
			for (int i = 0; i < Selected->Parent->child.size(); i++) {
				if (Selected == Selected->Parent->child[i]) {
					SGo = i;
				}
			}

			parent->child.push_back(Selected);

			for (int i = SGo; i < Selected->Parent->child.size() - 1; i++)
			{
				Selected->Parent->child[i] = Selected->Parent->child[i + 1];
			}
			Selected->Parent = parent;

			parent->Parent->child.pop_back();


			Selected = nullptr;
		}
		ImGui::EndDragDropTarget();
	}

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT))
	{
		Selected = nullptr;
	}
}