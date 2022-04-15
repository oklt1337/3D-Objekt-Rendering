#pragma once
#include <DirectXMath.h>
#include <vector>
#include "Vertex.h"

struct mesh_data
{
    std::vector<vertex> vertices = {};
    std::vector<USHORT> indices = {};
};