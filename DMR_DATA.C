#include <windows.h>    // includes basic windows functionality
#include <io.h>
#include <dir.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <mem.h>
#include <fcntl.h>
#include <shlobj.h>
#include <commctrl.h>

#ifdef STRICT
#define TYPEOFPROC WNDPROC
#else
#define TYPEOFPROC FARPROC
#endif

int DMR_Filename_array(char *files[500], char *path)  //���������� ���-�� ������ ���������� ���, ������������� �� ���� path,
																	 //� ��������� ������ ����� � ������� ������ ���������� ���, �� �������
                                                    //��������� files.
{
   char path_wildcard[512], file_pathname[512], file_string[30];
   struct ffblk ffblk;
   int done, Nf_001=0;
   FILE *stream;
  
   strcpy(path_wildcard, path);
   strcat(path_wildcard, "\\");
   strcat(path_wildcard, "*.dmr");

   done = findfirst(path_wildcard,&ffblk,0);//����� ������ ���� � ����������� "dmr"

   while (!done)//���� ���� ����� � ����������� "dmr"
   {
      strcpy(file_pathname, path);
      strcat(file_pathname, "\\");
      strcat(file_pathname, ffblk.ff_name);

   	/* open a file for read in a text mode */
   	stream = fopen(file_pathname, "rt");

   	/* read a string from the file */
   	fgets(file_string, 30, stream);//��������� ������ ������ �����

      if (file_string!=NULL)//���� �� ����� ����� � �� ������
      {
      	if(isdigit((int)file_string[0]))
            {
            	fgets(file_string, 30, stream);//��������� ������ ������ �����
            	if (file_string!=NULL)//���� �� ����� ����� � �� ������
            	{
               	if(isdigit((int)file_string[0]))
            		{
            			fgets(file_string, 30, stream);//��������� ������ ������ �����
            			if (file_string!=NULL)//���� �� ����� ����� � �� ������
                     {
                     	if(isdigit((int)file_string[0]))
            				{
            					fgets(file_string, 30, stream);//��������� ��������� ������ �����
            					if (file_string!=NULL)//���� �� ����� ����� � �� ������
                           {
                     			if(isdigit((int)file_string[0]))
            						{
            							fgets(file_string, 30, stream);//��������� ����� ������ �����
            							if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                 {
                     					if(isdigit((int)file_string[0]))
            								{
            									fgets(file_string, 30, stream);//��������� ������ ������ �����
            									if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                       {
                     							if(isdigit((int)file_string[0]))
            										{
            											fgets(file_string, 30, stream);//��������� ������� ������ �����
            											if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                             {
                     									if(isdigit((int)file_string[0]))
            												{
            													fgets(file_string, 30, stream);//��������� ������� ������ �����
            													if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                   {
                     											if(isdigit((int)file_string[0]))
            														{
            															fgets(file_string, 30, stream);//��������� ������� ������ �����
            															if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                         {
                     													if(isdigit((int)file_string[0]))
            																{
            																	fgets(file_string, 30, stream);//��������� ������� ������ �����
            																	if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                               {
                     															if((isdigit((int)file_string[0])))
            																		{
            																			fgets(file_string, 30, stream);//��������� ������������ ������ �����
            																			if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                                     {
                     																	if(isdigit((int)file_string[0]))
            																				{
            																					fgets(file_string, 30, stream);//��������� ����������� ������ �����
            																					if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                                           {
                     																			if(isdigit((int)file_string[0]))
            																						{
            																							fgets(file_string, 30, stream);//��������� ����������� ������ �����
            																							if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                                                 {
                     																					if(isdigit((int)file_string[0]))
            																								{
            																									fgets(file_string, 30, stream);//��������� ������������� ������ �����
            																									if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                                                       {
                     																							if(isdigit((int)file_string[0]))
            																										{
            																											fgets(file_string, 30, stream);//��������� ����������� ������ �����
            																											if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                                                             {
                     																									if(isdigit((int)file_string[0]))
            																												{
                                                                                                   files[Nf_001]=malloc((size_t)(strlen(ffblk.ff_name)+1));
                                                                                                   strcpy(files[Nf_001], ffblk.ff_name);
                                                                                                	Nf_001++;
                        																								}
                     																								}
                        																						}
                     																						}
                        																				}
                     																				}
                        																		}
                     																		}
                        																}
                     																}
                        														}
                     														}
                        												}
                     												}
                        										}
                     										}
                        								}
                     								}
                        						}
                     						}
                        				}
                     				}
                        		}
                     		}
                        }
                     }
                  }
         		}
            }
      }

      fclose(stream);


      done = findnext(&ffblk);
   }

   return Nf_001;
} // ����� DMR_Filename_array


