#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "Module.h"

struct SDL_RWops;

int close_sdl_rwops(SDL_RWops *rw);

class FileSystem : public Module
{
public:
    FileSystem();

    // Destructor
    ~FileSystem() override;

    // Called before render is available
    bool Awake(pugi::xml_node &) override;

    // Called before quitting
    bool CleanUp() override;

    // Utility functions
    bool AddPath(const char *path_or_zip, const char *mount_point = nullptr);
    bool Exists(const char *file) const;
    bool IsDirectory(const char *file) const;
    const char *GetSaveDirectory() const
    {
        return "save/";
    }

    // Open for Read/Write
    unsigned int LoadFile(const char *file, char **buffer) const;
    SDL_RWops *LoadFile(const char *file) const;

    unsigned int Save(const char *file, const char *buffer, unsigned int size) const;

private:
    using Module::Load;
};

#endif // __FILESYSTEM_H__
