#include "MeshComp.h"
#include "glm/glm.hpp"

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

// Function to set an OBB equal to an AABB
void Meshes::SetOBBFromAABB(OBB& obb, const AABB& aabb) {
	// Calculate the center and half extents of the OBB
	glm::vec3 center = (aabb.minCorner + aabb.maxCorner) * 0.5f;
	glm::vec3 halfExtents = (aabb.maxCorner - aabb.minCorner) * 0.5f;

	// Set the transform and half extents for the OBB
	obb.transform = glm::translate(glm::mat4(1.0f), center); // Identity matrix with translation
	obb.halfExtents = halfExtents;
}

// Function to transform an OBB using glm
void Meshes::TransformOBB(OBB& obb, const glm::mat4& transformationMatrix) {
	// Multiply the existing transformation matrix of the OBB with the new transformation matrix
	obb.transform = transformationMatrix * obb.transform;

	// You may also need to transform the half extents, but it depends on your specific use case
}

// Method to enclose an AABB with an OBB
void Meshes::EncloseAABBWithOBB(AABB& aabb, const OBB& obb) {
	glm::mat4 obbMatrix = obb.transform;
	glm::vec3 obbHalfExtents = obb.halfExtents;

	glm::vec3 obbMin = -obbHalfExtents;
	glm::vec3 obbMax = obbHalfExtents;

	glm::vec3 obbCorners[8];
	obbCorners[0] = glm::vec3(obbMin.x, obbMin.y, obbMin.z);
	obbCorners[1] = glm::vec3(obbMax.x, obbMin.y, obbMin.z);
	obbCorners[2] = glm::vec3(obbMin.x, obbMax.y, obbMin.z);
	obbCorners[3] = glm::vec3(obbMax.x, obbMax.y, obbMin.z);
	obbCorners[4] = glm::vec3(obbMin.x, obbMin.y, obbMax.z);
	obbCorners[5] = glm::vec3(obbMax.x, obbMin.y, obbMax.z);
	obbCorners[6] = glm::vec3(obbMin.x, obbMax.y, obbMax.z);
	obbCorners[7] = glm::vec3(obbMax.x, obbMax.y, obbMax.z);

	for (int i = 0; i < 8; i++) {
		glm::vec4 obbCornerWorld = obbMatrix * glm::vec4(obbCorners[i], 1.0f);
		mesh->Laabb.minCorner = glm::min(mesh->Laabb.minCorner, glm::vec3(obbCornerWorld));
		mesh->Laabb.maxCorner = glm::max(mesh->Laabb.maxCorner, glm::vec3(obbCornerWorld));
	}
}

void Meshes::Update()
{
	// Generate global OBB
	SetOBBFromAABB(mesh->obb, mesh->Gbbox);
	TransformOBB(mesh->obb, GObjectSelected->transform->Transform_Matrix);

	//Generate global AABB
	/*mesh->Laabb.SetNegativeInfinity();*/
	// Set Laabb to represent negative infinity inline
	mesh->Laabb.minCorner = glm::vec3(-std::numeric_limits<float>::infinity());
	mesh->Laabb.maxCorner = glm::vec3(-std::numeric_limits<float>::infinity());

	/*mesh->Laabb.Enclose(mesh->obb);*/
	EncloseAABBWithOBB(mesh->Laabb, mesh->obb);
}


void Meshes::Inspector()
{
	ImGui::Text("Number Vertices: %d", mesh->num_vertices);
	ImGui::Text("Number Indices: %d", mesh->num_indices);
	//ImGui::Checkbox("Visible Object", &mesh->IsVisible);
}