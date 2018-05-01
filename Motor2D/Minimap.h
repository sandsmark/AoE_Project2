#ifndef __MINIMAP_H__
#define __MINIMAP_H__

#include "Module.h"
#include "p2Point.h"

class Minimap : public Module
{
public:
    Minimap();

    //Destructor
    ~Minimap() override;

    //Start Elements
    bool Start() override;

    // Update Elements
    bool Update(float dt) override;

    //Called before quitting
    bool CleanUp() override;

    //Initialize minimap
    void GetClickableArea(std::pair<int, int> position);

    //Draw terrain
    void DrawTerrain(int x, int y, int r, int g, int b);

    //Draw units
    void DrawUnits();

    //Draw Camera
    void DrawCamera();

private:
    SDL_Rect minimapClickable;
    iPoint minimapPos;
    SDL_Point minimapNewPos;

    double minimapRatio;

    int offsetX = 0;
    int offsetY = 0;
};

#endif //__MINIMAP_H__