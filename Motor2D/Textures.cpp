#include "Application.h"
#include "Render.h"
#include "FileSystem.h"
#include "Textures.h"
#include "p2Log.h"
#include "Window.h"

#include <SDL2/SDL_image.h>
#pragma comment(lib, "SDL_image/libx86/SDL2_image.lib")

Textures::Textures() :
    Module()
{
    name = "textures";
}

// Destructor
Textures::~Textures()
{
}

// Called before render is available
bool Textures::Awake(pugi::xml_node &config)
{
    LOG("Init Image library", 0);
    bool ret = true;
    // load support for the PNG image format
    int flags = IMG_INIT_PNG;
    int init = IMG_Init(flags);

    if ((init & flags) != flags) {
        LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
        ret = false;
    }

    return ret;
}

// Called before the first frame
bool Textures::Start()
{
    LOG("start textures", 0);
    bool ret = true;
    return ret;
}

// Called before quitting
bool Textures::CleanUp()
{
    LOG("Freeing textures and Image library", 0);
    for (list<SDL_Texture *>::iterator it = textures.begin(); it != textures.end(); it++) {
        SDL_DestroyTexture((*it));
    }

    textures.clear();
    IMG_Quit();
    return true;
}

// Load new texture from file path
SDL_Texture *Textures::LoadTexture(const char *path)
{
    SDL_Texture *texture = nullptr;
    SDL_Surface *surface = IMG_Load_RW(App->fs->LoadFile(path), 1);

    if (surface == nullptr) {
        LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
    } else {
        texture = LoadStaticSurface(surface);
        SDL_FreeSurface(surface);
    }

    return texture;
}

SDL_Surface *Textures::LoadImageAsSurface(const char *path)
{

    SDL_Surface *surface = IMG_Load_RW(App->fs->LoadFile(path), 1);

    if (surface == nullptr) {
        LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
    }

    return surface;
}

// Unload texture
bool Textures::UnLoad(SDL_Texture *texture)
{

    for (std::list<SDL_Texture *>::iterator it = textures.begin(); it != textures.end(); it++) {
        if (texture == (*it)) {
            SDL_DestroyTexture((*it));
            textures.erase(it);
            return true;
        }
    }

    return false;
}

// Translate a surface into a static texture
SDL_Texture *Textures::LoadStaticSurface(SDL_Surface *surface)
{
    SDL_Texture *texture = SDL_CreateTextureFromSurface(App->render->renderer, surface);

    if (texture == nullptr) {
        LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
    } else {
        textures.push_back(texture);
    }

    return texture;
}

// Translate a surface into a streaming texture (can be locked/unlocked (modify pixels))
SDL_Texture *Textures::LoadStreamingTextureFromSurface(SDL_Surface *surface)
{
    SDL_Texture *texture = SDL_CreateTexture(App->render->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, surface->w, surface->h);

    if (texture == nullptr) {
        LOG("Unable to create streaming texture from surface! SDL Error: %s\n", SDL_GetError());
    } else {
        textures.push_back(texture);
    }

    return texture;
}

// Retrieve size of a texture
void Textures::GetSize(const SDL_Texture *texture, uint &width, uint &height) const
{
    SDL_QueryTexture((SDL_Texture *)texture, nullptr, nullptr, (int *)&width, (int *)&height);
}
