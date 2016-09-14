#include <windows.h>    // includes basic windows functionality
#include <winbase.h>
#include <commdlg.h>    // includes common dialog functionality
#include <dlgs.h>       // includes common dialog template defines
#include <cderr.h>      // includes the common dialog error codes
#include <string.h>
#include <commctrl.h>  // TreeView and ImageList declarations live here.
#include <shlobj.h>
#include <wingdi.h>

#define IDI_ProfIcon	1
#define IDD_ProfileChoosing	1
#define IDI_ClosedFolder	2
#define IDI_OpenedFolder	3
#define IDC_SYSTREEVIEW1	101
#define IDB_Desktop 4
#define ID_HEADER 1

#ifdef STRICT
#define TYPEOFPROC WNDPROC
#else
#define TYPEOFPROC FARPROC
#endif

TYPEOFPROC lpPrevWndFunc;
TYPEOFPROC lpPrevBroseFunc;

BOOL InitInstance( HANDLE hInstance, int nCmdShow);
LONG APIENTRY MainWndProc( HWND hWnd, UINT message, UINT wParam, LONG lParam);
//BOOL CALLBACK FChooseProfile(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK BrowseCallbackProc(HWND  hwnd,UINT  uMsg,LPARAM  lParam,LPARAM  lpData);
HWND DoCreateHeader(HWND hwndParent);
int DoInsertHeaderItem(HWND hwndHeader, int iInsertAfter,int nWidth, LPSTR lpsz);

HINSTANCE g_hInst;		// the current instance

// Global pointer to the shell's IMalloc interface. 
LPMALLOC g_pMalloc;

HFONT hShrift; // хэндл шрифта

#pragma argsused
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow )
{

    MSG msg;
	 OSVERSIONINFO osVer; // for GetVersionEx()

     // Verify presence of Windows 95
     osVer.dwOSVersionInfoSize = sizeof(osVer);
     if (!GetVersionEx(&osVer))
          return (FALSE);

     if (osVer.dwPlatformId != VER_PLATFORM_WIN32_WINDOWS)
     {
     MessageBox(NULL, "This example requires Windows 95.",
                    "Profile Explorer", MB_OK );
     return (FALSE);
     }

	 // Create the main window.
	 if (!InitInstance(hInstance, nCmdShow))
		  return (FALSE);

	// Acquire and dispatch messages until a WM_QUIT message is received.
	 while (GetMessage(&msg,NULL,0,0))
        {
            TranslateMessage(&msg);    // Translates virtual key codes.
            DispatchMessage(&msg);     // Dispatches message to window.
        }


    // Release the shell's allocator.
    //g_pMalloc->lpVtbl->Release(g_pMalloc);
    
    return (msg.wParam);           // Returns the value from PostQuitMessage.

}


