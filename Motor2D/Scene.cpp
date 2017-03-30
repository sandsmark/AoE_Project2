#include "Application.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "PathFinding.h"
#include "Scene.h"
#include "p2Log.h"
#include "EntityManager.h"
#include "Unit.h"
#include "FogOfWar.h"
#include "Gui.h"
#include "SceneManager.h"

Scene::Scene() : SceneElement("scene")
{
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node & config)
{
	LOG("Loading Scene");
	bool ret = true;
	active = true;
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	active = true;
	if (App->map->Load("rivendell.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->Load("maps/path2.png");

	//Test
	elvenArcher = App->entityManager->CreateUnit(350, 350, false, ELVEN_ARCHER);
	elvenArcher = App->entityManager->CreateUnit(100, 500, false, ELVEN_ARCHER);
	elvenArcher = App->entityManager->CreateUnit(320, 350, false, ELVEN_ARCHER);
	elvenArcher = App->entityManager->CreateUnit(100, 520, false, ELVEN_ARCHER);
	elvenArcher = App->entityManager->CreateUnit(380, 350, false, ELVEN_ARCHER);
	elvenArcher = App->entityManager->CreateUnit(140, 500, false, ELVEN_ARCHER);
	troll = App->entityManager->CreateUnit(600, 400, true, TROLL_MAULER);
	testBuilding = App->entityManager->CreateBuilding(150, 100, true, ORC_BARRACKS);
	tree = App->entityManager->CreateResource(400, 400, WOOD);

	App->fog->CreateFog(App->map->data.mapWidth, App->map->data.mapHeight);
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		debug = !debug;
	}
	App->map->Draw();
	App->gui->ScreenMoves(App->render->MoveCameraWithCursor(dt));
	

	if (debug) {
		const list<iPoint>* path = App->pathfinding->GetLastPath();

		for (list<iPoint>::const_iterator it = path->begin(); it != path->end(); it++) {
			iPoint pos = App->map->MapToWorld((*it).x, (*it).y);
			App->render->Blit(debug_tex, pos.x, pos.y);
		}
	}

	


	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		App->entityManager->CleanUp();
		App->sceneManager->ChangeScene(this, App->sceneManager->menu_scene);
	}
	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}