#pragma once

#include "displayfile.h"

#include <iosfwd>

DisplayFile parse_stream(std::istream&);
void export_object_file(DisplayFile df, std::string path);
