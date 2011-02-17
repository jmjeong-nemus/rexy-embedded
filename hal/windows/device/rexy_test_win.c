#include "rexy_test_win.h"
#include "rexy_unit_test.h"
#include "resource.h"

// Function Declarations.
static bool RegisteryTestWin(HINSTANCE hInstance);
static LRESULT CALLBACK TestWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK EditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static void PutString(const char* pstr);

static const char* pszTstWndName = "RexyTstWin";
static HWND		s_hTestWnd = NULL;
static HWND		s_hParentWnd = NULL;
static HWND		s_hEditWnd = NULL;
static HINSTANCE s_hInstance = NULL;
static WNDPROC	s_pfnEdit = NULL;
static HANDLE s_hThread = NULL;
static DWORD s_threadID = 0;
static CRITICAL_SECTION s_Critical;

#pragma warning(disable:4127)
extern HWND		g_hMainWnd;
static HWND	g_hTestWnd = NULL;

extern int	MemDumpLeakInfo();
extern int	MemInitLeakInfo();

// implementations.

HWND GetTestWindow(void)
{
    return s_hTestWnd;
}

bool InitTestWindow(HWND hParent, HINSTANCE hInstance)
{
    int x, y;
    int width, height;
    RECT rect;

    s_hParentWnd = hParent;
    s_hInstance = hInstance;

    GetWindowRect( hParent, &rect );
    x = rect.right+1;
    y = rect.top;
    width = 500;
    height = 700;
    if ( !RegisteryTestWin( s_hInstance ) )
    {
        return FALSE;
    }
    s_hTestWnd = CreateWindow(
                      pszTstWndName,
                      "",
                      WS_POPUP | WS_OVERLAPPED | WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX,
                      x, y,
                      width, height,
                      s_hParentWnd,
                      NULL,
                      s_hInstance,
                      NULL);
    if ( s_hTestWnd == NULL )
    {
        return FALSE;
    }
    else
    {
        g_hTestWnd = s_hTestWnd;
    }

	UnitTestRunner();
    ShowWindow( s_hTestWnd, SW_HIDE );
    return TRUE;
}

static bool RegisteryTestWin(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= (WNDPROC)TestWndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= NULL;
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= NULL;
    wcex.lpszClassName	= pszTstWndName;
    wcex.hIconSm		= NULL;

    return (RegisterClassEx(&wcex)!=0);

}

static bool InitVariables(void)
{
    InitializeCriticalSection( &s_Critical );

    return TRUE;
}

void test_send_msg(const char* pstr)
{
    char strbuf[256];
    char *pdst;

    pdst = strbuf;
    while( *pstr )
    {
        if ( *pstr == '\n' )
        {
            *pdst++ = '\r';
        }
        *pdst++ = *pstr++;
        if ( pdst == strbuf + 256 - 2)
        {
            *pdst = 0;
            PutString(strbuf);
            pdst = strbuf;
        }
    }
    *pdst = 0;
    PutString(strbuf);
}
static void PutString(const char* pstr)
{
    EnterCriticalSection( &s_Critical );
    SendMessage( s_hEditWnd, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)pstr );
    LeaveCriticalSection( &s_Critical );
}

static LRESULT CALLBACK EditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch ( message )
    {

    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
        SetFocus(hWnd); return 0;
    case WM_KEYDOWN:
        if ( wParam == VK_UP||wParam == VK_LEFT||wParam == VK_RIGHT||wParam == VK_DOWN)
            return 0;
        break;
    default: break;
    }
    return CallWindowProc(s_pfnEdit, hWnd, message, wParam, lParam);
}

static bool InitComponents(HWND hWnd)
{
    RECT rect;
    if ( !GetClientRect( hWnd, &rect ) ) return FALSE;

    s_hEditWnd = CreateWindow( "EDIT",
                               "",
                               ES_MULTILINE | ES_AUTOVSCROLL | WS_VISIBLE | WS_CHILD,
                               rect.left, rect.top,
                               rect.right - rect.left, rect.bottom - rect.top,
                               hWnd, NULL, s_hInstance, NULL );
    if ( !s_hEditWnd ) return FALSE;

    s_pfnEdit = (WNDPROC)SetWindowLong( s_hEditWnd, GWL_WNDPROC, (LONG)EditProc );
    if ( !s_pfnEdit ) return FALSE;

     return TRUE;
}

static LRESULT CALLBACK TestWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        if ( !InitVariables()) return -1;
        if ( !InitComponents(hWnd)) return -1;
        break;

    default:
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

