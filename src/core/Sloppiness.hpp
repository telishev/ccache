// Copyright (C) 2021-2022 Joel Rosdahl and other contributors
//
// See doc/AUTHORS.adoc for a complete list of contributors.
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 51
// Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#pragma once

#include <string>

namespace core {

enum class Sloppy : uint32_t {
  none = 0U,

  include_file_mtime = 1U << 0,
  include_file_ctime = 1U << 1,
  time_macros = 1U << 2,
  pch_defines = 1U << 3,
  // Allow us to match files based on their stats (size, mtime, ctime), without
  // looking at their contents.
  file_stat_matches = 1U << 4,
  // Allow us to not include any system headers in the manifest include files,
  // similar to -MM versus -M for dependencies.
  system_headers = 1U << 5,
  // Allow us to ignore ctimes when comparing file stats, so we can fake mtimes
  // if we want to (it is much harder to fake ctimes, requires changing clock)
  file_stat_matches_ctime = 1U << 6,
  // Allow us to not include the -index-store-path option in the manifest hash.
  clang_index_store = 1U << 7,
  // Ignore locale settings.
  locale = 1U << 8,
  // Allow caching even if -fmodules is used.
  modules = 1U << 9,
  // Ignore virtual file system (VFS) overlay file.
  ivfsoverlay = 1U << 10,
  // Allow us to include incorrect working directory in .gcno files.
  gcno_cwd = 1U << 11,
};

class Sloppiness
{
public:
  Sloppiness(Sloppy value = Sloppy::none);
  explicit Sloppiness(uint32_t value);

  void enable(Sloppy value);
  bool is_enabled(Sloppy value) const;
  uint32_t to_bitmask() const;

private:
  Sloppy m_sloppiness = Sloppy::none;
};

// --- Inline implementations ---

inline Sloppiness::Sloppiness(Sloppy value) : m_sloppiness(value)
{
}

inline Sloppiness::Sloppiness(uint32_t value)
  : m_sloppiness(static_cast<Sloppy>(value))
{
}

inline void
Sloppiness::enable(Sloppy value)
{
  m_sloppiness = static_cast<Sloppy>(static_cast<uint32_t>(m_sloppiness)
                                     | static_cast<uint32_t>(value));
}

inline bool
Sloppiness::is_enabled(Sloppy value) const
{
  return static_cast<uint32_t>(m_sloppiness) & static_cast<uint32_t>(value);
}

inline uint32_t
Sloppiness::to_bitmask() const
{
  return static_cast<uint32_t>(m_sloppiness);
}

} // namespace core
