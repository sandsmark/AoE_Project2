#include "Application.h"
#include "FogOfWar.h"
#include "Scene.h"
#include "p2Log.h"
#include "Input.h"
#include "Entity.h"
#include "Textures.h"
#include "FileSystem.h"
#include "SceneManager.h"
#include "Map.h"

FogOfWar::FogOfWar() :
    Module()
{
    name = "fog";
}

FogOfWar::~FogOfWar()
{
}

bool FogOfWar::AddEntity(Entity *new_entity)
{
    if (new_entity->collider != nullptr && new_entity->faction == FREE_MEN) {
        in_fog_entity new_ally;

        new_ally.pos = App->map->WorldToMap(new_entity->collider->pos.x, new_entity->collider->pos.y);

        Unit *aux = (Unit *)new_entity;
        Building *aux2 = (Building *)new_entity;

        if (new_entity->collider->type == COLLIDER_UNIT) {
            new_ally.radium = aux->los->r / 48;
        } else if (new_entity->collider->type == COLLIDER_BUILDING && aux2->range != nullptr) {
            new_ally.radium = aux2->range->r / 49;
        } else {
            new_ally.radium = 4;
        }

        GetEntitiesCircleArea(new_ally);
        new_ally.id = new_entity->entityID;
        entities_on_fog.push_back(new_ally);

        FillFrontier();
        SoftEdges();
    } else {
        entities_not_in_fog.push_back(new_entity);
        ManageEntities();
    }

    return true;
}

uint FogOfWar::Get(int x, int y)
{
    if (((y * App->map->data.width) + x) < dataSize) {
        return data[(y * App->map->data.width) + x];
    }
    return 0;
}

bool FogOfWar::Awake(pugi::xml_node &conf)
{
    LOG("Init True Type Font library", 0);
    active = false;
    path_texture1 = conf.child("texture").attribute("file1").as_string("");
    path_texture2 = conf.child("texture").attribute("file2").as_string("");

    return true;
}

bool FogOfWar::Start()
{
    active = true;

    dataSize = App->map->data.width * App->map->data.height;
    data = new uint[dataSize];

    memset(data, 0, dataSize * sizeof(uint));
    texture_grey = App->tex->LoadTexture(path_texture1.c_str());
    texture_black = App->tex->LoadTexture(path_texture2.c_str());

    return true;
}

void FogOfWar::Update(iPoint prev_pos, iPoint next_pos, uint id)
{
    // We look for the direction that the player is moving
    for (vector<in_fog_entity>::iterator curr = entities_on_fog.begin(); curr != entities_on_fog.end(); curr++) {
        if (curr->id == id) {
            if (prev_pos.x < next_pos.x) {
                MoveFrontier(prev_pos, "right", id, (next_pos.x - prev_pos.x));
            } else if (prev_pos.x > next_pos.x) {
                MoveFrontier(prev_pos, "left", id, (prev_pos.x - next_pos.x));
            }

            if (prev_pos.y < next_pos.y) {
                MoveFrontier(prev_pos, "down", id, (next_pos.y - prev_pos.y));
            } else if (prev_pos.y > next_pos.y) {
                MoveFrontier(prev_pos, "up", id, (prev_pos.y - next_pos.y));
            }

            SoftEdges();
        }
    }

    ManageEntities();
}

void FogOfWar::GetEntitiesCircleArea(in_fog_entity &new_player)
{
    new_player.frontier = App->map->PropagateBFS({ new_player.pos.x, new_player.pos.y }, new_player.radium);

    DeletePicks(new_player);

    for (list<iPoint>::iterator it = new_player.frontier.begin(); it != new_player.frontier.end(); it++) {
        data[(*it).y * App->map->data.width + (*it).x] = fow_clear;
    }
}

void FogOfWar::MoveFrontier(iPoint prev_pos, const char *direction, uint id, int amount)
{
    string direction_str(direction);

    for (vector<in_fog_entity>::iterator it = entities_on_fog.begin(); it != entities_on_fog.end(); it++) {
        if (it->id == id) {
            MoveArea(*it, direction, id, amount);
        }
    }
}

