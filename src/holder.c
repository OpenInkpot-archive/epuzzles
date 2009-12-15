/* Coordinate holder:  container with identification by x,y of object
   (coordinates is fake and used only in identification purposes */

#include <Eina.h>
#include <Evas.h>
#include "sprites.h"
#include "holder.h"


struct object_holder {
    Evas_Object* canvas; /* drawable canvas */
    Eina_List* objects;
};

typedef struct _object _object;
struct _object {
    int x; /* fake x, y for identification */
    int y;
    int id; /* id inside canvas */
    bool used;
    const char* filename;
};

object_holder*
epuzzle_oh_new(Evas_Object* drawable)
{
    object_holder* oh = calloc(sizeof(object_holder), 1);
    oh->canvas = drawable;
    return oh;
}

_object*
epuzzle_oh_find_object(object_holder* oh, int fx, int fy, const char* filename)
{
    Eina_List* tmp;
    _object* each;
    EINA_LIST_FOREACH(oh->objects, tmp, each)
    {
        if(each->x == fx && each->y == fy && each->used)
            if(filename)
                if(!strcmp(each->filename, filename))
                    return each;
            else
                return each;
    }
    return NULL;
}

_object*
epuzzle_oh_find_object_by_filename(object_holder* oh, const char* filename,
                                    bool used)
{
    Eina_List* tmp;
    _object* each;
    EINA_LIST_FOREACH(oh->objects, tmp, each)
    {
        if(!strcmp(each->filename, filename) && each->used == used)
            return each;
    }
    return NULL;
}

void
epuzzle_ob_insert_object(object_holder* oh, const char* filename,
                        int fx, int fy, int rx, int ry)
{
    _object* object = calloc(sizeof(_object), 1);
    object->x = fx;
    object->y = fy;
    object->used = true;
    object->id = sprites_add_sprite(oh->canvas, filename, NULL);
    sprites_sprite_move(oh->canvas, object->id, rx, ry);
    object->filename = eina_stringshare_add(filename);
    oh->objects = eina_list_append(oh->objects, object);
}

void
epuzzle_oh_put_object(object_holder* oh, int fx, int fy, int rx, int ry,
                      const char* filename)
{
    _object* object = epuzzle_oh_find_object(oh, fx, fy, filename);
    if(object)
        sprites_sprite_move(oh->canvas, object->id, rx, ry);
    else
    {
        object = epuzzle_oh_find_object_by_filename(oh, filename, false);
        if(object)
        {
            sprites_sprite_move(oh->canvas, object->id, rx, ry);
            sprites_sprite_show(oh->canvas, object->id);
            object->used = true;
        }
        else
            epuzzle_oh_insert_object(oh, filename, fx, fy, rx, ry);
    }
}

void
epuzzle_oh_drop_object(object_holder* oh, int fx, int fy)
{
    while(1)
    {
        _object* object = epuzzle_oh_find_object(oh, fx, fy, NULL);
        if(object)
        {
            sprites_sprite_hide(oh->canvas, object->id);
            object->used = false;
        }
        else
            return;
    }
}

void
epuzzle_oh_del(object_holder* oh)
{
    Eina_List* tmp;
    _object* object;
    EINA_LIST_FOREACH(oh->objects, tmp, object)
    {
        eina_stringshare_del(object->filename);
        free(object);
    }
    eina_list_free(oh->objects);
    free(oh);
}
