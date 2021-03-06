#include "Unit.h"
#include "Render.h"
#include "Application.h"
#include "EntityManager.h"
#include "Pathfinding.h"
#include "p2Log.h"
#include <cmath>
#include "Map.h"
#include "Application.h"
#include "Collision.h"
#include "Textures.h"
#include "p2Defs.h"
#include "Scene.h"
#include "Gui.h"
#include "SceneManager.h"
#include "Hero.h"
#include "Orders.h"
#include "Villager.h"
#include "Audio.h"
#include "AI.h"
#include "QuestManager.h"
#include "FogOfWar.h"

Unit::Unit()
{
}

Unit::Unit(int posX, int posY, Unit *unit)
{
    name = unit->name;
    entityPosition.x = posX;
    entityPosition.y = posY;
    type = unit->type;
    faction = unit->faction;
    unitPiercingDamage = unit->unitPiercingDamage;
    unitMovementSpeed = unit->unitMovementSpeed;
    currentDirection = unit->currentDirection;
    unitIdleTexture = unit->unitIdleTexture;
    unitMoveTexture = unit->unitMoveTexture;
    unitAttackTexture = unit->unitAttackTexture;
    unitDieTexture = unit->unitDieTexture;
    selectionRadius = unit->selectionRadius;
    selectionAreaCenterPoint = unit->selectionAreaCenterPoint;
    cooldown_time = unit->cooldown_time;

    Life = unit->Life;
    MaxLife = unit->MaxLife;
    Attack = unit->Attack;
    Defense = unit->Defense;
    cost = unit->cost;
    range_value = unit->range_value;

    //Animations
    idleAnimations = unit->idleAnimations;
    movingAnimations = unit->movingAnimations;
    attackingAnimations = unit->attackingAnimations;
    dyingAnimations = unit->dyingAnimations;

    entityTexture = unitIdleTexture;

    currentAnim = &idleAnimations;

    SDL_Rect r = currentAnim->at(currentDirection).GetCurrentFrame();
    collider = App->collision->AddCollider({ entityPosition.x, entityPosition.y + selectionAreaCenterPoint.y }, r.w / 2, COLLIDER_UNIT, App->entityManager, (Entity *)this);
    los = App->collision->AddCollider({ entityPosition.x, entityPosition.y + selectionAreaCenterPoint.y }, unit->los_value, COLLIDER_LOS, App->entityManager, (Entity *)this);

    if (unit->range_value) {
        range = App->collision->AddCollider({ entityPosition.x, entityPosition.y + selectionAreaCenterPoint.y }, unit->range_value, COLLIDER_RANGE, App->entityManager, (Entity *)this);
    } else {
        range = App->collision->AddCollider({ entityPosition.x, entityPosition.y + selectionAreaCenterPoint.y }, collider->r, COLLIDER_RANGE, App->entityManager, (Entity *)this);
    }

    next_pos = destinationTileWorld = entityPosition;

    entityType = ENTITY_UNIT;
}

Unit::~Unit()
{
    for (list<Order *>::iterator it = order_list.begin(); it != order_list.end(); it++)
        RELEASE(*it);
}

bool Unit::Update(float dt)
{
    r = currentAnim->at(currentDirection).GetCurrentFrame();

    if (state != DESTROYED) {

        if (IsHero) {
            Hero *hero = (Hero *)this;
            hero->HeroUpdate();
        }

        if (!order_list.empty()) {

            if (order_list.front()->state == NEEDS_START) {
                order_list.front()->Start(this);
            }

            if (order_list.front()->state == EXECUTING) {
                order_list.front()->Execute(this);
            }

            if (order_list.front()->state == COMPLETED) {
                order_list.pop_front();
            }
        } else {
            if (entityTexture != unitIdleTexture) {
                SetTexture(state = IDLE);
            }
        }
    } else {

        if (entityTexture != unitDieTexture) {
            SetTexture(state = DESTROYED);
            App->audio->PlayUnitDeadSound(this);
        }

        if (currentAnim->at(currentDirection).Finished()) {
            if (faction == App->entityManager->player->faction) {
                App->entityManager->player->units.remove(this);
                if (IsVillager) {
                    App->entityManager->player->villagers.remove((Villager *)this);
                }
            } else {
                App->entityManager->AI_faction->units.remove(this);
                if (IsVillager) {
                    App->entityManager->AI_faction->villagers.remove((Villager *)this);
                }
            }

            App->collision->DeleteCollider(collider);
            App->collision->DeleteCollider(range);
            App->collision->DeleteCollider(los);

            if (faction == FREE_MEN) {
                App->fog->DeleteEntityFog(this->entityID);
            }

            App->entityManager->DeleteEntity(this);
        }
    }

    return true;
}

