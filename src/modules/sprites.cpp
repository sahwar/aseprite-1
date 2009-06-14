/* ASE - Allegro Sprite Editor
 * Copyright (C) 2001-2009  David Capello
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "config.h"

#include <cassert>

#include "jinete/jlist.h"

#include "ase/ui_context.h"
#include "effect/effect.h"
#include "modules/sprites.h"
#include "raster/cel.h"
#include "raster/image.h"
#include "raster/layer.h"
#include "raster/mask.h"
#include "raster/sprite.h"
#include "raster/stock.h"
#include "util/misc.h"

static ImageRef *images_ref_get_from_layer(Sprite* sprite, Layer *layer, int target, bool write);
static void layer_get_pos(Sprite* sprite, Layer *layer, int target, bool write, int **x, int **y, int *count);

//////////////////////////////////////////////////////////////////////

ImageRef *images_ref_get_from_sprite(Sprite* sprite, int target, bool write)
{
  Layer *layer = target & TARGET_ALL_LAYERS ? sprite->set:
					      sprite->layer;

  return images_ref_get_from_layer(sprite, layer, target, write);
}

void images_ref_free(ImageRef* image_ref)
{
  ImageRef *p, *next;

  for (p=image_ref; p; p=next) {
    next = p->next;
    jfree(p);
  }
}

static ImageRef *images_ref_get_from_layer(Sprite* sprite, Layer *layer, int target, bool write)
{
#define ADD_IMAGES(images)			\
  {						\
    if (first_image == NULL) {			\
      first_image = images;			\
      last_image = images;			\
    }						\
    else {					\
      assert(last_image != NULL);		\
      last_image->next = images;		\
    }						\
						\
    while (last_image->next != NULL)		\
      last_image = last_image->next;		\
  }

#define NEW_IMAGE(layer, cel)					\
  {								\
    ImageRef *image_ref = jnew(ImageRef, 1);			\
								\
    image_ref->image = layer->sprite->stock->image[cel->image];	\
    image_ref->layer = layer;					\
    image_ref->cel = cel;					\
    image_ref->next = NULL;					\
								\
    ADD_IMAGES(image_ref);					\
  }
  
  ImageRef *first_image = NULL;
  ImageRef *last_image = NULL;
  int frame = sprite->frame;

  if (!layer_is_readable(layer))
    return NULL;

  if (write && !layer_is_writable(layer))
    return NULL;

  switch (layer->type) {

    case GFXOBJ_LAYER_IMAGE: {
      if (target & TARGET_ALL_FRAMES) {
	for (frame=0; frame<sprite->frames; frame++) {
	  Cel *cel = layer_get_cel(layer, frame);
	  if (cel != NULL)
	    NEW_IMAGE(layer, cel);
	}
      }
      else {
	Cel *cel = layer_get_cel(layer, frame);
	if (cel != NULL)
	  NEW_IMAGE(layer, cel);
      }
      break;
    }

    case GFXOBJ_LAYER_SET: {
      ImageRef *sub_images;
      JLink link;

      JI_LIST_FOR_EACH(layer->layers, link) {
	sub_images = images_ref_get_from_layer
	  (sprite, reinterpret_cast<Layer*>(link->data), target, write);

	if (sub_images != NULL)
	  ADD_IMAGES(sub_images);
      }
      break;
    }

  }

  return first_image;
}

static void layer_get_pos(Sprite* sprite, Layer *layer, int target, bool write, int **x, int **y, int *count)
{
  int frame = sprite->frame;

  if (!layer_is_readable(layer))
    return;

  if (write && !layer_is_writable(layer))
    return;

  switch (layer->type) {

    case GFXOBJ_LAYER_IMAGE: {
      Image *image;
      int u, v;

      if (target & TARGET_ALL_FRAMES) {
	for (frame=0; frame<sprite->frames; frame++) {
	  image = GetLayerImage(layer, &u, &v, frame);
	  if (image) {
	    (*x)[*count] = u;
	    (*y)[*count] = v;
	    (*count)++;
	  }
	}
      }
      else {
	image = GetLayerImage(layer, &u, &v, frame);
	if (image) {
	  (*x)[*count] = u;
	  (*y)[*count] = v;
	  (*count)++;
	}
      }
      break;
    }

    case GFXOBJ_LAYER_SET: {
      JLink link;
      JI_LIST_FOR_EACH(layer->layers, link)
	layer_get_pos(sprite, reinterpret_cast<Layer*>(link->data), target, write, x, y, count);
      break;
    }

  }
}