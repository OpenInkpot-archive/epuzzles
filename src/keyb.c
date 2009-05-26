#include <stdio.h>
#include <Ewl.h>
#include "puzzles.h"
#include "frontend.h"
#include "gui_util.h"

extern game * single;

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
    HANDLE_KEY("KP_0", MOD_NUM_KEYPAD | '0') else
    HANDLE_KEY("KP_9", MOD_NUM_KEYPAD | '9') else
    HANDLE_KEY("KP_8", MOD_NUM_KEYPAD | '8') else
    HANDLE_KEY("KP_7", MOD_NUM_KEYPAD | '7') else
    HANDLE_KEY("KP_6", MOD_NUM_KEYPAD | '6') else
    HANDLE_KEY("KP_5", MOD_NUM_KEYPAD | '5') else
    HANDLE_KEY("KP_4", MOD_NUM_KEYPAD | '4') else
    HANDLE_KEY("KP_3", MOD_NUM_KEYPAD | '3') else
    HANDLE_KEY("KP_2", MOD_NUM_KEYPAD | '2') else
    HANDLE_KEY("KP_1", MOD_NUM_KEYPAD | '1') else
    HANDLE_KEY("Up",  CURSOR_UP) else
    HANDLE_KEY("Right",  CURSOR_RIGHT) else
    HANDLE_KEY("Left",  CURSOR_LEFT) else
    HANDLE_KEY("Down",  CURSOR_DOWN) else

    /* For emulation on PC */
    HANDLE_KEY("j",  CURSOR_UP) else
    HANDLE_KEY("h",  CURSOR_RIGHT) else
    HANDLE_KEY("l",  CURSOR_LEFT) else
    HANDLE_KEY("k",  CURSOR_DOWN) else
    if(!strcmp(k,"Enter") || !strcmp(k,"Return")) {
        keyval=CURSOR_SELECT;
    } else
    if(!strcmp(k, "F2") || !strcmp(k, "Menu")){
        if(!single)
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
