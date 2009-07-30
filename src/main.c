#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <Evas.h>
#include <Ecore_Evas.h>
#include <edrawable.h>
#include "frontend.h"
#include "puzzles.h"

#define PROG    "E Puzzles"
#define CANVAS_SIZE  400

static char * theme_file = THEME_DIR "epuzzle.edj";

/* globals */
struct frontend *_frontend = NULL;

struct game * single = NULL;

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

void
create_game(struct frontend *fe, struct game *thegame) {
    destroy_game(fe); /* close old gaming */
    fe->first_time = 0;
    fe->me = midend_new(fe, thegame, &e_drawing_api, fe);
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


static void run() {
    struct frontend * fe;
    Ecore_Evas* main_win = ecore_evas_software_x11_new(0, 0, 0, 0, 600, 800);

    fe = snew(struct frontend);
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
    Evas_Object* main_canvas_edje = edje_object_add(main_canvas);
    evas_object_name_set(main_canvas_edje, "main_canvas_edje");
    edje_object_file_set(main_canvas_edje, THEME_DIR "/epuzzle.edj", "epuzzle");

    fe->window = main_canvas;

    fe->area = edrawable_add(main_canvas, CANVAS_SIZE, CANVAS_SIZE);

    evas_object_move(main_canvas_edje, 0, 0);
    evas_object_resize(main_canvas_edje, 600, 800);
    gui_set_key_handler(fe);

    if(single)
        create_game(fe, single);

    gui_redraw(fe);
    evas_object_show(main_canvas_edje);
    evas_object_show(fe->area);
    edje_object_part_swallow(main_canvas_edje, "epuzzle/drawable",  fe->area );
    ecore_evas_show(main_win);
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
    textdomain("gm");
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
        single = lookup_game_by_name(argv[1]);
        if(!single)
            fatal("Don't know this game");
        run();
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

