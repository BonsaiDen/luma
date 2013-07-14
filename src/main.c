#include "../include/game.h"

int main(int argc, char **argv) {

    int ret = 0;
    if (game_init(argc, argv)) {

        if (!game_update()) {
            ret = 2;
        }

        if (!game_shutdown()) {
            ret = 3;
        }

    } else {
        ret = 1;
    }

    return ret;

}

