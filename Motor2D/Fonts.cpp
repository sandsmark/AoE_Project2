#include "Application.h"
#include "Textures.h"
#include "FileSystem.h"
#include "Fonts.h"
#include "p2Log.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

Fonts::Fonts() :
    Module()
{
    name = "fonts";
}

// Destructor
Fonts::~Fonts()
{
}

// Called before render is available
bool Fonts::Awake(pugi::xml_node &conf)
{
    LOG("Init True Type Font library", 0);
    bool ret = true;

    if (TTF_Init() == -1) {
        LOG("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        ret = false;
    } else {
        default_path = conf.child("default_font").attribute("file").as_string(DEFAULT_FONT);
        int size = conf.child("default_font").attribute("size").as_int(DEFAULT_FONT_SIZE);
        defaultFont = LoadFile(default_path, size);
    }

    return ret;
}

bool Fonts::Start()
{
    LoadFile(nullptr, 16);
    LoadFile(nullptr, 20);
    LoadFile(nullptr, 14);
    LoadFile(nullptr, 18);
    LoadFile(nullptr, 26);
    LoadFile(nullptr, 80);
    return true;
}

// Called before quitting
bool Fonts::CleanUp()
{
    LOG("Freeing True Type fonts and library", 0);

    for (vector<TTF_Font *>::iterator it = fonts.begin(); it != fonts.end(); ++it) {
        TTF_CloseFont((*it));
    }

    fonts.clear();
    TTF_Quit();
    return true;
}

// Load new texture from file path
TTF_Font *Fonts::LoadFile(const char *path, int size)
{
    TTF_Font *font;
    if (path != nullptr) {
        font = TTF_OpenFontRW(App->fs->LoadFile(path), 1, size);
    } else {
        font = TTF_OpenFontRW(App->fs->LoadFile(default_path), 1, size);
    }
    if (font == nullptr) {
        LOG("Could not load TTF font with path: %s. TTF_OpenFont: %s", path, TTF_GetError());
    } else {
        LOG("Successfully loaded font %s size %d", path, size);
        fonts.push_back(font);
    }

    return font;
}

// Print text using font
SDL_Texture *Fonts::Print(const char *text, SDL_Color color, _TTF_Font *font)
{
    SDL_Texture *ret = nullptr;
    SDL_Surface *surface = TTF_RenderText_Blended((font) ? font : defaultFont, text, color);

    if (surface == nullptr) {
        LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
        ret = App->tex->LoadStaticSurface(surface);
        SDL_FreeSurface(surface);
    }

    return ret;
}

// calculate size of a text
bool Fonts::CalcSize(const char *text, int &width, int &height, _TTF_Font *font) const
{
    bool ret = false;

    if (TTF_SizeText((font) ? font : defaultFont, text, &width, &height) != 0) {
        LOG("Unable to calc size of text surface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
        ret = true;
    }

    return ret;
}

bool Fonts::DeleteFont(_TTF_Font *font)
{
    bool ret = true;
    if (font != nullptr) {
        if (font != defaultFont) {
            for (vector<_TTF_Font *>::iterator it = fonts.begin(); it != fonts.end(); ++it) {

                if ((*it) == font) {
                    TTF_CloseFont(font);
                    fonts.erase(it);
                }
            }
        } else {
            LOG("Cannot delete default font :S", 0);
            ret = false;
        }
    } else {
        LOG("Cannot delete font.", 0);
        ret = false;
    }
    return ret;
}
