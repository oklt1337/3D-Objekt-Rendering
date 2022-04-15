#include "timeLoop.h"
#include <string>

#include "Camera.h"

using namespace std;

INT time_loop::init()
{
    last_timestamp_ = NOW;
    return 0;
}

void time_loop::update()
{
    const time_point now = NOW;
    const duration diff = now - last_timestamp_;
    delta_time_ = diff.count();
    total_time_ += delta_time_;
    last_timestamp_ = now;

    fps_++;
    fps_time_ += delta_time_;

#if _DEBUG

    // print delta time
#if UNICODE
    wstring output = to_wstring(delta_time_);
#else
    string output = to_string(deltaTime);
#endif // UNICODE
    output = TEXT("deltaTime: ") + output + TEXT("s\n");
    OutputDebugString(output.c_str());

    // print fps extrapolated
#if UNICODE
     output = to_wstring(1.0f / delta_time_);
#else
    output = to_string(1.0f / deltaTime);
#endif // UNICODE
    output = TEXT("fps (extrapolated): ") + output + TEXT("\n");
    OutputDebugString(output.c_str());

    // print fps
    if (fps_time_ >= 1.0f)
    {
        last_fps_ = fps_;
        fps_ = 0;
        fps_time_ -= 1.0f;
    }

#if UNICODE
    output = to_wstring(last_fps_);
#else
    output = to_string(lastFPS);
#endif // UNICODE
    output = TEXT("fps: ") + output + TEXT("\n");
    OutputDebugString(output.c_str());

#endif // _DEBUG
}

void time_loop::de_init()
{
}
