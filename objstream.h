#pragma once

#include "displayfile.h"

#include <iosfwd>

DisplayFile parse_stream(std::istream&);
void dump_stream(const DisplayFile&, std::ostream&);
