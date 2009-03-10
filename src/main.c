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

void init_gui() {
};

/* lets go */
int main(int argc, char ** argv) {
    ewl_init(&argc, argv);
    init_gui();
    ewl_main();
    return 0;
}

