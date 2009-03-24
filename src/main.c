#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <Ewl.h>
#include <edrawable.h>
#include "frontend.h"
#include "puzzles.h"

#define PROG    "EWL Puzzles"

/* globals */
static Ewl_Widget *fd_win = NULL;
struct frontend *_frontend = NULL;

void main_nav_menubtn(Ewl_Widget *widget, unsigned char lp)
{
 //   show_main_menu(); 
     Ewl_Widget *curwidget=ewl_widget_name_find("okmenu");
     ewl_menu_cb_expand(curwidget,NULL,NULL);
     ewl_widget_focus_send(EWL_WIDGET(EWL_MENU(curwidget)->popup));

}

void main_nav_up(Ewl_Widget *widget, unsigned char lp)
{
}

void main_nav_down(Ewl_Widget *widget, unsigned char lp)
{
}

void main_nav_left(Ewl_Widget *widget, unsigned char lp)
{
}

void main_nav_right(Ewl_Widget *widget, unsigned char lp)
{
}

void main_nav_sel(Ewl_Widget *widget, unsigned char lp)
{
}

/*
static key_handler_info_t main_info =
{
    .ok_handler = &main_ok,
    .nav_up_handler=&main_nav_up,
    .nav_down_handler=&main_nav_down,
    .nav_left_handler=&main_nav_left,
    .nav_right_handler=&main_nav_right,
    .nav_sel_handler=&main_nav_sel,
    .nav_menubtn_handler=&main_nav_menubtn,
    .esc_handler = &main_esc,
    .shift_handler= &main_shift,
    .item_handler = &main_item,
}; */


void
destroy_game(struct frontend *fe) {
    gui_delete_colors(fe);
    if(fe->me) {
        midend_free(fe->me);
        fe->me = NULL;
    }
    gui_unset_key_handler(fe);

}

void gui_redraw ( Ewl_Widget *w, struct frontend *fe) {
    int x, y;
    if (fe->me){
        x = CURRENT_W(w);
        y = CURRENT_H(w);
        printf("got size %d X %d\n", x, y);
        fe->w = x;
        fe->h = y;
        midend_size(fe->me, &x, &y, TRUE);
        printf("got size from midend %d X %d\n", x, y);
        fe->pw = x;
        fe->ph = y;
        fe->ox = (fe->w - fe->pw) / 2;
        fe->oy = (fe->h - fe->ph) / 2;
        midend_force_redraw(fe->me);
        dputs("redrawn\n");
    }
}
extern struct drawing_api e_drawing_api;

void
create_game(struct frontend *fe, struct game *thegame) {
    destroy_game(fe); /* close old gaming */
    fe->me = midend_new(fe, thegame, &e_drawing_api, fe);
    gui_setup_colors(fe);
    midend_new_game(fe->me);
    dputs("Game created\n");
    gui_redraw(fe->area, fe);
    gui_set_key_handler(fe);
}

void new_game_cb ( Ewl_Widget *w, void *event, void *data ) {
    struct game * thegame = (struct game *) data;
    create_game( _frontend, thegame);
}

void configure_cb ( Ewl_Widget *w, void *event, void *data ) {
    struct frontend *fe = (struct frontend *) data;
    dputs("CONFIGURE CB\n");
    if (fe->me){
        gui_redraw(w, fe);
    }
}

void destroy_cb ( Ewl_Widget *w, void *event, void *data )
{
    destroy_game((struct frontend *) data);
    ewl_widget_destroy ( w);
    ewl_main_quit();
}

void exit_cb( Ewl_Widget *w, void *event, void *data) {
    destroy_cb( EWL_WIDGET(_frontend->window), event, (void *) _frontend );
}

Ewl_Menu_Info  file_menu [] = {
    {"Exit", NULL, exit_cb },
    { NULL, NULL, NULL },
};

Ewl_Menubar_Info menubar_info [] = {
    {"File", file_menu },
    { NULL, NULL },
};

