#include "puzzles.h"
#include "frontend.h"

extern game fifteen;
extern game bridges;
extern game twiddle;
extern game flip;
extern game pegs;

const game  *gamelist[] = {
    &fifteen,
    &flip,
    &twiddle,
    &pegs,
    &bridges,
    0,
};

static void
_choice_close(int choice, Ewl_Widget *parent,void *userdata) {
    struct frontend * fe = (struct frontend *) userdata;
    printf("parent %x %x\n", parent, userdata);
//    if(fe->first_time)
//        exit_cb(fe->window, NULL, userdata);
}

static void
_choice_handler(int choice, Ewl_Widget *parent,void *userdata) {
    struct frontend * fe = (struct frontend *) userdata;
    fe->first_time = 0;
    printf("choicebox %x %x\n", parent, userdata);
    fini_choicebox(parent);
    create_game(fe, gamelist[choice]);
}

void gamelist_menu(Ewl_Widget *parent, void * userdata) {
    char **choices;
    const struct game **each;
    struct game *cur;
    const char **ptr;
    Ewl_Widget *w;
    int i;

    i = 0;
    each = gamelist;
    while(*each) {
       i++; 
       each++;
    };

    choices = malloc(sizeof(char *) * (i+1));
    ptr=choices;

    each = gamelist;
    while(*each) {
        cur = *each;
       *ptr=cur->name;
       each++;
       ptr++;
    };
    *ptr=NULL;

    w = init_choicebox(choices, NULL, i, _choice_handler, _choice_close,
        "Select puzzle", parent, userdata, TRUE);
    ewl_widget_show(w);
    ewl_widget_data_set(EWL_WIDGET(parent), (void *)"choicebox",
                (void *) w);
    ewl_window_raise(w);
    ewl_widget_focus_send(w);
    printf("choicebox %x %x\n", w, userdata);
    printf("parent %x\n", parent);
    free(choices);
};

struct game *
lookup_game_by_name(const char * name) {
    struct game **each;
    each = gamelist;
    while(*each) {
        if(!strcasecmp((*each)->name, name))
            return *each;
        ++each;
    }
}


void
setup_gamelist(Ewl_Widget *w) {
    struct game **each;
    each = gamelist;
    while(*each){
       append_game(w, *each);
       ++each;
    }
}
