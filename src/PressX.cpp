// PressX.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include "PressX.h"

#include <px/shell/renderer.h>
#include <px/shell/wingl.h>
#include <px/shell/key_bindings.h>
#include <px/key.h>
#include <px/core.h>

using namespace px;
using namespace px::shell;

std::unique_ptr<key_bindings<WPARAM, key>> g_bindings;
std::unique_ptr<core> g_core;

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

	#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

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
		g_core.reset(new core(renderer::opengl_handle(new wingl(hWnd))));
		g_bindings.reset(new key_bindings<WPARAM, key>());

		g_bindings->bind('W', VK_UP, VK_NUMPAD8, key::move_north);
		g_bindings->bind('A', VK_LEFT, VK_NUMPAD4, key::move_west);
		g_bindings->bind('S', VK_DOWN, VK_NUMPAD2, key::move_south);
		g_bindings->bind('D', VK_RIGHT, VK_NUMPAD6, key::move_east);
		g_bindings->bind(VK_END, VK_NUMPAD1, key::move_southwest);
		g_bindings->bind(VK_NEXT, VK_NUMPAD3, key::move_southeast);
		g_bindings->bind(VK_HOME, VK_NUMPAD7, key::move_northwest);
		g_bindings->bind(VK_PRIOR, VK_NUMPAD9, key::move_northeast);
		g_bindings->bind(VK_SPACE, VK_NUMPAD5, key::move_none);

		g_bindings->bind('1', key::action1);
		g_bindings->bind('2', key::action2);
		g_bindings->bind('3', key::action3);
		g_bindings->bind('4', key::action4);
		g_bindings->bind('5', key::action5);
		g_bindings->bind('6', key::action6);
		g_bindings->bind('7', key::action7);
		g_bindings->bind('8', key::action8);
		g_bindings->bind('9', key::action9);
		g_bindings->bind('0', key::action0);
		g_bindings->bind('E', key::action_use);

		g_bindings->bind(VK_F5, key::quick_save);
		g_bindings->bind(VK_F9, key::quick_load);

		g_bindings->bind(VK_RETURN, key::command_ok);
		g_bindings->bind(VK_ESCAPE, key::command_cancel);
		g_bindings->bind('I', key::panel_inventory);
		g_bindings->bind('P', key::panel_skills);
		g_bindings->bind('K', key::panel_craft); // k is for kraft

		for (bool run = true; run; run &= g_core->run())
		{
			g_core->frame();

			// dispatch windows messages
			while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) != 0)
			{
				run &= (GetMessage(&msg, NULL, 0, 0) == TRUE);
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}
		g_core->shutdown();
	}
	catch (std::exception &exc)
	{
		wchar_t message[1028];
		MultiByteToWideChar(CP_ACP, 0, exc.what(), -1, message, 1024);
		MessageBox(NULL, message, NULL, NULL);
	}

	g_core.reset();
	g_bindings.reset();

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

	bool fullscreen = false;

	hWnd = CreateWindow(szWindowClass, szTitle, fullscreen ? WS_POPUP : WS_OVERLAPPED,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, fullscreen ? SW_MAXIMIZE : nCmdShow);
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
		if (g_bindings && g_core)
		{
			key vkey;
			if (g_bindings->find(wParam, vkey))
			{
				g_core->press(vkey);
			}
		}
		break;
	case WM_MOUSEMOVE:
		if (!g_core) break;
		g_core->hover({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
		break;
	case WM_LBUTTONDOWN:
		if (!g_core) break;
		g_core->click({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }, 1);
		break;
	case WM_RBUTTONDOWN:
		if (!g_core) break;
		g_core->click({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }, 2);
		break;
	case WM_MOUSEWHEEL:
		if (!g_core) break;
		g_core->scroll(GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}