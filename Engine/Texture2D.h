#pragma once
#include "Component.h"
#include <string>
#include <memory>

class Texture2D : public Component
{

private:
	unsigned int _id;
	std::string texName;

	unsigned int texHeight;
	unsigned int texWidth;

public:
	Texture2D(GameObject& owner) : Component(owner), _id(0) {}

	explicit Texture2D(GameObject& owner, const std::string& path);
	Texture2D(Texture2D&& tex) noexcept;
	void bind() const;
	void unbind() const;
	~Texture2D();

	void Update() override;
	std::string getName();
	void eraseBeforeDelimiter(std::string& str);
	const unsigned int getWidth();
	const unsigned int getHeight();

	//Assigning of Type for later IMGUI Sorting
	Type getType() const override {
		return Type::TEXTURE;
	}

private:
	Texture2D(const Texture2D&);
	Texture2D operator=(const Texture2D&) = delete;;

};

