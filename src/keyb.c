#include <stdio.h>
#include <Evas.h>
#include <Ecore.h>
#include "puzzles.h"
#include "frontend.h"
#include "gui_util.h"

extern game * single;

static void _key_handler(void* param __attribute__((unused)),
         Evas* e __attribute__((unused)),
         Evas_Object *r, void* event_info) {
    Evas_Event_Key_Down* ev = (Evas_Event_Key_Down*)event_info;
    struct frontend *fe = (struct frontend *)param;

    const char *k = ev->keyname;
    unsigned int lp = 0;
//    if(e->base.modifiers & EWL_KEY_MODIFIER_ALT)
//        lp = 1;

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

    if(!strcmp(k, "Escape")) {
       ecore_main_loop_quit();
    }

    if (keyval) {
        if (lp)
            keyval |= MOD_SHFT;
        printf("processing key %s %d\n",k, keyval);
        midend_process_key(fe->me, 0, 0, keyval);
    } else {
        printf("No keyval\n");
    }
};

void gui_set_key_handler(struct frontend *fe) {
    evas_object_event_callback_add(fe->area,
                       EVAS_CALLBACK_KEY_UP,
                      &_key_handler,
                      fe);
    evas_object_focus_set(fe->area, 1);
}

