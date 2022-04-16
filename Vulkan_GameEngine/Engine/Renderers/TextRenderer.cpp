#include "TextRenderer.h"

std::unique_ptr<TextRenderer> TextRenderer::Instance = nullptr;
FT_Library TextRenderer::FontLibrary;

TextRenderer* TextRenderer::GetInstance()
{
	if (Instance.get() == nullptr) {
		Instance.reset(new TextRenderer);
	}
	return Instance.get();
}

TextRenderer::TextRenderer()
{
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::CreateTexture(struct S_Texture& texture, unsigned char* pixels, unsigned int imageWidth, unsigned int imageHeight)
{
    //Pixels have valid dimensions
    if (imageWidth > 0 && imageHeight > 0)
    {
        //Copy pixels
        unsigned int size = imageWidth * imageHeight;
        texture.Pixels = new unsigned char[size];
        memcpy(texture.Pixels, pixels, size);

        //Copy pixel data
        texture.Width = imageWidth;
        texture.Height = imageHeight;

        //Set pixel format
        texture.BytesPerPixel = 1;//TODO not sure if this is right
    }
}

bool TextRenderer::LoadFont(FT_Library& freeType, std::string path, const unsigned int numberOfCharacters)
{
	FT_Face fontFace;
	if (FT_New_Face(freeType, path.c_str(), 0, &fontFace)) return false;

	// set height and width of font
	if (FT_Set_Pixel_Sizes(fontFace, 0, Size)) return false; // width will dinamically resize if set to 0

	// generate texture for each character
	for (unsigned char c = 0; c < numberOfCharacters; c++)
	{
		if (FT_Load_Char(fontFace, c, FT_LOAD_RENDER)) continue;
		Characters[c] = S_Character();
		CreateTexture(Characters[c].Texture, fontFace->glyph->bitmap.buffer, fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows);
		Characters[c].Bearing = IVector2(fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top);
		Characters[c].Advance = IVector2(fontFace->glyph->advance.x, fontFace->glyph->advance.y);
	}

	FT_Done_Face(fontFace);
	return true;

//    //Error flag
//    FT_Error error = NULL;
//
//    //Get cell dimensions
//    unsigned int cellW = 0;
//    unsigned int cellH = 0;
//    int maxBearing = 0;
//    int minHang = 0;
//
//    //Character data
//    S_Texture bitmaps[256];
//    FT_Glyph_Metrics metrics[256];
//
//    //Load face
//    FT_Face face = NULL;
//    error = FT_New_Face(freeType, path.c_str(), 0, &face);
//    if (!error)
//    {
//        //Set face size
//        error = FT_Set_Pixel_Sizes(face, 0, Size);
//        if (!error)
//        {
//            //Go through extended ASCII to get glyph data
//            for (int i = 0; i < 256; ++i)
//            {
//                //Load and render glyph
//                error = FT_Load_Char(face, i, FT_LOAD_RENDER);
//                if (!error)
//                {
//                    //Get metrics
//                    metrics[i] = face->glyph->metrics;
//
//                    //Copy glyph bitmap
//                    CreateTexture(bitmaps[i], face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows);
//
//                    //Calculate max bearing
//                    if (metrics[i].horiBearingY / 64 > maxBearing)
//                    {
//                        maxBearing = metrics[i].horiBearingY / 64;
//                    }
//
//                    //Calculate max width
//                    if (metrics[i].width / 64 > cellW)
//                    {
//                        cellW = metrics[i].width / 64;
//                    }
//
//                    //Calculate gylph hang
//                    int glyphHang = (metrics[i].horiBearingY - metrics[i].height) / 64;
//                    if (glyphHang < minHang)
//                    {
//                        minHang = glyphHang;
//                    }
//                }
//                else
//                {
//                    printf("Unable to load glyph. FreeType error:%X\n", error);
//                    error = NULL;
//                }
//            }
//
//
//
//            //Create bitmap font
//            cellH = maxBearing - minHang;
//            createPixels8(cellW * 16, cellH * 16);
//
//            //Begin creating bitmap font
//            GLuint currentChar = 0;
//            LFRect nextClip = { 0.f, 0.f, cellW, cellH };
//
//            //Blitting coordinates
//            int bX = 0;
//            int bY = 0;
//
//            //Go through cell rows
//            for (unsigned int rows = 0; rows < 16; rows++)
//            {
//                //Go through each cell column in the row
//                for (unsigned int cols = 0; cols < 16; cols++)
//                {
//                    //Set base offsets
//                    bX = cellW * cols;
//                    bY = cellH * rows;
//
//                    //Initialize clip
//                    nextClip.x = bX;
//                    nextClip.y = bY;
//                    nextClip.w = metrics[currentChar].width / 64;
//                    nextClip.h = cellH;
//
//                    //Blit character
//                    bitmaps[currentChar].blitPixels8(bX, bY + maxBearing - metrics[currentChar].horiBearingY / 64, *this);
//
//                    //Go to the next character
//                    mClips.push_back(nextClip);
//                    currentChar++;
//                }
//            }
//
//            //Make texture power of two
//            padPixels8();
//
//            //Create texture
//            if (loadTextureFromPixels8())
//            {
//                //Build vertex buffer from sprite sheet data
//                if (!generateDataBuffer(LSPRITE_ORIGIN_TOP_LEFT))
//                {
//                    printf("Unable to create vertex buffer for bitmap font!");
//                    error = 0xA2;
//                }
//            }
//            else
//            {
//                printf("Unable to create texture from generated bitmap font!\n");
//                error = 0xA2;
//            }
//
//            //Set font texture wrap
//            glBindTexture(GL_TEXTURE_2D, getTextureID());
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
//
//            //Set spacing variables
//            mSpace = cellW / 2;
//            mNewLine = maxBearing;
//            mLineHeight = cellH;
//        }
//        else
//        {
//            printf("Unable to set font size. FreeType error:%X\n", error);
//        }
//
//        //Free face
//        FT_Done_Face(face);
//    }
//    else
//    {
//        printf("Unable to load load font face. FreeType error:%X\n", error);
//        return false;
//    }
//
//	// load Textures
//	for (unsigned char c = 0; c < numberOfCharacters; c++)
//	{
//		// load glyph
//		if (FT_Load_Char(FontFace, c, FT_LOAD_RENDER)) continue;
//
//		// generate texture
//
//	}
//
//	return false;
}
