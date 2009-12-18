#include "puzzles.h"
#include "zebra.h"

void
draw_zebra(drawing* dr, int x1, int y1, int x2, int y2,
            int steps, int c1, int c2)
{
    while(steps--)
        draw_rect(dr, ++x1, ++y1, --x2, --y2, (steps & 1) ? c1 : c2 );
}
