#ifndef __RENDER_H__
#define __RENDER_H__

#include <SDL2/SDL.h>
#include "Module.h"
#include "p2Point.h"

#include <cstdint>

#include <deque>

struct Sprite
{
    SDL_Rect rect = { 0, 0, 0, 0 };
    SDL_Texture *texture = nullptr;
    iPoint pos = { 0, 0 };
    int priority = 0;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    int r = 0;
    int g = 0;
    int b = 0;
    int a = 255;
    int radius = 0;
    bool filled = true;
    bool change_color = false;
};

struct CameraLimit
{
    int up, down, left, right;
};

class Render : public Module
{
public:
    Render();

    // Destructor
    ~Render() override;

    // Called before render is available
    bool Awake(pugi::xml_node &) override;

    // Called before the first frame
    bool Start() override;

    // Called each loop iteration
    bool PreUpdate() override;
    bool PostUpdate() override;

    // Called before quitting
    bool CleanUp() override;

    // Load / Save
    bool Load(pugi::xml_node &) override;
    bool Save(pugi::xml_node &) const override;

    // Utils
    void SetViewPort(const SDL_Rect &rect);
    void ResetViewPort();
    iPoint ScreenToWorld(int x, int y) const;

    // Draw & Blit
    bool Blit(SDL_Texture *texture, int x, int y, const SDL_Rect *section = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE, bool use_camera = true, float speed = 1.0f, double angle = 0, int pivot_x = INT32_MAX, int pivot_y = INT32_MAX) const;
    bool DrawQuad(const SDL_Rect &rect, Uint8 r, Uint8 g, Uint8 b, bool filled = true, Uint8 a = 255, bool use_camera = true) const;
    bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
    bool DrawCircle(int x1, int y1, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool isIsometric = false, bool use_camera = true) const;
    bool DrawIsometricRect(iPoint center, uint width) const;
    bool DrawIsometricCircle(int x1, int y1, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;

    // Set background color
    void SetBackgroundColor(SDL_Color color);

    //Move camera with cursor
    pair<int, int> MoveCameraWithCursor(float dt);
    bool CullingCam(iPoint point);

public:
    SDL_Renderer *renderer = nullptr;
    SDL_Rect camera;
    SDL_Rect viewport;
    SDL_Color background;
    SDL_Rect culling_cam;
    std::deque<Sprite> sprites_toDraw;
    std::deque<Sprite> ui_toDraw;
    Sprite cursor;

    CameraLimit cameraScene;

private:
    bool vsync = false;
};

#endif // __RENDER_H__
