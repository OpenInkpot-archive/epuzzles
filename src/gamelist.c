#include "puzzles.h"
#include "frontend.h"

extern game fifteen;

const game  *gamelist[] = {
    &fifteen,
    0,
};

static void
_choice_close(int choice, Ewl_Widget *parent,void *userdata) {
}

static void
_choice_handler(int choice, Ewl_Widget *parent,void *userdata) {
    struct frontend * fe = (struct frontend *) userdata;
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
    free(choices);
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