void FogOfWar::GetCurrentPointsFromFrontier(in_fog_entity &player)
{

    //First we add the frontier

    for (list<iPoint>::iterator it = player.frontier.begin(); it != player.frontier.end(); it++) {
        player.current_points.push_back(*it);
    }

    // Now we add the tiles inside

    bool advance = false;

    for (list<iPoint>::iterator it = player.frontier.begin(); it != player.frontier.end(); it++) {
        if (it->x < player.pos.x && it->y != player.pos.y + (player.radium - 1) && it->y != player.pos.y - (player.radium - 1)) {
            for (int x = it->x + 1;; x++) {
                if (IsFrontier(iPoint(x, it->y), player)) {
                    break;

                } else {
                    player.current_points.push_back({ x, it->y });
                }
            }
        }
    }

    for (list<iPoint>::iterator it = player.current_points.begin(); it != player.current_points.end(); it++) {
        data[(*it).y * App->map->data.width + (*it).x] = fow_clear;
    }
}

void FogOfWar::FillFrontier()
{
    for (vector<in_fog_entity>::iterator it = entities_on_fog.begin(); it != entities_on_fog.end(); it++) {
        GetCurrentPointsFromFrontier(*it);
    }
}

void FogOfWar::MoveArea(in_fog_entity &player_unity, string direction_str, uint id, int amount)
{
    if (App->sceneManager->current_scene == App->sceneManager->level1_scene) {
        for (list<iPoint>::iterator it = player_unity.current_points.begin(); it != player_unity.current_points.end(); it++) {
            if (Get((*it).x, (*it).y + 1) == fow_black && Get((*it).x + 1, (*it).y) == fow_black) {
                data[(*it).y * App->map->data.width + (*it).x] = fow_grey_rdown;

            } else if (Get((*it).x, (*it).y + 1) == fow_black && Get((*it).x - 1, (*it).y) == fow_black) {
                data[(*it).y * App->map->data.width + (*it).x] = fow_grey_rleft;

            } else if (Get((*it).x, (*it).y - 1) == fow_black && Get((*it).x + 1, (*it).y) == fow_black) {
                data[(*it).y * App->map->data.width + (*it).x] = fow_grey_rright;

            } else if (Get((*it).x, (*it).y - 1) == fow_black && Get((*it).x - 1, (*it).y) == fow_black) {
                data[(*it).y * App->map->data.width + (*it).x] = fow_grey_rup;

            } else {
                data[(*it).y * App->map->data.width + (*it).x] = fow_grey;
            }
        }
    } else {
        for (list<iPoint>::iterator it = player_unity.current_points.begin(); it != player_unity.current_points.end(); it++) {
            data[(*it).y * App->map->data.width + (*it).x] = fow_black;
        }
    }

    if (direction_str == "right") {
        for (int i = 0; i < amount; i++) {
            for (list<iPoint>::iterator it = player_unity.current_points.begin(); it != player_unity.current_points.end(); it++) {
                (*it).x += 1;
            }

            for (list<iPoint>::iterator it = player_unity.frontier.begin(); it != player_unity.frontier.end(); it++) {
                (*it).x += 1;
            }

            player_unity.pos.x += amount;
        }
    }

    else if (direction_str == "left") {
        for (int i = 0; i < amount; i++) {
            for (list<iPoint>::iterator it = player_unity.current_points.begin(); it != player_unity.current_points.end(); it++) {
                (*it).x -= 1;
            }

            for (list<iPoint>::iterator it = player_unity.frontier.begin(); it != player_unity.frontier.end(); it++) {
                (*it).x -= 1;
            }

            player_unity.pos.x -= amount;
        }
    }

    else if (direction_str == "up") {
        for (int i = 0; i < amount; i++) {
            for (list<iPoint>::iterator it = player_unity.current_points.begin(); it != player_unity.current_points.end(); it++) {
                (*it).y -= 1;
            }

            for (list<iPoint>::iterator it = player_unity.frontier.begin(); it != player_unity.frontier.end(); it++) {
                (*it).y -= 1;
            }

            player_unity.pos.y -= amount;
        }
    }

    else if (direction_str == "down") {
        for (int i = 0; i < amount; i++) {
            for (list<iPoint>::iterator it = player_unity.current_points.begin(); it != player_unity.current_points.end(); it++) {
                (*it).y += 1;
            }

            for (list<iPoint>::iterator it = player_unity.frontier.begin(); it != player_unity.frontier.end(); it++) {
                (*it).y += 1;
            }

            player_unity.pos.y += amount;
        }
    }

    for (list<iPoint>::iterator it = player_unity.current_points.begin(); it != player_unity.current_points.end(); it++) {
        data[(*it).y * App->map->data.width + (*it).x] = fow_clear;
    }

    // Redraw the others in case of overlaping

    for (vector<in_fog_entity>::iterator it = entities_on_fog.begin(); it != entities_on_fog.end(); it++) {
        if (it->id != id) {
            for (list<iPoint>::iterator it2 = it->current_points.begin(); it2 != it->current_points.end(); it2++) {
                data[(*it2).y * App->map->data.width + (*it2).x] = fow_clear;
            }
        }
    }
}

