#include "puzzles.h"
#include "frontend.h"

extern game fifteen;

void
setup_gamelist(Ewl_Widget *w) {
   append_game(w, &fifteen);
}
