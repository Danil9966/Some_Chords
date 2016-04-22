
#include "stdafx.h"
#include "Win32Project5.h"
#include "Windowsx.h"

#define MAX_LOADSTRING 100
#define FILENAMESZ 260

// Ãëîáàëüíûå ïåðåìåííûå:
HINSTANCE hInst;								// òåêóùèé ýêçåìïëÿð
TCHAR szTitle[MAX_LOADSTRING];					// Òåêñò ñòðîêè çàãîëîâêà
TCHAR szWindowClass[MAX_LOADSTRING];			// èìÿ êëàññà ãëàâíîãî îêíà
// Îòïðàâèòü îáúÿâëåíèÿ ôóíêöèé, âêëþ÷åííûõ â ýòîò ìîäóëü êîäà:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Exit(HWND, UINT, WPARAM, LPARAM);

RECT rect_1;
HWND hWnd;
DWORD Ex=0;
 
HKEY hK;
DWORD hu;
TCHAR szFile[FILENAMESZ];//áóôåð äëÿ èìåíè ôàéëà

int flag = 0;//÷òîáû SAVE è SAVEAS áûëî íà êðàñîòå
int flag2 = 1;//÷òîáû ðååñòð è ID_OPEN 

HANDLE hf;
OPENFILENAME yo;
DWORD lpcbData = sizeof(RECT);

const int numButtons = 7;
TBBUTTON tbButtons[numButtons] =
{
	{ 0, ID_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0 },
	{ 1, ID_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0 }, 0, 0 /*(INT_PTR)L"  " */ },
	{ 2, ID_SAVE_AS, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0 }, 0, 0/*(INT_PTR)L"Save us"*/ },
	{ 3, IDM_COPY, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0 }, 0, 0  /*(INT_PTR)L"Copy"*/ },
	{ 4, IDM_CUT, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0 }, 0, 0 /*(INT_PTR)L"Cut" */ },
	{ 5, IDM_PASTE, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0 }, 0, 0 /*(INT_PTR)L"Paste"*/ },
	{ 6, IDM_CANCEL, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0 }, 0, 0 /*(INT_PTR)L"Cancell"*/ },
};

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ðàçìåñòèòå êîä çäåñü.
	MSG msg;
	HACCEL hAccelTable;

	// Èíèöèàëèçàöèÿ ãëîáàëüíûõ ñòðîê
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32PROJECT5, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Âûïîëíèòü èíèöèàëèçàöèþ ïðèëîæåíèÿ:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT5));

	// Öèêë îñíîâíîãî ñîîáùåíèÿ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(hWnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ÔÓÍÊÖÈß: MyRegisterClass()
