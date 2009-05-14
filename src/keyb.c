#include <stdio.h>
#include <Ewl.h>
#include "puzzles.h"
#include "frontend.h"
#include "gui_util.h"



static void _key_handler(Ewl_Widget* w, void *event, void *context) {
    Ewl_Event_Key_Up* e = (Ewl_Event_Key_Up*)event;
    struct frontend *fe = (struct frontend *)context;

    const char *k = e->base.keyname;
    unsigned int lp = 0;
    if(e->base.modifiers & EWL_KEY_MODIFIER_ALT)
        lp = 1;

    int keyval = 0;

    printf("In handler %s\n", k);

#define HANDLE_KEY(kk, vv)  if( !strcmp(k, kk) ){ keyval = vv; }
    HANDLE_KEY("9", MOD_NUM_KEYPAD | '9') else
    HANDLE_KEY("Page_Up", MOD_NUM_KEYPAD | '9') else
    HANDLE_KEY("8",   CURSOR_UP) else
    HANDLE_KEY("Up",  CURSOR_UP) else
    HANDLE_KEY("7",  MOD_NUM_KEYPAD | '7') else
    HANDLE_KEY("Home",  MOD_NUM_KEYPAD | '7') else
    HANDLE_KEY("6",  CURSOR_RIGHT) else
    HANDLE_KEY("Right",  CURSOR_RIGHT) else
    HANDLE_KEY("5",  MOD_NUM_KEYPAD | '5') else
    // FIXME: equivalent of GDK_KP_Begin
    HANDLE_KEY("Begin",  MOD_NUM_KEYPAD | '0') else
    HANDLE_KEY("4",  CURSOR_LEFT) else
    HANDLE_KEY("Left",  CURSOR_LEFT) else
    HANDLE_KEY("3",  MOD_NUM_KEYPAD | '3') else
    HANDLE_KEY("Page_Down",  MOD_NUM_KEYPAD | '3') else
    HANDLE_KEY("2",  CURSOR_DOWN) else
    HANDLE_KEY("Down",  CURSOR_DOWN) else
    HANDLE_KEY("1",  MOD_NUM_KEYPAD | '1') else
    HANDLE_KEY("End",  MOD_NUM_KEYPAD | '1') else
    HANDLE_KEY("0",  MOD_NUM_KEYPAD | '0') else
    HANDLE_KEY("Insert",  MOD_NUM_KEYPAD | '0') else
    if(!strcmp(k,"Enter") || !strcmp(k,"Return")) {
        keyval=CURSOR_SELECT;
    } else
    if(!strcmp(k, "F2") || !strcmp(k, "Menu")){
        gamelist_menu(fe->window, fe);
        return;
    }

    if(!strcmp(k, "Escape")) {
       exit_cb( w, event, context); 
    }

    if (keyval) {
        if (lp)
            keyval |= MOD_SHFT;
        printf("processing key %s %d\n",k, keyval);
        midend_process_key(fe->me, 0, 0, keyval);
//        gui_redraw ( fe->window, fe);
    }
};

void gui_set_key_handler(struct frontend *fe) {
    ewl_callback_append(fe->window, EWL_CALLBACK_KEY_UP, &_key_handler, fe);
}

void gui_unset_key_handler(struct frontend *fe) {
    ewl_callback_del_with_data(fe->window, EWL_CALLBACK_KEY_UP,
            &_key_handler, fe);
}
