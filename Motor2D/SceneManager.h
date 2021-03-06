#ifndef __SCENE_MANAGER__H
#define __SCENE_MANAGER__H

#include "Module.h"
#include "Scene.h"
#include "MenuScene.h"
#include "PlayScene.h"

class SceneManager;

class SceneManager : public Module
{
public:
    SceneManager();

    // Destructor
    ~SceneManager() override;

    // Called when before render is available
    bool Awake(pugi::xml_node &) override;

    // Call before first frame
    bool Start() override;

    // Called before all Updates
    bool PreUpdate() override;

    // Update Elements
    bool Update(float dt) override;

    // Called after all Updates
    bool PostUpdate() override;

    // Called before quitting
    bool CleanUp() override;

    void ChangeScene(SceneElement *last_scene, SceneElement *new_scene);

    Scene *level1_scene = nullptr;
    MenuScene *menu_scene = nullptr;
    PlayScene *play_scene = nullptr;

    list<SceneElement *> scenes;
    SceneElement *current_scene = nullptr;
};

#endif // !__SCENE_MANAGER__
