#ifndef VGE_SDLTEXTURELOADER_H
#define VGE_SDLTEXTURELOADER_H

class SDL_Surface;

class VGE_SDLTextureLoader
{
public:
	//--------Constructors--------\\

	VGE_SDLTextureLoader();

	//--------Destructors--------\\

	~VGE_SDLTextureLoader();

	//--------Functions--------\\

	//Loads a 2D image from a ginve path into a given SDL_Surface.
	bool LoadTexture(const char* filePath, SDL_Surface** surface);
};
#endif
