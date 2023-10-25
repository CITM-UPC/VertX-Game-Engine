#include "GameObject.h"
#include "Transform.h"
#include "ModuleRenderer.h"

#include<vector>

GameObject::GameObject()
{
	name = "root";
	Parent = nullptr;
	transform = new Transform(this);
	Comp.push_back(transform);
}

GameObject::GameObject(GameObject* parent)
{
	name = "GameObject";
	Parent = parent;

	//CamGOGame = new CObject(this);

	transform = new Transform(this);
	Comp.push_back(transform);

	//GOMesh = new Meshes(this);
	if (parent != nullptr)
	{
		parent->child.push_back(this);
	}
}

GameObject::~GameObject()
{


	if (Parent != nullptr)
	{
		for (size_t i = 0; i < child.size(); i++)
		{
			delete child[i];
			child[i] = nullptr;
		}
	}

	//transform = nullptr;
	child.clear();
	for (size_t i = 0; i < Comp.size(); i++)
	{
		delete Comp[i];
		Comp[i] = nullptr;
	}
	Comp.clear();

}

void GameObject::CreateInspector()
{

	if (ImGui::Begin("Inspector")) {

		ImGui::InputText("GO Name", Title, IM_ARRAYSIZE(Title), ImGuiInputTextFlags_EnterReturnsTrue);
		if (ImGui::IsKeyDown(ImGuiKey_Enter)) {
			name = Title;
		}


		for (size_t i = 0; i < Comp.size(); i++)
		{
			Comp[i]->Inspector();
			//if (ImGui::Checkbox("Visible Object", &Comp[i+1]->GObjectSelected->GOMesh->mesh->IsVisible));
		}
	}

	ImGui::End();
}