#ifndef _GUI_UTIL_H
#define _GUI_UTIL_H 1

void gui_redraw ( struct frontend *fe);
void gui_apply_color(struct frontend *fe, int color);
void gui_setup_colors(struct frontend *fe);
void gui_delete_colors(struct frontend *fe);

void gui_set_key_handler(struct frontend *fe);

extern void gui_get_size(frontend *fe, int *px, int *py);
extern int gui_get_color(frontend *fe, int color, int component);

#define FE_COLOR_RED 0
#define FE_COLOR_GREEN 1
#define FE_COLOR_BLUE 2

#define SET_COLOR(setter, fe, object, color) \
    setter(object, gui_get_color(fe, color, FE_COLOR_RED),\
                   gui_get_color(fe, color, FE_COLOR_GREEN,\
                   gui_get_color(fe, color, FE_COLOR_BLUE))\

#endif
