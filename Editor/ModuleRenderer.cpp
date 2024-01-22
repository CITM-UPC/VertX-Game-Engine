#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer.h"

ModuleRenderer::ModuleRenderer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer::~ModuleRenderer()
{}

// Called before render is available
bool ModuleRenderer::Init()
{
	App->logHistory.push_back("[Editor] Creating Renderer context");
	//LOG("Creating Renderer context");

	bool ret = true;

	ret = App->game_engine->renderer3D_engine->Init();
	ret = App->game_engine->scene->Init();

	vsync = VSYNC;

	App->game_engine->cameraGO.GetComponent<Transform>()->Rotate(-30, vec3(0, 1, 0), Transform::Space::GLOBAL);
	App->game_engine->cameraGO.GetComponent<Transform>()->Rotate(25, vec3(1, 0, 0));
	FocusCamera();

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer::PreUpdate()
{
	App->game_engine->renderer3D_engine->PreUpdate();
	App->game_engine->scene->PreUpdate();

	return UPDATE_CONTINUE;
}

update_status ModuleRenderer::Update()
{
	DoCameraInput();

	if (!App->game_engine->scene->paused)
	{
		App->game_engine->renderer3D_engine->Update();
		App->game_engine->scene->Update();

		if (App->game_engine->scene->step)
		{
			App->game_engine->scene->step = false;
			App->game_engine->scene->paused = true;
		}
	}

	if (App->game_engine->scene->step && App->game_engine->scene->paused)App->game_engine->scene->paused = false;

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer::PostUpdate()
{
	App->game_engine->renderer3D_engine->DrawGrid(100, 1, true);

	App->game_engine->renderer3D_engine->PostUpdate();

	App->game_engine->scene->PostUpdate();

	App->imgui->RenderUI();

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer::CleanUp()
{
	//LOG("Destroying Renderer");

	App->game_engine->scene->CleanUp();
	App->game_engine->renderer3D_engine->CleanUp();

	return true;
}

void ModuleRenderer::DoCameraInput()
{
	DoZoom();

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT))
	{
		keysInputFPS();
		mouseInputFPS();
	}
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT))
		{
			mouseCamOrbit();
		}
		if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE))
		{
			mouseCameraPan();
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		FocusCamera();
	}
}

void ModuleRenderer::keysInputFPS()
{
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) { camSpeed = 0.2; }
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP) { camSpeed = 0.05; }

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		App->game_engine->cameraGO.GetComponent<Transform>()->Move(glm::dvec3(0, 0, camSpeed));
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		App->game_engine->cameraGO.GetComponent<Transform>()->Move(glm::dvec3(0, 0, -camSpeed));
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		App->game_engine->cameraGO.GetComponent<Transform>()->Move(glm::dvec3(camSpeed, 0, 0));
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		App->game_engine->cameraGO.GetComponent<Transform>()->Move(glm::dvec3(-camSpeed, 0, 0));
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		App->game_engine->cameraGO.GetComponent<Transform>()->Move(glm::dvec3(0, camSpeed, 0), Transform::Space::GLOBAL);
	}
	if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
	{
		App->game_engine->cameraGO.GetComponent<Transform>()->Move(glm::dvec3(0, -camSpeed, 0), Transform::Space::GLOBAL);
	}
}
void ModuleRenderer::mouseInputFPS()
{
	double sensitivity = 0.05;

	int dx = -App->input->GetMouseXMotion();
	int dy = App->input->GetMouseYMotion();

	App->game_engine->cameraGO.GetComponent<Transform>()->Rotate(dx * sensitivity, glm::vec3(0, 1, 0), Transform::Space::GLOBAL);
	App->game_engine->cameraGO.GetComponent<Transform>()->Rotate(dy * sensitivity, glm::vec3(1, 0, 0));
}

void ModuleRenderer::mouseCamOrbit()
{
	double sensitivity = 0.1;

	int dx = -App->input->GetMouseXMotion();
	int dy = App->input->GetMouseYMotion();

	App->game_engine->cameraGO.GetComponent<Transform>()->MoveTo(App->game_engine->cameraGO.GetComponent<Camera>()->lookAtPos);

	App->game_engine->cameraGO.GetComponent<Transform>()->Rotate(dx * sensitivity, vec3(0, 1, 0), Transform::Space::GLOBAL);
	App->game_engine->cameraGO.GetComponent<Transform>()->Rotate(dy * sensitivity, vec3(1, 0, 0));

	vec3 finalPos = App->game_engine->cameraGO.GetComponent<Transform>()->position() - (App->game_engine->cameraGO.GetComponent<Transform>()->forward() * App->game_engine->cameraGO.GetComponent<Camera>()->camOffset);
	App->game_engine->cameraGO.GetComponent<Transform>()->MoveTo(finalPos);
}
void ModuleRenderer::mouseCameraPan()
{
	int dx = App->input->GetMouseXMotion();
	int dy = App->input->GetMouseYMotion();

	float panSpeed = 0.01f;

	App->game_engine->cameraGO.GetComponent<Transform>()->Move(vec3(dx * panSpeed, 0, 0));
	App->game_engine->cameraGO.GetComponent<Transform>()->Move(vec3(0, dy * panSpeed, 0));
}

