#include <Ewl.h>
#include "puzzles.h"
#include "frontend.h"
#include "gui_util.h"


void frontend_default_colour(struct frontend *fe, float *output)
{
   output[0] = 0;
   output[1] = 0;
   output[2] = 0;
}

void
gui_setup_colors(struct frontend *fe) 
{
        float * colours;
        int ncolours;
        int i;

        colours = midend_colours(fe->me, &ncolours);
        fe->ncolours = ncolours;
        fe->colours = snewn(ncolours, Ewl_Color_Set);
        for (i = 0; i < ncolours; i++) {
            fe->colours[i].r = colours[i*3] * 0xFF;
            fe->colours[i].g = colours[i*3+1] * 0xFF;
            fe->colours[i].b = colours[i*3+2] * 0xFF;
            fe->colours[i].a = 0xFF;
        }
}

void
gui_apply_color(struct frontend *fe, int color) 
{
    if(color > fe->ncolours)
        fatal("requested color %d index greater then total num of colors: %d\n",
            color, fe->ncolours);
    ewl_drawable_set_colors(fe->area, &fe->colours[color]);
}

void
gui_delete_colors(struct frontend *fe)
{
    if(fe->colours)
        sfree(fe->colours);
}

void gui_get_size(struct frontend *fe, int *px, int *py)
{
    int x, y;

    /*
     * Currently I don't want to make the GTK port scale large
     * puzzles to fit on the screen. This is because X does permit
     * extremely large windows and many window managers provide a
     * means of navigating round them, and the users I consulted
     * before deciding said that they'd rather have enormous puzzle
     * windows spanning multiple screen pages than have them
     * shrunk. I could change my mind later or introduce
     * configurability; this would be the place to do so, by
     * replacing the initial values of x and y with the screen
     * dimensions.
     */
    x = INT_MAX;
    y = INT_MAX;
    midend_size(fe->me, &x, &y, FALSE);
    *px = x;
    *py = y;
}

int gui_get_color(frontend *fe, int color, int component) {
        int i, ncolours;
        float *colours;

        colours = midend_colours(fe->me, &ncolours);
        fe->ncolours = ncolours;
//        fe->colours = snewn(ncolours, Ewl_Color_Set);
        return colours[color*3+component] * 0xFFFF;
}
