
/////////////////////////////////////////////////
//Модуль: FontDir
//Программа: FontDir.cpp
//Опиcание:
/////////////////////////////////////////////////
#include <windows.h>
#include <windowsx.h>
#include <shlobj.h>
#include <commctrl.h>
#define STRICT
#define WIN32_LEAN_AND_MEAN
#pragma hdrstop
#pragma warning (disable: 4068)
#pragma warning (disable: 4100)
    #ifdef _DEBUG
        #pragma comment(lib, "COMCTL32.LIB")
    #else
        #pragma comment(lib, "COMCTL32.LIB")
    #endif
//#include "Abutton.h"
//#include "ALBox.h"
//#include "FontSee.h"
//#include "FontDir.h"
//#include "MyBox.h"
/////////////////////////////////////////////////
//Функции
/////////////////////////////////////////////////
int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
BOOL CALLBACK FontDirDlgProc(HWND hdlg, UINT Message, WPARAM wParam, LPARAM lParam);
HBRUSH FontDir_OnCtlColor(HWND hwnd, HDC hdc, HWND hwndChild, int type);
void FontDir_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
BOOL GetPathFromTree(HWND hwnd);
void FillFileList(HWND hwnd);
//Окна, элементы управления
//Переменные
char szDirName[MAX_PATH];
char szTmpDirName[MAX_PATH];
static HINSTANCE hInstance;
extern HCURSOR CursorHand;
DLGPROC OldDirDlgProc;
extern HFONT FontList;
int nRetCode; //-1, 0, 1
HIMAGELIST hIList;
/////////////////////////////////////////////////
//cоздание диалога
/////////////////////////////////////////////////
int GetFontDir(HWND hwnd, HINSTANCE hInst)
{

BROWSEINFO biBI;
hIList=NULL;
hInstance=hInst;
RegisterMyBox(hInst);
ZeroMemory(&biBI, sizeof(biBI));
biBI.hwndOwner=hwnd;
biBI.lpfn=(BFFCALLBACK)BrowseCallbackProc;
lstrcpy(szTmpDirName, szDirName);
SHBrowseForFolder(&biBI);
if(nRetCode)
lstrcpy(szDirName, szTmpDirName);
return nRetCode;
}
/////////////////////////////////////////////////
//
/////////////////////////////////////////////////
BOOL GetPathFromTree(HWND hwnd)
{
LPSTR lpSTR=szTD;
int l=lstrlen(szTD);
char S[MAX_PATH]={""};
char S1[MAX_PATH]={""};
char szTD[MAX_PATH]={""};
TV_ITEM tvi;
tvi.mask=TVIF_HANDLE|TVIF_TEXT;
tvi.hItem=TreeView_GetSelection(hwnd);;
tvi.pszText=S;
tvi.cchTextMax=sizeof S;
do{
TreeView_GetItem(hwnd, &tvi);
wsprintf(S1, "%s\\%s", S, szTD);
lstrcpy(szTD, S1);
}while(tvi.hItem=TreeView_GetParent(hwnd, tvi.hItem));

for(int i=0; i<l; i++)
if(lpSTR[i]=='('){
lstrcpy(szTD, &lpSTR[i+1]);
lstrcpy(&szTD[2], &szTD[3]);
lstrcpy(szTmpDirName, szTD);
return TRUE;}
return FALSE;
}
/////////////////////////////////////////////////
//
/////////////////////////////////////////////////
LPSTR GetFileExt(LPSTR szFileName)
{
static const char *szSIExt[]={{"exe"}, {"dll"}, {"lnk"}, {"pif"}, {"cur"}, {"ico"}, {"ani"}, {" "}};
LPSTR szFileExt=" ";
int l=lstrlen(szFileName)-1;
for(int i=l; i>0; i--)
if(szFileName[i]=='.'){
szFileExt=&szFileName[i+1];
break;}
for(i=0; i<8; i++)
if(!lstrcmpi(szFileExt, szSIExt[i]))
return NULL;
return szFileExt;
}
/////////////////////////////////////////////////
//
/////////////////////////////////////////////////
void FillFileList(HWND hwnd)
{
char szFName[MAX_PATH];
char szFExt[MAX_PATH];
WIN32_FIND_DATA fdFD;
LPSTR szExt;
LV_ITEM lvi;
lvi.mask=LVIF_TEXT|LVIF_IMAGE;
lvi.iItem=0;
lvi.iSubItem=0;
lvi.lParam=0;
lvi.pszText=&fdFD.cFileName[0];
lvi.cchTextMax=MAX_PATH;
ListView_DeleteAllItems(hwnd);
HWND hExtList=GetDlgItem(GetParent(hwnd), CM_EXTLIST);
SendMessage(hExtList, LB_RESETCONTENT, 0, 0);
int cMZP=0;
static int NNN=0;
wsprintf(szFName, "%s*.*", szTmpDirName);
HANDLE hFindFile=FindFirstFile(szFName, &fdFD);
if(hFindFile)
do
if(!(fdFD.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
if(szExt=GetFileExt(fdFD.cFileName))
NNN=SendMessage(hExtList, LB_ADDSTRING, 0, (LPARAM)szExt);
else
cMZP++;
while(FindNextFile(hFindFile, &fdFD));
FindClose(hFindFile);
int cFileExt=SendMessage(hExtList, LB_GETCOUNT, 0, 0);
hIList=ImageList_Create(16, 16, ILC_COLOR24, cFileExt+cMZP, 0);
ImageList_Destroy(ListView_SetImageList(hwnd, hIList, LVSIL_SMALL));
WORD iIcon;
for(int i=0; i<cFileExt; i++)
{
SendMessage(hExtList, LB_GETTEXT, i, (LPARAM)szFExt);
wsprintf(szFName, "%s*.%s", szTmpDirName, szFExt);
ImageList_AddIcon(hIList, ExtractAssociatedIcon(hInstance, szFName, &iIcon));
}
wsprintf(szFName, "%s*.*", szTmpDirName);
hFindFile=FindFirstFile(szFName, &fdFD);
if(hFindFile)
do
if(!(fdFD.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
{
if(!fdFD.cAlternateFileName[0]){
CharLower(lvi.pszText);CharUpperBuff(lvi.pszText, 1);}
wsprintf(szFName, "%s%s", szTmpDirName, lvi.pszText);
if(szExt=GetFileExt(fdFD.cFileName))
lvi.iImage=(unsigned int)SendMessage(hExtList, LB_FINDSTRINGEXACT, -1, (LPARAM)szExt);
else{
ImageList_AddIcon(hIList, ExtractAssociatedIcon(hInstance, szFName, &iIcon));
lvi.iImage=cFileExt++;}
ListView_InsertItem(hwnd, &lvi);
}
while(FindNextFile(hFindFile, &fdFD));
FindClose(hFindFile);
} 
Rebel
4 - 12.12.2001 - 15:21 /////////////////////////////////////////////////
//Процедура, обрабатывающая cообщения
/////////////////////////////////////////////////
#pragma argsused
int CALLBACK BrowseCallbackProc(HWND hDlg, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
if(uMsg==BFFM_INITIALIZED)
{
SetWindowLong(hDlg, GWL_STYLE, LONG(WS_CLIPCHILDREN|DS_MODALFRAME|WS_CHILD|WS_POPUP));
DestroyWindow(GetDlgItem(hDlg, 14146));
DestroyWindow(GetDlgItem(hDlg, 14147));
DestroyWindow(GetDlgItem(hDlg, 1)); //OK
DestroyWindow(GetDlgItem(hDlg, 2)); //cancel
SetWindowPos(hDlg, NULL, 0, 0, 400, 244, SWP_NOMOVE|SWP_NOZORDER);
RECT Rc, RcD;
GetWindowRect(GetParent(hDlg), &Rc);
GetClientRect(hDlg, &RcD);
SetWindowPos(hDlg, NULL, Rc.left+(Rc.right-Rc.left-RcD.right)/2, Rc.top+(Rc.bottom-Rc.top-RcD.bottom)/2, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
SetWindowPos(GetDlgItem(hDlg, 14145), NULL, 8, 8, 168, 160, SWP_NOZORDER);
CreateWindow("static", szTmpDirName, WS_VISIBLE|WS_CHILD,
8, 175, 380, 15, hDlg, HMENU(14146), hInstance, NULL);
SendMessage(GetDlgItem(hDlg, 14146), WM_SETFONT, WPARAM(FontList), 0);
EnableCursor(hDlg, CursorHand);
CreateAniButton(hDlg, "BtnB_pic",
10, 198, 130, 30, 0, 0, CM_SYS, hInstance, NULL, FALSE);
CreateAniButton(hDlg, "BtnB_pic",
200, 198, 86, 30, 130, 0, CM_NO, hInstance, NULL, FALSE);
CreateAniButton(hDlg, "BtnB_pic",
300, 198, 86, 30, 216, 0, CM_OK, hInstance, (szTmpDirName[0]?0:BS_CHECKBOX), FALSE);
CreateWindow(WC_LISTVIEW, NULL, LVS_SORTASCENDING|LVS_SINGLESEL|LVS_LIST|WS_VISIBLE|WS_CHILD|WS_TABSTOP,
178, 10, 208, 156, hDlg, (HMENU)CM_LISTVIEW, hInstance, NULL);
CreateWindow("mybox", NULL, WS_VISIBLE|WS_CHILD,
176, 8, 212, 160, hDlg, NULL, hInstance, NULL);
ShowWindow(CreateActListbox(hDlg, 0, 0, 10, 10, 0, 0, NULL, CM_EXTLIST, hInstance, LBS_NODOUBLE), SW_HIDE);
if(szTmpDirName[0])
FillFileList(GetDlgItem(hDlg, CM_LISTVIEW));
OldDirDlgProc=(DLGPROC)SetWindowLong(hDlg, GWL_WNDPROC, LONG(FontDirDlgProc));
}
if(uMsg==BFFM_SELCHANGED)
{
if(GetPathFromTree(GetDlgItem(hDlg, 14145)))
FillFileList(GetDlgItem(hDlg, CM_LISTVIEW));
SetWindowText(GetDlgItem(hDlg, 14146), szTmpDirName);
if(szTmpDirName[0])
Button_SetStyle(GetDlgItem(hDlg, CM_OK), 0, TRUE);
}
return 0;
}
/////////////////////////////////////////////////
//Оконная процедура, обрабатывающая cообщeния
/////////////////////////////////////////////////
BOOL CALLBACK FontDirDlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{
switch(Message)
{
HANDLE_MSG(hDlg, WM_LBUTTONDOWN,FontsExp_OnLButtonDown);
HANDLE_MSG(hDlg, WM_LBUTTONUP,FontsExp_OnLButtonUp);
HANDLE_MSG(hDlg, WM_MOUSEMOVE,FontsExp_OnMouseMove);
HANDLE_MSG(hDlg, WM_COMMAND, FontDir_OnCommand);
HANDLE_MSG(hDlg, WM_CTLCOLORSTATIC, FontsExp_OnCtlColor);
HANDLE_MSG(hDlg, WM_CTLCOLORDLG, FontsExp_OnCtlColor);
}
return CallWindowProc((WNDPROC)OldDirDlgProc, hDlg, Message, wParam, lParam);
}
/////////////////////////////////////////////////
//Процeдура управлeния
/////////////////////////////////////////////////
#pragma argsused
void FontDir_OnCommand(HWND hDlg, int id, HWND hwndCtl, UINT codeNotify)
{
switch(id)
{
case CM_OK:
case CM_NO:
case CM_SYS:
nRetCode=CM_NO-id;
ImageList_Destroy(hIList);
EndDialog(hDlg, TRUE);
break;
}
} 