//
//  ÍÀÇÍÀ×ÅÍÈÅ: ðåãèñòðèðóåò êëàññ îêíà.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT5));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); COLOR_3DSHADOW;//6  7
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN32PROJECT5);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	
	return RegisterClassEx(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	DWORD regFlag;
   RECT rect;
   DWORD dSize = sizeof(DWORD);
   DWORD fNameSize = FILENAMESZ * 2;
   hInst = hInstance; 
   InitCommonControls();

   RegCreateKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\YobaCorp\\notpad"), 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hK, &hu);
   flag2 = RegQueryValueEx(hK, _T("FileName"), NULL, NULL, (LPBYTE)szFile, &fNameSize);
   if (szFile[0] == _T('\0'))flag2 = 2;
   regFlag=RegQueryValueEx(hK, _T("YOSO"), NULL, NULL, (LPBYTE)&rect, &lpcbData);


   int height = rect.bottom - rect.top;
   int width = rect.right - rect.left;

   if (hu == REG_OPENED_EXISTING_KEY && regFlag==ERROR_SUCCESS)
	   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CAPTION | CS_HREDRAW | CS_VREDRAW,
	   rect.left, rect.top, width, height, NULL, NULL, hInstance, NULL);
   else
  hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CAPTION | CS_HREDRAW | CS_VREDRAW,
    0, 0, 600, 400, NULL, NULL, hInstance, NULL);

  
   yo.lStructSize = sizeof(yo);
   yo.hwndOwner = hWnd;
   yo.lpstrFile = szFile;
   // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
   // use the contents of szFile to initialize itself.
   yo.nMaxFile = sizeof(szFile);
   yo.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
   yo.nFilterIndex = 1;
   yo.lpstrFileTitle = NULL;
   yo.nMaxFileTitle = NULL;
   yo.lpstrInitialDir = NULL;
   yo.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

   if (!hWnd)
      return FALSE;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   
   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hfield = 0;
	static HWND edit = 0;
	static HWND button1 = 0;
	static HWND button2 = 0;
	static HWND button3 = 0;
	static HWND hText = 0;
	static HWND htbar = 0;
	static TCHAR* buf;
	static TCHAR fname[200];
	static DWORD count = 0;
	static DWORD dFilesize = 0;
	static RECT rect = { 10,10,100,100 };
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_COPY:
			SendMessage(edit, WM_COPY, 0, 0);
			break;
		case IDM_PASTE:
			SendMessage(edit, WM_PASTE, 0, 0);
			break;
		case IDM_CUT:
			SendMessage(edit, WM_CUT, 0, 0); break;
		case IDM_CANCEL:SendMessage(edit, WM_UNDO, 0, 0); break;
		case ID_OPEN:
		{
						if (((flag2 == ERROR_SUCCESS) && (szFile[0] != _T('\0'))) || (flag2 == 1)&&(GetOpenFileName(&yo) == TRUE) )
						{
							hf = CreateFile(szFile,
								GENERIC_READ | GENERIC_WRITE,
								0,
								(LPSECURITY_ATTRIBUTES)NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								(HANDLE)NULL);

							if (hf == INVALID_HANDLE_VALUE) break;
							dFilesize = GetFileSize(hf, 0);
							buf = new TCHAR[dFilesize / 2 + 1];///2/2/2

							ReadFile(hf, buf, dFilesize, &count, NULL);
							buf[dFilesize / 2] = 0;
							CloseHandle(hf);
							SendMessage(edit, WM_SETTEXT, WPARAM(0), LPARAM(""));
							SendMessage(edit, WM_SETTEXT, 1000000000, (LPARAM)buf);
							SendMessage(hText, WM_SETTEXT, 100, (LPARAM)yo.lpstrFile);
							delete[]buf;
							flag = 1;
							SetFocus(edit);

						}
						flag2 = 1;

						break;

		}
		case ID_SAVE:
		
		{				if (flag)
		{
			GetWindowText(hText, fname, 200);
			hf = CreateFile(fname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			DWORD textlen = GetWindowTextLength(edit);
			buf = new TCHAR[textlen + 1];
			GetWindowText(edit, buf, textlen + 1);
			WriteFile(hf, buf, textlen * 2, &count, NULL);
			CloseHandle(hf);
			delete[]buf;
		//	flag = 0;
			break;
		}

		} 
		  case ID_SAVE_AS:
		{
						   if (GetSaveFileName(&yo) == TRUE)
						   {
							   hf = CreateFile(yo.lpstrFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
								   FILE_ATTRIBUTE_NORMAL, NULL);
							   DWORD textlen = GetWindowTextLength(edit);
							   buf = new TCHAR[textlen + 1];

							   GetWindowText(edit, buf, textlen + 1);
							   WriteFile(hf, buf, textlen * 2, &count, NULL);
							   CloseHandle(hf);
							   delete[]buf;
						   }
						   break;
		}

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}break;

	case WM_CREATE:
		{
					   htbar = CreateToolbarEx(hWnd,
			WS_CHILD | WS_BORDER | WS_VISIBLE | TBSTYLE_WRAPABLE | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | CCS_ADJUSTABLE,
						-  1,         // èäåíòèôèêàòîð îðãàíà Toolbar 
						   7,                   // êîëè÷åñòâî ïèêòîãðàìì
						   hInst,               // èäåíòèôèêàòîð ïðèëîæåíèÿ
						   /*IDB_BITMAP2*/	 IDD_TI_1, // èäåíòèôèêàòîð áèòîâîãî èçîáðàæåíèÿ êíîïîê
						   (LPCTBBUTTON)&tbButtons, // àäðåñ îïèñàíèÿ êíîïîê
						   7,                  // êîëè÷åñòâî êíîïîê
						   24, 24,              // øèðèíà è âûñîòà êíîïîê
						   24, 24,              // øèðèíà è âûñîòà ïèêòîãðàìì
						   sizeof(TBBUTTON));
					/*   (hwnd, WS_CHILD | WS_VISIBLE | CCS_TOP, 1,
						   0, HINST_COMMCTRL, IDB_STD_SMALL_COLOR, tbb, 3, 0, 0, 0, 0, sizeof(TBBUTTON));*/
					   edit = CreateWindowEx(WS_EX_ACCEPTFILES/*WS_EX_TRANSPARENT*/, L"EDIT", NULL,
						   WS_CHILD | WS_VSCROLL /*| ES_AUTOHSCROLL*//* | ES_AUTOVSCROLL*/ | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
						   30, 40, 500, 300, hWnd, NULL, hInst, NULL);
					   button1 = CreateWindowEx(0, _T("BUTTON"), // Predefined class; Unicode assumed 
						   L"Open", // Button text 
						   WS_TABSTOP | WS_VISIBLE | WS_CHILD, // Styles 
						   200, // x position 
						   350, // y position 
						   80, // Button width 
						   30, // Button height 
						   hWnd, // Parent window 
						   (HMENU)ID_OPEN, // No menu. 
						   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
						   NULL);
					   button2 = CreateWindowEx(0, L"BUTTON", // Predefined class; Unicode assumed 
						   L"Save", // Button text 
						   WS_TABSTOP | WS_VISIBLE | WS_CHILD, // Styles 
						   200, // x position 
						   350, // y position 
						   100, // Button width 
						   50, // Button height 
						   hWnd, // Parent window 
						   (HMENU)ID_SAVE, // No menu. 
						   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
						   NULL);
					   button3 = CreateWindowEx(0, L"BUTTON", // Predefined class; Unicode assumed 
						   L"Save as", // Button text 
						   WS_TABSTOP | WS_VISIBLE | WS_CHILD, // Styles 
						   200, // x position 
						   350, // y position 
						   100, // Button width 
						   50, // Button height
						   hWnd, // Parent window 
						   (HMENU)ID_SAVE_AS, // No menu. 
						   (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
						   NULL);
					   hText = CreateWindow(L"STATIC", L"",
						   WS_CHILD | WS_VISIBLE | WS_BORDER,
						   30, 100, 150, 18,
						   hWnd,
						   NULL,
						   NULL, NULL);
					   hfield = CreateWindow(L"STATIC", L"",
						   WS_CHILD | WS_VISIBLE,
						   30, 100, 150, 18,
						   hWnd,
						   NULL,
						   NULL, NULL);
				
					   HFONT MyFont = CreateFont(
						   15,        // logical height of font
						   7,  // logical average character width
						   0,  // angle of escapement
						   0,  // base-line orientation angle
						   10,        // font weight
						   0,  // italic attribute flag
						   0,  // underline attribute flag
						   0,  // strikeout attribute flag
						   DEFAULT_CHARSET,    // character set identifier
						   OUT_DEFAULT_PRECIS, // output precision
						   CLIP_DEFAULT_PRECIS,        // clipping precision
						   PROOF_QUALITY,      // output quality
						   FIXED_PITCH,        // pitch and family
						   L"Arial");
					  // SetFocus(edit);
					   SendMessage(edit, WM_SETFONT, (WPARAM)MyFont, 0);
					   SendMessage(button1, WM_SETFONT, (WPARAM)MyFont, 0);
					   SendMessage(button2, WM_SETFONT, (WPARAM)MyFont, 0);
					   SendMessage(button3, WM_SETFONT, (WPARAM)MyFont, 0);
					   SendMessage(hText, WM_SETFONT, (WPARAM)MyFont, 0);
					   SetFocus(edit);

					   break;
	}

	case WM_SHOWWINDOW:
		SendMessage(hWnd, WM_COMMAND, ID_OPEN, ID_OPEN);
		break;
	case WM_GETMINMAXINFO: 
		{
	MINMAXINFO *pInfo = (MINMAXINFO *)lParam;
	POINT Min = { 400, 200 };
	pInfo->ptMinTrackSize = Min; // Óñòàíîâèëè ìèíèìàëüíûé ðàçìåð
	break;
	}
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code)
		{
		case TTN_NEEDTEXT:
		{
			LPTOOLTIPTEXT lpt1 = LPTOOLTIPTEXT(lParam);
			int idButton = lpt1->hdr.idFrom;
			switch (idButton){
				case ID_OPEN:
					lpt1->lpszText = (L"Open");
					break;
				case ID_SAVE:
					lpt1->lpszText = (L"Save");
					break;
				case ID_SAVE_AS:
					lpt1->lpszText = (L"Save us");
					break;
				case IDM_COPY:
					lpt1->lpszText = (L"Copy");
					break;
				case IDM_CUT:
					lpt1->lpszText = (L"Cut");
					break;
				case IDM_PASTE:
					lpt1->lpszText = (L"Paste");
					break;
				case IDM_CANCEL:
					lpt1->lpszText = (L"Cancel");
					break;
							 
							 }
		}
		}break;
	case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
	break; 
	case WM_SYSCOMMAND:
		
		if (wParam == SC_MINIMIZE || wParam == SC_MAXIMIZE || wParam == 61458)
			GetWindowRect(hWnd, &rect_1);
		 DefWindowProc(hWnd, message, wParam, lParam);
		break;

	case WM_SIZE:
		
				{

						 GetWindowRect(hWnd, &rect);
						 int height = rect.bottom - rect.top;
						 int width = rect.right - rect.left;
					     SetWindowPos(button1, 0, 3, height - 90, 80, 25, SWP_SHOWWINDOW);
						 SetWindowPos(button2, 0, 90,height-90, 80, 25, SWP_SHOWWINDOW);
						 SetWindowPos(button3, 0,177, height - 90, 80, 25, SWP_SHOWWINDOW);
						 SetWindowPos(hfield, 0, 260, height - 90, width-280, 25,SWP_SHOWWINDOW);
						 if (height < 300){
							 SetWindowPos(htbar,0 , 110, 1000, 1000, 100, SWP_HIDEWINDOW);
							 SetWindowPos(edit,
								 0, 5, 5,
								 (width)-25,
								 (height)-100, SWP_SHOWWINDOW);
							 SetWindowPos(hText, 0, 10, (height)-116, (width)-27, 18, SWP_HIDEWINDOW);

						 }
						 else {
							 SetWindowPos(htbar,0, 0, 0, 0, 0, SWP_SHOWWINDOW);
							 SetWindowPos(edit,
								 0, 3, 40,
								 (width)-23,
								 (height)-160, SWP_SHOWWINDOW);
							 SetWindowPos(hText, 0, 3, (height)-116, (width)-23, 18, SWP_SHOWWINDOW);
				 }
 break;
				}	
	
	
	case WM_CLOSE:
				{
					 bool b = Edit_GetModify(edit);
					 if (!b) SendMessage(hWnd, WM_DESTROY, 0, 0);
					 else{
						 DialogBox(hInst, MAKEINTRESOURCE(IDD_EXITBOX), hWnd, Exit);
						 if (Ex == 1){
							 SendMessage(hWnd, WM_COMMAND, ID_SAVE, 0);
							 SendMessage(hWnd, WM_DESTROY, 0, 0);
						 }
						 else if (Ex == -1) SendMessage(hWnd, WM_DESTROY, 0, 0);
					 }
	}
		break;

	case WM_DESTROY:	
		
		GetWindowRect(hWnd, &rect);
		if (!(IsIconic(hWnd) ||IsZoomed(hWnd)) )
			RegSetValueEx(hK, _T("YOSO"),
			0,                                        
			REG_BINARY,                         
			(LPBYTE)&rect,               
			sizeof(RECT));
		else  RegSetValueEx(hK, _T("YOSO"),
			0,
			REG_BINARY,
			(LPBYTE)&rect_1,
			sizeof(RECT));
		RegSetValueEx(hK, _T("FileName"), 0, REG_NONE, (LPBYTE)szFile, FILENAMESZ*2);
		RegCloseKey(hK);
		PostQuitMessage(0);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;

}

// Îáðàáîò÷èê ñîîáùåíèé äëÿ îêíà "Î ïðîãðàììå".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == ID_OK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Exit(HWND hEx, UINT message, WPARAM wParam, LPARAM lParam){
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == ID_CANCELb|| LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hEx, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else 	if (LOWORD(wParam) == ID_NOSAVE){
			Ex = -1;
			EndDialog(hEx, LOWORD(wParam));
			return (INT_PTR)TRUE;

		}
		else 	if (LOWORD(wParam) == ID_SAVE_AS){
			Ex = 1;
			EndDialog(hEx, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
