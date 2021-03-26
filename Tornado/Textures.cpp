#include "Textures.h"

#include <vector>
#include <iostream>

#include "picoPNG.h"

std::vector <Texture*> Textures::textures;
std::vector <std::string> Textures::textureNames;

GLuint Textures::LoadTexture(const char* filename, std::string textureName)
{
	// load and decode
	std::vector<unsigned char> buffer;
	std::vector<unsigned char> image;
	unsigned long width, height;

	loadFile(buffer, filename);
	
	int error = decodePNG(image, width, height, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size());

	//if there's an error, display it
	if (error != 0) std::cout << "ERROR - loading texture " << textureName << "\n";

	textures.push_back(new Texture());
	textureNames.push_back(textureName);

	// allocate a texture name
	GLuint texture;
	glGenTextures(1, &texture);

	// select our current texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);

	// when texture area is small, bilinear filter the closest mipmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	// when texture area is large, bilinear filter the first mipmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// texture should tile
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// build our texture mipmaps

	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image.data());

	unsigned index = textures.size() - 1;
	textures[index]->height = height;
	textures[index]->width = width;
	textures[index]->textureID = texture;

	return texture;
}

GLuint Textures::getTexture(std::string textureName)
{
	for (unsigned i = 0; i < textureNames.size(); i++)
	{
		if (textureNames[i].compare(textureName) == 0)
			return textures[i]->textureID;
	}
}

void Textures::clean()
{
	for (int i = 0; i < textures.size(); i++)
		delete(textures[i]);
}
