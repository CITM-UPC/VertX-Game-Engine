#include "Application.h"

Application::Application()
{
	game_engine = new ModuleGameEngine();
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer = new ModuleRenderer(this);
	imgui = new ModuleImGUI(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(input);
	AddModule(renderer);
	AddModule(imgui);
}

Application::~Application()
{
	list_modules.clear();	//If this does not yield the expected results, 
							//try using list::erase(i) traversing the list in reverse order.
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	for (auto const& item:list_modules)
	{
		item->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (auto const& item : list_modules)
	{
		item->Start();
	}
	
	return ret;
}


// ---------------------------------------------
void Application::PrepareUpdate()
{
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = update_status::UPDATE_CONTINUE;
	PrepareUpdate();

	for (auto const& item : list_modules)
	{
		ret = item->PreUpdate();
		if (ret != update_status::UPDATE_CONTINUE) return ret;
	}

	for (auto const& item : list_modules)
	{
		ret = item->Update();
		if (ret != update_status::UPDATE_CONTINUE) return ret;
	}

	for (auto const& item : list_modules)
	{
		ret = item->PostUpdate();
		if (ret != update_status::UPDATE_CONTINUE) return ret;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	//This seems to work, but I think it is doing it in the normal order, not reverse.
	for (auto const& item: list_modules)
	{
		ret = item->CleanUp();
		if (ret != true) return ret;
	}

	delete game_engine;

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::OpenWebLink(const char *webLink)
{
	LOG("Opening the following link: %s", webLink);
	
	// Open web link using Shell Execute command line
	ShellExecute(NULL, "open", webLink, 0, 0, SW_SHOWDEFAULT);
}