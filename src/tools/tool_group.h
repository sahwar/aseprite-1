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

#ifndef TOOLS_TOOL_GROUP_H_INCLUDED
#define TOOLS_TOOL_GROUP_H_INCLUDED

#include <string>

namespace tools {

// A group of tools.
class ToolGroup
{
public:
  ToolGroup(const char* name,
	    const char* label) : m_name(name)
			       , m_label(label) { }

  const std::string& getName() const { return m_name; }
  const std::string& getLabel() const { return m_label; }

private:
  std::string m_name;
  std::string m_label;
};

} // namespace tools

#endif	// TOOLS_TOOL_GROUP_H_INCLUDED

