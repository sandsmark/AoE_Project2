#ifndef _CONSOLE_H
#define _CONSOLE_H_

#include "Module.h"
#include "Fonts.h"
#include "Gui.h"
#include <list>
using namespace std;

class Console : public Module
{
public:
    Console();

    // Destructor
    ~Console() override;

    // Called when before render is available
    bool Awake(pugi::xml_node &) override;

    // Call before first frame
    bool Start() override;

    // Called before all Updates
    bool PreUpdate() override;

    // Called after all Updates
    bool Update(float dt) override;

    // Called before quitting
    bool CleanUp() override;

    // Load and save before
    bool Save(pugi::xml_node &) const override;

public:
    bool show = false;
    int window_w = 0, window_h = 0;
    list<string> ctext;
    list<UIElement *> console_elements;
    Quad *cbackground = nullptr;
    Quad *cinput = nullptr;
    ScrollBar *cscroll = nullptr;
    InputText *ctextinput = nullptr;
    int h = 0;

private:
    void SetConsoleText();
    void MoveEverything();
};

#endif // CONSOLE_H