int DMR_dataline_array(char * dataline[9], char *file_pathname)//��������� ������ dataline ����������� �� ������
																						 //�������, �� ��� ����� �������� ��������� file_pathname
                                                                   //dataline[0]-> ��� ����� ������ ��������� ���
                                                                   //dataline[1]-> ���� ���������� ��������� ����� ������ ��������� ���
                                                                   //dataline[2]-> �������� ������� �����
                                                                   //dataline[3]-> �������� ������� ������
                                                                   //dataline[4]-> ����� ���������, ��
                                                                   //dataline[5]-> ������ ������, ����., ���.
                                                                   //dataline[6]-> �������� ������, ����., ���.
                                                                   //dataline[7]-> ������� ����� �����
                                                                   //dataline[8]-> ������� ����� ������
                                                                   //���������� 0 - � ������ �����
                                                                   //			  1 - ���� �� ������� ������
{
   FILE * DMR_handle;
   int i,j=0,k, len_pathfilename,ind_kol_simv_PFileName,
      kol_simv_PFileName,
      ind_kol_simv_PLeftStationName,
      kol_simv_PLeftStationName,
      ind_kol_simv_PRightStationName,
      kol_simv_PRightStationName,
      ind_kol_simv_ForwardAzimuth,
      kol_simv_ForwardAzimuth,ind_kol_simv_BackwardAzimuth,kol_simv_BackwardAzimuth;
   char str_buf[30], R[30], Otm_l[30],
   		Otm_r[30];
   double zagolovok[6]={0,0,0,0,0,0}, *DMR;
   struct ftime ftimep;


   /* open a file for read in a text mode */
	DMR_handle=fopen(file_pathname, "rt");

      do
      {
         fgets(str_buf, 30, DMR_handle);
         if((j<=zagolovok[3]+zagolovok[4]) && j>4 || j<=4)//���� ����� �������� ������ �� ����������� ������ ������ ���������� ������� �������� ������� ������
         {
            	if(j<=11)  // ������ ��������� �����
            		{
             			zagolovok[j]=atof(str_buf);
            		}
            		else
            			if(j==12)    //��������� ������ � ���������� ������� DMR ���������� �����
               		{
                        DMR=malloc((zagolovok[3]+zagolovok[4]+1)*sizeof(double));// allocate space for DMR (�� �������� ������� ������ ������������)
								if (DMR==NULL)
      							return 1;           // if allocation is insufficient return the error code
                  		for(k=0;k<=11;k++)
                  			DMR[k]=zagolovok[k];
                  		DMR[j]=atof(str_buf);
               		}
               		else
            				if(j<=zagolovok[3]+zagolovok[4]) // j <= ������ ������ ���������� ������� �������� ������� ������
                  			DMR[j]=atof(str_buf);
            j++;
         }

      }while((j<=zagolovok[3]+zagolovok[4]) && j>4 || j<=4);//���� ����� �������� ������ �� ����������� ������ ������ ���������� ������� �������� ������� ������

      len_pathfilename=strlen(file_pathname); //���������� �������� � ���� ����� �������
      k=len_pathfilename-1;
      while(file_pathname[k]!='\\')
      	k--;
      k++;
      dataline[0]=malloc(strlen(&file_pathname[k])+1);
      strcpy(dataline[0], &file_pathname[k]);//���������� ����� �����

      getftime(fileno(DMR_handle), &ftimep);//����������� ���� � ������� ���������� ��������� �����
      dataline[1]=malloc(27);
      strcpy(dataline[1],gcvt((double) ftimep.ft_day, 2, str_buf));
      strcat(dataline[1],".");
      strcat(dataline[1],gcvt((double) ftimep.ft_month, 2, str_buf));
      strcat(dataline[1],".");
      strcat(dataline[1],gcvt((double) ftimep.ft_year+1980., 4, str_buf));
      strcat(dataline[1]," �. ");
      strcat(dataline[1],gcvt((double) ftimep.ft_hour, 2, str_buf));
      strcat(dataline[1],":");
      strcat(dataline[1],gcvt((double) ftimep.ft_min, 2, str_buf));
      strcat(dataline[1]," �:���");

      fclose(DMR_handle);

      dataline[2]=malloc(DMR[2]+1);
      for(i=0;i<DMR[2];i++)
      	dataline[2][i]=(char)DMR[DMR[1]+1+i];//���������� �������� ������� �����
      dataline[2][DMR[2]]=(char)0;
//      OemToChar(stancia_A,stancia_A);

      dataline[3]=malloc(DMR[4]+1);
      for(i=0;i<DMR[4];i++)
      	dataline[3][i]=(char)DMR[DMR[3]+1+i];//���������� �������� ������� ������
      dataline[3][DMR[4]]=(char)0;

      gcvt(DMR[19+((int)DMR[8]-1)*3], 5, R);
      dataline[4]=malloc(strlen(R)+1);
      strcpy(dataline[4],R);//���������� ����� ���������


      ind_kol_simv_PFileName=25+DMR[8]*3+DMR[7]*4+DMR[11]*2;
      kol_simv_PFileName=DMR[ind_kol_simv_PFileName];
      ind_kol_simv_PLeftStationName=ind_kol_simv_PFileName+kol_simv_PFileName+1;
      kol_simv_PLeftStationName=DMR[ind_kol_simv_PLeftStationName];
      ind_kol_simv_PRightStationName=ind_kol_simv_PLeftStationName+kol_simv_PLeftStationName+1;
      kol_simv_PRightStationName=DMR[ind_kol_simv_PRightStationName];
      ind_kol_simv_ForwardAzimuth=ind_kol_simv_PRightStationName+kol_simv_PRightStationName+1;
      kol_simv_ForwardAzimuth=DMR[ind_kol_simv_ForwardAzimuth];
      dataline[5]=malloc(kol_simv_ForwardAzimuth+1);
      for(i=0;i<kol_simv_ForwardAzimuth;i++)
      	dataline[5][i]=(char)DMR[ind_kol_simv_ForwardAzimuth+1+i];//���������� ������� �������
      dataline[5][kol_simv_ForwardAzimuth]=(char)0;

      ind_kol_simv_BackwardAzimuth=ind_kol_simv_ForwardAzimuth+kol_simv_ForwardAzimuth+1;
      kol_simv_BackwardAzimuth=DMR[ind_kol_simv_BackwardAzimuth];
      dataline[6]=malloc(kol_simv_ForwardAzimuth+1);
      for(i=0;i<kol_simv_BackwardAzimuth;i++)
      	dataline[6][i]=(char)DMR[ind_kol_simv_BackwardAzimuth+1+i];//���������� ��������� �������
      dataline[6][kol_simv_BackwardAzimuth]=(char)0;

      gcvt(DMR[20], 5, Otm_l);
      dataline[7]=malloc(strlen(Otm_l)+1);
      strcpy(dataline[7],Otm_l);//���������� ������� ����� �����

      gcvt(DMR[20+((int)DMR[8]-1)*3], 5, Otm_r);
      dataline[8]=malloc(strlen(Otm_r)+1);
      strcpy(dataline[8],Otm_r);//���������� ������� ����� ������


	return 0;

}// ����� DMR_dataline_array