BOOL InitInstance( HANDLE hInstance, int nCmdShow)
{
    HWND            hWnd;

    BROWSEINFO  bi;
    LPSTR chFolderBuff;
    LPITEMIDLIST pidlBrowse;    // PIDL selected by user
    char Path[_MAX_PATH];

    // Save off the handle to the current instance.
    g_hInst = hInstance;


    // Create a main window for this application instance.
    hWnd = CreateWindow(
        "LISTBOX"	/*"ChooseProfileWindowClass"*/,
        "Выбор профиля",
        WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|LBS_MULTICOLUMN,            // Window style.
        CW_USEDEFAULT,                  // Default horizontal position.
        CW_USEDEFAULT,                  // Default vertical position.
        CW_USEDEFAULT,                  // Default width.
        CW_USEDEFAULT,                  // Default height.
        NULL,                           // Overlapped windows have no parent.
        NULL,                           // Use the window class menu.
        hInstance,                        // This instance owns this window.
        NULL                            // Pointer not needed.
    );

    // If window could not be created, return "failure".
    if (!hWnd)
        return (FALSE);

    SetClassLong(hWnd,	// handle of window
    										GCL_HICON,	// index of value to change
    										(LONG)  LoadIcon(g_hInst,	MAKEINTRESOURCE(IDI_ProfIcon)) 	// new value
   										);


    lpPrevWndFunc=(TYPEOFPROC)GetWindowLong(
											hWnd,	// handle of window
    										GWL_WNDPROC	// offset of value to set
                                 );
    SetWindowLong(hWnd,	// handle of window
    										GWL_WNDPROC,	// offset of value to set
                                 (LONG)MainWndProc 	// new value
                                 );



    // Get the shell's allocator.
    if (!SUCCEEDED(SHGetMalloc(&g_pMalloc)))
        return 1;
    // Allocate a buffer to receive browse information. 

    if ((chFolderBuff = (LPSTR) g_pMalloc->lpVtbl->Alloc(
            g_pMalloc, MAX_PATH)) == NULL)
        return (FALSE);
    ZeroMemory(&bi, sizeof(bi));
    bi.hwndOwner=NULL;
    bi.pidlRoot=NULL;
    bi.pszDisplayName=chFolderBuff;
    bi.lpszTitle="Выбор директории";
    bi.ulFlags=BIF_STATUSTEXT|BIF_RETURNONLYFSDIRS;
    bi.lpfn=(BFFCALLBACK)BrowseCallbackProc;
    bi.lParam=(LPARAM)Path;
    bi.iImage=NULL;


	// Make the window visible; update its client area; and return "success".
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
//    UpdateWindow(hTVDirectory);

    // Browse for a folder and return its PIDL. 
    pidlBrowse=SHBrowseForFolder(&bi);
    if (pidlBrowse != NULL)
    {
 
        if (SHGetPathFromIDList(pidlBrowse, chFolderBuff))
            SetWindowText(hWnd, chFolderBuff);

        // Free the PIDL returned by SHBrowseForFolder. 

        g_pMalloc->lpVtbl->Free(g_pMalloc, pidlBrowse);
    } 

    // Clean up. 
    g_pMalloc->lpVtbl->Free(g_pMalloc, chFolderBuff);

    // Free the PIDL for the Programs folder.
//    g_pMalloc->lpVtbl->Free(g_pMalloc, ppidl);

    // Release the shell's allocator.
    	g_pMalloc->lpVtbl->Release(g_pMalloc);


    return (TRUE);

}



LRESULT CALLBACK MainWndProc( HWND hWnd, UINT message, UINT wParam, LONG lParam)
{
	//LOGFONT lf; 
 

    switch (message) {

        case WM_CREATE:
            /*ZeroMemory(&lf, sizeof(lf));
        		lf.lfHeight = 8;
    			lstrcpy(lf.lfFaceName, "Times New Roman");
    			hShrift=CreateFontIndirect(&lf);*/
    			hShrift=CreateFont(
            	10,	// logical height of font
               10,	// logical average character width
               0,	// angle of escapement
               0,	// base-line orientation angle
               FW_NORMAL,	// font weight
               FALSE,	// italic attribute flag
               FALSE,	// underline attribute flag
               FALSE,	// strikeout attribute flag
               RUSSIAN_CHARSET,	// character set identifier
               OUT_DEFAULT_PRECIS,	// output precision
               CLIP_DEFAULT_PRECIS,	// clipping precision
               DEFAULT_QUALITY,	// output quality
               DEFAULT_PITCH,	// pitch and family
               "Arial" 	// address of typeface name string
               );

        return (CallWindowProc(
            			lpPrevWndFunc,	// address of previous procedure
                     hWnd,	// handle of window
                     message,	// message
                     wParam,	// first message parameter
                     lParam 	// second message parameter
                     )
                     /*DefWindowProc(hWnd, message, wParam, lParam)*/);



        case WM_DESTROY:                  // message: window being destroyed
        		DeleteObject(hShrift 	// handle of graphic object
   				);
            PostQuitMessage(0);
            break;


        default:
            return (CallWindowProc(
            			lpPrevWndFunc,	// address of previous procedure
                     hWnd,	// handle of window
                     message,	// message
                     wParam,	// first message parameter
                     lParam 	// second message parameter
                     )
                     /*DefWindowProc(hWnd, message, wParam, lParam)*/
                   );
    }
    return (0);
}

