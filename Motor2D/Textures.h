#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include "Module.h"

class Textures : public Module
{
public:
    Textures();

    // Destructor
    ~Textures() override;

    // Called before render is available
    bool Awake(pugi::xml_node &) override;

    // Called before the first frame
    bool Start() override;

    // Called before quitting
    bool CleanUp() override;

    // Load Texture
    SDL_Texture *LoadTexture(const char *path);
    SDL_Surface *LoadImageAsSurface(const char *path);
    bool UnLoad(SDL_Texture *texture);
    SDL_Texture *LoadStaticSurface(SDL_Surface *surface);
    SDL_Texture *LoadStreamingTextureFromSurface(SDL_Surface *surface);
    void GetSize(const SDL_Texture *texture, uint &width, uint &height) const;

public:
    list<SDL_Texture *> textures;

private:
    using Module::Load;

};

#endif // __TEXTURES_H__
