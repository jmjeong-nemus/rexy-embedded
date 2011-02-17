#include "rexy_debug_win.h"
#include "resource.h"

// Function Declarations.
static bool RegisteryDebugWin(HINSTANCE hInstance);
static LRESULT CALLBACK DebugWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK EditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static void PutString(const char* pstr);

static const char* pszDbgWndName = "RexyDbgWin";
static HWND		s_hDebugWnd = NULL;
static HWND		s_hParentWnd = NULL;
static HWND		s_hEditWnd = NULL;
static HINSTANCE s_hInstance = NULL;
static WNDPROC	s_pfnEdit = NULL;
static HANDLE s_hThread = NULL;
static DWORD s_threadID = 0;
static CRITICAL_SECTION s_Critical;

#pragma warning(disable:4127)
extern HWND		g_hMainWnd;
static HWND	g_hDebugWnd = NULL;

extern int	MemDumpLeakInfo();
extern int	MemInitLeakInfo();

// implementations.

HWND GetDebugWindow(void)
{
    return s_hDebugWnd;
}
bool InitDebugWindow(HWND hParent, HINSTANCE hInstance)
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
    if ( !RegisteryDebugWin( s_hInstance ) )
    {
        return FALSE;
    }
    s_hDebugWnd = CreateWindow(
                      pszDbgWndName,
                      "",
                      WS_POPUP | WS_OVERLAPPED | WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX,
                      x, y,
                      width, height,
                      s_hParentWnd,
                      NULL,
                      s_hInstance,
                      NULL);
    if ( s_hDebugWnd == NULL )
    {
        return FALSE;
    }
    else
    {
        g_hDebugWnd = s_hDebugWnd;
    }

    ShowWindow( s_hDebugWnd, SW_HIDE );
    return TRUE;
}

static bool RegisteryDebugWin(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= (WNDPROC)DebugWndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= NULL;
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= NULL;
    wcex.lpszClassName	= pszDbgWndName;
    wcex.hIconSm		= NULL;

    return (RegisterClassEx(&wcex)!=0);

}

bool InitVariables(void)
{
    InitializeCriticalSection( &s_Critical );

    return TRUE;
}

void mon_send_msg(const char* pstr)
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
#define COMMAND_BUFFER_SIZE 256
#define MAX_TOKEN_LEN	24
#define MAX_TOKEN_NUM	8
static char command_buffer[COMMAND_BUFFER_SIZE] = {0,};
static UINT command_idx = 0;

static char* GetToken( char* token, char* str)
{
    char ch;
    char *ptok;
    char *pstr;

    ptok = token;
    pstr = str;
    while(1) {
        ch = *pstr;
        if ( ch != ' ' && ch != '\t' )
        {
            break;
        }
        pstr++;
    }
    while(1)
    {
        ch = *pstr;
        if ( ch == ' ' || ch == '\t' || ch == 0 ) break;
        *ptok++ = *pstr++;
        if ( ptok >= token+MAX_TOKEN_LEN-1 ) break;
    }
    *ptok = 0;
    return pstr;
}

#include "rexy.h"

// must be called by TestThreadProc
static PushKey(int key)
{
    PostMessage(g_hMainWnd, AWM_KEYDEVICE, key, AWM_KEYDOWN);	Sleep(500);
    PostMessage(g_hMainWnd, AWM_KEYDEVICE, key, AWM_KEYUP);	Sleep(500);
}

