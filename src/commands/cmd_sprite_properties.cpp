/* ASE - Allegro Sprite Editor
 * Copyright (C) 2001-2011  David Capello
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

#include <allegro/unicode.h>

#include "base/bind.h"
#include "gui/gui.h"

#include "app/color.h"
#include "base/mem_utils.h"
#include "commands/command.h"
#include "modules/gui.h"
#include "raster/image.h"
#include "raster/palette.h"
#include "raster/sprite.h"
#include "document_wrappers.h"
#include "widgets/color_button.h"

//////////////////////////////////////////////////////////////////////
// sprite_properties

class SpritePropertiesCommand : public Command
{
public:
  SpritePropertiesCommand();
  Command* clone() { return new SpritePropertiesCommand(*this); }

protected:
  bool onEnabled(Context* context);
  void onExecute(Context* context);
};

SpritePropertiesCommand::SpritePropertiesCommand()
  : Command("SpriteProperties",
	    "Sprite Properties",
	    CmdUIOnlyFlag)
{
}

bool SpritePropertiesCommand::onEnabled(Context* context)
{
  return context->checkFlags(ContextFlags::ActiveDocumentIsWritable |
			     ContextFlags::HasActiveSprite);
}

void SpritePropertiesCommand::onExecute(Context* context)
{
  JWidget name, type, size, frames, ok, box_transparent;
  base::string imgtype_text;
  char buf[256];
  ColorButton* color_button = NULL;

  // Load the window widget
  FramePtr window(load_widget("sprite_properties.xml", "sprite_properties"));
  get_widgets(window,
	      "name", &name,
	      "type", &type,
	      "size", &size,
	      "frames", &frames,
	      "ok", &ok,
	      "box_transparent", &box_transparent, NULL);

  // Get sprite properties and fill frame fields
  {
    const ActiveDocumentReader document(context);
    const Sprite* sprite(document ? document->getSprite(): NULL);

    // Update widgets values
    switch (sprite->getImgType()) {
      case IMAGE_RGB:
	imgtype_text = "RGB";
	break;
      case IMAGE_GRAYSCALE:
	imgtype_text = "Grayscale";
	break;
      case IMAGE_INDEXED:
	sprintf(buf, "Indexed (%d colors)", sprite->getPalette(0)->size());
	imgtype_text = buf;
	break;
      default:
	ASSERT(false);
	imgtype_text = "Unknown";
	break;
    }

    // Filename
    name->setText(document->getFilename());

    // Color mode
    type->setText(imgtype_text.c_str());

    // Sprite size (width and height)
    usprintf(buf, "%dx%d (%s)",
	     sprite->getWidth(),
	     sprite->getHeight(),
	     get_pretty_memory_size(sprite->getMemSize()).c_str());

    size->setText(buf);

    // How many frames
    frames->setTextf("%d", sprite->getTotalFrames());

    if (sprite->getImgType() == IMAGE_INDEXED) {
      color_button = new ColorButton(Color::fromIndex(sprite->getTransparentColor()),
				     IMAGE_INDEXED);

      box_transparent->addChild(color_button);
    }
    else {
      box_transparent->addChild(new Label("(only for indexed images)"));
    }
  }

  window->remap_window();
  window->center_window();

  load_window_pos(window, "SpriteProperties");
  window->setVisible(true);
  window->open_window_fg();

  if (window->get_killer() == ok) {
    if (color_button) {
      ActiveDocumentWriter document(context);
      Sprite* sprite(document->getSprite());

      // If the transparent color index has changed, we update the
      // property in the sprite.
      int index = color_button->getColor().getIndex();
      if (index != sprite->getTransparentColor()) {
	// TODO Add undo handling
	sprite->setTransparentColor(color_button->getColor().getIndex());
	update_screen_for_document(document);
      }
    }
  }

  save_window_pos(window, "SpriteProperties");
}

//////////////////////////////////////////////////////////////////////
// CommandFactory

Command* CommandFactory::createSpritePropertiesCommand()
{
  return new SpritePropertiesCommand;
}
