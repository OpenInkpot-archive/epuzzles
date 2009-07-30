#include "puzzles.h"
#include "frontend.h"

extern game fifteen;
extern game bridges;
extern game twiddle;
extern game flip;
extern game pegs;
extern game unequal;
extern game solo;

const game  *gamelist[] = {
    &fifteen,
    &flip,
    &twiddle,
    &pegs,
    &bridges,
    &unequal,
    &solo,
    0,
};


struct game *
lookup_game_by_name(const char * name) {
    struct game **each;
    each = gamelist;
    while(*each) {
        if(!strcasecmp((*each)->name, name) || \
           !strcasecmp((*each)->htmlhelp_topic, name))
                return *each;
        ++each;
    }
}

