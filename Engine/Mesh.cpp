#include "Globals.h"
#include "Application.h"
#include "Mesh.h"
#include "Transform.h"
#include "Mesh.h"
#include <vector>
#include "MeshComp.h"

#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>

ModuleLoadFBX::ModuleLoadFBX(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}
bool ModuleLoadFBX::Start()
{
	bool ret = true;

	/*MeshObject = LoadFile("");
	MeshObject->transform->rotate = glm::vec3(0, -90, 0);*/
	return ret;
}

MyMesh::MyMesh() : id_indices(0), id_vertices(0)
{
}

MyMesh::~MyMesh() {
	delete[] vertices;
	delete[] indices;
	vertices = nullptr;
	indices = nullptr;
	glDeleteBuffers(1, &id_vertices);
	glDeleteBuffers(1, &id_indices);
	id_vertices = 0;
	id_indices = 0;
}
void MyMesh::Render()
{
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glPushMatrix();
	glMultMatrixf(glm::value_ptr(OBmesh->transform->Transform_Matrix));

	if (!OBmesh->child.empty())
	{
		for (int i = 0; i < OBmesh->child.size(); i++)
		{

			OBmesh->child[i]->transform->Transform_Matrix[0][0] = ((cos(OBmesh->child[i]->transform->rotate.z) * cos(OBmesh->child[i]->transform->rotate.x)) * (OBmesh->child[i]->transform->scale.x * OBmesh->child[i]->transform->scale.x)) + ((cos(OBmesh->transform->rotate.z) * cos(OBmesh->transform->rotate.x)) * (OBmesh->transform->scale.x * OBmesh->transform->scale.x));
			OBmesh->child[i]->transform->Transform_Matrix[3][0] = (cos(OBmesh->child[i]->transform->rotate.z) * sin(OBmesh->transform->rotate.x)) + (cos(OBmesh->transform->rotate.z) * sin(OBmesh->transform->rotate.x));
			OBmesh->child[i]->transform->Transform_Matrix[0][2] = -sin(OBmesh->child[i]->transform->rotate.z) + -sin(OBmesh->transform->rotate.z);
			OBmesh->child[i]->transform->Transform_Matrix[3][0] = OBmesh->child[i]->transform->position.x + OBmesh->transform->position.x;


			OBmesh->child[i]->transform->Transform_Matrix[1][0] = ((sin(OBmesh->child[i]->transform->rotate.y) * sin(OBmesh->child[i]->transform->rotate.z) * cos(OBmesh->child[i]->transform->rotate.x)) - (cos(OBmesh->child[i]->transform->rotate.y) * sin(OBmesh->child[i]->transform->rotate.x))) + ((sin(OBmesh->transform->rotate.y) * sin(OBmesh->transform->rotate.z) * cos(OBmesh->transform->rotate.x)) - (cos(OBmesh->transform->rotate.y) * sin(OBmesh->transform->rotate.x)));
			OBmesh->child[i]->transform->Transform_Matrix[1][1] = (((sin(OBmesh->child[i]->transform->rotate.y) * sin(OBmesh->child[i]->transform->rotate.z) * sin(OBmesh->child[i]->transform->rotate.x)) + (cos(OBmesh->child[i]->transform->rotate.y) * cos(OBmesh->child[i]->transform->rotate.x))) * (OBmesh->child[i]->transform->scale.y * OBmesh->child[i]->transform->scale.y)) + (((sin(OBmesh->transform->rotate.y) * sin(OBmesh->transform->rotate.z) * sin(OBmesh->transform->rotate.x)) + (cos(OBmesh->transform->rotate.y) * cos(OBmesh->transform->rotate.x))) * (OBmesh->transform->scale.y * OBmesh->transform->scale.y));
			OBmesh->child[i]->transform->Transform_Matrix[1][2] = (sin(OBmesh->child[i]->transform->rotate.y) * cos(OBmesh->child[i]->transform->rotate.z)) + (sin(OBmesh->transform->rotate.y) * cos(OBmesh->transform->rotate.z));
			OBmesh->child[i]->transform->Transform_Matrix[3][1] = OBmesh->child[i]->transform->position.y + OBmesh->transform->position.y;

			OBmesh->child[i]->transform->Transform_Matrix[2][0] = ((cos(OBmesh->child[i]->transform->rotate.y) * sin(OBmesh->child[i]->transform->rotate.z) * cos(OBmesh->child[i]->transform->rotate.x)) + (sin(OBmesh->child[i]->transform->rotate.y) * sin(OBmesh->child[i]->transform->rotate.x))) + ((cos(OBmesh->transform->rotate.y) * sin(OBmesh->transform->rotate.z) * cos(OBmesh->transform->rotate.x)) + (sin(OBmesh->transform->rotate.y) * sin(OBmesh->transform->rotate.x)));
			OBmesh->child[i]->transform->Transform_Matrix[2][1] = ((cos(OBmesh->child[i]->transform->rotate.y) * sin(OBmesh->child[i]->transform->rotate.z) * sin(OBmesh->child[i]->transform->rotate.x)) - (sin(OBmesh->child[i]->transform->rotate.y) * cos(OBmesh->child[i]->transform->rotate.x))) + ((cos(OBmesh->transform->rotate.y) * sin(OBmesh->transform->rotate.z) * sin(OBmesh->transform->rotate.x)) - (sin(OBmesh->transform->rotate.y) * cos(OBmesh->transform->rotate.x)));
			OBmesh->child[i]->transform->Transform_Matrix[2][2] = ((cos(OBmesh->child[i]->transform->rotate.y) * cos(OBmesh->child[i]->transform->rotate.z)) * (OBmesh->child[i]->transform->scale.z * OBmesh->child[i]->transform->scale.z)) + ((cos(OBmesh->transform->rotate.y) * cos(OBmesh->transform->rotate.z)) * (OBmesh->transform->scale.z * OBmesh->transform->scale.z));
			OBmesh->child[i]->transform->Transform_Matrix[3][2] = OBmesh->child[i]->transform->position.z + OBmesh->transform->position.z;
		}
	}

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

	glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY);

}

