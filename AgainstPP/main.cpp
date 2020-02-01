#include <Windows.h>
#include <string>

#include "game.hpp"
#include "error.hpp"

std::unique_ptr<game> g (new game ());

LRESULT CALLBACK window_proc (
	HWND WindowHandle,
	UINT Msg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (Msg)
	{
	case WM_DESTROY:
		break;

	case WM_CLOSE:
		PostQuitMessage (0);
		break;

	case WM_KEYDOWN:
		g->process_keyboard_input (wParam, lParam);
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
	_In_ int cmd_show
)
{
	WNDCLASS WC = { 0 };

	WC.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	WC.lpfnWndProc = window_proc;
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

	ShowWindow (hWnd, cmd_show);
	UpdateWindow (hWnd);

	MSG Msg;
	ZeroMemory (&Msg, sizeof (Msg));
	
	egraphics_result result = g->init (hInstance, hWnd);

	if (result != egraphics_result::e_success)
	{
		log_error (result);
		g->exit ();
		DestroyWindow (hWnd);

		return 1;
	}

	while (Msg.message != WM_QUIT)
	{
		if (PeekMessage (&Msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage (&Msg);
			DispatchMessage (&Msg);
		}

		result = g->main_loop ();

		if (result != egraphics_result::e_success)
		{
			log_error (result);
			g->exit ();
			DestroyWindow (hWnd);
			return 1;
		}
	}

	g->exit ();

	DestroyWindow (hWnd);

	/*try
	{
		std::unique_ptr<game> g (game::get_instance (hInstance, cmd_show));
		g->main_loop ();
	}
	catch (const char* s)
	{
		wchar_t Buff[64];
		swprintf (Buff, 64, L"%hs\n", s);
		OutputDebugString (Buff);
	}
	catch (egraphics_error e)
	{
		log_error (e);
	}
	catch (std::runtime_error e)
	{
		wchar_t Buff[64];
		swprintf (Buff, 64, L"%hs\n", e.what ());
		OutputDebugString (Buff);
	}*/

	return 0;
}