#include <iostream>
#include <sstream>

#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "SceneManager.h"
#include "FileSystem.h"
#include "Map.h"
#include "Pathfinding.h"
#include "Fonts.h"
#include "ParticleManager.h"
#include "Application.h"
#include "p2Log.h"
#include "EntityManager.h"
#include "Collision.h"
#include "Gui.h"
#include "FogOfWar.h"
#include "AI.h"
#include "Minimap.h"
#include "QuestManager.h"
#include "CutSceneManager.h"
#include "Video.h"

// Constructor
Application::Application(int argc, char *args[]) :
    argc(argc), args(args)
{
    PERF_START(ptimer);

    input = new Input();
    win = new Window();
    render = new Render();
    tex = new Textures();
    audio = new Audio();
    sceneManager = new SceneManager();
    fs = new FileSystem();
    map = new Map();
    pathfinding = new PathFinding();
    font = new Fonts();
    particlemanager = new ParticleManager();
    entityManager = new EntityManager();
    collision = new Collision();
    gui = new Gui();
    fog = new FogOfWar();
    ai = new AI();
    minimap = new Minimap();
    quest = new QuestManager();
    cutscene = new CutSceneManager();
    video = new Video();

    // Ordered for awake / Start / Update
    // Reverse order of CleanUp
    AddModule(fs);
    AddModule(input);
    AddModule(win);
    AddModule(tex);
    AddModule(audio);
    AddModule(map);
    AddModule(pathfinding);
    AddModule(font);
    AddModule(particlemanager);
    AddModule(gui);
    AddModule(ai);
    AddModule(cutscene);
    AddModule(video);

    // scene last
    AddModule(sceneManager);

    AddModule(entityManager);
    AddModule(collision);
    AddModule(fog);
    AddModule(minimap);
    AddModule(quest);

    // render last to swap buffer
    AddModule(render);

    PERF_PEEK(ptimer);
}

// Destructor
Application::~Application()
{
    // release modules
    for (list<Module *>::iterator it = modules.begin(); it != modules.end(); it++) {
        RELEASE(*it);
    }

    modules.clear();
}

void Application::AddModule(Module *module)
{
    module->Init();
    modules.push_back(module);
}

// Called before render is available
bool Application::Awake()
{
    PERF_START(ptimer);

    pugi::xml_document config_file;
    pugi::xml_node config;
    pugi::xml_node app_config;

    bool ret = false;

    config = LoadConfig(config_file);

    if (config.empty() == false) {
        // self-config
        ret = true;
        app_config = config.child("app");
        title = app_config.child("title").child_value();
        organization = app_config.child("organization").child_value();

        int cap = app_config.attribute("framerate_cap").as_int(-1);

        if (cap > 0) {
            capped_ms = 1000 / cap;
        }
    }

    if (ret == true) {
        for (Module *module : modules) {
            std::cout << "initializing " << module->name << std::endl;
            pugi::xml_node node = config.child(module->name.c_str());
            ret = module->Awake(node);
        }
    }

    PERF_PEEK(ptimer);

    return ret;
}

// Called before the first frame
bool Application::Start()
{
    PERF_START(ptimer);
    bool ret = true;

    for (list<Module *>::iterator it = modules.begin(); it != modules.end(); it++) {
        if ((*it)->active) {
            ret = (*it)->Start();
        }
    }

    startup_time.Start();

    PERF_PEEK(ptimer);

    return ret;
}

// Called each loop iteration
bool Application::Update()
{
    bool ret = true;
    PrepareUpdate();

    if (ret == true) {
        ret = PreUpdate();
    }

    if (ret == true) {
        ret = DoUpdate();
    }

    if (ret == true) {
        ret = PostUpdate();
    }

    FinishUpdate();

    if (quit || input->GetWindowEvent(WE_QUIT) == true) {
        ret = false;
    }

    return ret;
}

// ---------------------------------------------
pugi::xml_node Application::LoadConfig(pugi::xml_document &config_file) const
{
    pugi::xml_node ret;

    char *buf = nullptr;
    int size = App->fs->LoadFile("config.xml", &buf);
    pugi::xml_parse_result result = config_file.load_buffer(buf, size);
    RELEASE_ARRAY(buf);

    if (result == NULL) {
        LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
    } else {
        ret = config_file.child("config");
    }

    return ret;
}

pugi::xml_node Application::LoadGameDataFile(pugi::xml_document &gameDatafile) const
{
    pugi::xml_node ret;

    char *buf = nullptr;
    int size = App->fs->LoadFile("GameData.xml", &buf);
    pugi::xml_parse_result result = gameDatafile.load_buffer(buf, size);
    RELEASE_ARRAY(buf);

    if (result == NULL) {
        LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
    } else {
        ret = gameDatafile.child("GameData");
    }

    return ret;
}

pugi::xml_node Application::LoadHUDDataFile(pugi::xml_document &HUDDatafile) const
{
    pugi::xml_node ret;

    char *buf = nullptr;
    int size = App->fs->LoadFile("HUDData.xml", &buf);
    pugi::xml_parse_result result = HUDDatafile.load_buffer(buf, size);
    RELEASE_ARRAY(buf);

    if (result == NULL) {
        LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
    } else {
        ret = HUDDatafile.child("HUDData");
    }

    return ret;
}

