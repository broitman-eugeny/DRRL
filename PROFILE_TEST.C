#include <windows.h>    // includes basic windows functionality
#include<io.h>
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

int Profile_Filename_array(char *files[500], char *path)  //���������� ���-�� ������ ��������, ������������� �� ���� path,
																	 //� ��������� ������ ����� � ������� ������ ��������, �� �������
                                                    //��������� files.
{
   char path_wildcard[512], file_pathname[512], file_string[30];
   struct ffblk ffblk;
   int done, Nf_001=0;
   FILE *stream;
  
   strcpy(path_wildcard, path);
   strcat(path_wildcard, "\\");
   strcat(path_wildcard, "*.001");

   done = findfirst(path_wildcard,&ffblk,0);//����� ������ ���� � ����������� "001"

   while (!done)//���� ���� ����� � ����������� "001"
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
      	fgets(file_string, 30, stream);//��������� ������ ������ �����
         if (file_string!=NULL)//���� �� ����� ����� � �� ������
         {
         	if(file_string[0]==' ' && isdigit((int)file_string[1]))
            {
            	fgets(file_string, 30, stream);//��������� ������ ������ �����
            	if (file_string!=NULL)//���� �� ����� ����� � �� ������
            	{
               	if(file_string[0]==' ' && isdigit((int)file_string[1]))
            		{
            			fgets(file_string, 30, stream);//��������� ��������� ������ �����
            			if (file_string!=NULL)//���� �� ����� ����� � �� ������
                     {
                     	if(file_string[0]==' ' || file_string[0]=='.' || isdigit((int)file_string[0]))
            				{
            					fgets(file_string, 30, stream);//��������� ����� ������ �����
            					if (file_string!=NULL)//���� �� ����� ����� � �� ������
                           {
                     			if(file_string[0]==' '&& isdigit((int)file_string[1]))
            						{
            							fgets(file_string, 30, stream);//��������� ������ ������ �����
            							if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                 {
                     					if(file_string[0]==' '&& isdigit((int)file_string[1]))
            								{
            									fgets(file_string, 30, stream);//��������� ������� ������ �����
            									if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                       {
                     							if(file_string[0]==' '&& isdigit((int)file_string[1]))
            										{
            											fgets(file_string, 30, stream);//��������� ������� ������ �����
            											if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                             {
                     									if(file_string[0]==' '&& isdigit((int)file_string[1]))
            												{
            													fgets(file_string, 30, stream);//��������� ������� ������ �����
            													if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                   {
                     											if(file_string[0]==' '&& isdigit((int)file_string[1]))
            														{
            															fgets(file_string, 30, stream);//��������� ������� ������ �����
            															if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                         {
                     													if(file_string[0]==' '&& isdigit((int)file_string[1]))
            																{
            																	fgets(file_string, 30, stream);//��������� ������������ ������ �����
            																	if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                               {
                     															if((file_string[0]==' '||file_string[0]=='-' || file_string[0]=='.' || isdigit((int)file_string[0])) && (file_string[1]=='.' || isdigit((int)file_string[1])))
            																		{
            																			fgets(file_string, 30, stream);//��������� ����������� ������ �����
            																			if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                                     {
                     																	if(file_string[0]==' '&& isdigit((int)file_string[1]))
            																				{
            																					fgets(file_string, 30, stream);//��������� ����������� ������ �����
            																					if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                                           {
                     																			if(file_string[0]==' '&& isdigit((int)file_string[1]))
            																						{
            																							fgets(file_string, 30, stream);//��������� ������������� ������ �����
            																							if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                                                 {
                     																					if(file_string[0]==' '&& isdigit((int)file_string[1]))
            																								{
            																									fgets(file_string, 30, stream);//��������� ����������� ������ �����
            																									if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                                                       {
                     																							if(file_string[0]==' '&& isdigit((int)file_string[1]))
            																										{
            																											fgets(file_string, 30, stream);//��������� ������������ ������ �����
            																											if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                                                             {
                     																									if(file_string[0]==' '&& file_string[1]=='0')
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

      }

      fclose(stream);


      done = findnext(&ffblk);
   }

   return Nf_001;
} // ����� Profile_Filename_array


int Profile_dataline_array(char * dataline[9], char *file_pathname)//��������� ������ dataline ����������� �� ������
																						 //�������, �� ��� ����� �������� ��������� file_pathname
                                                                   //dataline[0]-> ��� ����� �������
                                                                   //dataline[1]-> ���� ���������� ��������� ����� �������
                                                                   //dataline[2]-> �������� ������� �����
                                                                   //dataline[3]-> �������� ������� ������
                                                                   //dataline[4]-> ����� ���������, ��
                                                                   //dataline[5]-> ������ ������, ����., ���.
                                                                   //dataline[6]-> �������� ������, ����., ���.
                                                                   //dataline[7]-> ������� ����� �����
                                                                   //dataline[8]-> ������� ����� ������
                                                                   //���������� 0 - � ������ ����
                                                                   //			  1 - ���� �� ������� ������
{
   int Profile_handle,i,j=-1,k,l,m, Not_End,len_pathfilename,len_for_azimuth,len_back_azimuth;
   char simvol, str_buf[50],str_buf2[50],azimuty[100]="������ ������: ",stancia_A[50],stancia_B[50], R[30], Otm_l[30],
   		Otm_r[30];
   double zagolovok[6], *Profile;
   struct ftime ftimep;

	Profile_handle=open(file_pathname, O_RDONLY|O_BINARY);

      do
      {
      	i=0;
         do
         {
      		Not_End=read(Profile_handle, &simvol, 1);
            if(simvol!=0x0D)
            {
            	str_buf[i]=simvol;
               i++;
            }
            else
            	Not_End=read(Profile_handle, &simvol, 1);
         }while(simvol!=0x0A && Not_End!=0);
         if(Not_End!=0)
         {
         	str_buf[i]='\0';
            if(j==-1)
            {
            }
            else
            	if(j<=4 && j>=0)  // ������ ��������� �����
            		{
             			zagolovok[j]=atof(str_buf);
            		}
            		else
            			if(j==5)    // ���������� ������� Profile ���������� �����
               		{
                        Profile=malloc((18+zagolovok[1]*3+zagolovok[0]*4+zagolovok[4]*2)*sizeof(double));// allocate space for Profile
								if (Profile==NULL)
      							return 1;           // if allocation is insufficient return the error code
                  		for(k=0;k<=4;k++)
                  			Profile[k]=zagolovok[k];
                  		Profile[j]=atof(str_buf);
               		}
               		else
            				if(j<18+zagolovok[1]*3+zagolovok[0]*4+zagolovok[4]*2) // j < ������ ��������� ������ � ������� Profile +1
                  			Profile[j]=atof(str_buf);
                        else
            					if(j==18+zagolovok[1]*3+zagolovok[0]*4+zagolovok[4]*2)
                           	strcpy(stancia_A,str_buf);
                           else
            						if(j==19+zagolovok[1]*3+zagolovok[0]*4+zagolovok[4]*2)
                           		strcpy(stancia_B,str_buf);
                              else
            							if(j==20+zagolovok[1]*3+zagolovok[0]*4+zagolovok[4]*2)   //���������� ��������
                                 {
                                 	k=0;//������ ������� � ������ str_buf (������ �������� � ����� �������)
                                    m=0;//���������: 0 - ������� ������� �������
                                    	 //			  1 - ������ ������� �������
                                        //           2 - ������� ��������� �������
                                    	 //			  3 - ������ ��������� �������
                                    while(str_buf[k]!='\0')
                                    {
                                    	while(!(isdigit(str_buf[k]) || str_buf[k]=='.') && str_buf[k]!='\0')//����� ������� ����� � ������ ��������
                                       	k++;
                                    	l=0;// ������ ������� � ������ str_buf2 (������� ����� �� ������ ��������)
                                       while(isdigit(str_buf[k]) || str_buf[k]=='.')
                                       {
                                       	str_buf2[l]=str_buf[k];
                                          l++;
                                          k++;
                                       }
                                       if((m==0 || m==2) && l!=0)
                                       {
                                          str_buf2[l]='\0';
                                          strcat(str_buf2," ����. ");
                                       	strcat(azimuty,str_buf2);
                                          m++;
                                       }
                                       else
                                       	if(m==1 && l!=0)
                                          {
                                           	str_buf2[l]='\0';
                                          	strcat(str_buf2," ���.\n�������� ������: ");
                                       		strcat(azimuty,str_buf2);
                                          	m++;
                                       	}
                                          else
                                       		if(m==3 && l!=0)
                                          	{
                                           		str_buf2[l]='\0';
                                          		strcat(str_buf2," ���.");
                                       			strcat(azimuty,str_buf2);
                                          		m++;
                                       		}

                                    }
                                 }
            j++;
         }

      }while(Not_End!=0);

      len_pathfilename=strlen(file_pathname); //���������� �������� � ���� ����� �������
      k=len_pathfilename-1;
      while(file_pathname[k]!='\\')
      	k--;
      k++;
      dataline[0]=malloc(strlen(&file_pathname[k])+1);
      strcpy(dataline[0], &file_pathname[k]);//���������� ����� �����

      getftime(Profile_handle, &ftimep);//����������� ���� � ������� ���������� ��������� �����
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

      close(Profile_handle);

      OemToChar(stancia_A,stancia_A);
      dataline[2]=malloc(strlen(stancia_A)+1);
      strcpy(dataline[2],stancia_A);//���������� �������� ������� �����

      OemToChar(stancia_B,stancia_B);
      dataline[3]=malloc(strlen(stancia_B)+1);
      strcpy(dataline[3],stancia_B);//���������� �������� ������� ������

      gcvt(Profile[12+((int)Profile[1]-1)*3], 5, R);
      dataline[4]=malloc(strlen(R)+1);
      strcpy(dataline[4],R);//���������� ����� ���������

      len_for_azimuth=0;
      i=0;
      while(azimuty[i]!=':') i++;
      i++; //i - ������ ������� ������� � ������� azimuty
      while(azimuty[i+len_for_azimuth] !='\n' && azimuty[i+len_for_azimuth] !='\0') len_for_azimuth++; //len_for_azimuth - ���������� �������� � ������ �������
      dataline[5]=malloc(len_for_azimuth+1);
      memcpy(dataline[5], &azimuty[i], len_for_azimuth);
      dataline[5][len_for_azimuth]='\0';//���������� ������� �������

      dataline[6]=malloc(2);
      strcpy(dataline[6],"?");
      if(m>2)
      {
      	len_back_azimuth=0;
         while(azimuty[i]!=':') i++;
      	i++; //i - ������ ��������� ������� � ������� azimuty
      	while(azimuty[i+len_back_azimuth] !='\0') len_back_azimuth++; //len_back_azimuth - ���������� �������� � �������� �������
      	dataline[6]=malloc(len_back_azimuth+1);
      	memcpy(dataline[6], &azimuty[i], len_back_azimuth);
      	dataline[6][len_back_azimuth]='\0';//���������� ��������� �������
      }

      gcvt(Profile[13], 5, Otm_l);
      dataline[7]=malloc(strlen(Otm_l)+1);
      strcpy(dataline[7],Otm_l);//���������� ������� ����� �����

      gcvt(Profile[13+((int)Profile[1]-1)*3], 5, Otm_r);
      dataline[8]=malloc(strlen(Otm_r)+1);
      strcpy(dataline[8],Otm_r);//���������� ������� ����� ������


	return 0;

}// ����� Profile_dataline_array


TYPEOFPROC lpPrevWndFunc;

BOOL BrowseForProfile(char *file_pathname, HANDLE hInstance);
LONG APIENTRY MainWndProc( HWND hWnd, UINT message, UINT wParam, LONG lParam);
int CALLBACK BrowseCallbackProc(HWND  hwnd,UINT  uMsg,LPARAM  lParam,LPARAM  lpData);
LRESULT CALLBACK ProfileLVProc( HWND hWnd, UINT message, UINT wParam, LONG lParam);

HINSTANCE g_hInst;		// the current instance (������ ���� ����� ���������� � ���������� ���������)
// Global pointer to the shell's IMalloc interface.
LPMALLOC g_pMalloc;
BOOL Profile_Choosed; //TRUE - ���� ������� ������, FALSE - ���� �� ������.
char Profile_PathFile_Name[_MAX_PATH];//���������� ���� � ��� ���������� ����� �������
static char szPath[_MAX_PATH];

/* ��������� ��� ��������� �������*/

#pragma argsused
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow )
{

    OSVERSIONINFO osVer; // for GetVersionEx()
    char strprof[_MAX_PATH];
    BOOL choosed;
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
    choosed=BrowseForProfile(strprof ,hInstance);
	 if (!choosed)
		  return (FALSE);
	return (TRUE);
}//����� WinMain



#pragma argsused
BOOL BrowseForProfile(char *file_pathname, HANDLE hInstance)//��������� ������ �� ������� ��������� file_pathname �����
																				//� ���������� ����� �������, ���������� TRUE. ���� ������� ��
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
    if (pidlBrowse != NULL)
    {
      if(Profile_Choosed)
      {
         strcpy(file_pathname,Profile_PathFile_Name);

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

}//����� BrowseForProfile



#define N_Columns 9 //���-�� ������� � ListView
#pragma argsused
int CALLBACK BrowseCallbackProc(HWND hwnd,UINT message,LPARAM lParam,LPARAM lpData)
{
 static HWND hTable/*,hTable_header*/;
 char *files[500], *dataline[9], selected_filename[100];
 static char process_dir_001_dmr_file[_MAX_PATH];
 RECT  Rect, ClientRect;
 static int HIndex[N_Columns];
 int N_profiles_in_folder,i,j;
 FILE *cur_dir_file;
 LV_COLUMN col[]={{LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH,LVCFMT_LEFT,90,"���� �������",13,0},
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
                                 (LONG)ProfileLVProc 	// new value
                                 );
            for(j=0;j<N_Columns;j++)
            {
            	HIndex[j]=ListView_InsertColumn(hTable, j,(const LV_COLUMN FAR*) &(col[j]));
            }

            if(GetCurrentDirectory(_MAX_PATH,	// size, in characters, of directory buffer
    													process_dir_001_dmr_file 	// address of buffer for current directory
   													))//��������� ������� ���������� ������ ��������
            {
            	if(access(strcat(process_dir_001_dmr_file,"\\cur_001_dmr_dir"), 0)==0)//���� ���� cur_001_dmr_dir ����������
   	         {
      	      	cur_dir_file=fopen(process_dir_001_dmr_file,"rt");
                  fread(Profile_PathFile_Name,_MAX_PATH+1,1,cur_dir_file);
                  fclose(cur_dir_file);
                  if(access(Profile_PathFile_Name, 0)==0)//���� ����������, ���������� � ����� cur_001_dmr_dir ����������
         	   		SendMessage(hwnd, BFFM_SETSELECTION , TRUE, (LPARAM)Profile_PathFile_Name);//������� ��
            	}
            }


               // Make the window visible; update its client area; and return "success".
    				ShowWindow(hTable, SW_SHOWNA);
    				UpdateWindow(hTable);


        break;

        case	BFFM_SELCHANGED:
        		Profile_Choosed=FALSE;
        		ListView_DeleteAllItems(hTable);
            SHGetPathFromIDList((LPITEMIDLIST)lParam,szPath);
            SendMessage(hwnd, BFFM_SETSTATUSTEXT, NULL, (LPARAM)szPath);
            N_profiles_in_folder=Profile_Filename_array(files, szPath);  //���������� ���-�� ������ ��������, ������������� �� ���� path,
																	 //� ��������� ������ ����� � ������� ������ ��������, �� �������
                                                    //��������� files.
            if(N_profiles_in_folder>0)
            {
            	for(i=0;i<N_profiles_in_folder;i++)
               {
                  strcpy(Profile_PathFile_Name,szPath);
                  strcat(Profile_PathFile_Name,"\\");
                  strcat(Profile_PathFile_Name,files[i]);
                  Profile_dataline_array(dataline, Profile_PathFile_Name);
                  item.iItem=i;
               	item.iSubItem=HIndex[0];
                  item.pszText=files[i];
                  SendMessage(hTable, LVM_INSERTITEM, NULL, (LPARAM)(const LV_ITEM FAR*) &item);
                  for(j=1;j<N_Columns;j++)
                  {
                  	ListView_SetItemText(hTable, i,HIndex[j], dataline[j]);
                  }
               }
//               ListView_SetItemState(hTable, 0,LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

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
         		strcpy(Profile_PathFile_Name,szPath);
         		strcat(Profile_PathFile_Name,"\\");
         		strcat(Profile_PathFile_Name,selected_filename);
               Profile_Choosed=TRUE;

               cur_dir_file=fopen(process_dir_001_dmr_file,"wt");
               fwrite(szPath,strlen(szPath)+1,1,cur_dir_file);
               fclose(cur_dir_file);
            }
            else
            	SendMessage(hwnd, BFFM_ENABLEOK , NULL, NULL);
    }
    return (NULL);

}//����� BrowseCallbackProc



LRESULT CALLBACK ProfileLVProc( HWND hWnd, UINT message, UINT wParam, LONG lParam)
{
	int i,N,j, i_to_select, i_selected;
   char selected_filename[100];
//   LRESULT ret;
   RECT FAR rc;
   LV_ITEM item={LVIF_STATE,0,0,0,0,NULL,NULL,NULL,NULL};

	switch (message)
   {
   		case WM_LBUTTONDOWN:// WM_MOUSEACTIVATE:// WM_LBUTTONUP
         case WM_LBUTTONDBLCLK:
         case WM_RBUTTONDOWN:
         case WM_RBUTTONDBLCLK:

         SetFocus(hWnd );
/*         ret=CallWindowProc(lpPrevWndFunc,	// address of previous procedure
                     hWnd,	// handle of window
                     message,	// message
                     wParam,	// first message parameter
                     lParam 	// second message parameter
                     );
*/
         N=ListView_GetItemCount(hWnd);
         Profile_Choosed=FALSE;
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

//            ListView_SetItemState(hWnd, i,LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
            ListView_GetItemText(hWnd, i,0, selected_filename, 100);
         	strcpy(Profile_PathFile_Name,szPath);
         	strcat(Profile_PathFile_Name,"\\");
         	strcat(Profile_PathFile_Name,selected_filename);
         	Profile_Choosed=TRUE;
         }
         return (/*ret*/0);

   }

   return (CallWindowProc(lpPrevWndFunc,	// address of previous procedure
                     hWnd,	// handle of window
                     message,	// message
                     wParam,	// first message parameter
                     lParam 	// second message parameter
                     ));
}//����� ProfileLVProc