void
append_game(Ewl_Widget *menu, struct game * thegame) {
    Ewl_Widget *item;
    item = ewl_menu_item_new();
    ewl_button_label_set(EWL_BUTTON(item), thegame->name);
    ewl_button_image_set(EWL_BUTTON(item), NULL, NULL);
    ewl_container_child_append(EWL_CONTAINER(menu), item);
    ewl_callback_append(item, EWL_CALLBACK_CLICKED, new_game_cb, thegame);
    ewl_widget_show(item);
}

void init_gui() {
    struct frontend * fe;
    Ewl_Widget *main_win = NULL;
    Ewl_Widget *box;
    Ewl_Widget *menubar;
    Ewl_Widget *menu;

    fe = snew(struct frontend);
    _frontend = fe;

    fe->me = NULL;
    fe->colours = NULL;
    fe->timer_id = fe->timer_active = NULL;

    main_win = ewl_window_new();
    fe->window = main_win;
    ewl_window_title_set ( EWL_WINDOW ( main_win ), "EWL_WINDOW" );
    ewl_window_name_set ( EWL_WINDOW ( main_win ), "EWL_WINDOW" );
    ewl_window_class_set ( EWL_WINDOW ( main_win ), "EWLWindow" );
    ewl_object_size_request ( EWL_OBJECT ( main_win ), 600, 800 );
    ewl_object_maximum_w_set(EWL_OBJECT(main_win),600);
    ewl_callback_append ( main_win, EWL_CALLBACK_DELETE_WINDOW, destroy_cb, fe);
    //set_key_handler(main_win, &main_info);
    ewl_widget_name_set(main_win,"mainwindow");
    ewl_widget_show ( main_win );

    box =  ewl_vbox_new();
    ewl_container_child_append(EWL_CONTAINER(main_win),box);
    ewl_object_fill_policy_set(EWL_OBJECT(box), EWL_FLAG_FILL_ALL);
    ewl_widget_show(box);

    menubar = ewl_menubar_new();
    ewl_menubar_from_info(EWL_MENUBAR(menubar), menubar_info);
    ewl_widget_name_set(menubar, "okmenu");

    menu = ewl_menu_new();
    ewl_button_label_set(EWL_BUTTON(menu),"Puzzles");

    setup_gamelist(menu);
    ewl_widget_show(menu);

    ewl_container_child_append(EWL_CONTAINER(menubar), menu);
    ewl_object_fill_policy_set(EWL_OBJECT(menu),
        EWL_FLAG_FILL_HSHRINKABLE | EWL_FLAG_FILL_VFILL);

    ewl_container_child_append(EWL_CONTAINER(box), menubar);
    ewl_widget_show(menubar);

    fe->area = ewl_drawable_new();
    ewl_object_fill_policy_set(EWL_OBJECT(fe->area), EWL_FLAG_FILL_ALL);
    ewl_container_child_append(EWL_CONTAINER(box), EWL_WIDGET(fe->area));
    ewl_callback_append ( EWL_WIDGET(fe->area),
            EWL_CALLBACK_CONFIGURE, configure_cb, fe);
    ewl_widget_show(EWL_WIDGET(fe->area));

    fe->statusbar = ewl_statusbar_new();
    ewl_container_child_append(EWL_CONTAINER(box), EWL_WIDGET(fe->statusbar));
    ewl_statusbar_push(fe->statusbar,"Select puzzle from menu...");
    ewl_widget_show(EWL_WIDGET(fe->statusbar));

};

/* lets go */
int main(int argc, char ** argv) {
/*    if(!eina_mempool_init()){
        fatal("can't init mempools\n");
    };
    if(!eina_init()){
        fatal("can't init EINA\n");
    };
    if(!evas_init()) {
        fatal("can't init evas\n");
    }; */
    if(!ewl_init(&argc, argv)) {
        fatal("can't init ewl");
    };
    init_gui();
    ewl_main();
    return 0;
}

