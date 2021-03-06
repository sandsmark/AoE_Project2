#include "Resource.h"
#include "Render.h"
#include "Application.h"
#include "p2Log.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Collision.h"
#include "FogOfWar.h"
#include "Textures.h"

Resource::Resource()
{
}

Resource::Resource(int posX, int posY, Resource *resource)
{
    entityPosition.x = posX;
    entityPosition.y = posY;
    name = resource->name;
    res_type = resource->res_type;
    contains = resource->contains;
    MaxLife = resource->Life;
    Life = resource->Life;
    entityTexture = resource->entityTexture;
    selectionWidth = resource->selectionWidth;
    selectionAreaCenterPoint = resource->selectionAreaCenterPoint;

    int rectID = rand() % resource->blit_rects.size();
    blit_rect = resource->blit_rects.at(rectID);

    if (res_type >= MOUNT_1 && res_type <= MOUNT_6) {
        if (selectionWidth) {
            collider = App->collision->AddCollider({ entityPosition.x, entityPosition.y - selectionAreaCenterPoint.y + 200 }, selectionWidth, COLLIDER_RESOURCE, (Module *)App->entityManager, this);
        } else {
            collider = App->collision->AddCollider({ entityPosition.x, entityPosition.y - selectionAreaCenterPoint.y + 200 }, blit_rect.w / 2, COLLIDER_RESOURCE, (Module *)App->entityManager, this);
        }
    } else {
        if (selectionWidth) {
            collider = App->collision->AddCollider({ entityPosition.x, entityPosition.y }, selectionWidth, COLLIDER_RESOURCE, (Module *)App->entityManager, this);
        } else {
            collider = App->collision->AddCollider({ entityPosition.x, entityPosition.y }, blit_rect.w / 2, COLLIDER_RESOURCE, (Module *)App->entityManager, this);
        }
    }

    entityType = ENTITY_RESOURCE;

    faction = NATURE;
    App->fog->AddEntity(this);
}

Resource::Resource(int posX, int posY, Resource *resource, SDL_Rect rect)
{
    entityPosition.x = posX;
    entityPosition.y = posY;

    res_type = resource->res_type;
    contains = resource->contains;
    MaxLife = resource->Life;
    Life = resource->Life;
    entityTexture = resource->entityTexture;
    selectionWidth = resource->selectionWidth;
    selectionAreaCenterPoint = resource->selectionAreaCenterPoint;

    blit_rect = rect;

    if (selectionWidth) {
        collider = App->collision->AddCollider({ entityPosition.x, entityPosition.y - selectionAreaCenterPoint.y }, selectionWidth, COLLIDER_RESOURCE, (Module *)App->entityManager, this);
    } else {
        collider = App->collision->AddCollider({ entityPosition.x, entityPosition.y - selectionAreaCenterPoint.y }, blit_rect.w / 2, COLLIDER_RESOURCE, (Module *)App->entityManager, this);
    }

    entityType = ENTITY_RESOURCE;

    faction = NATURE;
    App->fog->AddEntity(this);
}

Resource::~Resource()
{
}

bool Resource::Draw()
{
    Sprite resource;

    if (res_type >= MOUNT_1 && res_type <= MOUNT_6) {
        resource.pos.x = entityPosition.x - (blit_rect.w / 2);
        resource.pos.y = entityPosition.y - selectionAreaCenterPoint.y - 200;
        resource.texture = entityTexture;
    } else {
        resource.pos.x = entityPosition.x - (blit_rect.w / 2);
        resource.pos.y = entityPosition.y - selectionAreaCenterPoint.y + 15;
        resource.texture = entityTexture;
    }

    if (collider != nullptr) {
        resource.priority = collider->pos.y;
    } else {
        resource.priority = entityPosition.y /* - (r.h / 2) + r.h*/;
    }
    resource.rect = blit_rect;

    App->render->sprites_toDraw.push_back(resource);
    return true;
}

void Resource::Destroy()
{

    state = DESTROYED;

    if (collider != nullptr) {
        App->collision->DeleteCollider(collider);
    }
    App->entityManager->DeleteEntity(this);
}
