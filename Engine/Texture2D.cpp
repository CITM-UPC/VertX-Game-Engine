#include "Texture2D.h"
#include <GL/glew.h>
#include <IL/il.h>

#include <algorithm>

using namespace std;

Texture2D::Texture2D(GameObject& owner, const std::string& path) : Component(owner) {

    //load image data using devil
    auto img = ilGenImage();
    ilBindImage(img);
    ilLoadImage(path.c_str());
    //Passing of Data Values to getters
    texName = path.c_str();
    auto width = ilGetInteger(IL_IMAGE_WIDTH);
    texWidth = width;
    auto height = ilGetInteger(IL_IMAGE_HEIGHT);
    texHeight = height;
    auto channels = ilGetInteger(IL_IMAGE_CHANNELS);
    auto data = ilGetData();

    //load image as a texture in VRAM
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);
    glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    //now we can delete image from RAM
    ilDeleteImage(img);
}

Texture2D::Texture2D(Texture2D&& tex) noexcept : Component(tex.gameObject), _id(tex._id) {
    tex._id = 0;
}

Texture2D::~Texture2D() {
    if(_id) glDeleteTextures(1, &_id);
}

void Texture2D::Update()
{
}

std::string Texture2D::getName() {

    eraseBeforeDelimiter(texName);
    return texName;
}

const unsigned int Texture2D::getHeight() {
    return texHeight;
}

const unsigned int Texture2D::getWidth() {
    return texWidth;
}

void Texture2D::bind() const {
    glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture2D::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::eraseBeforeDelimiter(std::string& str) {
    size_t found = str.find_last_of("\\/");
    if (found != std::string::npos) {
        str.erase(0, found + 1);
    }
}
