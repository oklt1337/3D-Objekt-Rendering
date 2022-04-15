#include <list>
#include <Windows.h>
#include <random>

#include "Camera.h"
#include "D3D.h"
#include "TimeLoop.h"
#include "Window.h"
#include "Utils.h"
#include "light.h"
#include "Object.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
    constexpr UINT width = 1600;
    constexpr UINT height = 900;
    constexpr BOOL is_fullscreen = FALSE;

    // 1. create window
    window window = {};
    INT error = window.init(hInstance, width, height);
    CHECK_ERROR(error);

    // 2. connection to Direct3D11
    d_3d d3d = {};
    error = d3d.init(window.get_window_handle(), width, height, is_fullscreen);
    CHECK_ERROR(error);

    // 3. create objects
    vector<object*> objects = {};

    //Obj 1
    object cube = {};
    XMFLOAT3 cube_position = {};
    cube_position.x = 0.0f;
    cube_position.y = 0.0f;
    cube_position.z = 0.0f;

    XMFLOAT3 cube_scale = {};
    cube_scale.x = 1.0f;
    cube_scale.y = 1.0f;
    cube_scale.z = 1.0f;

    error = cube.init(d3d.get_device(), material::wall, mesh::sphere, cube_position, cube_scale, material::matt);
    CHECK_ERROR(error);
    objects.push_back(&cube);

    //Obj 2
    object cylinder = {};
    XMFLOAT3 cylinder_position = {};
    cylinder_position.x = -3.0f;
    cylinder_position.y = 0.0f;
    cylinder_position.z = 0.0f;

    XMFLOAT3 cylinder_scale = {};
    cylinder_scale.x = 1.0f;
    cylinder_scale.y = 1.0f;
    cylinder_scale.z = 1.0f;
    
    error = cylinder.init(d3d.get_device(), material::floor, mesh::sphere, cylinder_position, cylinder_scale, material::rough);
    CHECK_ERROR(error);
    objects.push_back(&cylinder);

    //Obj 3
    object sphere = {};
    XMFLOAT3 sphere_position = {};
    sphere_position.x = 3.0f;
    sphere_position.y = 0.0f;
    sphere_position.z = 0.0f;

    XMFLOAT3 sphere_scale = {};
    sphere_scale.x = 1.0f;
    sphere_scale.y = 1.0f;
    sphere_scale.z = 1.0f;

    error = sphere.init(d3d.get_device(), material::solid, mesh::sphere, sphere_position, sphere_scale, material::glossy);
    CHECK_ERROR(error);
    objects.push_back(&sphere);

    // 4. create camera
    camera camera = {};
    error = camera.init(width, height);
    CHECK_ERROR(error);

    // 5. set up time
    time_loop time = {};
    error = time.init();
    CHECK_ERROR(error);

    // 7. create light
    light::light_data light_data = {};
    light_data.light_direction = {-1.0f, -1.0f, 1.0f};
    light_data.light_diffuse_color = {1.0f, 1.0f, 1.0f, 1.0f};
    light_data.light_intensity = 1.0f;
    light_data.ambient_color = {0.1f, 0.1f, 0.1f, 1.0f};
    light_data.specular_color = {1.0f, 1.0f, 1.0f, 1.0f};
    light_data.specular_power = 32.0f;
    light_data.glossy = 1.0f;
    light_data.self_illumination_color = {0.0f, 0.0f, 0.0f, 0.0f};

    light light = {};
    error = light.init(d3d.get_device(), light_data);
    CHECK_ERROR(error);

    // 8. run application
    while (window.run())
    {
        // 8.1. update obj
        time.update();
        camera.update(time.get_delta_time());
        
        

        // 8.2. draw obj
        d3d.begin_scene(0.0f, 0.0f, 0.0f);

        

        // render
        for (auto value : objects)
        {
            value->render(d3d.get_device_context(), camera.get_view_matrix(), camera.get_projection_matrix(), camera.get_camera_position());
        }
        light.render(d3d.get_device_context());

        for (auto value : objects)
        {
            value->update(time.get_delta_time());
        }

        d3d.end_scene();
    }

    // 9. clean
    light.de_init();
    for (auto value : objects)
    {
        value->de_init();
    }
    time.de_init();
    camera.de_init();
    d3d.de_init();
    window.de_init();

    return 0;
}
