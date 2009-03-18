/*
 * gtk.c: GTK front end for my puzzle collection.
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include <edrawable.h>
#include "puzzles.h"
#include "frontend.h"

/* ----------------------------------------------------------------------
 * GTK front end to puzzles.
 */


struct font {
    int type;
    int size;
};

void e_start_draw(void *handle)
{
    struct frontend * fe = (struct frontend *) handle;
    Ewl_Drawable *d = fe->area;

    printf("e_start_draw()\n");
    ewl_drawable_draw_rectangle_fill(d, 0, 0, fe->w, fe->h);
    fe->bbox_l = fe->w;
    fe->bbox_r = 0;
    fe->bbox_u = fe->h;
    fe->bbox_d = 0;
}

void e_clip(void *handle, int x, int y, int w, int h)
{
    frontend *fe = (frontend *)handle;

    printf("e_clip(%d,%d,%d,%d)\n", x, y, w, h);
    ewl_drawable_set_clip(fe->area, x, y, w, h);
}

void e_unclip(void *handle)
{
    frontend *fe = (frontend *)handle;
    printf("e_unclup()\n");
    ewl_drawable_reset_clip(fe->area);
}

void e_draw_text(void *handle, int x, int y, int fonttype, int fontsize,
		   int align, int color, char *text)
{
    frontend *fe = (frontend *)handle;
    int i;
    printf("e_draw_text(...,%d, %d, \"%s\")\n", x, y, text);
#if 0
    /*
     * Find or create the font.
     */
    for (i = 0; i < fe->nfonts; i++)
        if (fe->fonts[i].type == fonttype && fe->fonts[i].size == fontsize)
            break;

    if (i == fe->nfonts) {
        if (fe->fontsize <= fe->nfonts) {
            fe->fontsize = fe->nfonts + 10;
            fe->fonts = sresize(fe->fonts, fe->fontsize, struct font);
        }

        fe->nfonts++;

        fe->fonts[i].type = fonttype;
        fe->fonts[i].size = fontsize;

#ifdef USE_PANGO
        /*
         * Use Pango to find the closest match to the requested
         * font.
         */
        {
            PangoFontDescription *fd;

            fd = pango_font_description_new();
            /* `Monospace' and `Sans' are meta-families guaranteed to exist */
            pango_font_description_set_family(fd, fonttype == FONT_FIXED ?
                                              "Monospace" : "Sans");
            pango_font_description_set_weight(fd, PANGO_WEIGHT_BOLD);
            /*
             * I found some online Pango documentation which
             * described a function called
             * pango_font_description_set_absolute_size(), which is
             * _exactly_ what I want here. Unfortunately, none of
             * my local Pango installations have it (presumably
             * they're too old), so I'm going to have to hack round
             * it by figuring out the point size myself. This
             * limits me to X and probably also breaks in later
             * Pango installations, so ideally I should add another
             * CHECK_VERSION type ifdef and use set_absolute_size
             * where available. All very annoying.
             */
#ifdef HAVE_SENSIBLE_ABSOLUTE_SIZE_FUNCTION
            pango_font_description_set_absolute_size(fd, PANGO_SCALE*fontsize);
#else
            {
                Display *d = GDK_DISPLAY();
                int s = DefaultScreen(d);
                double resolution =
                    (PANGO_SCALE * 72.27 / 25.4) * 
                    ((double) DisplayWidthMM(d, s) / DisplayWidth (d, s));
                pango_font_description_set_size(fd, resolution * fontsize);
            }
#endif
            fe->fonts[i].desc = fd;
        }

#else
	/*
	 * In GTK 1.2, I don't know of any plausible way to
	 * pick a suitable font, so I'm just going to be
	 * tedious.
	 */
	fe->fonts[i].font = gdk_font_load(fonttype == FONT_FIXED ?
					  "fixed" : "variable");
#endif

    }

    /*
     * Set the colour.
     */
    gdk_gc_set_foreground(fe->gc, &fe->colours[colour]);

#ifdef USE_PANGO

    {
	PangoLayout *layout;
	PangoRectangle rect;

	/*
	 * Create a layout.
	 */
	layout = pango_layout_new(gtk_widget_get_pango_context(fe->area));
	pango_layout_set_font_description(layout, fe->fonts[i].desc);
	pango_layout_set_text(layout, text, strlen(text));
	pango_layout_get_pixel_extents(layout, NULL, &rect);

        if (align & ALIGN_VCENTRE)
            rect.y -= rect.height / 2;
	else
	    rect.y -= rect.height;

        if (align & ALIGN_HCENTRE)
            rect.x -= rect.width / 2;
        else if (align & ALIGN_HRIGHT)
            rect.x -= rect.width;

	gdk_draw_layout(fe->pixmap, fe->gc, rect.x + x, rect.y + y, layout);

	g_object_unref(layout);
    }

#else
    /*
     * Find string dimensions and process alignment.
     */
    {
        int lb, rb, wid, asc, desc;

	/*
	 * Measure vertical string extents with respect to the same
	 * string always...
	 */
        gdk_string_extents(fe->fonts[i].font,
			   "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
                           &lb, &rb, &wid, &asc, &desc);
        if (align & ALIGN_VCENTRE)
            y += asc - (asc+desc)/2;
	else
            y += asc;

	/*
	 * ... but horizontal extents with respect to the provided
	 * string. This means that multiple pieces of text centred
	 * on the same y-coordinate don't have different baselines.
	 */
        gdk_string_extents(fe->fonts[i].font, text,
                           &lb, &rb, &wid, &asc, &desc);

        if (align & ALIGN_HCENTRE)
            x -= wid / 2;
        else if (align & ALIGN_HRIGHT)
            x -= wid;

    }

    /*
     * Actually draw the text.
     */
    gdk_draw_string(fe->pixmap, fe->fonts[i].font, fe->gc, x, y, text);
#endif
#endif
}

