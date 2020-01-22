#include <Windows.h>
#include <string>

#include "Game.hpp"
#include "Error.hpp"

LRESULT CALLBACK WindowProc (
	HWND WindowHandle, 
	UINT Msg, 
	WPARAM wParam, 
	LPARAM lParam
)
{
	switch (Msg)
	{
	case WM_DESTROY:
		game::process_window_destroy ();
		break;

	case WM_CLOSE:
		PostQuitMessage (0);
		break;

	case WM_KEYDOWN:
		game::process_keyboard_input (wParam, lParam);
		break;

	default:
		break;
	}

	return DefWindowProc (
		WindowHandle, 
		Msg, 
		wParam, 
		lParam
	);
}

int WINAPI wWinMain (
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE previous_hInstance, 
	_In_ PWSTR cmd_line, 
	_In_ int CmdShow
)
{
	WNDCLASS WC = { 0 };

	WC.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	WC.lpfnWndProc = WindowProc;
	WC.hInstance = hInstance;
	WC.lpszClassName = L"Against";
	WC.hCursor = LoadCursor (hInstance, IDC_ARROW);

	if (!RegisterClass (&WC))
	{
		return 1;
	}

	HWND hWnd = CreateWindow (
		L"Against", 
		L"Against", 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		1280, 
		720, 
		NULL, 
		NULL, 
		hInstance, 
		NULL
	);

	if (!hWnd)
	{
		return 1;
	}

	ShowWindow (hWnd, CmdShow);
	UpdateWindow (hWnd);

	MSG Msg;
	ZeroMemory (
		&Msg, 
		sizeof (Msg)
	);

	try
	{
		game::init (
			hInstance, 
			hWnd
		);

		while (Msg.message != WM_QUIT)
		{
			if (PeekMessage (&Msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage (&Msg);
				DispatchMessage (&Msg);
			}

			game::draw ();
		}
	}
	catch (egraphics_error Err)
	{
		log_error (Err);
	}
	catch (std::runtime_error Err)
	{
		wchar_t Buff[64];
		swprintf (Buff, 64, L"%hs\n", Err.what ());
		OutputDebugString (Buff);
	}

	game::exit ();

	DestroyWindow (hWnd);

	return 0;
}