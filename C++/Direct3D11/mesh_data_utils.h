#pragma once
#include "mesh_data.h"

class mesh_data_utils
{
public:
    static mesh_data create_sphere(float radius, int slice_count, int stack_count);
    static mesh_data create_cube(XMFLOAT3 scale);
    static mesh_data create_cylinder(float bottom_radius, float top_radius, float height, int slice_count, int stack_count);

private:
    static void build_cylinder_top_cap(float top_radius, float height, int slice_count, mesh_data* mesh_data);
    static void build_cylinder_bottom_cap(float bottom_radius, float height, int slice_count, mesh_data* mesh_data);
};
