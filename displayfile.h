#pragma once

#include "shapes.h"

#include <iosfwd>
#include <unordered_set>

using DisplayFile = std::unordered_set<Shape>;

DisplayFile parse_stream(std::istream&);

