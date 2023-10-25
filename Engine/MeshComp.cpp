#include "MeshComp.h"

Meshes::Meshes() : Meshes(nullptr)
{

}

Meshes::Meshes(GameObject* Meshobject) : Component(Meshobject)
{
	GObjectSelected = Meshobject;
	mesh = nullptr;
	type = Type::MeshRenderer;

}

Meshes::~Meshes()
{
	GObjectSelected = nullptr;
	mesh = nullptr;

}


void Meshes::Update()
{
	// Generate global OBB
	mesh->obb = mesh->Gbbox;
	mesh->obb.Transform(GObjectSelected->transform->Transform_Matrix);

	//Generate global AABB
	mesh->Laabb.SetNegativeInfinity();
	mesh->Laabb.Enclose(mesh->obb);
}


void Meshes::Inspector()
{
	ImGui::Text("Number Vertices: %d", mesh->num_vertices);
	ImGui::Text("Number Indices: %d", mesh->num_indices);
	//ImGui::Checkbox("Visible Object", &mesh->IsVisible);
}