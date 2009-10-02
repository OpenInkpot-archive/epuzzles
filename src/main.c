#define _GNU_SOURCE 1
#include <err.h>
#include <errno.h>
#include <libintl.h>
#include <locale.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <Evas.h>
#include <Edje.h>
#include <Ecore_Evas.h>
#include <libeoi.h>
#include <edrawable.h>
#include "frontend.h"
#include "puzzles.h"
#include "custom_drawable.h"

#define PROG    "E Puzzles"
#define CANVAS_SIZE  440

static char * theme_file = THEME_DIR "epuzzle.edj";

/* globals */
struct frontend *_frontend = NULL;


static void die(const char* fmt, ...)
{
   va_list ap;
   va_start(ap, fmt);
   vfprintf(stderr, fmt, ap);
   va_end(ap);
   exit(EXIT_FAILURE);
}



void
destroy_game(struct frontend *fe) {
    gui_delete_colors(fe);
    if(fe->keys)
        keys_free(fe->keys);
    if(fe->me) {
        midend_free(fe->me);
        fe->me = NULL;
    }
}

void gui_redraw ( struct frontend *fe) {
    int x, y;
    if (fe->me){
        x = CANVAS_SIZE;
        y = CANVAS_SIZE;
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
extern struct drawing_api fake_drawing_api;

void
create_game(struct frontend *fe) {
    destroy_game(fe); /* close old gaming */
    fe->first_time = 0;
    fe->keys = keys_alloc("epuzzles");
    struct game * thegame = lookup_game_by_name(fe->name);
    if(!fe->keys)
        err(1, "Can't load keys\n");
    fe->me = midend_new(fe, thegame, fe->draw_api, fe);
    gui_setup_colors(fe);
    midend_new_game(fe->me);
    dputs("Game created\n");
    gui_redraw( fe);
    gui_set_key_handler(fe);
}

void terminate ( struct frontend * fe)
{
    destroy_game(fe);
    ecore_main_loop_quit();
}

static void main_win_close_handler(Ecore_Evas* main_win __attribute__((unused)))
{
   ecore_main_loop_quit();
}


static void run(const char* gamename) {
    struct frontend * fe;
    Ecore_Evas* main_win = ecore_evas_software_x11_new(0, 0, 0, 0, 600, 800);

    fe = calloc(1, sizeof(struct frontend));
    fe->name = gamename;
    _frontend = fe;


    ecore_evas_title_set(main_win, "EPuzzles");
    ecore_evas_name_class_set(main_win, "EPuzzles", "Epuzzles");
    ecore_evas_callback_delete_request_set(main_win, main_win_close_handler);

    fe->first_time = 1;
    fe->me = NULL;
    fe->colours = NULL;
    fe->timer_id = NULL;
    fe->timer_active = 0;

    Evas* main_canvas = ecore_evas_get(main_win);
    Evas_Object* main_edje = eoi_main_window_create(main_canvas);
    evas_object_name_set(main_edje, "main_edje");

    Evas_Object* contents = edje_object_add(main_canvas);
    edje_object_file_set(contents, THEME_DIR "/epuzzle.edj", "epuzzle");
    evas_object_name_set(contents, "contents");
    evas_object_show(contents);
    edje_object_part_swallow(main_edje, "contents", contents);

    fe->window = main_edje;

    if(!strcmp(fe->name, "fifteen"))
    {
        printf("Unsing custom canvas\n");
        fe->area = custom_drawable_fifteen(main_canvas, CANVAS_SIZE);
        fe->draw_api = &fake_drawing_api;
        fe->support_colors = 0;
    }
    else
    {
        fe->area = edrawable_add(main_canvas, CANVAS_SIZE, CANVAS_SIZE);
        fe->draw_api = &e_drawing_api;
        fe->support_colors = 1;
    }
    fe->default_alpha = 0xFF; /* Default alphachannel for drawing */

    evas_object_name_set(fe->area, "puzzle");

    evas_object_move(main_edje, 0, 0);
    evas_object_resize(main_edje, 600, 800);
    evas_object_focus_set(contents, true);

    create_game(fe);

    gui_redraw(fe);

    edje_object_part_text_set(contents, "epuzzle/help",
            epuzzles_hint_by_name(fe->name));

    edje_object_part_text_set(contents, "epuzzle/title",
            gettext("Puzzles"));

    evas_object_show(fe->area);
    evas_object_show(main_edje);
    edje_object_part_swallow(contents, "epuzzle/drawable",  fe->area );
    ecore_evas_show(main_win);
    init_clock(main_edje);
    init_battery(main_edje);
    ecore_main_loop_begin();
};

static
void exit_all(void* param __attribute__((unused))) {
    ecore_main_loop_quit();
}

static int exit_handler(void* param __attribute__((unused)),
                        int ev_type __attribute__((unused)),
                        void* event __attribute__((unused)))
{
   ecore_main_loop_quit();
   return 1;
}

/* lets go */
int main(int argc, char ** argv) {
    printf("argc=%d\n", argc);
    setlocale(LC_ALL, "");
    textdomain("epuzzles");
    if(!evas_init())
       die("Unable to initialize Evas\n");
    if(!ecore_init())
       die("Unable to initialize Ecore\n");
    if(!ecore_evas_init())
       die("Unable to initialize Ecore_Evas\n");
    if(!edje_init())
       die("Unable to initialize Edje\n");
    if(!efreet_init())
       die("Unable to initialize Efreet\n");

    ecore_x_io_error_handler_set(exit_all, NULL);
    ecore_event_handler_add(ECORE_EVENT_SIGNAL_EXIT, exit_handler, NULL);

    if(argc == 2) {
        if(!lookup_game_by_name(argv[1]))
            fatal("Don't know this game");
        run(argv[1]);
    } else {
        printf("game name required");
    };
    efreet_shutdown();
    edje_shutdown();
    ecore_evas_shutdown();
    ecore_shutdown();
    evas_shutdown();
    return 0;
}

