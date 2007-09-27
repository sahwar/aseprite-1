/* ase -- allegro-sprite-editor: the ultimate sprites factory
 * Copyright (C) 2007  David A. Capello
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

#ifndef USE_PRECOMPILED_HEADER

#include "jinete.h"

#include "core/app.h"
#include "modules/sprites.h"
#include "raster/sprite.h"

#endif

bool command_enabled_close_all_file(const char *argument)
{
  return !jlist_empty(get_sprite_list());
}

void command_execute_close_all_files(const char *argument)
{
  Sprite *sprite = get_first_sprite();
  Sprite *clipboard = get_clipboard_sprite();

  while (sprite) {
    sprite = current_sprite;
    
    /* check if this sprite is modified */
    if (sprite_is_modified(sprite) &&
	(!clipboard || sprite->gfxobj.id != clipboard->gfxobj.id)) {
      command_execute_close_file();
      break;
    }
    sprite = get_next_sprite(sprite);
  }

  /* close the window */
  jwindow_close(app_get_top_window(), 0);
}