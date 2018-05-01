#ifndef __PARTICLEMANAGER_H__
#define __PARTICLEMANAGER_H__

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL.h>
#include "Module.h"
#include "Timer.h"
#include "Animation.h"
#include "Collision.h"
#include <list>
#include <string>

using namespace std;

#define GRAVITY 10
#define MAX_PARTICLES 50
#define EMITTER_SPEED 0.05

struct SDL_Texture;
class Collider;

enum ParticleType {
    ARROW
};

// Info holds the data gathered in Awake.
struct ParticleInfo
{
    string name;
    int id;
    int lifespan;
    Animation anim;
    string path;
    SDL_Texture *texture;
    uint states;

    ParticleInfo(string argname, string argpath, int argid, int arglifespan, Animation arganim, uint argstates) :
        name(argname), path(argpath), id(argid), lifespan(arglifespan), anim(arganim), states(argstates) {}
};

class Particle
{
public:
    pair<float, float> pos, curr_spd;

    int lifetime; // (s)
    bool alive = true;
    float dg;
    ParticleType type;
    SDL_Texture *texture = nullptr;
    string name;
    Collider *collider = nullptr;
    Animation anim;
    Timer timer;
    uint states;

    virtual void Update(){};
    virtual void Draw(){};
    virtual bool IsAlive() { return true; };
};

class Arrow : public Particle
{
public:
    Arrow(int spd, pair<int, int> source, pair<int, int> dest);
    pair<float, float> src, dst, pos;
    float speed;
    pair<float, float> spd;
    float spd_x, spd_y;
    float time;
    vector<SDL_Rect> blits;
    SDL_Rect collider, blit;
    bool flipped = false;
    void Update() override;
    void Draw() override;
    bool IsAlive() override;
};

class MovableParticle : public Particle
{
public:
    MovableParticle(bool gravity, pair<float, float> startingforce, pair<float, float> startingposition);
    pair<float, float> spd, force;
    bool gravity = false;
    void Update() override;
    void Draw() override;
    bool IsAlive() override;
};
class StaticBucle : public Particle
{
public:
    StaticBucle(pair<float, float> startingposition, bool);
    bool finite = false;
    void Update() override;
    void Draw() override;
    bool IsAlive() override;
};

class Emitter
{
public:
    Emitter(pair<float, float> startingposition, bool finite, float duration);
    pair<float, float> pos, force;
    // It is not actual force as in m/s^2.
    Timer timer;
    // Just a helper:
    pair<float, float> speed_orig;
    // Emision frequency:
    float speed, lifetime;
    bool finite = false;
    bool alive = true;
    ParticleType type;
    void Update(float dt);
    bool IsAlive();
    void SetPos(pair<float, float> pos);
    void SetSpd(pair<float, float> extra_speed);
};
class ParticleManager : public Module
{
public:
    ParticleManager();

    ~ParticleManager() override;

    bool Awake(pugi::xml_node &) override;

    bool Start() override;

    bool Update(float dt) override;

    bool PostUpdate() override;

    bool CleanUp() override;
    bool LoadParticleData();

    Particle *CreateArrow(int spd, pair<uint, uint> source, pair<uint, uint> destination);
    Particle *CreateMovableParticle(pair<float, float> startingposition, pair<float, float> startingforce, bool gravity, ParticleType type);
    Particle *CreateStaticBucle(pair<float, float> startingposition, bool finite, ParticleType type);
    Emitter *CreateEmitter(pair<float, float> startingposition, bool finite, float duration, ParticleType type);

private:
    bool DestroyParticle(Particle *curr);
    bool DestroyEmitter(Emitter *curr);

public:
    pair<int, int> window_size;
    Timer update;

private:
    list<Particle *> particles;
    list<Emitter *> emitters;
    vector<ParticleInfo> info;
};

#endif