TYPEOFPROC lpPrevWndFunc;

BOOL BrowseForDMR(char *file_pathname, HANDLE hInstance);
LONG APIENTRY MainWndProc( HWND hWnd, UINT message, UINT wParam, LONG lParam);
int CALLBACK BrowseCallbackProc(HWND  hwnd,UINT  uMsg,LPARAM  lParam,LPARAM  lpData);
LRESULT CALLBACK DMRLVProc( HWND hWnd, UINT message, UINT wParam, LONG lParam);

HINSTANCE g_hInst;		// the current instance (������ ���� ����� ���������� � ���������� ���������)
// Global pointer to the shell's IMalloc interface.
LPMALLOC g_pMalloc;
BOOL DMR_Choosed; //TRUE - ���� ���� ������ ������, FALSE - ���� �� ������.
char DMR_PathFile_Name[_MAX_PATH];//���������� ���� � ��� ���������� ����� �������
char szPath[_MAX_PATH];
BOOL bPathExist=FALSE, bSHBFFInitialised=FALSE;

/* ��������� ��� ��������� �������*/
/*
#pragma argsused
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow )
{

    OSVERSIONINFO osVer; // for GetVersionEx()
    char strDMR[_MAX_PATH];
    BOOL choosed;
     // Verify presence of Windows 95
     osVer.dwOSVersionInfoSize = sizeof(osVer);
     if (!GetVersionEx(&osVer))
          return (FALSE);

     if (osVer.dwPlatformId != VER_PLATFORM_WIN32_WINDOWS)
     {
     MessageBox(NULL, "This example requires Windows 95.",
                    "DMR Explorer", MB_OK );
     return (FALSE);
     }

	 // Create the main window.
    choosed=BrowseForDMR(strDMR ,hInstance);
	 if (!choosed)
		  return (FALSE);
	return (TRUE);
}//����� WinMain

*/