void Unit::Destroy()
{

    if (faction == App->entityManager->player->faction) {
        App->entityManager->player->units.remove(this);
        if (IsVillager) {
            App->entityManager->player->villagers.remove((Villager *)this);
        }
    } else {
        App->entityManager->AI_faction->units.remove(this);
        if (IsVillager) {
            App->entityManager->AI_faction->villagers.remove((Villager *)this);
        }
    }

    App->collision->DeleteCollider(collider);
    App->collision->DeleteCollider(range);
    App->collision->DeleteCollider(los);

    if (faction == FREE_MEN) {
        App->fog->DeleteEntityFog(this->entityID);
    }

    App->entityManager->DeleteEntity(this);
}

bool Unit::Draw()
{
    Sprite aux;

    aux.rect = r;
    aux.texture = entityTexture;
    aux.pos.x = entityPosition.x - (r.w / 2);
    aux.pos.y = entityPosition.y - (r.h / 2);
    if (collider != nullptr) {
        aux.priority = collider->pos.y;
    } else {
        aux.priority = entityPosition.y /* - (r.h / 2) + r.h*/;
    }
    aux.flip = currentAnim->at(currentDirection).flip;

    App->render->sprites_toDraw.push_back(aux);

    if (last_life != Life) {
        lifebar_timer.Start();
        last_life = Life;
    }

    if (lifebar_timer.ReadSec() < 5) {
        drawLife({ entityPosition.x - 25, entityPosition.y - (r.h / 2) }); //25:  HPBAR_WIDTH / 2
    }

    if (!path.empty() && App->collision->debug) {
        for (vector<iPoint>::iterator it = path.begin(); it != path.end(); it++) {
            iPoint toMap = App->map->WorldToMap((*it).x, (*it).y);
            iPoint toWorld = App->map->MapToWorld((*it).x, (*it).y);
            App->render->DrawIsometricRect((*it), 96);
        }
    }

    return true;
}

void Unit::LookAt(fPoint dest)
{
    float angle = atan2f(dest.y, dest.x) * RADTODEG;

    if (angle < 22.5 && angle > -22.5) {
        currentDirection = RIGHT;
    } else if (angle >= 22.5 && angle <= 67.5) {
        currentDirection = DOWN_RIGHT;
    } else if (angle > 67.5 && angle < 112.5) {
        currentDirection = DOWN;
    } else if (angle >= 112.5 && angle <= 157.5) {
        currentDirection = DOWN_LEFT;
    } else if (angle > 157.5 || angle < -157.5) {
        currentDirection = LEFT;
    } else if (angle >= -157.5 && angle <= -112.5) {
        currentDirection = UP_LEFT;
    } else if (angle > -112.5 && angle < -67.5) {
        currentDirection = UP;
    } else if (angle >= -67.5 && angle <= -22.5) {
        currentDirection = UP_RIGHT;
    }
}

void Unit::SetTexture(EntityState texture_of)
{
    Villager *villager = nullptr;

    switch (texture_of) {
    case IDLE:
        entityTexture = unitIdleTexture;
        currentAnim = &idleAnimations;
        break;
    case MOVING:
        entityTexture = unitMoveTexture;
        currentAnim = &movingAnimations;
        break;
    case ATTACKING:
        entityTexture = unitAttackTexture;
        currentAnim = &attackingAnimations;
        break;
    case DESTROYED:
        entityTexture = unitDieTexture;
        currentAnim = &dyingAnimations;
        break;
    case GATHERING:
        villager = (Villager *)this;
        entityTexture = villager->unitChoppingTexture;
        currentAnim = &villager->choppingAnimations;
        break;
    case CONSTRUCTING:
        villager = (Villager *)this;
        entityTexture = villager->unitChoppingTexture;
        currentAnim = &villager->choppingAnimations;
        break;
    }
}

void Unit::SubordinatedMovement(iPoint p)
{

    if (sub_movement == nullptr) {
        sub_movement = new MoveToOrder(this, p);
        if (sub_movement->state == NEEDS_START) {
            sub_movement->Start(this);
        }
    } else {
        if (sub_movement->state == COMPLETED) {
            RELEASE(sub_movement);
            sub_movement = nullptr;
        } else {
            sub_movement->Execute(this);
        }
    }
}
