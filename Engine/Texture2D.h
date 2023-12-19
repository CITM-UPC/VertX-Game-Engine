#pragma once
#include "Component.h"
#include <string>
#include <memory>

#define CHECKERS_HEIGHT 100
#define CHECKERS_WIDTH  100

class Texture2D : public Component
{

private:
	unsigned int _id;
	unsigned int _id_checker;

public:
	//Constructor -- 
	Texture2D();
	//Constructor with Implementation of Component Management within GameObject Class
	Texture2D(GameObject* owner) : Component(owner), _id(0), _id_checker(0) {}
	//Grabbing Texture path and assigning to GO
	Texture2D(GameObject* owner, const std::string& path);


	Texture2D(const Texture2D& cpy);
	Texture2D(Texture2D&& tex) noexcept;

	~Texture2D();

	void bind() const;
	void unbind() const;
	void checker() const;

	void Update() override;

	Type getType() const override {
		return Type::TEXTURE2D;
	}

	std::string path;
	int width, height;

private:

	Texture2D operator=(const Texture2D&) = delete;
};