#pragma argsused
BOOL BrowseForDMR(char *file_pathname, HANDLE hInstance)//��������� ������ �� ������� ��������� file_pathname �����
																				//� ���������� ����� DMR, ���������� TRUE. ���� ���� ��
                                                            //������ - ���������� FALSE
{
    BROWSEINFO  bi;
    LPSTR chFolderBuff;
    LPITEMIDLIST pidlBrowse;    // PIDL selected by user
    char Path[_MAX_PATH];
    // Save off the handle to the current instance.
    g_hInst = hInstance;


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
    bi.lpszTitle="����� ����������";
    bi.ulFlags=BIF_STATUSTEXT|BIF_RETURNONLYFSDIRS;
    bi.lpfn=(BFFCALLBACK)BrowseCallbackProc;
    bi.lParam=(LPARAM)Path;
    bi.iImage=NULL;



    // Browse for a folder and return its PIDL.
    pidlBrowse=SHBrowseForFolder(&bi);
    bPathExist=FALSE;
    bSHBFFInitialised=FALSE;
    szPath[0]=0;
    if (pidlBrowse != NULL)
    {
      if(DMR_Choosed)
      {
         strcpy(file_pathname,DMR_PathFile_Name);

         // Free the PIDL returned by SHBrowseForFolder.
			g_pMalloc->lpVtbl->Free(g_pMalloc, pidlBrowse);

      	// Clean up.
      	g_pMalloc->lpVtbl->Free(g_pMalloc, chFolderBuff);

         // Release the shell's allocator.
    		g_pMalloc->lpVtbl->Release(g_pMalloc);

    		return (TRUE);
      }
      // Free the PIDL returned by SHBrowseForFolder.
      g_pMalloc->lpVtbl->Free(g_pMalloc, pidlBrowse);

      // Clean up.
      g_pMalloc->lpVtbl->Free(g_pMalloc, chFolderBuff);

      // Release the shell's allocator.
      g_pMalloc->lpVtbl->Release(g_pMalloc);

      return (FALSE);
    }

    // Clean up.
    g_pMalloc->lpVtbl->Free(g_pMalloc, chFolderBuff);


    // Release the shell's allocator.
    g_pMalloc->lpVtbl->Release(g_pMalloc);


    return (FALSE);

}//����� BrowseForDMR



