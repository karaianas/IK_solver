#pragma once

#include <vector>

#include "Loader.h"

/**
 * OBJLoader class is object file parser.
 */
class OBJLoader
{
public:
    static Model* loadOBJ(const char* obj, Loader *loader);
};