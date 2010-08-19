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
#include <libeoi_battery.h>
#include <libeoi_clock.h>
#include <libeoi_themes.h>
#include <edrawable.h>
#include "frontend.h"
#include "puzzles.h"
#include "custom_drawable.h"

#define PROG    "E Puzzles"
#define CANVAS_SIZE  440

static char * theme_file = THEME_DIR "epuzzle.edj";

/* globals */
struct frontend *_frontend = NULL;

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

static const char *
epuzzle_orientation(int w, int h)
{
    if (w > h)
        return "horizontal";
    else
        return "vertical";
}

static void fill_texts(Evas *canvas)
{
    Evas_Object* area = evas_object_name_find(canvas, "puzzle");
    Evas_Object* main_edje = evas_object_name_find(canvas, "main_edje");
    Evas_Object* contents = evas_object_name_find(canvas, "contents");
    struct frontend * fe = (struct frontend *)
                        evas_object_data_get(area, "frontend");

    char *hint = epuzzles_hint_by_name(fe);
    edje_object_part_text_set(contents, "epuzzle/help", hint);
    free(hint);

    edje_object_part_text_set(main_edje, "title",
            gettext("Puzzles"));

    edje_object_part_text_set(contents, "epuzzle/title",
            gettext(lookup_game_by_name(fe->name)->name));

    // FIXME: hack
    fe->window = main_edje;
}


static void epuzzle_edje_resize(Ecore_Evas *ee __attribute__((unused)),
                                Evas_Object *edje,
                                int w, int h,
                                void *param)
{
    const char *file;
    const char *collection;
    const char *replacement = epuzzle_orientation(w, h);
    edje_object_file_get(edje, &file, &collection);
    if(!strcmp(collection, replacement))
        return;
    edje_object_file_set(edje, file, replacement);
    Evas *evas = (Evas *) param;
    fill_texts(evas);
}

static void main_edje_resize(Ecore_Evas *ee __attribute__((unused)),
                             Evas_Object *main_edje,
                             int w, int h,
                             void *param __attribute__((unused)))
{
    if (w > h)
        eoi_main_window_footer_hide(main_edje);
    else
        eoi_main_window_footer_show(main_edje);

}

static void
create_edjes(Evas *canvas, Ecore_Evas *main_win)
{
    int w, h;
    evas_output_size_get(canvas, &w, &h);
    Evas_Object *main_edje = eoi_main_window_create(canvas);
    Evas_Object *contents = eoi_create_themed_edje(canvas, "epuzzle",
                                                   epuzzle_orientation(w, h));
    if(w > h)
    {
        eoi_main_window_footer_hide(main_edje);
    }
    evas_object_name_set(main_edje, "main_edje");
    evas_object_move(main_edje, 0, 0);
    evas_object_resize(main_edje, w, h);
    evas_object_name_set(contents, "contents");
    edje_object_part_swallow(main_edje, "contents", contents);
    Evas_Object* area = evas_object_name_find(canvas, "puzzle");
    edje_object_part_swallow(contents, "epuzzle/drawable",  area );
    eoi_fullwindow_object_register(main_win, main_edje);
    eoi_resize_object_register(main_win, main_edje, main_edje_resize, canvas);
    eoi_resize_object_register(main_win, contents, epuzzle_edje_resize, canvas);
    eoi_run_battery(main_edje);
    eoi_run_clock(main_edje);
    evas_object_show(main_edje);
    evas_object_show(contents);
    fill_texts(canvas);
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
    Ecore_Evas* main_win = ecore_evas_software_x11_8_new(0, 0, 0, 0, 600, 800);
    Evas* main_canvas = ecore_evas_get(main_win);

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

    epuzzle_create_canvas(fe, main_canvas, CANVAS_SIZE);
    evas_object_data_set(fe->area, "frontend", fe);

    fe->default_alpha = 0xFF; /* Default alphachannel for drawing */

    evas_object_name_set(fe->area, "puzzle");
    evas_object_show(fe->area);

    create_game(fe);

    gui_redraw(fe);

    create_edjes(main_canvas, main_win);


    ecore_evas_show(main_win);
    ecore_main_loop_begin();
};

static
void exit_all(void* param __attribute__((unused))) {
    ecore_main_loop_quit();
}

static Eina_Bool exit_handler(void* param __attribute__((unused)),
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
       err(1, "Unable to initialize Evas\n");
    if(!ecore_init())
       err(1, "Unable to initialize Ecore\n");
    if(!ecore_evas_init())
       err(1, "Unable to initialize Ecore_Evas\n");
    if(!edje_init())
       err(1, "Unable to initialize Edje\n");
    if(!efreet_init())
       err(1, "Unable to initialize Efreet\n");

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
    ecore_evas_shutdown();
    ecore_shutdown();
    evas_shutdown();
    edje_shutdown();
    return 0;
}

