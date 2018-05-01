#include <stdlib.h>

#include "Application.h"
#include "p2Log.h"

#include <SDL2/SDL.h>

enum MainState {
    CREATE = 1,
    AWAKE,
    START,
    LOOP,
    CLEAN,
    FAIL,
    EXIT
};

Application *App = NULL;

int main(int argc, char *args[])
{
    LOG("Engine starting ... %d", 0);

    MainState state = MainState::CREATE;
    int result = EXIT_FAILURE;

    while (state != EXIT) {
        switch (state) {

        // Allocate the engine --------------------------------------------
        case CREATE:
            LOG("CREATION PHASE ===============================", 0);

            App = new Application(argc, args);

            if (App != NULL)
                state = AWAKE;
            else
                state = FAIL;

            break;

        // Awake all modules -----------------------------------------------
        case AWAKE:
            LOG("AWAKE PHASE ===============================", 0);
            if (App->Awake() == true)
                state = START;
            else {
                LOG("ERROR: Awake failed", 0);
                state = FAIL;
            }

            break;

        // Call all modules before first frame  ----------------------------
        case START:
            LOG("START PHASE ===============================", 0);
            if (App->Start() == true) {
                state = LOOP;
                LOG("UPDATE PHASE ===============================", 0);
            } else {
                state = FAIL;
                LOG("ERROR: Start failed", 0);
            }
            break;

        // Loop all modules until we are asked to leave ---------------------
        case LOOP:
            if (App->Update() == false)
                state = CLEAN;
            break;

        // Cleanup allocated memory -----------------------------------------
        case CLEAN:
            LOG("CLEANUP PHASE ===============================", 0);
            if (App->CleanUp() == true) {
                RELEASE(App);
                result = EXIT_SUCCESS;
                state = EXIT;
            } else
                state = FAIL;

            break;

        // Exit with errors and shame ---------------------------------------
        case FAIL:
            LOG("Exiting with errors :(", 0);
            result = EXIT_FAILURE;
            state = EXIT;
            break;
        }
    }

    LOG("... Bye! :)\n", 0);

    // Dump memory leaks
    return result;
}
