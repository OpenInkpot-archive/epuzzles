#include "puzzles.h"
#include "frontend.h"

extern game fifteen;

const game  *gamelist[] = {
    &fifteen,
    0,
};

void
setup_gamelist(Ewl_Widget *w) {
    struct game **each;
    each = gamelist;
    while(*each){
       append_game(w, *each);
       ++each;
    }
}