void e_draw_rect(void *handle, int x, int y, int w, int h, int color)
{
    frontend *fe = (frontend *)handle;
    printf("e_draw_rect()\n");
    gui_apply_color(fe, color);
    ewl_drawable_draw_rectangle(fe->area, x, y, w, h);
}

void e_draw_line(void *handle, int x1, int y1, int x2, int y2, int color)
{
    frontend *fe = (frontend *)handle;
    printf("e_draw_line()\n");
    gui_apply_color(fe, color);
    ewl_drawable_draw_line(fe->area, x1, y1, x2, y2);
}

void e_draw_poly(void *handle, int *coords, int npoints,
		   int fillcolor, int outlinecolor)
{
    frontend *fe = (frontend *)handle;
    EDrawablePolygon *p;
    int i;

    printf("e_draw_poly()\n");

    p = ewl_drawable_polygon_new();
    for (i = 0; i < npoints; i++) {
          ewl_drawable_polygon_add(p, coords[i*2], coords[i*2+1]);
//        points[i].x = coords[i*2];
//        points[i].y = coords[i*2+1];
    }

    if (fillcolor >= 0) {
        gui_apply_color(fe, fillcolor);
        ewl_drawable_draw_polygon_fill(fe->area, p);
    }
    assert(outlinecolor >= 0);
    gui_apply_color(fe, outlinecolor);
    ewl_drawable_draw_polygon(fe->area, p);
    ewl_drawable_polygon_delete(p);

    /*
     * In principle we ought to be able to use gdk_draw_polygon for
     * the outline as well. In fact, it turns out to interact badly
     * with a clipping region, for no terribly obvious reason, so I
     * draw the outline as a sequence of lines instead.
     */
#if 0
    for (i = 0; i < npoints; i++)
	gdk_draw_line(fe->pixmap, fe->gc,
		      points[i].x, points[i].y,
		      points[(i+1)%npoints].x, points[(i+1)%npoints].y);

    sfree(points);
#endif
}

void e_draw_circle(void *handle, int cx, int cy, int radius,
		     int fillcolor, int outlinecolor)
{
    frontend *fe = (frontend *)handle;
    printf("e_draw_circle()\n");
    if (fillcolor >= 0) {
        gui_apply_color(fe, fillcolor);
        ewl_drawable_draw_ellipse_filled(fe, cx, cy, radius, radius);
    }

    assert(outlinecolor >= 0);
    gui_apply_color(fe, outlinecolor);
    ewl_drawable_draw_ellipse(fe, cx, cy, radius, radius);
}

