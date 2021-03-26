#pragma once
#include <GL\freeglut.h>
#include <vector>
#include <string>

// struktura tekstury
struct Texture
{
	GLuint textureID;
	unsigned long width;
	unsigned long height;
};

static class Textures
{
public:
	static GLuint LoadTexture(const char* filename, std::string textureName);
	static GLuint getTexture(std::string textureName);
	static void clean();
private:
	static std::vector <Texture*> textures;
	static std::vector <std::string> textureNames;
};

