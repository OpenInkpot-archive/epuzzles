#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <Ewl.h>

#define PROG    "EWL Puzzles"

/* globals */
static Ewl_Widget *main_win = NULL;
static Ewl_Widget *fd_win = NULL;

void main_nav_menubtn(Ewl_Widget *widget, unsigned char lp)
{
 //   show_main_menu();    
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

void destroy_cb ( Ewl_Widget *w, void *event, void *data )
{
    ewl_widget_destroy ( w );
    ewl_main_quit();
}

void init_gui() {
    main_win = ewl_window_new();
    ewl_window_title_set ( EWL_WINDOW ( main_win ), "EWL_WINDOW" );
    ewl_window_name_set ( EWL_WINDOW ( main_win ), "EWL_WINDOW" );
    ewl_window_class_set ( EWL_WINDOW ( main_win ), "EWLWindow" );
    ewl_object_size_request ( EWL_OBJECT ( main_win ), 600, 800 );
    ewl_object_maximum_w_set(EWL_OBJECT(main_win),600);
    ewl_callback_append ( main_win, EWL_CALLBACK_DELETE_WINDOW, destroy_cb, NULL );
    //set_key_handler(main_win, &main_info);
    ewl_widget_name_set(main_win,"mainwindow");
    ewl_widget_show ( main_win );
};

/* lets go */
int main(int argc, char ** argv) {
    ewl_init(&argc, argv);
    init_gui();
    ewl_main();
    return 0;
}