DWORD WINAPI TestThreadProc(  LPVOID lpParameter )
{
    int i;
    // Launch Text Memo.
    PushKey(VK_USER_OK);
    PushKey(VK_USER_LEFT);
    PushKey(VK_USER_UP);
    PushKey(VK_USER_OK);

    // Enter test characters.
    PushKey(VK_USER_OK);
    PushKey(VK_USER_OK);
    PushKey(VK_PAD_STAR);
    PushKey(VK_USER_OK);
    PushKey(VK_PAD_STAR);
    PushKey(VK_USER_OK);
    PushKey(VK_PAD_STAR);
    PushKey(VK_USER_OK);
    PushKey(VK_PAD_STAR);
    PushKey(VK_USER_OK);
    PushKey(VK_PAD_STAR);
    PushKey(VK_USER_OK);

    for ( i = 0 ;i < 6 ; i++ )
        PushKey(VK_USER_LEFT);
    for ( i = 0 ;i < 6 ; i++ )
        PushKey(VK_USER_RIGHT);

    PutString("the end of test thread\r\n");

    return 0;
}

// Joshua re-arranged trace commands on 18-Sep-2006
static void ProcessCommand(void)
{
    char token[MAX_TOKEN_NUM][MAX_TOKEN_LEN];
    char* argv[MAX_TOKEN_NUM];
    char *pcmd;
    int i;

    pcmd = command_buffer;

    for ( i = 0 ; i < MAX_TOKEN_NUM ; i++ )
    {
        token[i][0] = 0;
        argv[i] = token[i];
    }

    for ( i = 0 ; i < MAX_TOKEN_NUM ; i++ )
    {
        pcmd = GetToken(token[i], pcmd);
        if ( token[i][0] == 0 ) break;
    }

    if ( i > 0 )
    {
		if ( !TrcStrCmp( token[0], "trace" ) )
		{
			if ( !TrcStrCmp( token[1], "init" ) )				// Command "init" **********************
			{
				if ( !TrcStrCmp(token[2],"LeakInfo") )
				{
					MemInitLeakInfo();
				}
				else 
				{
					TrcPrint("Usage : trace init {LeakInfo} \n");
				}
			}
			else if ( !TrcStrCmp(token[1],"dump"))			// Command "dump" **********************
			{
				if ( !TrcStrCmp(token[2],"mem"))
				{
					int address;
					int DumpByteSize;
					int	NumColumn;

					address = StrAToI(token[3]);
					DumpByteSize = StrAToI(token[4]);
					NumColumn = StrAToI(token[5]);

					if ( !DumpByteSize ) DumpByteSize = 80;
					if ( !NumColumn ) NumColumn = 8;
					if ( address ) TrcDumpMemory((void *)address,DumpByteSize,NumColumn);
					else 
					{
						TrcPrint("Usage : trace dump mem {ADDRESS} [size] [num of columns]\n");
					}
				}
				else if ( !TrcStrCmp(token[2],"LeakInfo") )
				{
					MemDumpLeakInfo();
				}
				else if ( !TrcStrCmp(token[2],"TraceStatus"))
				{
					TrcDumpAreaStatus();
				}
				else if ( !TrcStrCmp(token[2],"break"))
				{
					TrcDumpBreakPoints();
				}
				else
				{
					TrcPrint("Usage: trace dump { mem | LeakInfo | TraceStatus | break }\n");
				}
			}
			else if ( !TrcStrCmp( token[1], "add" ) ) 	// Command "add" **********************
			{
				if ( !TrcStrCmp( token[2], "break" ) )
				{
					if ( *token[3] ) TrcAddBreakFunction(token[3]);
					else TrcPrint("Usage : trace add break {FUNCTION_NAME}\n");
				}
				else TrcPrint("Usage : trace add break\n");
			}
			else if ( !TrcStrCmp( token[1], "go") )
			{
				TrcReleaseBreak();
			}
			else if ( !TrcStrCmp( token[1], "del") )
			{
				if ( !TrcStrCmp( token[2], "break" ) )
				{
					if ( *token[3] )
					{
						int SlotNumber;

						if ( !TrcStrCmp( token[3],"all") ) SlotNumber = -1;	// all
						else SlotNumber = RalStrAToI(token[3]);

						TrcDelBreakFunction(SlotNumber);
					}
					else TrcPrint("Usage : trace del break { all | SLOT_NUMBER }\n");
				}
				else TrcPrint("Usage : trace del break\n");
			}
			else if ( !TrcStrCmp( token[1], "on" ) )	// Command "on" **********************
			{
				if (!TrcStrCmp(token[2],"all")) TrcControlTraceAll(TRUE);
				else if ( TrcControlTraceArea( token[2], 0 ) != RxErrNone )
				{
					TrcPrint("Usage : trace on { ");
					TrcPrintAreaName();
					TrcPrint(" }\n");
				}
				
			}
			else if ( !TrcStrCmp( token[1], "off" ) )		// Command "off" **********************
			{
				if (!TrcStrCmp(token[2],"all")) TrcControlTraceAll(FALSE);
				else if ( TrcControlTraceArea( token[2], -1 ) != RxErrNone )
				{
					TrcPrint("Usage : trace off { ");
					TrcPrintAreaName();
					TrcPrint(" }\n");
				}

			}
			else if ( !TrcStrCmp( token[1], "level" ) )	// Command "level" **********************
			{
				int new_level;
				new_level = StrAToI(token[2]);
				if ( new_level == 0 ) 
				{
					TrcPrint("Usage : trace level { 1 /* most talkative */ | 2 | 3 | 4 } { ");
					TrcPrintAreaName();
					TrcPrint(" }\n");
				}
				else
				{
					if (!TrcStrCmp(token[3],"all")) TrcSetTraceLevel(new_level);
					else if ( TrcControlTraceArea( token[3], new_level ) != RxErrNone )
					{
						TrcPrint("Usage : trace level { 1 /* most talkative */ | 2 | 3 | 4 } { ");
						TrcPrintAreaName();
						TrcPrint(" }\n");
					}
				}				
			}
			else
			{
				TrcPrint("Usage : trace { on | off | level | dump | init | add | del | go }\n");
			}
		}
        else if ( !TrcStrCmp( token[0], "test" ) )		// Command "test" **********************
        {
            if ( s_hThread )
            {
                DWORD exitCode;
                GetExitCodeThread(s_hThread, &exitCode);
                if ( exitCode == STILL_ACTIVE )
                {
                    PutString("test thread is running already.");
                    return;
                }
            }
            s_hThread = CreateThread( NULL, 0, TestThreadProc, g_hMainWnd, 0, &s_threadID );
        }
        else if ( !TrcStrCmp( token[0], "nandf") || !TrcStrCmp( token[0], "romfs") )
        {
            PutString("target specific command -_-");
        }
        else
        {
            PutString("unknown command");
        }
    }

}

