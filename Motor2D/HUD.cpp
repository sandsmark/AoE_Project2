#include "p2Defs.h"
#include "p2Log.h"
#include "Application.h"
#include "Render.h"
#include "Textures.h"
#include "Fonts.h"
#include "Input.h"
#include "Window.h"
#include "Gui.h"
#include "SceneManager.h"
#include "Window.h"
#include "Orders.h"
#include <cstdlib>
#include "TechTree.h"
#include "EntityManager.h"
#include "Hero.h"
#include "Audio.h"

//HUD
HUD::HUD()
{
}

void HUD::Start()
{
    App->win->GetWindowSize(x, y);
    // 0
    buttons_positions.push_back({ (int)x / 30 - CAMERA_OFFSET_X, (int)y - (int)y / 5 - CAMERA_OFFSET_Y, 39, 40 });
    // 1
    buttons_positions.push_back({ (int)x / 15 - CAMERA_OFFSET_X, (int)y - (int)y / 5 - CAMERA_OFFSET_Y, 39, 40 });
    // 2
    buttons_positions.push_back({ (int)x / 10 - CAMERA_OFFSET_X, (int)y - (int)y / 5 - CAMERA_OFFSET_Y, 39, 40 });
    // 3
    buttons_positions.push_back({ (int)x / 7 - (int)(x / 100) - CAMERA_OFFSET_X, (int)y - (int)y / 5 - CAMERA_OFFSET_Y, 39, 40 });
    // 4
    buttons_positions.push_back({ (int)x / 6 - CAMERA_OFFSET_X, (int)y - (int)y / 5 - CAMERA_OFFSET_Y, 39, 40 });
    // 5
    buttons_positions.push_back({ (int)x / 5 - CAMERA_OFFSET_X, (int)y - (int)y / 5 - CAMERA_OFFSET_Y, 39, 40 });
    // 6
    buttons_positions.push_back({ (int)x / 30 - CAMERA_OFFSET_X, (int)y - (int)y / 7 + (int)(y / 100) - CAMERA_OFFSET_Y, 39, 40 });
    //7
    buttons_positions.push_back({ (int)x / 15 - CAMERA_OFFSET_X, (int)y - (int)y / 7 + (int)(y / 100) - CAMERA_OFFSET_Y, 39, 40 });
    // 8
    buttons_positions.push_back({ (int)x / 10 - CAMERA_OFFSET_X, (int)y - (int)y / 7 + (int)(y / 100) - CAMERA_OFFSET_Y, 39, 40 });
    // 9
    buttons_positions.push_back({ (int)x / 7 - (int)(x / 100) - CAMERA_OFFSET_X, (int)y - (int)y / 7 + (int)(y / 100) - CAMERA_OFFSET_Y, 39, 40 });
    // 10
    buttons_positions.push_back({ (int)x / 6 - CAMERA_OFFSET_X, (int)y - (int)y / 7 + (int)(y / 100) - CAMERA_OFFSET_Y, 39, 40 });

    buttons_positions.push_back({ (int)x / 30 - CAMERA_OFFSET_X, (int)y - (int)y / 8 - CAMERA_OFFSET_Y, 39, 40 });
    // 12
    buttons_positions.push_back({ (int)x / 4 - (int)x / 40 - CAMERA_OFFSET_X, (int)y - (int)y / 10 - CAMERA_OFFSET_Y, 39, 40 });
    posx = x / 3 - x / 50;
    posy = y - (y / 6);

    vector<SDL_Rect> blit_sections;

    // Buildings
    blit_sections.push_back({ 0, 0, 33, 32 });
    blit_sections.push_back({ 0, 0, 33, 32 });
    buildings_rects.push_back(blit_sections);
    blit_sections.clear();
    blit_sections.push_back({ 33, 0, 33, 32 });
    blit_sections.push_back({ 33, 0, 33, 32 });
    buildings_rects.push_back(blit_sections);
    blit_sections.clear();
    blit_sections.push_back({ 99, 0, 33, 32 });
    blit_sections.push_back({ 99, 0, 33, 32 });
    buildings_rects.push_back(blit_sections);
    blit_sections.clear();
    blit_sections.push_back({ 132, 0, 33, 32 });
    blit_sections.push_back({ 132, 0, 33, 32 });
    buildings_rects.push_back(blit_sections);
    blit_sections.clear();
    blit_sections.push_back({ 165, 0, 33, 32 });
    blit_sections.push_back({ 165, 0, 33, 32 });
    buildings_rects.push_back(blit_sections);
    blit_sections.clear();
    blit_sections.push_back({ 198, 0, 33, 32 });
    blit_sections.push_back({ 198, 0, 33, 32 });
    buildings_rects.push_back(blit_sections);
    blit_sections.clear();
    blit_sections.push_back({ 231, 0, 33, 32 });
    blit_sections.push_back({ 231, 0, 33, 32 });
    buildings_rects.push_back(blit_sections);
    blit_sections.clear();
    blit_sections.push_back({ 264, 0, 33, 32 });
    blit_sections.push_back({ 264, 0, 33, 32 });
    buildings_rects.push_back(blit_sections);
    blit_sections.clear();
    blit_sections.push_back({ 66, 32, 33, 32 });
    blit_sections.push_back({ 66, 32, 33, 32 });
    buildings_rects.push_back(blit_sections);
    blit_sections.clear();
    blit_sections.push_back({ 99, 32, 33, 32 });
    blit_sections.push_back({ 99, 32, 33, 32 });
    buildings_rects.push_back(blit_sections);
    blit_sections.clear();
    blit_sections.push_back({ 132, 32, 33, 32 });
    blit_sections.push_back({ 132, 32, 33, 32 });
    buildings_rects.push_back(blit_sections);
    blit_sections.clear();

    // Units
    blit_sections.push_back({ 0, 0, 33, 32 });
    blit_sections.push_back({ 0, 0, 33, 32 });
    units_rects.push_back(blit_sections);
    blit_sections.clear();
    blit_sections.push_back({ 33, 0, 33, 32 });
    blit_sections.push_back({ 33, 0, 33, 32 });
    units_rects.push_back(blit_sections);
    blit_sections.clear();
    blit_sections.push_back({ 66, 0, 33, 32 });
    blit_sections.push_back({ 66, 0, 33, 32 });
    units_rects.push_back(blit_sections);
    blit_sections.clear();
    blit_sections.push_back({ 99, 0, 33, 32 });
    blit_sections.push_back({ 99, 0, 33, 32 });
    units_rects.push_back(blit_sections);
    blit_sections.clear();
    blit_sections.push_back({ 132, 0, 33, 32 });
    blit_sections.push_back({ 132, 0, 33, 32 });
    units_rects.push_back(blit_sections);
    blit_sections.clear();
    blit_sections.push_back({ 165, 0, 33, 32 });
    blit_sections.push_back({ 165, 0, 33, 32 });
    units_rects.push_back(blit_sections);
    blit_sections.clear();
    blit_sections.push_back({ 198, 0, 33, 32 });
    blit_sections.push_back({ 198, 0, 33, 32 });
    units_rects.push_back(blit_sections);
    blit_sections.clear();
    blit_sections.push_back({ 231, 0, 33, 32 });
    blit_sections.push_back({ 231, 0, 33, 32 });
    units_rects.push_back(blit_sections);
    blit_sections.clear();
    blit_sections.push_back({ 264, 0, 33, 32 });
    blit_sections.push_back({ 264, 0, 33, 32 });
    units_rects.push_back(blit_sections);
    blit_sections.clear();
}

