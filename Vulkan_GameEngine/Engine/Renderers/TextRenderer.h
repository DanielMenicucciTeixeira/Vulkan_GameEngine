#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "Math/IVector2.h"
#include "RenderObject.h"

#include <memory>
#include <unordered_map>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

struct S_Character
{
    S_Texture Texture;			// glyph texture 
    IVector2   Bearing;         // Offset from baseline to left/top of glyph
	IVector2 Advance;       // Offset to advance to next glyph
};

class TextRenderer
{
	//-------Singleton Setup----------------------
public:
	static TextRenderer* GetInstance();

	TextRenderer(const TextRenderer&) = delete;
	TextRenderer(TextRenderer&&) = delete;
	TextRenderer& operator=(const TextRenderer&) = delete;
	TextRenderer& operator=(TextRenderer&&) = delete;

private:
	TextRenderer();
	~TextRenderer();
	static std::unique_ptr<TextRenderer> Instance;
	friend std::default_delete<TextRenderer>;
	//----------------------------------------------

	std::unordered_map<char, S_Character> Characters;
	unsigned int Size;
	
	//Font TTF library
	static FT_Library FontLibrary;
	void CreateTexture(struct S_Texture& texture, unsigned char* pixels, unsigned int imageWidth, unsigned int imageHeight);

public:
	bool LoadFont(FT_Library& freeType, std::string path, const unsigned int numberOfCharacters = 128);
	void CleanUp();
};

#endif