static bool ProcessKey(const char ch)
{
    switch(ch)
    {
    case 13: // CR
        command_buffer[command_idx] = 0;
        PutString("\r\n");
        ProcessCommand();
        PutString("\r\n<mon> ");
        command_idx = 0;
        return TRUE;
    case 8: // Back Space
        if ( command_idx == 0 ) return TRUE;
        command_buffer[--command_idx] = 0;
        return FALSE;
    default:
        break;
    }
    if ( command_idx == COMMAND_BUFFER_SIZE-1 ) return TRUE;
    command_buffer[command_idx++] = ch;
    return FALSE;
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
    case WM_CHAR:
        if ( ProcessKey((char)wParam) ) return 0;
        break;
    default: break;
    }
    return CallWindowProc(s_pfnEdit, hWnd, message, wParam, lParam);
}

bool InitComponents(HWND hWnd)
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

    PutString( "<mon> " );
    return TRUE;
}
void trace(const char* fmt, ...)
{
    char strbuf[256];
    va_list arglist;

    va_start(arglist, fmt);
    wvsprintf( strbuf, fmt, arglist );
    OutputDebugString("DBG_WND>");
    OutputDebugString(strbuf);
    OutputDebugString("\n");
    va_end(arglist);
}

static LRESULT CALLBACK DebugWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