void ModuleRenderer::DoZoom()
{
	int scrollWheel = App->input->GetMouseZ();

	if (scrollWheel != 0)
	{
		float zoomSensitivity = 0.3f;

		App->game_engine->cameraGO.GetComponent<Camera>()->camOffset -= scrollWheel * zoomSensitivity;

		if (App->game_engine->cameraGO.GetComponent<Camera>()->camOffset <= 0.1f)
		{
			App->game_engine->cameraGO.GetComponent<Camera>()->camOffset = 0.1f;
			return;
		}
		else if (App->game_engine->cameraGO.GetComponent<Camera>()->camOffset >= 50.0f)
		{
			App->game_engine->cameraGO.GetComponent<Camera>()->camOffset = 50.0f;
			return;
		}

		App->game_engine->cameraGO.GetComponent<Transform>()->Move(vec3(0, 0, scrollWheel * zoomSensitivity));
	}
}

void ModuleRenderer::FocusCamera()
{
	vec3 targetPos = App->imgui->GetSelectedObjectPos() - App->game_engine->cameraGO.GetComponent<Transform>()->forward() * App->game_engine->cameraGO.GetComponent<Camera>()->camOffset;

	App->game_engine->cameraGO.GetComponent<Transform>()->MoveTo(targetPos);
}

GameObject* ModuleRenderer::DoClickRayCast()
{
	Ray ray = CalculateRay();

	DebugRay(ray);

	std::map<float, GameObject*> hitObjectsMap;

	for (const auto& object : App->game_engine->scene->currentScene.gameObjectList)
	{
		if (RayAABBIntersection(ray, object.get()))
		{
			CheckMeshCollisionRecursive(ray, object.get(), hitObjectsMap);
		}
	}

	float closestHitPoint = std::numeric_limits<float>::infinity();

	map<float, GameObject*>::iterator it = hitObjectsMap.begin();

	// Iterate through the map and print the elements
	while (it != hitObjectsMap.end()) {
		closestHitPoint = glm::min(closestHitPoint, it->first);
		++it;
	}

	if (hitObjectsMap.size() > 0)
	{
		LOG("Closest hit object is %s", hitObjectsMap[closestHitPoint]->name.c_str());
		return hitObjectsMap[closestHitPoint];
	}

	//LOG("Hit nothing");
	return nullptr;
}

Ray ModuleRenderer::CalculateRay()
{
	vec2 nearPlaneSize;
	nearPlaneSize.y = glm::tan(glm::radians(App->game_engine->cameraGO.GetComponent<Camera>()->fov / 2)) * App->game_engine->cameraGO.GetComponent<Camera>()->clippingPlaneViewNear;
	nearPlaneSize.x = nearPlaneSize.y * App->game_engine->cameraGO.GetComponent<Camera>()->aspectRatio;

	vec2 finalPos;
	finalPos.x = (nearPlaneSize.x / App->window->width) * (App->input->GetMouseX() - App->window->width) * 2 + nearPlaneSize.x;
	finalPos.y = (nearPlaneSize.y / App->window->height) * (App->input->GetMouseY() - App->window->height) * 2 + nearPlaneSize.y;

	Ray ray;
	ray.origin = App->game_engine->cameraGO.GetComponent<Transform>()->position();

	ray.origin += App->game_engine->cameraGO.GetComponent<Transform>()->forward() * App->game_engine->cameraGO.GetComponent<Camera>()->clippingPlaneViewNear;
	ray.origin += -App->game_engine->cameraGO.GetComponent<Transform>()->up() * finalPos.y;
	ray.origin += -App->game_engine->cameraGO.GetComponent<Transform>()->right() * finalPos.x;

	ray.direction = glm::normalize((vec3)ray.origin - App->game_engine->cameraGO.GetComponent<Transform>()->position());

	return ray;
}

