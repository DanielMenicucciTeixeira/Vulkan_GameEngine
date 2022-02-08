#ifndef SDLTEXTUREHANDLER_H
#define SDLTEXTUREHANDLER_H

#include <memory>
#include <string>
#include <vector>
#include <set>

struct SDL_Surface;
struct S_Texture;

class TextureHandler
{
public:
	//----Deleted Constructors----\\

	static TextureHandler* GetInstance();

	TextureHandler(const TextureHandler&) = delete;
	TextureHandler(TextureHandler&&) = delete;
	TextureHandler& operator=(const TextureHandler&) = delete;
	TextureHandler& operator=(TextureHandler&&) = delete;

	//--------Functions--------\\
	
	void CleanUp();
	const S_Texture* GetTextureData(const std::string& textureName) const;

	//Loads a 2D image from a ginve path into a given SDL_Surface.
	static bool LoadTexture(const std::string& textureName, const std::string& filePath, S_Texture* outTexture);

private:
	TextureHandler();
	~TextureHandler();

	static std::unique_ptr<TextureHandler> Instance;
	friend std::default_delete<TextureHandler>;
	static std::vector<S_Texture*> Textures;
	
	std::set<SDL_Surface*> Surfaces;
};
#endif
