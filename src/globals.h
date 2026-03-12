#pragma once

#include <QFont>
#include <string>

namespace Globals {
inline std::string currentFile{};
inline std::string oldFile{"placeholder"};
inline bool textDirty = false;
inline constexpr const char* windowTitle{"Notepad (Awesome Edition)"};
inline QFont font;
};  // namespace Globals