int CALLBACK BrowseCallbackProc(HWND hwnd,UINT message,LPARAM lParam,LPARAM lpData)
{
 HWND hTable/*,hTable_header*/;
 char szPath[_MAX_PATH];
 RECT  Rect, ClientRect;
 int HIndex_File_pofile,HIndex_Date_File,HIndex_Left_station,HIndex_Right_station,HIndex_R,HIndex_For_Azimuth,HIndex_Back_Azimuth,
 		HIndex_Left_Earth,HIndex_Right_Earth;
 LV_COLUMN col[]={{LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,LVCFMT_LEFT,70,"Файл профиля",13,0},
 						{LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,LVCFMT_LEFT,70,"Дата последнего изменения файла",32,1},
 						{LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,LVCFMT_LEFT,70,"Название станции слева",23,2},
                  {LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,LVCFMT_LEFT,70,"Название станции справа",24,3},
                  {LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,LVCFMT_LEFT,70,"Длина интервала, км",20,4},
                  {LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,LVCFMT_LEFT,70,"Прямой азимут, град. мин.",26,5},
                  {LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,LVCFMT_LEFT,70,"Обратный азимут, град. мин.",28,6},
                  {LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,LVCFMT_LEFT,70,"Отметка земли слева, м",23,7},
                  {LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,LVCFMT_LEFT,70,"Отметка земли справа, м",24,8}};
 LV_ITEM item;

 switch (message) {

 		  case BFFM_INITIALIZED:
        		GetWindowRect(hwnd,&Rect);
            GetClientRect(hwnd,&ClientRect);
            SetWindowPos(hwnd, HWND_TOP, 0, 0, 3*Rect.right-Rect.left-200, Rect.bottom, SWP_NOMOVE);

            InitCommonControls();

            hTable=CreateWindow(
					WC_LISTVIEW,	// address of registered class name
    				"Данные интервалов",	// address of window name
    				WS_CHILD|WS_BORDER|LVS_SINGLESEL|LVS_REPORT,	// window style
    				ClientRect.right+2,	// horizontal position of window
    				2,	// vertical position of window
    				2*Rect.right-202,	// window width
    				Rect.bottom-27,	// window height
    				hwnd,	// handle of parent or owner window
    				NULL,	// handle of menu or child-window identifier
    				g_hInst,	// handle of application instance
    				NULL 	// address of window-creation data
   				);

            HIndex_File_pofile=ListView_InsertColumn(hTable, 0,(const LV_COLUMN FAR*) &(col[0]));
            HIndex_Date_File=ListView_InsertColumn(hTable, 1,(const LV_COLUMN FAR*) &(col[1]));
            HIndex_Left_station=ListView_InsertColumn(hTable, 2,(const LV_COLUMN FAR*) &(col[2]));
            HIndex_Right_station=ListView_InsertColumn(hTable, 3,(const LV_COLUMN FAR*) &(col[3]));
            HIndex_R=ListView_InsertColumn(hTable, 4,(const LV_COLUMN FAR*) &(col[4]));
            HIndex_For_Azimuth=ListView_InsertColumn(hTable, 5,(const LV_COLUMN FAR*) &(col[5]));
            HIndex_Back_Azimuth=ListView_InsertColumn(hTable, 6,(const LV_COLUMN FAR*) &(col[6]));
            HIndex_Left_Earth=ListView_InsertColumn(hTable, 7,(const LV_COLUMN FAR*) &(col[7]));
            HIndex_Right_Earth=ListView_InsertColumn(hTable, 8,(const LV_COLUMN FAR*) &(col[8]));


/*            hTable_header=DoCreateHeader(hTable);
            HIndex_File_pofile=DoInsertHeaderItem(hTable_header, 0, 70, "Файл профиля");
            HIndex_Date_File=DoInsertHeaderItem(hTable_header, HIndex_File_pofile+1, 70, "Дата последнего изменения файла");
            HIndex_Left_station=DoInsertHeaderItem(hTable_header, HIndex_Date_File+1, 70, "Название станции слева");
            HIndex_Right_station=DoInsertHeaderItem(hTable_header, HIndex_Left_station+1, 70, "Название станции справа");
            HIndex_R=DoInsertHeaderItem(hTable_header, HIndex_Right_station+1, 70, "Длина интервала, км");
            HIndex_For_Azimuth=DoInsertHeaderItem(hTable_header, HIndex_R+1, 70, "Прямой азимут, град. мин.");
            HIndex_Back_Azimuth=DoInsertHeaderItem(hTable_header, HIndex_For_Azimuth+1, 70, "Обратный азимут, град. мин.");
            HIndex_Left_Earth=DoInsertHeaderItem(hTable_header, HIndex_Back_Azimuth+1, 70, "Отметка земли слева, м");
            HIndex_Right_Earth=DoInsertHeaderItem(hTable_header, HIndex_Left_Earth+1, 70, "Отметка земли справа, м");
*/

				item.mask=LVIF_TEXT;
    			item.iItem=0;
    			item.iSubItem=HIndex_File_pofile;
    			item.state=0;
    			item.stateMask=0;
    			item.pszText="crc0.001";
    			item.cchTextMax=9;
    			item.iImage=NULL;       // index of the list view item's icon
    			item.lParam=NULL;
            SendMessage(hTable, LVM_INSERTITEM, NULL, (LPARAM)(const LV_ITEM FAR*) &item);

            ListView_SetItemText(hTable, 0,HIndex_Date_File, "01.02.05");
/*            item.pszText="01.02.05";
            item.iSubItem=HIndex_Date_File;
            SendMessage(hTable, LVM_INSERTITEM, NULL, (LPARAM)(const LV_ITEM FAR*) &item);
*/

               // Make the window visible; update its client area; and return "success".
    				ShowWindow(hTable, SW_SHOWNA);
    				UpdateWindow(hTable);


        break;

        case	BFFM_SELCHANGED:
            SHGetPathFromIDList((LPITEMIDLIST)lParam,szPath);
            SendMessage(hwnd, BFFM_SETSTATUSTEXT, NULL, (LPARAM)szPath);
        break;

        case	HDN_TRACK:

        		return FALSE;


    }
    return (0);

}



