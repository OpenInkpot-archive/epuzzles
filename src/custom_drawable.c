#define _GNU_SOIRCE 1
#include <err.h>
#include <Evas.h>
#include "custom_drawable.h"
#include "sprites.h"

#define FIFTEEN "/usr/share/epuzzle/sprites/fifteen/"

Evas_Object *
custom_drawable_fifteen(Evas *evas, int xy)
{
    Evas_Object *sprites = sprites_add(evas, xy, xy);
    if(!sprites)
        err(1, "Can't alloc sprites\n");
    sprites_bg_file_set(sprites, FIFTEEN "bg.png", NULL);
    int i;
    for(i=1; i < 16; i++)
    {
        char buf[1024];
        snprintf(buf, 1024, FIFTEEN "%02x.png", i);
        printf("Loading %s\n", buf);
        sprites_add_sprite(sprites, buf, NULL);
    };
    return sprites;
}

