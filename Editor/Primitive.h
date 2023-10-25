//#pragma once
//#include "glmath.h"
//#include "glm/glm.hpp"
//#include "Color.h"
//#include <glm/gtc/type_ptr.hpp>
//
//enum PrimitiveTypes
//{
//	Primitive_Point,
//	Primitive_Line,
//	Primitive_Plane,
//	Primitive_Cube,
//	Primitive_Sphere,
//	Primitive_Cylinder
//};
//
//class Primitive
//{
//public:
//
//	Primitive();
//
//	virtual void	Render() const;
//	virtual void	InnerRender() const;
//	void			SetPos(float x, float y, float z);
//	void			SetRotation(float angle, const glm::vec3& u);
//	void			Scale(float x, float y, float z);
//	PrimitiveTypes	GetType() const;
//
//public:
//
//	//Color color;
//	glm::mat4x4 transform;
//	const GLfloat* transformFloat = glm::value_ptr(transform);
//
//	bool axis, wire;
//
//protected:
//	PrimitiveTypes type;
//};
//
//// ============================================
//class Cube : public Primitive
//{
//public:
//	Cube();
//	Cube(float sizeX, float sizeY, float sizeZ);
//	void InnerRender() const;
//public:
//	glm::vec3 size;
//};
//
//// ============================================
//class Sphere : public Primitive
//{
//public:
//	Sphere();
//	Sphere(float radius);
//	void InnerRender() const;
//public:
//	float radius;
//};
//
//// ============================================
//class Cylinder : public Primitive
//{
//public:
//	Cylinder();
//	Cylinder(float radius, float height);
//	void InnerRender() const;
//public:
//	float radius;
//	float height;
//};
//
//// ============================================
//class Line : public Primitive
//{
//public:
//	Line();
//	Line(float x, float y, float z);
//	void InnerRender() const;
//public:
//	glm::vec3 origin;
//	glm::vec3 destination;
//};
//
//// ============================================
//class Grid : public Primitive
//{
//public:
//	Grid();
//	Grid(float x, float y, float z, float d);
//	void InnerRender() const;
//public:
//	glm::vec3 normal;
//	float constant;
//};