struct blitter {
    Evas *evas;
    int w, h, x, y;
};

blitter *e_blitter_new(void *handle, int w, int h)
{
    printf("e_blitter_new()\n");
#if 0
    /*
     * We can't create the pixmap right now, because fe->window
     * might not yet exist. So we just cache w and h and create it
     * during the firs call to blitter_save.
     */
    blitter *bl = snew(blitter);
    bl->pixmap = NULL;
    bl->w = w;
    bl->h = h;
    return bl;
#endif
}

void e_blitter_free(void *handle, blitter *bl)
{
    printf("e_blitter_free()\n");
#if 0
    if (bl->pixmap)
        gdk_pixmap_unref(bl->pixmap);
    sfree(bl);
#endif
}

void e_blitter_save(void *handle, blitter *bl, int x, int y)
{
    printf("e_blitter_save()\n");
#if 0
    frontend *fe = (frontend *)handle;
    if (!bl->pixmap)
        bl->pixmap = gdk_pixmap_new(fe->area->window, bl->w, bl->h, -1);
    bl->x = x;
    bl->y = y;
    gdk_draw_pixmap(bl->pixmap,
                    fe->area->style->fg_gc[GTK_WIDGET_STATE(fe->area)],
                    fe->pixmap,
                    x, y, 0, 0, bl->w, bl->h);
#endif
}

void e_blitter_load(void *handle, blitter *bl, int x, int y)
{
    printf("e_blitted_load()\n");
#if 0
    frontend *fe = (frontend *)handle;
    assert(bl->pixmap);
    if (x == BLITTER_FROMSAVED && y == BLITTER_FROMSAVED) {
        x = bl->x;
        y = bl->y;
    }
    gdk_draw_pixmap(fe->pixmap,
                    fe->area->style->fg_gc[GTK_WIDGET_STATE(fe->area)],
                    bl->pixmap,
                    0, 0, x, y, bl->w, bl->h);
#endif
}

void e_draw_update(void *handle, int x, int y, int w, int h)
{
    frontend *fe = (frontend *)handle;
    printf("e_draw_update(%d,%d,%d,%d)\n",x,y,w,h);
    if (fe->bbox_l > x  ) fe->bbox_l = x  ;
    if (fe->bbox_r < x+w) fe->bbox_r = x+w;
    if (fe->bbox_u > y  ) fe->bbox_u = y  ;
    if (fe->bbox_d < y+h) fe->bbox_d = y+h;
}

void e_end_draw(void *handle)
{
    frontend *fe = (frontend *)handle;
    printf("e_end_draw()\n");
    ewl_drawable_commit(fe->area);
#if 0
    gdk_gc_unref(fe->gc);
    fe->gc = NULL;

    if (fe->bbox_l < fe->bbox_r && fe->bbox_u < fe->bbox_d) {
	gdk_draw_pixmap(fe->area->window,
			fe->area->style->fg_gc[GTK_WIDGET_STATE(fe->area)],
			fe->pixmap,
                        fe->bbox_l, fe->bbox_u,
                        fe->ox + fe->bbox_l, fe->oy + fe->bbox_u,
                        fe->bbox_r - fe->bbox_l, fe->bbox_d - fe->bbox_u);
    }
#endif
}

void
e_status_bar(void *handler, char *text) {
    struct frontend *fe = (struct frontend *) handler;
    ewl_statusbar_pop(fe->statusbar);
    ewl_statusbar_push(fe->statusbar, text);
}

const struct drawing_api e_drawing_api = {
    e_draw_text,
    e_draw_rect,
    e_draw_line,
    e_draw_poly,
    e_draw_circle,
    e_draw_update,
    e_clip,
    e_unclip,
    e_start_draw,
    e_end_draw,
    e_status_bar,
    e_blitter_new,
    e_blitter_free,
    e_blitter_save,
    e_blitter_load,
    NULL, NULL, NULL, NULL, NULL, NULL, /* {begin,end}_{doc,page,puzzle} */
    NULL,			       /* line_width */
};