void FogOfWar::DeletePicks(in_fog_entity &player)
{
    int count = 0;

    for (list<iPoint>::iterator it = player.frontier.begin(); it != player.frontier.end(); it++) {
        if ((*it) == iPoint(player.pos.x, player.pos.y + player.radium)) {
            (*it) = iPoint((*it).x, (*it).y - 1);

        } else if ((*it) == iPoint(player.pos.x, player.pos.y - player.radium)) {
            (*it) = iPoint((*it).x, (*it).y + 1);

        } else if ((*it) == iPoint(player.pos.x + player.radium, player.pos.y)) {
            (*it) = iPoint((*it).x - 1, (*it).y);

        } else if ((*it) == iPoint(player.pos.x - player.radium, player.pos.y)) {
            (*it) = iPoint((*it).x + 1, (*it).y);
        }
    }
}

void FogOfWar::ManageEntities()
{
    for (list<Entity *>::iterator it = entities_not_in_fog.begin(); it != entities_not_in_fog.end(); it++) {
        if ((*it) != nullptr && (*it)->state != DESTROYED && (*it)->collider != nullptr) {
            if (IsVisible(App->map->WorldToMap((*it)->collider->pos.x, (*it)->collider->pos.y), (*it))) {
                (*it)->isActive = true;
                if ((*it)->collider->type == COLLIDER_BUILDING) {
                    Building *aux = (Building *)(*it);
                    if (!App->quest->TriggerCallback(aux)) {
                        App->quest->StepCallback(aux);
                    }
                }
            } else {
                (*it)->isActive = false;
            }
        }
    }
}

bool FogOfWar::IsVisible(iPoint char_pos, Entity *e)
{
    bool ret = false;

    if (e->faction == NATURE && Get(char_pos.x, char_pos.y) != fow_black) {
        ret = true;
    }
    if (e->faction == SAURON_ARMY) {
        if (e->entityType == ENTITY_BUILDING && Get(char_pos.x, char_pos.y) != fow_black) {
            ret = true;
        }
        if (e->entityType == ENTITY_UNIT && Get(char_pos.x, char_pos.y) == fow_clear) {
            ret = true;
        }
    }
    if (e->faction == FREE_MEN) {
        ret = true;
    }

    return ret;
}

bool FogOfWar::IsFrontier(iPoint point, in_fog_entity &player)
{
    for (list<iPoint>::iterator it = player.frontier.begin(); it != player.frontier.end(); it++) {
        if (point == *it) {
            return true;
        }
    }

    return false;
}

bool FogOfWar::CleanUp()
{
    entities_on_fog.clear();
    entities_not_in_fog.clear();

    App->tex->UnLoad(texture_grey);
    App->tex->UnLoad(texture_black);
    return true;
}

