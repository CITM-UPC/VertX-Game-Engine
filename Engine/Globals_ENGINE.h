#pragma once
#include <windows.h>
#include <stdio.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

typedef unsigned int uint;

enum engine_update_status
{
	ENGINE_UPDATE_CONTINUE = 1,
	ENGINE_UPDATE_STOP,
	ENGINE_UPDATE_ERRORF
};

// GLM library type definitions --------
/* DOUBLE VECTORS */
typedef glm::dvec2 vec2;
typedef glm::dvec3 vec3;
typedef glm::dvec4 vec4;

/* FLOAT VECTORS */
typedef glm::vec2 vec2f;
typedef glm::vec3 vec3f;
typedef glm::vec4 vec4f;

/* DOUBLE MATRIXS */
typedef glm::dmat2x2 mat2;
typedef glm::dmat3x3 mat3;
typedef glm::dmat4x4 mat4;

/* FLOAT MATRIXS */
typedef glm::mat2x2 mat2f;
typedef glm::mat3x3 mat3f;
typedef glm::mat4x4 mat4f;