GameObject* ModuleLoadFBX::LoadFile(string file_path, string nameGO)
{
	const aiScene* scene = aiImportFile(file_path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		GameObject* meshGO = nullptr;
		if (App->imgui->Selected == nullptr)
		{
			meshGO = new GameObject(App->imgui->RootGO);
			if (nameGO == "")
			{
				nameGO = file_path.c_str();
			}
			meshGO->name = nameGO;
		}

		else if (App->imgui->Selected != nullptr)
		{
			meshGO = new GameObject(App->imgui->Selected);
			if (nameGO == "")
			{
				nameGO = file_path.c_str();
			}
			meshGO->name = nameGO;
		}

		for (int i = 0; i < scene->mNumMeshes; i++) {
			MyMesh* mesh = new MyMesh();

			//Copy fbx mesh info to Mesh struct
			mesh->num_vertices = scene->mMeshes[i]->mNumVertices;
			mesh->vertices = new float[mesh->num_vertices * 3];
			memcpy(mesh->vertices, scene->mMeshes[i]->mVertices, sizeof(float) * mesh->num_vertices * 3);

			//Load Faces
			if (scene->mMeshes[i]->HasFaces())
			{

				mesh->num_indices = scene->mMeshes[i]->mNumFaces * 3;
				mesh->indices = new uint[mesh->num_indices]; // assume each face is a triangle


				for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
				{
					//Check that faces are triangles
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3) {
					}
					else {
						memcpy(&mesh->indices[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint));
					}
				}
				LoadMesh(mesh);
				Meshes* component = new Meshes(meshGO);
				
				mesh->OBmesh = meshGO;
				component->mesh = mesh;
				if (meshGO->Comp.size() > 1) {
					meshGO->Comp.push_back(component);
				}
			}
			else {
				delete mesh;
			}
		}

		aiReleaseImport(scene);
		return meshGO;
	}

}

GameObject* ModuleLoadFBX::PrimitivesObjects(int Case)
{
	//draw Primitives
	GameObject* child;
	switch (Case)
	{
	case 1:
		if (App->imgui->Selected == nullptr)
		{
			child = new GameObject(App->imgui->RootGO);
		}

		else if (App->imgui->Selected != nullptr)
		{
			child = new GameObject(App->imgui->Selected);
		}
		break;
	case 2:
		MeshObject = LoadFile("Assets/Primitives/cube2.fbx", "Cube");
		break;
	case 3:
		MeshObject = LoadFile("Assets/Primitives/Plane.fbx", "Plane");
		break;
	case 4:
		MeshObject = LoadFile("Assets/Primitives/Pyramid.fbx", "Pyramid");
		break;
	case 5:
		MeshObject = LoadFile("Assets/Primitives/Sphere.fbx", "Sphere");
		break;
	case 6:
		MeshObject = LoadFile("Assets/Primitives/Cylinder.fbx", "Cylinder");
		break;
	}
	return nullptr;
}




void ModuleLoadFBX::LoadMesh(MyMesh* mesh) {


	glGenBuffers(1, (GLuint*)&(mesh->id_vertices));
	glGenBuffers(1, (GLuint*)&(mesh->id_indices));


	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, mesh->vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_indices, mesh->indices, GL_STATIC_DRAW);

	glDisableClientState(GL_VERTEX_ARRAY);

	meshes.push_back(mesh);


}

update_status ModuleLoadFBX::PostUpdate(float dt)
{

	for (int i = 0; i < meshes.size(); i++) {
		//if (App->imgui->Wireframe == true) {
		//	//Wireframe Mode
		//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//	glLineWidth(2);
		//}
		if (meshes[i]->IsVisible == false)
		{
			meshes[i]->Render();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return UPDATE_CONTINUE;
}
bool ModuleLoadFBX::CleanUp()
{
	for (int i = 0; i < meshes.size(); i++) {
		delete meshes[i];
		meshes[i] = nullptr;
	}
	meshes.clear();
	aiDetachAllLogStreams();
	return true;
}