void FogOfWar::SoftEdges()
{
    for (vector<in_fog_entity>::iterator curr_player = entities_on_fog.begin(); curr_player != entities_on_fog.end(); curr_player++) {
        for (list<iPoint>::iterator it = (*curr_player).current_points.begin(); it != (*curr_player).current_points.end(); it++) {
            // Rect edges

            if (Get((*it).x, (*it).y + 1) >= fow_clear && Get((*it).x, (*it).y + 1) <= fow_clear_sright && Get((*it).x, (*it).y - 1) >= fow_clear && Get((*it).x, (*it).y - 1) <= fow_clear_sright && (Get((*it).x - 1, (*it).y) >= fow_black && Get((*it).x - 1, (*it).y) <= fow_grey_rright)) {
                data[(*it).y * App->map->data.width + (*it).x] = fow_clear_rectupleft;

            } else if (Get((*it).x, (*it).y + 1) >= fow_clear && Get((*it).x, (*it).y + 1) <= fow_clear_sright && Get((*it).x, (*it).y - 1) >= fow_clear && Get((*it).x, (*it).y - 1) <= fow_clear_sright && (Get((*it).x + 1, (*it).y) >= fow_black && Get((*it).x + 1, (*it).y) <= fow_grey_rright)) {
                data[(*it).y * App->map->data.width + (*it).x] = fow_clear_rectdownright;

            } else if (Get((*it).x - 1, (*it).y) >= fow_clear && Get((*it).x - 1, (*it).y) <= fow_clear_sright && Get((*it).x + 1, (*it).y) >= fow_clear && Get((*it).x + 1, (*it).y) <= fow_clear_sright && (Get((*it).x, (*it).y + 1) >= fow_black && Get((*it).x, (*it).y + 1) <= fow_grey_rright)) {
                data[(*it).y * App->map->data.width + (*it).x] = fow_clear_rectdownleft;

            } else if (Get((*it).x - 1, (*it).y) >= fow_clear && Get((*it).x - 1, (*it).y) <= fow_clear_sright && Get((*it).x + 1, (*it).y) >= fow_clear && Get((*it).x + 1, (*it).y) <= fow_clear_sright && (Get((*it).x, (*it).y - 1) >= fow_black && Get((*it).x, (*it).y - 1) <= fow_grey_rright)) {
                data[(*it).y * App->map->data.width + (*it).x] = fow_clear_rectupright;

                // Rounded grey-black edges

            } else if (Get((*it).x, (*it).y + 1) == fow_black && Get((*it).x + 1, (*it).y) == fow_black) {
                data[(*it).y * App->map->data.width + (*it).x] = fow_black_rdown;

            } else if (Get((*it).x, (*it).y + 1) == fow_black && Get((*it).x - 1, (*it).y) == fow_black) {
                data[(*it).y * App->map->data.width + (*it).x] = fow_black_rleft;

            } else if (Get((*it).x, (*it).y - 1) == fow_black && Get((*it).x + 1, (*it).y) == fow_black) {
                data[(*it).y * App->map->data.width + (*it).x] = fow_black_rright;

            } else if (Get((*it).x, (*it).y - 1) == fow_black && Get((*it).x - 1, (*it).y) == fow_black) {
                data[(*it).y * App->map->data.width + (*it).x] = fow_black_rup;

                // Rounded grey & black edges - These are always grey texture

            } else if ((Get((*it).x, (*it).y + 1) >= fow_black && Get((*it).x, (*it).y + 1) <= fow_grey_rright) && (Get((*it).x + 1, (*it).y) >= fow_black && Get((*it).x + 1, (*it).y) <= fow_grey_rright)) {
                data[(*it).y * App->map->data.width + (*it).x] = fow_clear_rdown;

            } else if ((Get((*it).x, (*it).y + 1) >= fow_black && Get((*it).x, (*it).y + 1) <= fow_grey_rright) && (Get((*it).x - 1, (*it).y) >= fow_black && Get((*it).x - 1, (*it).y) <= fow_grey_rright)) {
                data[(*it).y * App->map->data.width + (*it).x] = fow_clear_rleft;

            } else if ((Get((*it).x, (*it).y - 1) >= fow_black && Get((*it).x, (*it).y - 1) <= fow_grey_rright) && (Get((*it).x + 1, (*it).y) >= fow_black && Get((*it).x + 1, (*it).y) <= fow_grey_rright)) {
                data[(*it).y * App->map->data.width + (*it).x] = fow_clear_rright;

            } else if ((Get((*it).x - 1, (*it).y) >= fow_black && Get((*it).x - 1, (*it).y) <= fow_grey_rright) && (Get((*it).x, (*it).y - 1) >= fow_black && Get((*it).x, (*it).y - 1) <= fow_grey_rright)) {
                data[(*it).y * App->map->data.width + (*it).x] = fow_clear_rup;

                // Small part to complete the rounded edge

            } else if (Get((*it).x + 1, (*it).y) >= fow_clear_rup && Get((*it).x + 1, (*it).y) <= fow_clear_rectdownleft && Get((*it).x, (*it).y + 1) >= fow_clear_rup && Get((*it).x, (*it).y + 1) <= fow_clear_rectdownleft && Get((*it).x + 1, (*it).y + 1) >= fow_black && Get((*it).x + 1, (*it).y + 1) <= fow_grey_rright) {
                data[(*it).y * App->map->data.width + (*it).x] = fow_clear_sdown;

            } else if (Get((*it).x - 1, (*it).y) >= fow_clear_rup && Get((*it).x - 1, (*it).y) <= fow_clear_rectdownleft && Get((*it).x, (*it).y - 1) >= fow_clear_rup && Get((*it).x, (*it).y - 1) <= fow_clear_rectdownleft && Get((*it).x - 1, (*it).y - 1) >= fow_black && Get((*it).x - 1, (*it).y - 1) <= fow_grey_rright) {
                data[(*it).y * App->map->data.width + (*it).x] = fow_clear_sup;

            } else if (Get((*it).x - 1, (*it).y) >= fow_clear_rup && Get((*it).x - 1, (*it).y) <= fow_clear_rectdownleft && Get((*it).x, (*it).y + 1) >= fow_clear_rup && Get((*it).x, (*it).y + 1) <= fow_clear_rectdownleft && Get((*it).x - 1, (*it).y + 1) >= fow_black && Get((*it).x - 1, (*it).y + 1) <= fow_grey_rright) {
                data[(*it).y * App->map->data.width + (*it).x] = fow_clear_sleft;

            } else if (Get((*it).x + 1, (*it).y) >= fow_clear_rup && Get((*it).x + 1, (*it).y) <= fow_clear_rectdownleft && Get((*it).x, (*it).y - 1) >= fow_clear_rup && Get((*it).x, (*it).y - 1) <= fow_clear_rectdownleft && Get((*it).x + 1, (*it).y - 1) >= fow_black && Get((*it).x + 1, (*it).y - 1) <= fow_grey_rright) {
                data[(*it).y * App->map->data.width + (*it).x] = fow_clear_sright;
            }
        }
    }
}

