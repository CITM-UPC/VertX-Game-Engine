#pragma once
#include <vector>
#include <memory>
#include <string>

#include "Globals_ENGINE.h"
#include "Graphic.h"
#include "Texture2D.h"
#include "Component.h"


class Mesh : public Graphic, public Component
{
public:
	enum Formats { F_V3, F_V3C4, F_V3T2 };
	struct V3 { vec3f v; };
	struct V3C4 { vec3f v; vec4f c; };
	struct V3T2 { vec3f v; vec2f t; };

	bool usingTexture = true;
	Texture2D* texture = nullptr;

private:
	std::string meshName;
	const enum Formats _format;

	unsigned int _vertex_buffer_id;
	const unsigned int _numVerts;

	unsigned int _indexs_buffer_id;
	const unsigned int _numIndexs, _numNormals, _numFaces, _numTexCoords;

public:
	Mesh(GameObject& owner) : Component(owner), _format(Formats::F_V3), _numVerts(0), _numIndexs(0), _numTexCoords(0), _numNormals(0), _numFaces(0) {
	}

	bool VertexNormDraw = false;
	bool FaceNormDraw = false;

	float normalsLength = 1.0f;

	std::vector<vec3f> meshVertices;
	std::vector<vec3f> meshNormals;
	std::vector<vec3f> meshFaceCenters;
	std::vector<vec3f> meshFaceNormals;

	Mesh(GameObject& owner, Formats format, const void* vertex_data, unsigned int numVerts, const unsigned int* indexs_data = nullptr, unsigned int numIndexs = 0, const unsigned int numTexCoords = 0, unsigned int numNormals = 0, unsigned int numFaces = 0);
	Mesh(Mesh&& b) noexcept;
	Mesh(const Mesh& cpy);
	void draw();
	~Mesh();

	std::string getName();
	void setName(std::string name);

	//Getters
	const unsigned int getNumVerts();
	const unsigned int getNumIndexs();
	const unsigned int getNumTexCoords();
	const unsigned int getNumNormals();
	const unsigned int getNumFaces();

	void Update() override;

	//Assigning of Type for later IMGUI Sorting
	Type getType() const override {
		return Type::MESH;
	}

private:
	Mesh& operator=(const Mesh&) = delete;
};

