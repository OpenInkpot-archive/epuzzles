#include <stdio.h>
#include <Ewl.h>
#include <Efreet.h>
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>
#include "puzzles.h"
#include "frontend.h"
#include "gui_util.h"


void frontend_default_colour(struct frontend *fe, float *output)
{
   output[0] = 1.0;
   output[1] = 1.0;
   output[2] = 1.0;
}

void
gui_setup_colors(struct frontend *fe) 
{
        float * colours;
        int ncolours;
        int i;

        printf("midend_colours(): enter\n");
        colours = midend_colours(fe->me, &ncolours);
        printf("midend_colours(): leave\n");
        fe->ncolours = ncolours;
        fe->colours = snewn(ncolours, Ewl_Color_Set);
        for (i = 0; i < ncolours; i++) {
            fe->colours[i].r = colours[i*3] * 0xFF;
            fe->colours[i].g = colours[i*3+1] * 0xFF;
            fe->colours[i].b = colours[i*3+2] * 0xFF;
            fe->colours[i].a = 0xFF;
            printf("color %d: red:%d green:%d blue:%d alpha:%d\n",
                i, fe->colours[i].r, fe->colours[i].g,
                fe->colours[i].b, fe->colours[i].a);
        }
        printf("Total %d colors\n", ncolours);
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

int
gui_get_config_color(char *puzzle, char *colorname, int *r, int *g, int *b) {
    uint16_t rr=0xFFFF;
    uint16_t gg=0xFFFF;
    uint16_t bb=0xFFFF;
    Efreet_Ini *config;
    const char * config_file = "epuzzles.rc";
    const char *line;
    int rc = 0;

    printf("gui_get_config_color(%s, %s);\n", puzzle, colorname);
    config = efreet_ini_new(config_file);

    if(!config){
        printf("Can't load config %s\n", config_file);
        goto exit;
    }

    if(!efreet_ini_section_set(config, puzzle))
        goto exit;

    line = efreet_ini_string_get(config, colorname);
    if(!line)
        goto exit;

    xcb_aux_parse_color(line, &rr, &gg, &bb);
    printf("loaded colorline %s, r:%x g:%x b:%x\n",line, rr, gg, bb);
    rc = 1;
    
    exit:
    if(config)
        efreet_ini_free(config);
    *r = rr >> 8;
    *g = gg >> 8;
    *b = bb >> 8;
    return rc;
}