#define N_Columns 9 //���-�� ������� � ListView
#pragma argsused
int CALLBACK BrowseCallbackProc(HWND hwnd,UINT message,LPARAM lParam,LPARAM lpData)
{
 static HWND hTable/*,hTable_header*/;
 char *files[500], *dataline[9], selected_filename[100];
 static char process_dir_001_dmr_file[_MAX_PATH];
 LPTSTR lpProcessCmdLine;
 RECT  Rect, ClientRect;
 static int HIndex[N_Columns];
 int N_DMR_in_folder,i,j,k;
 FILE *cur_dir_file;
 LV_COLUMN col[]={{LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,LVCFMT_LEFT,90,"���� DMR",9,0},
 						{LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,LVCFMT_LEFT,70,"���� ���������� ��������� �����",32,1},
 						{LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,LVCFMT_LEFT,150,"�������� ������� �����",23,2},
                  {LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,LVCFMT_LEFT,150,"�������� ������� ������",24,3},
                  {LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,LVCFMT_LEFT,70,"����� ���������, ��",20,4},
                  {LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,LVCFMT_LEFT,70,"������ ������, ����. ���.",26,5},
                  {LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,LVCFMT_LEFT,70,"�������� ������, ����. ���.",28,6},
                  {LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,LVCFMT_LEFT,70,"������� ����� �����, �",23,7},
                  {LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,LVCFMT_LEFT,70,"������� ����� ������, �",24,8}};
 LV_ITEM item={LVIF_TEXT|LVIF_STATE,0,0,0,0,"",30,NULL,NULL};

 switch (message) {

 		  case BFFM_INITIALIZED:
        		GetWindowRect(hwnd,&Rect);
            GetClientRect(hwnd,&ClientRect);
            SetWindowPos(hwnd, HWND_TOP, 0, 0, 3*Rect.right-Rect.left-200, Rect.bottom, SWP_NOMOVE);

            InitCommonControls();

            hTable=CreateWindow(
					WC_LISTVIEW,	// address of registered class name
    				"������ ����������",	// address of window name
    				WS_CHILD|WS_BORDER|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_REPORT,	// window style
    				ClientRect.right+2,	// horizontal position of window
    				2,	// vertical position of window
    				2*Rect.right-202,	// window width
    				Rect.bottom-27,	// window height
    				hwnd,	// handle of parent or owner window
    				NULL,	// handle of menu or child-window identifier
    				g_hInst,	// handle of application instance
    				NULL 	// address of window-creation data
   				);

            lpPrevWndFunc=(TYPEOFPROC)GetWindowLong(
											hTable,	// handle of window
    										GWL_WNDPROC	// offset of value to set
                                 );
    			SetWindowLong(hTable,	// handle of window
    										GWL_WNDPROC,	// offset of value to set
                                 (LONG)DMRLVProc 	// new value
                                 );
            for(j=0;j<N_Columns;j++)
            {
            	HIndex[j]=ListView_InsertColumn(hTable, j,(const LV_COLUMN FAR*) &(col[j]));
            }

            //��������� ���������� ������������ ����� ������ ��������
            lpProcessCmdLine=GetCommandLine();
            k=lstrlen(lpProcessCmdLine);//���������� �������� � ������ ���� ������� ��� ������������ �����
            while(lpProcessCmdLine[k]!='\\') k--;//������� �� ���������� ����� � ����\����� �����
            if(lstrcpyn(process_dir_001_dmr_file,(lpProcessCmdLine[0]=='\"')? &lpProcessCmdLine[1]:lpProcessCmdLine, k++))//�������� ���� ���������� � �����
            {
            	bPathExist=TRUE;
            	if(access(strcat(process_dir_001_dmr_file,"\\cur_001_dmr_dir"), 0)==0)//���� ���� cur_001_dmr_dir ����������
   	         {
      	      	cur_dir_file=fopen(process_dir_001_dmr_file,"rt");
                  fread(DMR_PathFile_Name,_MAX_PATH+1,1,cur_dir_file);
                  fclose(cur_dir_file);
                  if(access(DMR_PathFile_Name, 0)==0)//���� ����������, ���������� � ����� cur_001_dmr_dir ����������
         	   	{
                  	lstrcpy(szPath,DMR_PathFile_Name);
							SendMessage(hwnd, BFFM_SETSELECTION , TRUE, (LPARAM)DMR_PathFile_Name);//������� ��
                  }
            	}
            }

            SendMessage(hwnd, BFFM_SETSTATUSTEXT, NULL, (LPARAM)szPath);
            N_DMR_in_folder=DMR_Filename_array(files, szPath);  //���������� ���-�� ������ DMR, ������������� �� ���� path,
																	 //� ��������� ������ ����� � ������� ������ DMR, �� �������
                                                    //��������� files.
            if(N_DMR_in_folder>0)
            {
            	for(i=0;i<N_DMR_in_folder;i++)
               {
                  strcpy(DMR_PathFile_Name,szPath);
                  strcat(DMR_PathFile_Name,"\\");
                  strcat(DMR_PathFile_Name,files[i]);
                  DMR_dataline_array(dataline, DMR_PathFile_Name);
                  item.iItem=i;
               	item.iSubItem=HIndex[0];
                  item.pszText=files[i];
                  SendMessage(hTable, LVM_INSERTITEM, NULL, (LPARAM)(const LV_ITEM FAR*) &item);
                  for(j=1;j<N_Columns;j++)
                  {
                  	ListView_SetItemText(hTable, i,HIndex[j], dataline[j]);
                  }
               }

               item.mask=LVIF_STATE;
               item.iItem=0;
               item.state=LVIS_SELECTED|LVIS_FOCUSED;
               item.stateMask=LVIS_SELECTED|LVIS_FOCUSED;

               for(j=0;j<N_Columns;j++)
               {
               	item.iSubItem=j;
               	ListView_SetItem(hTable, &item);
               }

         		ListView_GetItemText(hTable, 0,0, selected_filename, 100);
         		strcpy(DMR_PathFile_Name,szPath);
         		strcat(DMR_PathFile_Name,"\\");
         		strcat(DMR_PathFile_Name,selected_filename);
               DMR_Choosed=TRUE;

               if( bPathExist )
               {
               	cur_dir_file=fopen(process_dir_001_dmr_file,"wt");
               	fwrite(szPath,strlen(szPath)+1,1,cur_dir_file);
               	fclose(cur_dir_file);
               }
            }
            else
            	SendMessage(hwnd, BFFM_ENABLEOK , NULL, NULL);

               bSHBFFInitialised=TRUE;

               // Make the window visible; update its client area; and return "success".
    				ShowWindow(hTable, SW_SHOWNA);
    				UpdateWindow(hTable);


        break;

        case	BFFM_SELCHANGED:

        		if(! bSHBFFInitialised ) break;

        		DMR_Choosed=FALSE;
        		ListView_DeleteAllItems(hTable);
            SHGetPathFromIDList((LPITEMIDLIST)lParam,szPath);
            SendMessage(hwnd, BFFM_SETSTATUSTEXT, NULL, (LPARAM)szPath);
            N_DMR_in_folder=DMR_Filename_array(files, szPath);  //���������� ���-�� ������ DMR, ������������� �� ���� path,
																	 //� ��������� ������ ����� � ������� ������ DMR, �� �������
                                                    //��������� files.
            if(N_DMR_in_folder>0)
            {
            	for(i=0;i<N_DMR_in_folder;i++)
               {
                  strcpy(DMR_PathFile_Name,szPath);
                  strcat(DMR_PathFile_Name,"\\");
                  strcat(DMR_PathFile_Name,files[i]);
                  DMR_dataline_array(dataline, DMR_PathFile_Name);
                  item.iItem=i;
               	item.iSubItem=HIndex[0];
                  item.pszText=files[i];
                  SendMessage(hTable, LVM_INSERTITEM, NULL, (LPARAM)(const LV_ITEM FAR*) &item);
                  for(j=1;j<N_Columns;j++)
                  {
                  	ListView_SetItemText(hTable, i,HIndex[j], dataline[j]);
                  }
               }

               item.mask=LVIF_STATE;
               item.iItem=0;
               item.state=LVIS_SELECTED|LVIS_FOCUSED;
               item.stateMask=LVIS_SELECTED|LVIS_FOCUSED;

               for(j=0;j<N_Columns;j++)
               {
               	item.iSubItem=j;
               	ListView_SetItem(hTable, &item);
               }

         		ListView_GetItemText(hTable, 0,0, selected_filename, 100);
         		strcpy(DMR_PathFile_Name,szPath);
         		strcat(DMR_PathFile_Name,"\\");
         		strcat(DMR_PathFile_Name,selected_filename);
               DMR_Choosed=TRUE;

               if( bPathExist )
               {
               	cur_dir_file=fopen(process_dir_001_dmr_file,"wt");
               	fwrite(szPath,strlen(szPath)+1,1,cur_dir_file);
               	fclose(cur_dir_file);
               }
            }
            else
            	SendMessage(hwnd, BFFM_ENABLEOK , NULL, NULL);
    }
    return (NULL);

}//����� BrowseCallbackProc

