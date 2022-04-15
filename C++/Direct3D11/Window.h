#pragma once
#include <Windows.h>

class window
{
public:
	INT init(HINSTANCE h_instance, UINT width, UINT height);
	static BOOL run();
	static void de_init();

	HWND get_window_handle() const { return h_wnd_; }

private:
	HWND h_wnd_ = nullptr;
};

LRESULT CALLBACK wnd_proc(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param);