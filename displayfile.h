#pragma once

#include "shapes.h"

#include <iosfwd>
#include <unordered_set>

struct DisplayFile
{
    const std::unordered_set<Shape> shapes;
};

DisplayFile parse_stream(std::istream&);

