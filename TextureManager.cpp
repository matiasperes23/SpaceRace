//**********************************************
//Singleton Texture Manager class
//Written by Ben English
//benjamin.english@oit.edu
//
//For use with OpenGL and the FreeImage library
//**********************************************

#include "TextureManager.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "imageloader.h"

TextureManager* TextureManager::m_inst(0);

TextureManager* TextureManager::Inst()
{
	if(!m_inst)
		m_inst = new TextureManager();

	return m_inst;
}

TextureManager::TextureManager()
{
	// call this ONLY when linking with FreeImage as a static library
	#ifdef FREEIMAGE_LIB
		FreeImage_Initialise();
	#endif
}

//these should never be called
//TextureManager::TextureManager(const TextureManager& tm){}
//TextureManager& TextureManager::operator=(const TextureManager& tm){}
	
TextureManager::~TextureManager()
{
	// call this ONLY when linking with FreeImage as a static library
	#ifdef FREEIMAGE_LIB
		FreeImage_DeInitialise();
	#endif

	UnloadAllTextures();
	m_inst = 0;
}

bool TextureManager::LoadTexture(const char* filename, const unsigned int texID, GLenum image_format, GLint internal_format, GLint level, GLint border)
{
    SDL_Surface* surface = IMG_Load(filename);
    GLenum texture_format;
    GLint  nOfColors;
    GLuint texture;
    
    
    if (surface == NULL)
    {
        fprintf(stderr, "Error al cargar la imagen de textura: %s\n", SDL_GetError());
    }
    
     nOfColors = surface->format->BytesPerPixel;
    if (nOfColors == 4) // contains an alpha channel
    {
        if (surface->format->Rmask == 0x000000ff)
            texture_format = GL_RGBA;
        else
            texture_format = GL_BGRA;
    }
    else if (nOfColors == 3) // no alpha channel
    {
        if (surface->format->Rmask == 0x000000ff)
            texture_format = GL_RGB;
        else
            texture_format = GL_BGR;
    }
    else {
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
    }
    
    glGenTextures(1, &texture);
    m_texID[texID] = texture;
    glBindTexture(GL_TEXTURE_2D, texture);
    
    if (border == 0)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    }
    else if (border == 1)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                      texture_format, GL_UNSIGNED_BYTE, surface->pixels);
   
    SDL_FreeSurface(surface);
    
    return true;
}

bool TextureManager::LoadTextureBMP(const char* filename, const unsigned int texID)
{
    /* Status indicator */
    int Status = FALSE;

    /* Create storage space for the texture */
    SDL_Surface * TextureImage;
    GLuint texture;
    /* Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit */
    if ((TextureImage = SDL_LoadBMP(filename))) {

        /* Set the status to true */
        Status = TRUE;

        /* Create The Texture */
        glGenTextures(1, &texture);
        m_texID[texID] = texture;
        
        /* Typical Texture Generation Using Data From The Bitmap */
        glBindTexture(GL_TEXTURE_2D, texture);

        /* Generate The Texture */
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->w,
                TextureImage->h, 0, GL_BGR,
                GL_UNSIGNED_BYTE, TextureImage->pixels);

        /* Linear Filtering */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    /* Free up any memory we may have used */
    if (TextureImage)
        SDL_FreeSurface(TextureImage);

    return Status;
}

bool TextureManager::LoadTextureAlphaTexture(const char* filenameImage, const char* filenameAlphaChannel,const unsigned int texID)
{
    Image* image = loadBMP(filenameImage);
    Image* alphaChannel = loadBMP(filenameAlphaChannel);
    
    
    char* pixels = new char[image->width * image->height * 4];
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            for (int j = 0; j < 3; j++) {
                pixels[4 * (y * image->width + x) + j] =
                        image->pixels[3 * (y * image->width + x) + j];
            }
            pixels[4 * (y * image->width + x) + 3] =
                    alphaChannel->pixels[3 * (y * image->width + x)];
        }
    }

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D,
            0,
            GL_RGBA,
            image->width, image->height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            pixels);

    delete pixels;
    
    m_texID[texID] = textureId;
    delete image;
    delete alphaChannel;
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


bool TextureManager::UnloadTexture(const unsigned int texID)
{
	bool result(true);
	//if this texture ID mapped, unload it's texture, and remove it from the map
	if(m_texID.find(texID) != m_texID.end())
	{
		glDeleteTextures(1, &(m_texID[texID]));
		m_texID.erase(texID);
	}
	//otherwise, unload failed
	else
	{
		result = false;
	}

	return result;
}

bool TextureManager::BindTexture(const unsigned int texID)
{
	bool result(true);
	//if this texture ID mapped, bind it's texture as current
	if(m_texID.find(texID) != m_texID.end())
		glBindTexture(GL_TEXTURE_2D, m_texID[texID]);
	//otherwise, binding failed
	else
		result = false;

	return result;
}

void TextureManager::UnloadAllTextures()
{
	//start at the begginning of the texture map
	std::map<unsigned int, GLuint>::iterator i = m_texID.begin();

	//Unload the textures untill the end of the texture map is found
	while(i != m_texID.end())
		UnloadTexture(i->first);

	//clear the texture map
	m_texID.clear();
}