// DoCreateHeader - creates a header control that is positioned along 
//     the top of the parent window's client area. 
// Returns the handle of the header control. 
// hwndParent - handle of the parent window 
// 

/*
HWND DoCreateHeader(HWND hwndParent)
{ 
        HWND hwndHeader; 
        RECT rcParent;
        HD_LAYOUT hdl;
        WINDOWPOS wp;
 
        // Ensure that the common control DLL is loaded, and then create 

        // the header control. 
        InitCommonControls();
 
        if ((hwndHeader = CreateWindowEx(0, WC_HEADER, (LPCTSTR) NULL, 
                WS_CHILD | WS_BORDER | HDS_BUTTONS | HDS_HORZ, 
                0, 0, 0, 0, hwndParent, (HMENU) ID_HEADER, g_hInst, 
                (LPVOID) NULL)) == NULL) 
            return (HWND) NULL;
 
        // Retrieve the bounding rectangle of the parent window's
        // client area, and then request size and position values 

        // from the header control. 
        GetClientRect(hwndParent, &rcParent); 
 
        hdl.prc = &rcParent; 
        hdl.pwpos = &wp; 
        if (!SendMessage(hwndHeader, HDM_LAYOUT, 0, (LPARAM) &hdl))
            return (HWND) NULL;

        SendMessage(hwndHeader, WM_SETFONT, (WPARAM) hShrift, MAKELPARAM(TRUE, 0));

        // Set the size, position, and visibility of the header control.
        SetWindowPos(hwndHeader, wp.hwndInsertAfter, wp.x, wp.y, 
            wp.cx, wp.cy, wp.flags | SWP_SHOWWINDOW); 
 
        return hwndHeader; 

}

// DoInsertItem - inserts items into a header control. 
// Returns the index of the newly added item. 
// hwndHeader - handle of the header control 
// iInsertAfter - index of the previous item 
// nWidth - width of the new item 
// lpsz - address of the item string 
int DoInsertHeaderItem(HWND hwndHeader, int iInsertAfter,
    int nWidth, LPSTR lpsz)
{ 
    HD_ITEM hdi; 
    int index; 


    hdi.mask = HDI_TEXT | HDI_FORMAT | HDI_WIDTH;
    hdi.pszText = lpsz; 

    hdi.cxy = nWidth;
    hdi.cchTextMax = lstrlen(hdi.pszText); 
    hdi.fmt = HDF_LEFT | HDF_STRING;
 
    index = SendMessage(hwndHeader, HDM_INSERTITEM,
        (WPARAM) iInsertAfter, (LPARAM) &hdi); 



    return index; 
 
} 
*/