bool FogOfWar::Save(pugi::xml_node &d) const
{
    pugi::xml_node dataInfo = d.append_child("Visibility");

    for (list<MapLayer *>::iterator it = App->map->data.layers.begin(); it != App->map->data.layers.end(); it++) {
        MapLayer *layer = *it;

        if (layer->properties.Get("Nodraw") != 0) {
            continue;
        }

        for (int y = 0; y < App->map->data.height; ++y) {
            for (int x = 0; x < App->map->data.width; ++x) {
                int tile_id = layer->Get(x, y);
                uint v = App->fog->Get(x, y);

                if (tile_id > 0) {
                    dataInfo.append_child("Tile").append_attribute("value") = v;
                }
            }
        }
    }

    return true;
}

bool FogOfWar::Load(pugi::xml_node &d)
{
    pugi::xml_node node = d.child("Visibility").child("Tile");

    for (list<MapLayer *>::iterator it = App->map->data.layers.begin(); it != App->map->data.layers.end(); it++) {
        MapLayer *layer = *it;

        if (layer->properties.Get("Nodraw") != 0) {
            continue;
        }

        for (int y = 0; y < App->map->data.height; ++y) {
            for (int x = 0; x < App->map->data.width; ++x) {
                int tile_id = layer->Get(x, y);
                if (tile_id > 0) {
                    data[(y * App->map->data.width) + x] = node.attribute("value").as_uint();
                    node = node.next_sibling("Tile");
                }
            }
        }
    }

    return true;
}

void FogOfWar::DeleteEntityFog(uint id)
{
    for (vector<in_fog_entity>::iterator curr = entities_on_fog.begin(); curr != entities_on_fog.end(); curr++) {
        if (curr->id == id) {
            // Put all clear area into grey area
            for (list<iPoint>::iterator it = (*curr).current_points.begin(); it != (*curr).current_points.end(); it++) {
                if (Get((*it).x, (*it).y + 1) == fow_black && Get((*it).x + 1, (*it).y) == fow_black) {
                    data[(*it).y * App->map->data.width + (*it).x] = fow_grey_rdown;

                } else if (Get((*it).x, (*it).y + 1) == fow_black && Get((*it).x - 1, (*it).y) == fow_black) {
                    data[(*it).y * App->map->data.width + (*it).x] = fow_grey_rleft;

                } else if (Get((*it).x, (*it).y - 1) == fow_black && Get((*it).x + 1, (*it).y) == fow_black) {
                    data[(*it).y * App->map->data.width + (*it).x] = fow_grey_rright;

                } else if (Get((*it).x, (*it).y - 1) == fow_black && Get((*it).x - 1, (*it).y) == fow_black) {
                    data[(*it).y * App->map->data.width + (*it).x] = fow_grey_rup;

                } else {
                    data[(*it).y * App->map->data.width + (*it).x] = fow_grey;
                }
            }

            // Erase from list
            entities_on_fog.erase(curr);
            break;
        }
    }

    // Redraw fow clear again in case of overlaping
    for (vector<in_fog_entity>::iterator it = entities_on_fog.begin(); it != entities_on_fog.end(); it++) {
        if (it->id != id) {
            for (list<iPoint>::iterator it2 = it->current_points.begin(); it2 != it->current_points.end(); it2++) {
                data[(*it2).y * App->map->data.width + (*it2).x] = fow_clear;
            }
            SoftEdges();
        }
    }
}
