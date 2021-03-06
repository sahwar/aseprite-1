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

#ifndef UNDOERS_REMAP_PALETTE_H_INCLUDED
#define UNDOERS_REMAP_PALETTE_H_INCLUDED

#include "undo/object_id.h"
#include "undoers/undoer_base.h"

#include <vector>

class Sprite;

namespace undoers {

class RemapPalette : public UndoerBase
{
public:
  RemapPalette(undo::ObjectsContainer* objects, Sprite* sprite, int frameFrom, int frameTo, const std::vector<uint8_t>& mapping);

  void dispose() OVERRIDE;
  int getMemSize() const OVERRIDE { return sizeof(*this); }
  void revert(undo::ObjectsContainer* objects, undo::UndoersCollector* redoers) OVERRIDE;

private:
  undo::ObjectId m_spriteId;
  uint32_t m_frameFrom;
  uint32_t m_frameTo;
  std::vector<uint8_t> m_mapping;
};

} // namespace undoers

#endif	// UNDOERS_REMAP_PALETTE_H_INCLUDED