pugi::xml_node Application::LoadParticleDataFile(pugi::xml_document &ParticleDatafile) const
{
    pugi::xml_node ret;

    char *buf = nullptr;
    int size = App->fs->LoadFile("ParticleData.xml", &buf);
    pugi::xml_parse_result result = ParticleDatafile.load_buffer(buf, size);
    RELEASE_ARRAY(buf);

    if (result == NULL) {
        LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
    } else {
        ret = ParticleDatafile.child("ParticleData");
    }

    return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
    frame_count++;
    last_sec_frame_count++;

    dt = frame_time.ReadSec();
    frame_time.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
    if (want_to_save == true) {
        SavegameNow();
    }

    if (want_to_load == true) {
        LoadGameNow();
    }

    // Framerate calculations --

    if (last_sec_frame_time.Read() > 1000) {
        last_sec_frame_time.Start();
        prev_last_sec_frame_count = last_sec_frame_count;
        last_sec_frame_count = 0;
    }

    float avg_fps = float(frame_count) / startup_time.ReadSec();
    float seconds_since_startup = startup_time.ReadSec();
    uint32 last_frame_ms = frame_time.Read();
    uint32 frames_on_last_update = prev_last_sec_frame_count;

    static char title[256];
    //sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %lu ",
    //		  avg_fps, last_frame_ms, frames_on_last_update, dt, seconds_since_startup, frame_count);
    //App->win->SetTitle(title);

    if (capped_ms > 0 && last_frame_ms < capped_ms) {
        PerfTimer t;
        SDL_Delay(capped_ms - last_frame_ms);
        LOG("We waited for %d milliseconds and got back in %f", capped_ms - last_frame_ms, t.ReadMs());
    }
}

// Call modules before each loop iteration
bool Application::PreUpdate()
{
    bool ret = true;
    for (list<Module *>::iterator it = modules.begin(); it != modules.end(); it++) {
        if ((*it)->active) {
            ret = (*it)->PreUpdate();
        }
    }

    return ret;
}

// Call modules on each loop iteration
bool Application::DoUpdate()
{
    bool ret = true;
    for (list<Module *>::iterator it = modules.begin(); it != modules.end(); it++) {
        if ((*it)->active) {
            ret = (*it)->Update(dt);
        }
    }

    return ret;
}

// Call modules after each loop iteration
bool Application::PostUpdate()
{
    bool ret = true;
    for (list<Module *>::iterator it = modules.begin(); it != modules.end(); it++) {
        if ((*it)->active) {
            ret = (*it)->PostUpdate();
        }
    }

    return ret;
}

// Called before quitting
bool Application::CleanUp()
{
    PERF_START(ptimer);
    bool ret = true;
    for (list<Module *>::reverse_iterator it = modules.rbegin(); it != modules.rend(); it++) {
        ret = (*it)->CleanUp();
    }

    PERF_PEEK(ptimer);
    return ret;
}

// ---------------------------------------
int Application::GetArgc() const
{
    return argc;
}

// ---------------------------------------
const char *Application::GetArgv(int index) const
{
    if (index < argc) {
        return args[index];
    } else {
        return nullptr;
    }
}

// ---------------------------------------
const char *Application::GetTitle() const
{
    return title.c_str();
}

// ---------------------------------------
float Application::GetDT() const
{
    return dt;
}

// ---------------------------------------
const char *Application::GetOrganization() const
{
    return organization.c_str();
}

// Load / Save
void Application::LoadGame()
{
    // we should be checking if that file actually exist
    // from the "GetSaveGames" list
    want_to_load = true;
    load_game = fs->GetSaveDirectory();
}

// ---------------------------------------
void Application::SaveGame() const
{
    // we should be checking if that file actually exist
    // from the "GetSaveGames" list ... should we overwrite ?

    want_to_save = true;
}

// ---------------------------------------
void Application::GetSaveGames(list<string> &list_to_fill) const
{
    // need to add functionality to file_system module for this to work
}

bool Application::LoadGameNow()
{
    bool ret = false;

    char *buffer;
    uint size = fs->LoadFile("save/save.xml", &buffer);

    if (size > 0) {
        pugi::xml_document data;
        pugi::xml_node root;

        pugi::xml_parse_result result = data.load_buffer(buffer, size);
        RELEASE_ARRAY(buffer);

        if (result != NULL) {
            LOG("Loading new Game State from %s...", load_game.c_str());

            root = data.child("Save");
            ret = true;

            list<Module *>::iterator it;
            for (it = modules.begin(); it != modules.end(); it++) {
                pugi::xml_node node = root.child((*it)->name.c_str());
                ret = (*it)->Load(node);
            }

            data.reset();
            if (ret == true) {
                LOG("...finished loading", 0);
            } else {
                LOG("...loading process interrupted with error on module %s", (*it)->name.c_str());
            }
        } else {
            LOG("Could not parse game state xml file %s. pugi error: %s", load_game.c_str(), result.description());
        }
    } else {
        LOG("Could not load game state xml file %s", load_game.c_str());
    }

    want_to_load = false;
    return ret;
}

bool Application::SavegameNow() const
{
    bool ret = true;

    LOG("Saving Game State to %s...", save_game.c_str());

    // xml object were we will store all data
    pugi::xml_document data;
    pugi::xml_node root;

    root = data.append_child("Save");

    list<Module *>::const_iterator it;
    for (it = modules.begin(); it != modules.end(); it++) {
        root.append_child((*it)->name.c_str());
        pugi::xml_node node = root.child((*it)->name.c_str());
        ret = (*it)->Save(node);
    }

    if (ret == true) {
        std::stringstream stream;
        data.save(stream);

        // we are done, so write data to disk
        fs->Save("save.xml", stream.str().c_str(), stream.str().length());
        LOG("... finished saving", save_game.c_str());
    } else {
        LOG("Save process halted from an error in module %s", (*it)->name.c_str());
    }

    data.reset();
    want_to_save = false;
    return ret;
}
