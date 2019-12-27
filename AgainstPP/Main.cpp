#include <Windows.h>
#include <string>

#include "Game.hpp"

std::unique_ptr<Game> G;

LRESULT CALLBACK WindowProc (HWND WindowHandle, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_DESTROY:
		PostQuitMessage (0);
		break;

	case WM_CLOSE:
		PostQuitMessage (0);
		break;

	case WM_KEYDOWN:
		G->ProcessKeyboardInput (wParam, lParam);
		break;

	default:
		break;
	}

	return DefWindowProc (WindowHandle, Msg, wParam, lParam);
}

int WINAPI wWinMain (_In_ HINSTANCE HInstance, _In_opt_ HINSTANCE PreviousHInstance, _In_ PWSTR CmdLine, _In_ int CmdShow)
{
	WNDCLASS WC = { 0 };

	WC.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	WC.lpfnWndProc = WindowProc;
	WC.hInstance = HInstance;
	WC.lpszClassName = L"Against";
	WC.hCursor = LoadCursor (HInstance, IDC_ARROW);

	if (!RegisterClass (&WC))
	{
		return 1;
	}

	HWND WindowHandle = CreateWindow (L"Against", L"Against", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, NULL, NULL, HInstance, NULL);

	if (!WindowHandle)
	{
		return 1;
	}

	ShowWindow (WindowHandle, CmdShow);
	UpdateWindow (WindowHandle);

	MSG Msg;
	ZeroMemory (&Msg, sizeof (Msg));
	
	G = std::make_unique<Game> ();

	while (Msg.message != WM_QUIT)
	{
		if (PeekMessage (&Msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage (&Msg);
			DispatchMessage (&Msg);
		}

		G->Run ();
	}

	return 0;
}