void HUD::Update()
{
    Sprite bar;
    int percent;
    int barPercent;
    string life_str;
    string cd_str;

    if (App->entityManager->selectedEntityList.size() > 0) {
        if (App->entityManager->selectedEntityList.front()->Life > 0) {
            switch (App->entityManager->selectedListType) {

            case COLLIDER_UNIT:
                if (App->entityManager->selectedEntityList.size() > 1) {

                    if (type != MULTIPLESELECTION) {
                        if (type == SINGLEINFO) {
                            ClearSingle();
                        } else if (type == BUILDINGINFO) {
                            ClearBuilding();
                        } else if (type == RESOURCEINFO) {
                            ClearResource();
                        }

                        type = MULTIPLESELECTION;
                        GetSelection();

                    } else {
                        int x = 0, y = 0;
                        for (list<UnitSprite>::iterator it_sprite = App->gui->SpriteUnits.begin(); it_sprite != App->gui->SpriteUnits.end(); ++it_sprite) {
                            for (list<Entity *>::iterator it_unit = App->entityManager->selectedEntityList.begin(); it_unit != App->entityManager->selectedEntityList.end(); ++it_unit) {
                                if (x >= max_width) {
                                    x = 0;
                                    y += App->gui->SpriteUnits.front().GetRect().h + 5;
                                }
                                Unit *unit = (Unit *)(*it_unit);
                                if (it_sprite->GetID() == unit->type && (*it_unit)->Life > 0) {
                                    if ((*it_unit)->MaxLife == 0) {
                                        (*it_unit)->MaxLife = (*it_unit)->Life;
                                    }
                                    percent = (((*it_unit)->MaxLife - (*it_unit)->Life) * 100) / (*it_unit)->MaxLife;
                                    barPercent = (percent * App->gui->SpriteUnits.front().GetRect().w) / 100;
                                    bar.rect.x = posx + x - App->render->camera.x;
                                    bar.rect.y = posy + y - 30 - App->render->camera.y + App->gui->SpriteBuildings.front().GetRect().h;
                                    bar.rect.w = App->gui->SpriteBuildings.front().GetRect().w;
                                    bar.rect.h = 5;
                                    bar.r = 255;
                                    bar.g = 255;
                                    bar.b = 255;
                                    bar.a = 255;
                                    App->render->ui_toDraw.push_back(bar);
                                    bar.rect.w = min(App->gui->SpriteBuildings.front().GetRect().w, max(App->gui->SpriteBuildings.front().GetRect().w - barPercent, 0));
                                    if (unit->faction == FREE_MEN) {
                                        bar.r = 0;
                                        bar.g = 255;
                                        bar.b = 0;
                                    } else {
                                        bar.r = 255;
                                        bar.g = 0;
                                        bar.b = 0;
                                    }
                                    App->render->ui_toDraw.push_back(bar);
                                    x += App->gui->SpriteUnits.front().GetRect().w;
                                }
                            }
                        }
                    }
                } else {
                    if (type != SINGLEINFO) {
                        if (type == MULTIPLESELECTION) {
                            ClearMultiple();
                        } else if (type == BUILDINGINFO) {
                            ClearBuilding();
                        } else if (type == RESOURCEINFO) {
                            ClearResource();
                        }

                        type = SINGLEINFO;
                        GetSelection();
                    } else {
                        Unit *unit = (Unit *)App->entityManager->selectedEntityList.front();
                        if (unit->type != id) {
                            ClearSingle();
                            GetSelection();
                        } else if (unit->Life > 0) {
                            damage = to_string(unit->Attack);
                            armor = to_string(unit->Defense);

                            damage_val->SetString(damage);
                            armor_val->SetString(armor);

                            max_life = unit->MaxLife;
                            curr_life = unit->Life;

                            currlife = to_string(curr_life);
                            life_str += currlife;
                            life_str += "/";
                            maxlife = to_string(max_life);
                            life_str += maxlife;

                            life->SetString(life_str);

                            if (max_life <= 0) {
                                max_life = curr_life;
                            }
                            percent = ((max_life - curr_life) * 100) / max_life;
                            barPercent = (percent * App->gui->SpriteUnits.front().GetRect().w) / 100;
                            bar.rect.x = posx - App->render->camera.x;
                            bar.rect.y = posy - App->render->camera.y + App->gui->SpriteBuildings.front().GetRect().h;
                            bar.rect.w = App->gui->SpriteBuildings.front().GetRect().w;
                            bar.rect.h = 5;
                            bar.r = 255;
                            bar.g = 255;
                            bar.b = 255;
                            bar.a = 255;
                            App->render->ui_toDraw.push_back(bar);
                            bar.rect.w = min(App->gui->SpriteBuildings.front().GetRect().w, max(App->gui->SpriteBuildings.front().GetRect().w - barPercent, 0));
                            if (unit->faction == FREE_MEN) {
                                bar.r = 0;
                                bar.g = 255;
                                bar.b = 0;
                            } else {
                                bar.r = 255;
                                bar.g = 0;
                                bar.b = 0;
                            }
                            App->render->ui_toDraw.push_back(bar);
                            if (name->str == "ELF VILLAGER") {
                                switch (villager_state) {
                                case VILLAGERMENU:
                                    if (villager_state != VILLAGERMENU) {
                                        HUDVillagerMenu();
                                    } else {
                                        if (create_building_bt->current == CLICKUP) {
                                            HUDCreateBuildings();
                                        }
                                    }
                                    break;
                                case VILLAGERCREATEBUILDINGS:
                                    if (villager_state != VILLAGERCREATEBUILDINGS) {
                                        HUDCreateBuildings();
                                    } else {
                                        if (cancel_bt->current == CLICKUP) {
                                            HUDVillagerMenu();
                                        } else {
                                            for (uint i = 0; i < App->gui->building_bt.size(); ++i) {
                                                if (App->gui->building_bt[i].button != nullptr) {

                                                    if (App->gui->building_bt[i].type == HOUSE && App->sceneManager->level1_scene->CheckHousesRoom() && App->sceneManager->level1_scene->CheckBuildingsRoom()) {
                                                        if (!App->entityManager->placingBuilding && App->gui->building_bt[i].button->current == CLICKUP) {

                                                            App->entityManager->placingBuilding = true;
                                                            App->entityManager->placing_type = App->gui->building_bt[i].type;
                                                        }
                                                    } else if (App->sceneManager->level1_scene->CheckBuildingsRoom() && App->gui->building_bt[i].type != HOUSE) {
                                                        if (!App->entityManager->placingBuilding && App->gui->building_bt[i].button->current == CLICKUP) {

                                                            App->entityManager->placingBuilding = true;
                                                            App->entityManager->placing_type = App->gui->building_bt[i].type;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    break;
                                }
                            } else {
                                Unit *unit = (Unit *)App->entityManager->selectedEntityList.front();
                                if (unit->IsHero) {
                                    Hero *hero = (Hero *)unit;

                                    max_life = hero->skill->cooldown;
                                    curr_life = (int)hero->skill_timer.ReadSec();
                                    if (curr_life >= max_life) {
                                        curr_life = max_life;
                                    }
                                    currlife = to_string(curr_life);
                                    life_str += "     ";
                                    life_str += currlife;
                                    life_str += "/";
                                    maxlife = to_string(max_life);
                                    life_str += maxlife;
                                    life_str += "(sec)";
                                    life->SetString(life_str);
                                    if (max_life == 0) {
                                        max_life = curr_life;
                                    }
                                    percent = ((max_life - curr_life) * 100) / max_life;
                                    barPercent = (percent * App->gui->SpriteBuildings.front().GetRect().w) / 100;
                                    bar.rect.y = bar.rect.y + bar.rect.h;
                                    bar.rect.h = 5;
                                    bar.r = 200;
                                    bar.g = 200;
                                    bar.b = 200;
                                    App->render->ui_toDraw.push_back(bar);
                                    bar.rect.w = min(App->gui->SpriteBuildings.front().GetRect().w, max(App->gui->SpriteBuildings.front().GetRect().w - barPercent, 0));
                                    bar.r = 0;
                                    bar.b = 255;
                                    bar.g = 100;
                                    App->render->ui_toDraw.push_back(bar);

                                    switch (hero_state) {
                                    case VILLAGERMENU:
                                        if (hero_state != HEROMENU) {
                                            HUDHeroMenu(hero->skill->type);
                                            if (skill_bt_support != nullptr) {
                                                skill_bt_support->enabled = false;
                                            }
                                        } else {
                                            if (hero->skill->ready) {
                                                if (skill_bt_support != nullptr) {
                                                    skill_bt_support->enabled = true;
                                                }
                                            } else if (skill_bt_support != nullptr) {
                                                skill_bt_support->enabled = false;
                                            }
                                            if ((skill_bt_support != nullptr) && (skill_bt_support->current == CLICKUP || App->input->GetKey(App->input->controls[HERO_SPECIAL_ATTACK]) == KEY_DOWN)) {
                                                hero->skill->Activate(hero);
                                            }
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    break;
                case COLLIDER_BUILDING:

                    if (type != BUILDINGINFO) {
                        ClearSingle();
                        ClearMultiple();
                        ClearResource();
                        type = BUILDINGINFO;
                        StartBuildingInfo();
                    } else {
                        Building *building = (Building *)App->entityManager->selectedEntityList.front();
                        if (building->type != id) {
                            ClearBuilding();
                            StartBuildingInfo();
                        }
                        for (list<UnitSprite>::iterator it = App->gui->SpriteBuildings.begin(); it != App->gui->SpriteBuildings.end(); ++it) {
                            if (it->GetID() == building->type) {
                                name->SetString(it->GetName());
                            }
                        }

                        DrawBuildingBar();
                        switch (building_state) {
                        case BUILDINGMENU:
                            if (building_state != BUILDINGMENU) {
                                HUDBuildingMenu();
                                tech_success = false;
                            } else {
                                if (create_unit_bt != nullptr) {
                                    if (create_unit_bt->current == CLICKUP) {
                                        HUDCreateUnits();
                                    }
                                }
                                if (create_villager_bt != nullptr && id == TOWN_CENTER) {
                                    if (create_villager_bt->current == CLICKUP) {
                                        if (App->sceneManager->level1_scene->CheckUnitsRoom()) {
                                            if (App->entityManager->player->resources.Spend(App->entityManager->unitsDB[ELF_VILLAGER]->cost)) {
                                                building->units_in_queue.push_back(ELF_VILLAGER);
                                            }
                                        } else {
                                            AlertText("NOT ENOUGH HOUSES", 5);
                                            App->audio->PlayFx(POPULATION_LIMIT);
                                        }
                                    }
                                }
                                /*if (create_hero_bt != nullptr && id == TOWN_CENTER)
							{
							if (create_hero_bt->current == CLICKUP)
							{
							HUDCreateHero();
							}
							}*/
                                /*if (!studying_tech) {*/
                                for (uint i = 0; i < App->gui->tech_bt.size(); ++i) {
                                    if (App->gui->tech_bt[i].button != nullptr) {
                                        if (App->gui->tech_bt[i].button->current == CLICKUP) {
                                            if (App->entityManager->player->tech_tree->researching_tech == false) {
                                                if (App->entityManager->player->resources.Spend(App->entityManager->player->tech_tree->all_techs.at(App->gui->tech_bt[i].type)->cost)) {
                                                    App->entityManager->player->tech_tree->StartResearch(App->gui->tech_bt[i].type);
                                                    studying_tech = true;
                                                    tech_studied = App->gui->tech_bt[i].type;
                                                }
                                            } else {
                                                App->audio->PlayFx(POPULATION_LIMIT);
                                            }
                                        }
                                    }
                                }
                                if (tech_success) {
                                    tech_success = false;
                                    HUDClearBuildingMenu();
                                    HUDBuildingMenu();
                                }
                                /*	}*/
                                /*else if (studying_tech) {

							max_life = App->entityManager->player->tech_tree->all_techs[tech_studied]->aux_timer + (App->entityManager->player->tech_tree->all_techs[tech_studied]->research_time * 1000);
							curr_life = (int)App->entityManager->player->tech_tree->all_techs[tech_studied]->research_timer.Read();
							if (curr_life >= max_life) curr_life = max_life;
							_itoa_s(curr_life, currlife, 65, 10);
							life_str += "     ";
							life_str += currlife;
							life_str += "/";
							_itoa_s(max_life, maxlife, 65, 10);
							life_str += maxlife;
							life_str += "(sec)";
							life->SetString(life_str);
							if (max_life == 0) max_life = curr_life;
							percent = ((max_life - curr_life) * 100) / max_life;
							barPercent = (percent * App->gui->SpriteBuildings.front().GetRect().w) / 100;
							bar.rect.y = bar.rect.y + bar.rect.h;
							bar.rect.h = 5;
							bar.r = 200;
							bar.g = 200;
							bar.b = 200;
							App->render->ui_toDraw.push_back(bar);
							bar.rect.w = min(App->gui->SpriteBuildings.front().GetRect().w, max(App->gui->SpriteBuildings.front().GetRect().w - barPercent, 0));
							bar.r = 0;
							bar.b = 255;
							bar.g = 100;
							App->render->ui_toDraw.push_back(bar);
							}*/
                            }
                            break;
                        case BUILDINGCREATEUNITS:
                            if (building_state != BUILDINGCREATEUNITS) {
                                HUDCreateUnits();
                            } else {
                                if (cancel_bt->current == CLICKUP) {
                                    HUDBuildingMenu();
                                } else {
                                    for (uint i = 0; i < App->gui->unit_bt.size(); ++i) {
                                        if (App->gui->unit_bt[i].button != nullptr) {
                                            if (App->gui->unit_bt[i].button->current == CLICKUP) {
                                                if (App->sceneManager->level1_scene->CheckUnitsRoom()) {
                                                    if (App->entityManager->player->resources.Spend(App->entityManager->unitsDB[App->gui->unit_bt[i].type]->cost)) {
                                                        building->units_in_queue.push_back(App->gui->unit_bt[i].type);
                                                    }
                                                } else {
                                                    AlertText("NOT ENOUGH HOUSES", 5);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            break;
                        case BUILDINGCREATEHERO:
                            if (building_state != BUILDINGCREATEHERO) {
                                HUDCreateHero();
                            }
                            if (cancel_bt->current == CLICKUP) {
                                HUDBuildingMenu();
                            }
                            for (uint i = 0; i < App->gui->unit_bt.size(); ++i) {
                                if (App->gui->unit_bt[i].button != nullptr) {
                                    if (App->gui->unit_bt[i].button->current == CLICKUP) {
                                        if (App->sceneManager->level1_scene->CheckUnitsRoom()) {
                                            if (App->entityManager->player->resources.Spend(App->entityManager->unitsDB[App->gui->unit_bt[i].type]->cost)) {
                                                building->units_in_queue.push_back(App->gui->unit_bt[i].type);
                                            }
                                        } else {
                                            AlertText("NOT ENOUGH HOUSES", 5);
                                        }
                                    }
                                }
                            }
                            break;
                        }
                    }
                    break;
                case COLLIDER_RESOURCE:
                    if (type != RESOURCEINFO) {
                        ClearSingle();
                        ClearMultiple();
                        ClearBuilding();
                        type = RESOURCEINFO;
                        StartResourceInfo();
                    } else {
                        Resource *resource = (Resource *)App->entityManager->selectedEntityList.front();
                        if (resource->contains != id) {
                            ClearResource();
                            StartResourceInfo();
                        }
                        for (list<UnitSprite>::iterator it = App->gui->SpriteResources.begin(); it != App->gui->SpriteResources.end(); ++it) {
                            if (it->GetID() == resource->contains) {
                                name->SetString(it->GetName());
                            }
                        }
                        DrawResourceBar();
                    }
                    break;
                }
            }
        }
    } else {

        if (type != NONE) {
            if (type == SINGLEINFO) {
                ClearSingle();
            } else if (type == MULTIPLESELECTION) {
                ClearMultiple();
            } else if (type == BUILDINGINFO) {
                ClearBuilding();
            } else if (type == RESOURCEINFO) {
                ClearResource();
            }
            type = NONE;
            ClearAll();
        }
    }
    bool free_unit = true;
    bool free_building = true;
    bool free_tech = true;
    bool free_skill = true;
    bool free_villager = true;

    for (uint i = 0; i < App->gui->unit_bt.size(); ++i) {
        if (App->gui->unit_bt[i].button != nullptr) {
            if (App->gui->unit_bt[i].button->current == HOVER) {
                BlitInfoUnit(App->gui->unit_bt[i]);
                free_unit = false;
            }
        }
    }

    for (uint i2 = 0; i2 < App->gui->building_bt.size(); ++i2) {
        if (App->gui->building_bt[i2].button != nullptr) {
            if (App->gui->building_bt[i2].button->current == HOVER) {
                BlitInfoBuilding(App->gui->building_bt[i2]);
                free_building = false;
            }
        }
    }

    for (uint i2 = 0; i2 < App->gui->tech_bt.size(); ++i2) {
        if (App->gui->tech_bt[i2].button != nullptr) {
            if (App->gui->tech_bt[i2].button->current == HOVER) {
                BlitInfoTech(App->gui->tech_bt[i2]);
                free_tech = false;
            }
        }
    }

    for (uint i = 0; i < App->gui->skill_bt.size(); ++i) {
        if (App->gui->skill_bt[i].button != nullptr) {
            if (App->gui->skill_bt[i].button->current == HOVER) {
                BlitInfoSkill(App->gui->skill_bt[i]);
                free_skill = false;
            }
        }
    }

    if (create_villager_bt != nullptr) {
        if (create_villager_bt->current == HOVER) {
            BlitInfoVillager();
            free_villager = false;
        }
    }

    if (free_unit == true && free_building == true && free_tech == true && free_skill == true && free_villager == true) {
        App->gui->DestroyUIElement(info_lbl);
        info_lbl = nullptr;
        App->gui->DestroyUIElement(desc_lbl);
        desc_lbl = nullptr;
        App->gui->DestroyUIElement(cost_lbl);
        cost_lbl = nullptr;
    }

    alert.Update();
}

void HUD::CleanUp()
{
    ClearMultiple();
    ClearSingle();
    ClearResource();
    ClearBuilding();
    ClearAll();
    buttons_positions.clear();
    buildings_rects.clear();
    units_rects.clear();
    type = NONE;
}

bool Gui::LoadHUDData()
{
    bool ret = false;
    pugi::xml_document HUDDataFile;
    pugi::xml_node HUDData;
    pugi::xml_node unitNodeInfo;
    pugi::xml_node buildingNodeInfo;
    pugi::xml_node resourceNodeInfo;

    HUDData = App->LoadHUDDataFile(HUDDataFile);

    if (HUDData.empty() == false) {
        for (unitNodeInfo = HUDData.child("Images").child("HUD"); unitNodeInfo; unitNodeInfo = unitNodeInfo.next_sibling("HUD")) {
            string name(unitNodeInfo.child("Info").child("Name").attribute("value").as_string());
            uint id = unitNodeInfo.child("Info").child("ID").attribute("value").as_uint();
            pair<int, int> position;
            position.first = unitNodeInfo.child("Info").child("Position").attribute("x").as_uint();
            position.second = unitNodeInfo.child("Info").child("Position").attribute("y").as_uint();
            string path(unitNodeInfo.child("Texture").child("Path").attribute("value").as_string());
            SDL_Rect rect;
            rect.x = unitNodeInfo.child("Texture").child("Rect").attribute("x").as_int();
            rect.y = unitNodeInfo.child("Texture").child("Rect").attribute("y").as_int();
            rect.w = unitNodeInfo.child("Texture").child("Rect").attribute("w").as_int();
            rect.h = unitNodeInfo.child("Texture").child("Rect").attribute("h").as_int();
            string scene(unitNodeInfo.child("Scenes").child("Scene").attribute("value").as_string());
            Info curr(name, id, position, path, rect, scene, IMAGE);
            info.push_back(curr);
        }
        for (unitNodeInfo = HUDData.child("Buttons").child("HUD"); unitNodeInfo; unitNodeInfo = unitNodeInfo.next_sibling("HUD")) {
            string name(unitNodeInfo.child("Info").child("Name").attribute("value").as_string());
            uint id = unitNodeInfo.child("Info").child("ID").attribute("value").as_uint();
            uint tier = unitNodeInfo.child("Info").child("Tier").attribute("value").as_uint();
            pair<int, int> position;
            position.first = unitNodeInfo.child("Info").child("Position").attribute("x").as_uint();
            position.second = unitNodeInfo.child("Info").child("Position").attribute("y").as_uint();
            string path(unitNodeInfo.child("Texture").child("Path").attribute("value").as_string());
            string scene(unitNodeInfo.child("Scenes").child("Scene").attribute("value").as_string());
            vector<SDL_Rect> blit;
            for (pugi::xml_node NodeInfo = unitNodeInfo.child("Blit").child("Section"); NodeInfo; NodeInfo = NodeInfo.next_sibling("Section")) {
                SDL_Rect rect;
                rect.x = NodeInfo.attribute("x").as_int();
                rect.y = NodeInfo.attribute("y").as_int();
                rect.w = NodeInfo.attribute("w").as_int();
                rect.h = NodeInfo.attribute("h").as_int();
                blit.push_back(rect);
            }
            vector<SDL_Rect> detect;
            for (pugi::xml_node NodeInfo = unitNodeInfo.child("Detect").child("Section"); NodeInfo; NodeInfo = NodeInfo.next_sibling("Section")) {
                SDL_Rect rect;
                rect.x = NodeInfo.attribute("x").as_int();
                rect.y = NodeInfo.attribute("y").as_int();
                rect.w = NodeInfo.attribute("w").as_int();
                rect.h = NodeInfo.attribute("h").as_int();
                detect.push_back(rect);
            }
            Info curr(name, id, position, path, { 0, 0, 0, 0 }, scene, BUTTON);
            curr.tier = (ButtonTier)tier;
            curr.blit_sections = blit;
            curr.detect_sections = detect;
            info.push_back(curr);
        }
        SDL_Rect proportions;
        proportions.w = HUDData.child("Sprites").child("Proportions").attribute("width").as_uint();
        proportions.h = HUDData.child("Sprites").child("Proportions").attribute("height").as_uint();

        for (unitNodeInfo = HUDData.child("Units").child("Unit"); unitNodeInfo; unitNodeInfo = unitNodeInfo.next_sibling("Unit")) {
            unit_button bt;
            EntityType type = ENTITY_UNIT;
            string name(unitNodeInfo.child("Name").attribute("value").as_string());
            bt.name = name;
            string desc(unitNodeInfo.child("Description").attribute("value").as_string());
            bt.desc = desc;
            int id = unitNodeInfo.child("ID").attribute("value").as_int();
            bt.type = (unitType)id;

            proportions.x = unitNodeInfo.child("Position").attribute("x").as_int();
            proportions.y = unitNodeInfo.child("Position").attribute("y").as_int();
            bt.blit_sections.push_back(proportions);
            bt.blit_sections.push_back(proportions);
            bt.button = nullptr;
            string wood(unitNodeInfo.child("Cost").attribute("woodCost").as_string());
            string food(unitNodeInfo.child("Cost").attribute("foodCost").as_string());
            string gold(unitNodeInfo.child("Cost").attribute("goldCost").as_string());
            string stone(unitNodeInfo.child("Cost").attribute("stoneCost").as_string());
            string costs = "wood: " + wood + " food: " + food + " gold: " + gold + " stone: " + stone;
            bt.cost = costs;

            unit_bt.push_back(bt);
            UnitSprite unit(type, proportions, id, name);
            SpriteUnits.push_back(unit);
        }
        for (unitNodeInfo = HUDData.child("Buildings").child("Building"); unitNodeInfo; unitNodeInfo = unitNodeInfo.next_sibling("Building")) {
            building_button bt;
            EntityType type = ENTITY_BUILDING;
            string name(unitNodeInfo.child("Name").attribute("value").as_string());
            bt.name = name;
            string desc(unitNodeInfo.child("Description").attribute("value").as_string());
            bt.desc = desc;
            int id = unitNodeInfo.child("ID").attribute("value").as_int();
            bt.type = (buildingType)id;

            proportions.x = unitNodeInfo.child("Position").attribute("x").as_int();
            proportions.y = unitNodeInfo.child("Position").attribute("y").as_int();
            bt.blit_sections.push_back(proportions);
            bt.blit_sections.push_back(proportions);

            bt.button = nullptr;

            string wood(unitNodeInfo.child("Cost").attribute("woodCost").as_string());
            string food(unitNodeInfo.child("Cost").attribute("foodCost").as_string());
            string gold(unitNodeInfo.child("Cost").attribute("goldCost").as_string());
            string stone(unitNodeInfo.child("Cost").attribute("stoneCost").as_string());
            string costs = "wood: " + wood + " food: " + food + " gold: " + gold + " stone: " + stone;
            bt.cost = costs;

            building_bt.push_back(bt);

            UnitSprite building(type, proportions, id, name);
            SpriteBuildings.push_back(building);
        }
        for (unitNodeInfo = HUDData.child("Resources").child("Resource"); unitNodeInfo; unitNodeInfo = unitNodeInfo.next_sibling("Resource")) {
            EntityType type = ENTITY_RESOURCE;
            string name(unitNodeInfo.child("Name").attribute("value").as_string());

            int id = unitNodeInfo.child("ID").attribute("value").as_int();
            proportions.x = unitNodeInfo.child("Position").attribute("x").as_int();
            proportions.y = unitNodeInfo.child("Position").attribute("y").as_int();

            UnitSprite unit(type, proportions, id, name);
            SpriteResources.push_back(unit);
        }
        /*for (unitNodeInfo = HUDData.child("FPTechs").child("Tech"); unitNodeInfo; unitNodeInfo = unitNodeInfo.next_sibling("Tech"))
		{
		tech_button bt;
		string name(unitNodeInfo.child("Name").attribute("value").as_string());
		bt.name = name;
		string desc(unitNodeInfo.child("Description").attribute("value").as_string());
		bt.desc = desc;
		int id = unitNodeInfo.child("ID").attribute("value").as_int();
		bt.type = (TechType)id;



		proportions.x += proportions.w;
		bt.blit_sections.push_back(proportions);
		bt.button = nullptr;

		string wood(unitNodeInfo.child("Cost").attribute("woodCost").as_string());
		string food(unitNodeInfo.child("Cost").attribute("foodCost").as_string());
		string gold(unitNodeInfo.child("Cost").attribute("goldCost").as_string());
		string stone(unitNodeInfo.child("Cost").attribute("stoneCost").as_string());
		string costs = "wood: " + wood + " food: " + food + " gold: " + gold + " stone: " + stone;
		bt.cost = costs;

		tech_bt.push_back(bt);
		}*/
        for (unitNodeInfo = HUDData.child("SATechs").child("Tech"); unitNodeInfo; unitNodeInfo = unitNodeInfo.next_sibling("Tech")) {
            tech_button bt;
            string name(unitNodeInfo.child("Name").attribute("value").as_string());
            bt.name = name;
            string desc(unitNodeInfo.child("Description").attribute("value").as_string());
            bt.desc = desc;
            int id = unitNodeInfo.child("ID").attribute("value").as_int();
            bt.type = (TechType)id;

            proportions.x = unitNodeInfo.child("Position").attribute("x").as_int();
            proportions.y = unitNodeInfo.child("Position").attribute("y").as_int();
            proportions.w = 39;
            proportions.h = 40;
            bt.blit_sections.push_back(proportions);
            proportions.x += proportions.w;
            bt.blit_sections.push_back(proportions);
            bt.button = nullptr;

            string wood(unitNodeInfo.child("Cost").attribute("woodCost").as_string());
            string food(unitNodeInfo.child("Cost").attribute("foodCost").as_string());
            string gold(unitNodeInfo.child("Cost").attribute("goldCost").as_string());
            string stone(unitNodeInfo.child("Cost").attribute("stoneCost").as_string());
            string costs = "wood: " + wood + " food: " + food + " gold: " + gold + " stone: " + stone;
            bt.cost = costs;

            sauron_tech_bt.push_back(bt);
        }
        for (pugi::xml_node Node = HUDData.child("Skills").child("Skill"); Node; Node = Node.next_sibling("Skill")) {
            skill_button bt;
            string name(Node.child("Name").attribute("value").as_string());
            bt.name = name;
            string desc(Node.child("Description").attribute("value").as_string());
            bt.desc = desc;

            string cooldown(Node.child("Cooldown").attribute("value").as_string());
            bt.cooldown = cooldown;

            string changes(Node.child("Changes").attribute("value").as_string());
            bt.changes = changes;

            int id = Node.child("ID").attribute("value").as_int();
            bt.type = (Skill_type)id;

            proportions.x = Node.child("Position").attribute("x").as_int();
            proportions.y = Node.child("Position").attribute("y").as_int();
            proportions.w = 39;
            proportions.h = 40;

            bt.blit_sections.push_back(proportions);
            proportions.x += proportions.w;
            bt.blit_sections.push_back(proportions);
            bt.button = nullptr;

            skill_bt.push_back(bt);
        }
    }
    return ret;
}

void Gui::LoadTechInfo()
{
    for (vector<Tech *>::iterator it = App->entityManager->player->tech_tree->all_techs.begin(); it != App->entityManager->player->tech_tree->all_techs.end(); ++it) {
        tech_button bt;
        string name((*it)->name);
        bt.name = name;
        string desc((*it)->desc);
        bt.desc = desc;
        int id = (*it)->id;
        bt.type = (TechType)id;

        bt.blit_sections.push_back((*it)->minature);

        SDL_Rect p = (*it)->minature;
        p.x += p.w;

        bt.blit_sections.push_back(p);
        bt.button = nullptr;

        bt.cost = "wood: " + std::to_string((*it)->cost.wood) + " food: " + std::to_string((*it)->cost.food) + " gold: " + std::to_string((*it)->cost.gold) + " stone: " + std::to_string((*it)->cost.stone);
        tech_bt.push_back(bt);
    }
}
