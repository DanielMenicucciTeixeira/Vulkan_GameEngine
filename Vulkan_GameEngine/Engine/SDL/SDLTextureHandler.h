#ifndef SDLTEXTUREHANDLER_H
#define SDLTEXTUREHANDLER_H

#include <memory>
#include <string>
#include <vector>

struct SDL_Surface;
struct S_Texture;

class SDLTextureHandler
{
public:
	//----Deleted Constructors----\\

	static SDLTextureHandler* GetInstance();

	SDLTextureHandler(const SDLTextureHandler&) = delete;
	SDLTextureHandler(SDLTextureHandler&&) = delete;
	SDLTextureHandler& operator=(const SDLTextureHandler&) = delete;
	SDLTextureHandler& operator=(SDLTextureHandler&&) = delete;

	//--------Functions--------\\
	
	void CleanUp();
	const S_Texture* GetTextureData(const std::string& textureName) const;

	//Loads a 2D image from a ginve path into a given SDL_Surface.
	static bool LoadTexture(const std::string& textureName, const std::string& filePath, S_Texture* outTexture);

private:
	SDLTextureHandler();
	~SDLTextureHandler();

	static std::unique_ptr<SDLTextureHandler> Instance;
	friend std::default_delete<SDLTextureHandler>;
	static std::vector<S_Texture*> Textures;
};
#endif