LRESULT CALLBACK DMRLVProc( HWND hWnd, UINT message, UINT wParam, LONG lParam)
{
	int i,N,j, i_to_select, i_selected;
   char selected_filename[100];
   RECT FAR rc;
   LV_ITEM item={LVIF_STATE,0,0,0,0,NULL,NULL,NULL,NULL};

	switch (message)
   {
   		case WM_LBUTTONDOWN:
         case WM_LBUTTONDBLCLK:
         case WM_RBUTTONDOWN:
         case WM_RBUTTONDBLCLK:

         SetFocus(hWnd );

         N=ListView_GetItemCount(hWnd);
         DMR_Choosed=FALSE;
         if(N>0)
         {
            item.iSubItem=1;

         	for(i=0;i<N;i++)
         	{
            	ListView_GetItemRect(hWnd,i, &rc, LVIR_BOUNDS);
               item.iItem=i;
					item.state=LVIS_SELECTED|LVIS_FOCUSED;
               item.stateMask=LVIS_SELECTED|LVIS_FOCUSED;
               ListView_GetItem(hWnd, &item);
               if(item.state==(LVIS_SELECTED|LVIS_FOCUSED))
               	i_selected=i;
            	if(MAKEPOINTS(lParam).y<=rc.bottom && MAKEPOINTS(lParam).y>=rc.top)
               	i_to_select=i;
         	}

            for(j=0;j<N_Columns;j++)
            {
            	item.iItem=i_selected;
					item.iSubItem=j;
               item.state=0;
               item.stateMask=LVIS_SELECTED|LVIS_FOCUSED;
             	ListView_SetItem(hWnd, &item);
               item.iItem=i_to_select;
               item.state=LVIS_SELECTED|LVIS_FOCUSED;
               item.stateMask=LVIS_SELECTED|LVIS_FOCUSED;
               ListView_SetItem(hWnd, &item);
            }

         	ListView_GetItemText(hWnd, i_to_select,0, selected_filename, 100);
         	strcpy(DMR_PathFile_Name,szPath);
         	strcat(DMR_PathFile_Name,"\\");
         	strcat(DMR_PathFile_Name,selected_filename);
         	DMR_Choosed=TRUE;
         }
         return (0);

   }

   return (CallWindowProc(lpPrevWndFunc,	// address of previous procedure
                     hWnd,	// handle of window
                     message,	// message
                     wParam,	// first message parameter
                     lParam 	// second message parameter
                     ));
}//����� DMRLVProc