void ModuleRenderer::DebugRay(Ray ray)
{
	App->game_engine->renderer3D_engine->origins.push_back(ray.origin);	//Debug only
	App->game_engine->renderer3D_engine->ends.push_back((vec3)ray.origin + (vec3)ray.direction * 100.0);	//Debug only
	App->game_engine->renderer3D_engine->camPos.push_back(App->game_engine->cameraGO.GetComponent<Transform>()->position());	//Debug only

	/*
	vec3 frame1 = App->game_engine->cameraGO.GetComponent<Transform>()->position() +
		App->game_engine->cameraGO.GetComponent<Transform>()->forward() * App->game_engine->cameraGO.GetComponent<Camera>()->clippingPlaneViewNear +
		-App->game_engine->cameraGO.GetComponent<Transform>()->right() * nearPlaneSize.x +
		App->game_engine->cameraGO.GetComponent<Transform>()->up() * nearPlaneSize.y;	//Debug only

	vec3 frame2 = App->game_engine->cameraGO.GetComponent<Transform>()->position() +
		App->game_engine->cameraGO.GetComponent<Transform>()->forward() * App->game_engine->cameraGO.GetComponent<Camera>()->clippingPlaneViewNear +
		App->game_engine->cameraGO.GetComponent<Transform>()->right() * nearPlaneSize.x +
		App->game_engine->cameraGO.GetComponent<Transform>()->up() * nearPlaneSize.y;	//Debug only

	vec3 frame3 = App->game_engine->cameraGO.GetComponent<Transform>()->position() +
		App->game_engine->cameraGO.GetComponent<Transform>()->forward() * App->game_engine->cameraGO.GetComponent<Camera>()->clippingPlaneViewNear +
		App->game_engine->cameraGO.GetComponent<Transform>()->right() * nearPlaneSize.x +
		-App->game_engine->cameraGO.GetComponent<Transform>()->up() * nearPlaneSize.y;	//Debug only

	vec3 frame4 = App->game_engine->cameraGO.GetComponent<Transform>()->position() +
		App->game_engine->cameraGO.GetComponent<Transform>()->forward() * App->game_engine->cameraGO.GetComponent<Camera>()->clippingPlaneViewNear +
		-App->game_engine->cameraGO.GetComponent<Transform>()->right() * nearPlaneSize.x +
		-App->game_engine->cameraGO.GetComponent<Transform>()->up() * nearPlaneSize.y;	//Debug only

	App->game_engine->renderer3D_engine->nearPlanes.push_back(frame1);	//Debug only
	App->game_engine->renderer3D_engine->nearPlanes.push_back(frame2);	//Debug only
	App->game_engine->renderer3D_engine->nearPlanes.push_back(frame3);	//Debug only
	App->game_engine->renderer3D_engine->nearPlanes.push_back(frame4);	//Debug only
	*/
}

Triangle ModuleRenderer::CalculateTriangle(GameObject* triObject, int iterator)
{
	vec4 vert0 = { triObject->GetComponent<Mesh>()->mVertices[iterator * 3], 1 };
	vec4 vert1 = { triObject->GetComponent<Mesh>()->mVertices[iterator * 3 + 1], 1 };
	vec4 vert2 = { triObject->GetComponent<Mesh>()->mVertices[iterator * 3 + 2], 1 };

	vert0 = vert0 * glm::inverse(triObject->getGlobalTransform());
	vert1 = vert1 * glm::inverse(triObject->getGlobalTransform());
	vert2 = vert2 * glm::inverse(triObject->getGlobalTransform());

	auto tri0 = (vec3)vert0 + (vec3)triObject->getGlobalTransform()[3];
	auto tri1 = (vec3)vert1 + (vec3)triObject->getGlobalTransform()[3];
	auto tri2 = (vec3)vert2 + (vec3)triObject->getGlobalTransform()[3];

	// Assuming _format is F_V3 (change if necessary)
	Triangle triangle{ tri0, tri1, tri2 };

	return triangle;
}

void ModuleRenderer::CheckMeshCollisionRecursive(Ray& ray, GameObject* object, std::map<float, GameObject*>& hitObjects)
{
	float closestIntersection = std::numeric_limits<float>::infinity();

	//Check for a mesh in the parent
	if (object->GetComponent<Mesh>() != nullptr)
	{
		for (size_t i = 0; i < object->GetComponent<Mesh>()->getNumFaces(); ++i)
		{
			Triangle triangle = CalculateTriangle(object, i);

			float currentT;
			if (RayTriangleIntersection(ray, triangle, currentT) && currentT < closestIntersection)
			{
				closestIntersection = currentT;
			}
		}

		if (closestIntersection != std::numeric_limits<float>::infinity())
		{
			//LOG("Hit %s", object->name.c_str());
			hitObjects[closestIntersection] = object;  //Adding to the map an entry which has the distance as an accessor and the GO as the object
		}
	}
	for (const auto& child : object->childs)
	{
		CheckMeshCollisionRecursive(ray, child.get(), hitObjects);
	}
}