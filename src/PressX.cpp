// PressX.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PressX.h"

#include "renderer.h"
#include "wingl.h"
#include "key_bindings.h"
#include "key.h"
#include "game.h"

using namespace px::shell;

std::unique_ptr<key_bindings<WPARAM, key>> bindings;
std::unique_ptr<renderer> graphics;
std::unique_ptr<game> g_game;

#define MAX_LOADSTRING 100

// Global Variables:
HWND hWnd;
HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PRESSX, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PRESSX));

	try
	{
		graphics.reset(new renderer(renderer::opengl_handle(new wingl(hWnd))));
		bindings.reset(new key_bindings<WPARAM, key>());
		g_game.reset(new game());

		bindings->bind('W', VK_UP, VK_NUMPAD8, key::move_north);
		bindings->bind('A', VK_DOWN, VK_NUMPAD2, key::move_south);
		bindings->bind('S', VK_LEFT, VK_NUMPAD4, key::move_west);
		bindings->bind('D', VK_RIGHT, VK_NUMPAD6, key::move_east);

		bool run = true;
		while (run)
		{
			// windows dispatcher
			while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) != 0)
			{
				run &= (GetMessage(&msg, NULL, 0, 0) == TRUE);
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			graphics->draw(*g_game, 0);
		}
	}
	catch (std::runtime_error &exc)
	{
		wchar_t message[1025];
		MultiByteToWideChar(CP_ACP, 0, exc.what(), -1, message, 1024);
		MessageBox(NULL, message, NULL, NULL);
	}

	graphics.reset();
	bindings.reset();

	_CrtDumpMemoryLeaks();

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PRESSX));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;// MAKEINTRESOURCE(IDC_PRESSX);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_DESTROY	- post a quit message and return
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE) PostQuitMessage(0);

		key vkey;
		if (bindings && bindings->find(wParam, vkey))
		{
			g_game->press(vkey);
		}
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}