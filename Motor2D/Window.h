#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

class Window : public Module
{
public:
    Window();

    // Destructor
    ~Window() override;

    // Called before render is available
    bool Awake(pugi::xml_node &) override;

    // Called before quitting
    bool CleanUp() override;

    // Changae title
    void SetTitle(const char *new_title);

    // Retrive window size
    void GetWindowSize(int &width, int &height) const;

    // Retrieve window scale
    uint GetScale() const;

    bool IsFullScreen() const;

public:
    //The window we'll be rendering to
    SDL_Window *window;

    //The surface contained by the window
    SDL_Surface *screen_surface;

    bool ChangeToFullScreen();
    bool ChangeToWindow();

private:
    bool fullscreen = false;
    string title;
    uint width = 0;
    uint height = 0;
    uint scale = 0;
};

#endif // __WINDOW_H__