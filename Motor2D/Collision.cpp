#include "Collision.h"
#include "Application.h"
#include "Input.h"
#include "p2Log.h"
#include "Render.h"

Collision::Collision() : Module()
{
	name = "collision";

	matrix[COLLIDER_UNIT][COLLIDER_UNIT] = true;
	matrix[COLLIDER_UNIT][COLLIDER_BUILDING] = true;
	matrix[COLLIDER_UNIT][COLLIDER_RESOURCE] = true;
	matrix[COLLIDER_UNIT][COLLIDER_RANGE] = true;
	matrix[COLLIDER_UNIT][COLLIDER_LOS] = true;

	matrix[COLLIDER_BUILDING][COLLIDER_UNIT] = true;
	matrix[COLLIDER_BUILDING][COLLIDER_BUILDING] = false;
	matrix[COLLIDER_BUILDING][COLLIDER_RESOURCE] = false;
	matrix[COLLIDER_BUILDING][COLLIDER_RANGE] = false;
	matrix[COLLIDER_BUILDING][COLLIDER_LOS] = false;

	matrix[COLLIDER_RESOURCE][COLLIDER_UNIT] = true;
	matrix[COLLIDER_RESOURCE][COLLIDER_BUILDING] = false;
	matrix[COLLIDER_RESOURCE][COLLIDER_RESOURCE] = false;
	matrix[COLLIDER_RESOURCE][COLLIDER_RANGE] = false;
	matrix[COLLIDER_RESOURCE][COLLIDER_LOS] = false;

	matrix[COLLIDER_RANGE][COLLIDER_UNIT] = true;
	matrix[COLLIDER_RANGE][COLLIDER_BUILDING] = true;
	matrix[COLLIDER_RANGE][COLLIDER_RESOURCE] = false;
	matrix[COLLIDER_RANGE][COLLIDER_RANGE] = false;
	matrix[COLLIDER_RANGE][COLLIDER_LOS] = false;

	matrix[COLLIDER_LOS][COLLIDER_UNIT] = false;
	matrix[COLLIDER_LOS][COLLIDER_BUILDING] = false;
	matrix[COLLIDER_LOS][COLLIDER_RESOURCE] = false;
	matrix[COLLIDER_LOS][COLLIDER_RANGE] = false;
	matrix[COLLIDER_LOS][COLLIDER_LOS] = false;

}


Collision::~Collision()
{
}

bool Collision::Awake(pugi::xml_node &)
{
	return true;
}

bool Collision::Start()
{
	return true;
}

bool Collision::PreUpdate()
{

	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); it++) {
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = colliders.erase(it);

			for (list<Collision_data*>::iterator it2 = collision_list.begin(); it2 != collision_list.end(); it2++) {
				if ((*it) == (*it2)->c1 || (*it) == (*it2)->c2)
					(*it2)->state = SOLVED;
			}
		}
	}

	Collider *c1;
	Collider *c2;

	for (list<Collider*>::iterator col1 = colliders.begin(); col1 != colliders.end(); col1++) {
		c1 = (*col1);

		if (c1->type != COLLIDER_UNIT)
			continue;

		for (list<Collider*>::iterator col2 = colliders.begin(); col2 != colliders.end(); col2++) {
			c2 = (*col2);

			if (c1->CheckCollision(c2) == true && c1 != c2)  {

				if ((matrix[c1->type][c2->type] && c1->callback) || (matrix[c2->type][c1->type] && c2->callback)) {

					if (!FindCollision(c1, c2)) {
						Collision_data* collision = new Collision_data(c1, c2);
						collision_list.push_back(collision);
					}
				}
			}
		}
	}
	

	for (list<Collision_data*>::iterator collisions = collision_list.begin(); collisions != collision_list.end(); collisions++) {

		if ((*collisions)->state == UNSOLVED) {
			(*collisions)->c1->callback->OnCollision(*(*collisions));
			continue;
		}

		if ((*collisions)->state == SOLVING) {
			if ((*collisions)->c1->CheckCollision((*collisions)->c2) == false)
				(*collisions)->state = SOLVED;
		}

		if ((*collisions)->state == SOLVED) {
			(*collisions)->c1->colliding = false;
			(*collisions)->c2->colliding = false;
			RELEASE(*collisions);
			collisions = collision_list.erase(collisions);
		}
	}

	return true;
}

bool Collision::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		debug = !debug;
	}

	if (debug) {
		DebugDraw();
	}

	return true;
}

bool Collision::CleanUp()
{
	LOG("Freeing colliders");
	list<Collider*>::reverse_iterator it = colliders.rbegin();

	while (it != colliders.rend())
	{
		RELEASE(*it);
		++it;
	}

	colliders.clear();

	return true;
}

Collider * Collision::AddCollider(iPoint position, int radius, COLLIDER_TYPE type, Module* callback, Entity* entity)
{
	Collider* ret = new Collider(position, radius, type, callback, entity);
	colliders.push_back(ret);

	return ret;
}

void Collision::DeleteCollider(Collider * collider)
{
	collider->to_delete = true;
}

bool Collider::CheckCollision(Collider* c2) const
{
	return (pos.DistanceTo(c2->pos) < (r + c2->r));
}

Unit* Collider::GetUnit() {

	Unit* unit = nullptr;

	if (type == COLLIDER_UNIT)
		unit = (Unit*)entity;

	return unit;
}

Building* Collider::GetBuilding() {

	Building* building = nullptr;

	if (type == COLLIDER_BUILDING)
		building = (Building*)entity;

	return building;
}

Resource* Collider::GetResource() {

	Resource* resource = nullptr;

	if (type == COLLIDER_RESOURCE)
		resource = (Resource*)entity;

	return resource;
}


void Collision::DebugDraw()
{

	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); it++)
	{
		if (App->render->CullingCam((*it)->pos)) {
			if ((*it)->colliding)
				App->render->DrawCircle((*it)->pos.x, (*it)->pos.y, (*it)->r, 255, 0, 0, 255);
			else
				App->render->DrawCircle((*it)->pos.x, (*it)->pos.y, (*it)->r, 0, 0, 255, 255);
		}
	}
}

bool Collision::FindCollision(Collider* col1, Collider* col2) {

	for (list<Collision_data*>::iterator it = collision_list.begin(); it != collision_list.end(); it++) {

		if (((*it)->c1 == col1 && (*it)->c2 == col2) || ((*it)->c2 == col1 && (*it)->c1 == col2))
			return true;
	}

	return false;

}
