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
		if (ImGui::BeginMenu("Quit"))
		{
			if (ImGui::MenuItem("Close the VertX Game Engine"))
				return UPDATE_STOP;

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
		

		ImGui::End();
	}
	if (demoWindow)
		ImGui::ShowDemoWindow();

	
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
