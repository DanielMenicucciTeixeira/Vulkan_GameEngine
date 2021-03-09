#ifndef SDLTEXTURELOADER_H
#define SDLTEXTURELOADER_H

class SDL_Surface;

class SDLTextureLoader
{
public:
	//--------Constructors--------\\

	SDLTextureLoader();

	//--------Destructors--------\\

	~SDLTextureLoader();

	//--------Functions--------\\

	//Loads a 2D image from a ginve path into a given SDL_Surface.
	bool LoadTexture(const char* filePath, SDL_Surface** surface);
};
#endif
