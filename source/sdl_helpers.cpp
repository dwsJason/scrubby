//
// SDL Helper Functions
//
// Stuff that's specifically for SDL, that I wish was built in
//

#include "sdl_helpers.h"

#include "log.h"

#include <map>

//------------------------------------------------------------------------------
/* Quick utility function for texture creation */
static int
power_of_two(int input)
{
    int value = 1;

    while (value < input) {
        value <<= 1;
    }
    return value;
}

GLuint
SDL_GL_LoadTexture(SDL_Surface * surface, GLfloat * texcoord)
{
    GLuint texture;
    int w, h;
    SDL_Surface *image;
    SDL_Rect area;
    SDL_BlendMode saved_mode;

    /* Use the surface width and height expanded to powers of 2 */
    w = power_of_two(surface->w);
    h = power_of_two(surface->h);
    texcoord[0] = 0.0f;         /* Min X */
    texcoord[1] = 0.0f;         /* Min Y */
    texcoord[2] = (GLfloat) surface->w / w;     /* Max X */
    texcoord[3] = (GLfloat) surface->h / h;     /* Max Y */

    image = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN     /* OpenGL RGBA masks */
                                 0x000000FF,
                                 0x0000FF00, 0x00FF0000, 0xFF000000
#else
                                 0xFF000000,
                                 0x00FF0000, 0x0000FF00, 0x000000FF
#endif
        );
    if (image == NULL) {
        return 0;
    }

    /* Save the alpha blending attributes */
    SDL_GetSurfaceBlendMode(surface, &saved_mode);
    SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);

    /* Copy the surface into the GL texture image */
    area.x = 0;
    area.y = 0;
    area.w = surface->w;
    area.h = surface->h;
    SDL_BlitSurface(surface, &area, image, &area);

    /* Restore the alpha blending attributes */
    SDL_SetSurfaceBlendMode(surface, saved_mode);

    /* Create an OpenGL texture for the image */
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    SDL_FreeSurface(image);     /* No longer needed */

    return texture;
}

//------------------------------------------------------------------------------

int SDL_Surface_CountUniqueColors(SDL_Surface* pSurface, std::map<Uint32,Uint32>* pGlobalHistogram )
{
	int width  = pSurface->w;
	int height = pSurface->h;

    SDL_Surface *pImage = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height,
											   32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN     /* OpenGL RGBA masks */
                                 0x000000FF,
                                 0x0000FF00, 0x00FF0000, 0xFF000000
#else
                                 0xFF000000,
                                 0x00FF0000, 0x0000FF00, 0x000000FF
#endif
											   );

	if (nullptr == pImage)
	{
		return 0;
	}

	// Copy the source image into 32bpp format
	{
		/* Save the alpha blending attributes */
		SDL_Rect area;
		SDL_BlendMode saved_mode;
		SDL_GetSurfaceBlendMode(pSurface, &saved_mode);
		SDL_SetSurfaceBlendMode(pSurface, SDL_BLENDMODE_NONE);

		/* Copy the surface into the GL texture image */
		area.x = 0;
		area.y = 0;
		area.w = pSurface->w;
		area.h = pSurface->h;
		SDL_BlitSurface(pSurface, &area, pImage, &area);

		/* Restore the alpha blending attributes */
		SDL_SetSurfaceBlendMode(pSurface, saved_mode);
	}

    if( SDL_MUSTLOCK(pImage) )
        SDL_LockSurface(pImage);

	std::map<Uint32,Uint32> local_histogram;

	for (int y = 0; y < pImage->h; ++y)
	{
		for (int x = 0; x < pImage->w; ++x)
		{
			Uint8 * pixel = (Uint8*)pImage->pixels;
			pixel += (y * pImage->pitch) + (x * sizeof(Uint32));

			Uint32 color = *((Uint32*)pixel);

			local_histogram[ color ] = 1;
			if (pGlobalHistogram)
			{
				(*pGlobalHistogram)[color] = 1;
			}
		}
	}

    if( SDL_MUSTLOCK(pImage) )
        SDL_UnlockSurface(pImage);

    SDL_FreeSurface(pImage);     /* No longer needed */

	return (int)local_histogram.size();
}

//------------------------------------------------------------------------------

