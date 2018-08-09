#pragma once

#include "shapes.h"

#include <iosfwd>
#include <unordered_map>

using DisplayFile = std::unordered_map<std::string, Shape>;

DisplayFile parse_stream(std::istream&);

