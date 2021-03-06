#ifndef __FONTS_H__
#define __FONTS_H__

#include "Module.h"
#include <SDL2/SDL_pixels.h>

#define DEFAULT_FONT "fonts/open_sans/timesbd.ttf"
#define DEFAULT_FONT_SIZE 12

struct SDL_Texture;
struct _TTF_Font;

class Fonts : public Module
{
public:
    Fonts();

    // Destructor
    ~Fonts() override;

    bool Start() override;
    // Called before render is available
    bool Awake(pugi::xml_node &) override;

    // Called before quitting
    bool CleanUp() override;

    // Load Font
    _TTF_Font *LoadFile(const char *path, int size = 12);

    // Create a surface from text
    SDL_Texture *Print(const char *text, SDL_Color color = { 255, 255, 255, 255 }, _TTF_Font *font = nullptr);

    bool CalcSize(const char *text, int &width, int &height, _TTF_Font *font = nullptr) const;
    bool DeleteFont(_TTF_Font *font = nullptr);

public:
    vector<_TTF_Font *> fonts;

    _TTF_Font *defaultFont = nullptr;

private:
    const char *default_path;
};

enum fontslist { EIGHT,
                 SIXTEEN,
                 TWENTY,
                 FOURTEEN,
                 EIGHTEEN,
                 TWENTYSIX,
                 EIGHTY };

#endif // __FONTS_H__
