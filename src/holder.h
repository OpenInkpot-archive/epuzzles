/* Coordinate holder:  container with identification by x,y of object
   (coordinates is fake and used only in identification purposes */

#ifndef EPUZZLE_HOLDER_H
#define EPUZZLE_HOLDER_H 1

#include <stdbool.h>

#ifndef HIDE_EVAS
#include <Evas.h>
#endif

typedef struct object_holder object_holder;

#ifndef HIDE_EVAS
object_holder*
epuzzle_oh_new(Evas_Object* drawable);
#endif
/*
void
epuzzle_ob_insert_object(object_holder* ob, const char* filename,
                        int fx, int fy, int rx, int ry);
*/
void
epuzzle_oh_put_object(object_holder* oh, int fx, int fy,
                      const char* filename, int align);

void
epuzzle_oh_drop_object(object_holder* oh, int fx, int fy);

void
epuzzle_oh_del(object_holder* oh);

void
epuzzle_oh_drop_all(object_holder *oh);

#endif
