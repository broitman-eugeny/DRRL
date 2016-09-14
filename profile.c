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
#include <alloc.h>



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
   	fgets(file_string, 30, stream);//��������� ������ ������ ����� (��������� "�-�" ACh 2Dh E5h)

      if (file_string!=NULL)//���� �� ����� ����� � �� ������
      {
      	fgets(file_string, 30, stream);//��������� ������ ������ ����� (���������� ����������� �� �������)
         if (file_string!=NULL)//���� �� ����� ����� � �� ������
         {
         	if(file_string[0]==' ' && isdigit((int)file_string[1]))//������ ���������� � ������� � ������ ������ �����?
            {
            	fgets(file_string, 30, stream);//��������� ������ ������ ����� (���������� �������� ������� ������� �� �������)
            	if (file_string!=NULL)//���� �� ����� ����� � �� ������
            	{
               	if(file_string[0]==' ' && isdigit((int)file_string[1]))//������ ���������� � ������� � ������ ������ �����?
            		{
            			fgets(file_string, 30, stream);//��������� ��������� ������ ����� (���������������)
            			if (file_string!=NULL)//���� �� ����� ����� � �� ������
                     {
                     	if(file_string[0]==' ' || file_string[0]=='.' || isdigit((int)file_string[0]))//������ ����� ���������� � ������� ��� � ���������� ����� ��� � �����
            				{
            					fgets(file_string, 30, stream);//��������� ����� ������ ����� (������� ���������, �)
            					if (file_string!=NULL)//���� �� ����� ����� � �� ������
                           {
                     			if((file_string[0]==' ' || file_string[0]=='-')&& isdigit((int)file_string[1]))//������ ���������� � ������� ��� ����� "-" � ������ ������ �����?
            						{
            							fgets(file_string, 30, stream);//��������� ������ ������ ����� (���������� ������ ��������)
            							if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                 {
                     					if(file_string[0]==' '&& isdigit((int)file_string[1]))//������ ���������� � ������� � ������ ������ �����?
            								{
            									fgets(file_string, 30, stream);//��������� ������� ������ ����� (������ �������� ������� �����, �)
            									if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                       {
                     							if(file_string[0]==' '&& isdigit((int)file_string[1]))//������ ���������� � ������� � ������ ������ �����?
            										{
            											fgets(file_string, 30, stream);//��������� ������� ������ ����� (������ ����������� ������� �����, �)
            											if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                             {
                     									if(file_string[0]==' '&& isdigit((int)file_string[1]))//������ ���������� � ������� � ������ ������ �����?
            												{
            													fgets(file_string, 30, stream);//��������� ������� ������ ����� (������ �������� ������� ������, �)
            													if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                   {
                     											if(file_string[0]==' '&& isdigit((int)file_string[1]))//������ ���������� � ������� � ������ ������ �����?
            														{
            															fgets(file_string, 30, stream);//��������� ������� ������ ����� (������ ����������� ������� ������, �)
            															if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                         {
                     													if(file_string[0]==' '&& isdigit((int)file_string[1]))//������ ���������� � ������� � ������ ������ �����?
            																{
            																	fgets(file_string, 30, stream);//��������� ������������ ������ ����� (������� ����� ����������: "0" - 1:1; "-1" - 1:100 000; "-.5" - 1:50 000; �.�. �=1:n*100 000, ��� n - ����� � ������������ ������)
            																	if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                               {
                     															if((file_string[0]==' '||file_string[0]=='-' || file_string[0]=='.' || isdigit((int)file_string[0])) && (file_string[1]=='.' || isdigit((int)file_string[1])))//������ ���������� � ������� ��� �� ����� ����� ��� ���������� ����� ��� ����� � ������ ������ �����?
            																		{
            																			fgets(file_string, 30, stream);//��������� ����������� ������ ����� (���������������, ���������� ���������� �������� ������)
            																			if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                                     {
                     																	if(file_string[0]==' '&& isdigit((int)file_string[1]))//������ ���������� � ������� � ������ ������ �����?
            																				{
            																					fgets(file_string, 30, stream);//��������� ����������� ������ ����� (���������������, ���������� ���������� �������� ������)
            																					if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                                           {
                     																			if(file_string[0]==' '&& isdigit((int)file_string[1]))//������ ���������� � ������� � ������ ������ �����?
            																						{
            																							fgets(file_string, 30, stream);//��������� ������������� ������ ����� (���������� �� ������ ����� ������� �� ������� - 0 ��)
            																							if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                                                 {
                     																					if(file_string[0]==' '&& isdigit((int)file_string[1]))//������ ���������� � ������� � ������ ������ �����?
            																								{
            																									fgets(file_string, 30, stream);//��������� ����������� ������ ����� (�������� ������� ������ ����� �������, �)
            																									if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                                                       {
                     																							if(file_string[0]==' '&& isdigit((int)file_string[1]))//������ ���������� � ������� � ������ ������ �����?
            																										{
            																											fgets(file_string, 30, stream);//��������� ������������ ������ ����� (����������� ����� ������� ������� - "0")
            																											if (file_string!=NULL)//���� �� ����� ����� � �� ������
                                                                                             {
                     																									if(file_string[0]==' '&& file_string[1]=='0')//������ ���������� � ������� � ������ ������ �����?
            																												{
                                                                                                   files[Nf_001]=malloc((size_t)(strlen(ffblk.ff_name)+1));//�������������� ������ ��� ����� ����� �������
                                                                                                   strcpy(files[Nf_001], ffblk.ff_name);//���������� ����� ����� ������� � ������� files
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
                                                                   //���������� 0 - � ������ �����
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
LPTSTR BrowseForDir(HWND hwnd, LPTSTR zagolovok);//���������� ��������� �� ������ �� ������� SHBrowseForFolder
																				//��������� ����� � �������.
                                                            //���� ����� ��
                                                            //������� - ���������� NULL.
                                                            //hwnd - ����������� ���� ��� ������� SHBrowseForFolder
                                                            //�.�. NULL.
                                                            //zagolovok - ��������� ������� SHBrowseForFolder
char ** Read_Profile(FILE *stream);			// ������ ���� ������� �� ������� ��������� stream, �������� � text mode (t).
														// ���������� ��������� �� ������ ����� - ����� ����� ������� (������ ��� ������� ������������� ����� ��).
char ** Reverse_Profile(char ** Profile_source);			// �������������� ������� Profile_source �� �����������.
																		// ���������� ��������� �� ������ ����� - ����� ����� ������������� ������� (������ ��� ������� ������������� ����� ��).
int Write_Profile(FILE *stream, char **Profile);// ���������� ������ ����� ������� Profile � ����, �� ������� ��������� stream, �������� � text mode (t).
															  // ��� ������ ������ � ���� ���������� EOF

HINSTANCE g_hInst;		// the current instance (������ ���� ����� ���������� � ���������� ���������)
// Global pointer to the shell's IMalloc interface.
LPMALLOC g_pMalloc;
BOOL Profile_Choosed; //TRUE - ���� ������� ������ � DRRL, FALSE - ���� �� ������ � DRRL.
char Profile_PathFile_Name[_MAX_PATH];//���������� ���� � ��� ���������� ����� ������� � DRRL
char szPath[_MAX_PATH];
BOOL bPathExist=FALSE, bSHBFFInitialised=FALSE;

/* ��������� ��� ��������� �������*/
/*
#pragma argsused
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow )
{

    OSVERSIONINFO osVer; // for GetVersionEx()
    char strprof[_MAX_PATH], **Profile, **Profile_rev, filter[_MAX_PATH], file_pathname[_MAX_PATH],file_name[_MAX_PATH],
    		process_dir_001_dmr_file[_MAX_PATH], DMR_PathFile_Name[_MAX_PATH];
    BOOL choosed;
    FILE *stream, *cur_dir_file;
    OPENFILENAME  ofn;
    DWORD oshibka;

     // Verify presence of Windows NT
     osVer.dwOSVersionInfoSize = sizeof(osVer);
     if (!GetVersionEx(&osVer))
          return (FALSE);

     if (osVer.dwPlatformId != VER_PLATFORM_WIN32_NT)
     {
     MessageBox(NULL, "This example requires Windows NT",
                    "Profile Explorer", MB_OK );
     return (FALSE);
     }

	 // Create the main window.
    choosed=BrowseForProfile(strprof ,hInstance);

    if (!choosed)
		  return (FALSE);

    stream=fopen(strprof, "rt");

    Profile=Read_Profile(stream);

    Profile_rev=Reverse_Profile(Profile);

    fclose(stream);

    //����������� ������� ���������� ����� �������---------------------------------------------------------------------------
                                                                                															//
    strcpy(filter, "�������, *.001 *.001 ��� �����, *.* *.* ");         		  															//
    filter[14]=0;                                                               															//
    filter[20]=0;                                                               															//
    filter[35]=0;                                                               															//
    filter[39]=0;                                                               															//
                                                                                															//
    strcpy(file_pathname,Profile_rev[19+atoi(Profile_rev[2])*3+atoi(Profile_rev[1])*4+atoi(Profile_rev[5])*2]);   			//
    file_pathname[strlen(file_pathname)-1]=0;
    strcat(file_pathname," - ");                                                                                           //
    strcat(file_pathname,Profile_rev[20+atoi(Profile_rev[2])*3+atoi(Profile_rev[1])*4+atoi(Profile_rev[5])*2]);            //
    file_pathname[strlen(file_pathname)-1]=0;
    OemToChar(file_pathname,file_pathname);       																								   //
                                                                                                                           //
    file_name[0]=0;                                                                                                        //
                                                                                                                           //
    if(GetCurrentDirectory(_MAX_PATH,	// size, in characters, of directory buffer                                         //
    													process_dir_001_dmr_file 	// address of buffer for current directory            //
   													))//��������� ������� ���������� ������ ��������                                 //
      {                                                                                                                    //
      	if(access(strcat(process_dir_001_dmr_file,"\\cur_001_dmr_dir"), 0)==0)//���� ���� cur_001_dmr_dir ����������      //
         {                                                                                                                 //
         	cur_dir_file=fopen(process_dir_001_dmr_file,"rt");                                                             //
            fread(DMR_PathFile_Name,_MAX_PATH+1,1,cur_dir_file);                                                           //
            fclose(cur_dir_file);                                                                                          //
            if(access(DMR_PathFile_Name, 0)==0)//���� ����������, ���������� � ����� cur_001_dmr_dir ����������            //
            	ofn.lpstrInitialDir=DMR_PathFile_Name;//������� ��                                                          //
            else                                                                                                           //
            	ofn.lpstrInitialDir=NULL;                                                                                   //
         }                                                                                                                 //
      }				       																															      //
                                                                                															//
    ofn.lStructSize=sizeof(OPENFILENAME);                                       															//
    ofn.hwndOwner=NULL;                                                         															//
    ofn.lpstrFilter=filter;                                                     															//
    ofn.lpstrCustomFilter=NULL;                                                 															//
    ofn.nFilterIndex=0;                                                         															//
    ofn.lpstrFile=file_pathname;                                                															//
    ofn.nMaxFile=_MAX_PATH;                                                           										 			//
    ofn.lpstrFileTitle=file_name;                                               															//
    ofn.nMaxFileTitle=_MAX_PATH;                                                      											 		//
    ofn.lpstrTitle="���������� ����� ������������� �������";                      	 													//
    ofn.Flags=OFN_EXPLORER|OFN_EXTENSIONDIFFERENT|OFN_OVERWRITEPROMPT;          															//
    ofn.lpstrDefExt="001";                                                      															//
                                                                                															//
    if(GetSaveFileName(&ofn)==FALSE)                                            															//
    {                                                                           															//
    	oshibka=CommDlgExtendedError();
		return (FALSE);                                                             														//
    }                                                                           													 		//
                                                                                												  			//
    //-----------------------------------------------------------------------------------------------------------------------


    stream=fopen(file_pathname, "wt");

    Write_Profile(stream, Profile_rev);

    fclose(stream);


	return (TRUE);
}//����� WinMain
*/


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
    bPathExist=FALSE;
    bSHBFFInitialised=FALSE;
    szPath[0]=0;
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
 LPTSTR lpProcessCmdLine;
 RECT  Rect, ClientRect;
 static int HIndex[N_Columns];
 int N_profiles_in_folder,i,j,k;
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
                  fread(Profile_PathFile_Name,_MAX_PATH+1,1,cur_dir_file);
                  fclose(cur_dir_file);
                  if(access(Profile_PathFile_Name, 0)==0)//���� ����������, ���������� � ����� cur_001_dmr_dir ����������
                  {
                  	lstrcpy(szPath,Profile_PathFile_Name);
							SendMessage(hwnd, BFFM_SETSELECTION , TRUE, (LPARAM)Profile_PathFile_Name);//������� ��
                  }
            	}
            }

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



LRESULT CALLBACK ProfileLVProc( HWND hWnd, UINT message, UINT wParam, LONG lParam)
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

            ListView_GetItemText(hWnd, i_to_select,0, selected_filename, 100);
         	strcpy(Profile_PathFile_Name,szPath);
         	strcat(Profile_PathFile_Name,"\\");
         	strcat(Profile_PathFile_Name,selected_filename);
         	Profile_Choosed=TRUE;
         }
         return (0);

   }

   return (CallWindowProc(lpPrevWndFunc,	// address of previous procedure
                     hWnd,	// handle of window
                     message,	// message
                     wParam,	// first message parameter
                     lParam 	// second message parameter
                     ));
}//����� ProfileLVProc


char ** Read_Profile(FILE *stream)			// ������ ���� ������� �� ������� ��������� stream, �������� � text mode (t).
														// ���������� ��������� �� ������ ����� - ����� ����� ������� (������ ��� ������� ������������� ����� ��).

{
    	char buf[6][128], **Profile;
      int i, number_of_strings;

    	fseek(stream, 0, SEEK_SET); //����������� ������� ���������� � ������ �����

      /* ������ ������ 6-�� ����� ����� � ����� buf
         [0] - ��������� "�-�"
         [1] - ���������� ����������� �� �������
         [2] - ���������� �������� ������� ������� �� �������
         [3] - ���������������
         [4] - ������� ���������
         [5] - ���������� ������ �������� */
      for(i=0; i<=5; i++)
      {
   		fgets(buf[i], 127, stream);

      }

      number_of_strings=23+3*atoi(buf[2])+4*atoi(buf[1])+2*atoi(buf[5]);//���������� ��������� �����  � ����� �������

      Profile= (char **) farcalloc(number_of_strings, sizeof(char*));//�������������� ������ ��� number_of_strings ���������� �� ������

      fseek(stream, 0, SEEK_SET); //����������� ������� ���������� � ������ �����

      for(i=0; i<number_of_strings; i++)
      {
      	fgets(buf[0], 127, stream);//���������� ����� ������ � �����
         Profile[i]= (char *) farcalloc(strlen(buf[0])+1, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � i-� ������ ������� Profile
         strcpy(Profile[i],buf[0]);
      }

      return (Profile);


}// konec Read_Profile

char ** Reverse_Profile(char ** Profile_source)			// �������������� ������� Profile_source �� �����������.
																		// ���������� ��������� �� ������ ����� - ����� ����� ������������� ������� (������ ��� ������� ������������� ����� ��).

{
	char **Profile_reverse, *Pryam_grad, *Pryam_min, *Obr_grad, *Obr_min, buf[256];
   int number_of_strings, i;
   double R/*������������� ���������*/, R1;

   R=atof(Profile_source[10+(atoi(Profile_source[2]))*3]);//������������� ���������

   number_of_strings=23+3*atoi(Profile_source[2])+4*atoi(Profile_source[1])+2*atoi(Profile_source[5]);//���������� ��������� �����  � �������

   Profile_reverse= (char **) farcalloc(number_of_strings, sizeof(char*));//�������������� ������ ��� number_of_strings ���������� �� ������

   for(i=0; i<=5; i++)//���������� ������ ����� ����� ������� ������������� �������
   {
      	Profile_reverse[i]= (char *) farcalloc(strlen(Profile_source[i])+1, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � i-� ������ ������� Profile_reverse
         strcpy(Profile_reverse[i],Profile_source[i]);
   }

   //������ ����� ������
   //�������� �����
   Profile_reverse[6]= (char *) farcalloc(strlen(Profile_source[8])+1, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � 6-� ������ ������� Profile_reverse
   strcpy(Profile_reverse[6],Profile_source[8]);
   //����������� �����
   Profile_reverse[7]= (char *) farcalloc(strlen(Profile_source[9])+1, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � 7-� ������ ������� Profile_reverse
   strcpy(Profile_reverse[7],Profile_source[9]);
   //�������� ������
   Profile_reverse[8]= (char *) farcalloc(strlen(Profile_source[6])+1, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � 8-� ������ ������� Profile_reverse
   strcpy(Profile_reverse[8],Profile_source[6]);
   //����������� ������
   Profile_reverse[9]= (char *) farcalloc(strlen(Profile_source[7])+1, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � 9-� ������ ������� Profile_reverse
   strcpy(Profile_reverse[9],Profile_source[7]);

   for(i=10; i<=12; i++)//���������� 10-12 ����� ������� ������������� �������
   {
      	Profile_reverse[i]= (char *) farcalloc(strlen(Profile_source[i])+1, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � i-� ������ ������� Profile_reverse
         strcpy(Profile_reverse[i],Profile_source[i]);
   }

   //���������� ���������� � �������� ������� ������� � ������������ �������
   for(i=0; i<atoi(Profile_source[2]); i++)//i - ���������� ����� ������� ������� ������� � "0"
   {
      	R1=atof(Profile_source[10+(atoi(Profile_source[2])-i)*3]);//���������� �� ����� ������� �� ������ ���������
         gcvt(R-R1,strlen(Profile_source[10+(atoi(Profile_source[2])-i)*3]),buf);//������ ���������� �� ����� ������� �� ����� ��������� � �����
         Profile_reverse[13+i*3]= (char *) farcalloc(strlen(buf)+4, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � (13+i*3)-� ������ ������� Profile_reverse (���������� �� ������� �������)
		 	strcat(Profile_reverse[13+i*3]," ");
         strcat(Profile_reverse[13+i*3],buf);
         strcat(Profile_reverse[13+i*3]," \n");
         Profile_reverse[14+i*3]= (char *) farcalloc(strlen(Profile_source[11+(atoi(Profile_source[2])-i)*3])+1, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � (14+i*3)-� ������ ������� Profile_reverse (������ ������� �������)
         strcpy(Profile_reverse[14+i*3],Profile_source[11+(atoi(Profile_source[2])-i)*3]);
         Profile_reverse[15+i*3]= (char *) farcalloc(strlen(Profile_source[12+(atoi(Profile_source[2])-i)*3])+1, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � (15+i*3)-� ������ ������� Profile_reverse (����������� ����� ��������� �������)
         strcpy(Profile_reverse[15+i*3],Profile_source[12+(atoi(Profile_source[2])-i)*3]);
   }

   //��������� ������������ ����� ������� ������� � �������������
   for(i=0; i<4; i++)//4 �����������
   {
			Profile_reverse[13+atoi(Profile_source[2])*3+i]= (char *) farcalloc(strlen(Profile_source[13+atoi(Profile_source[2])*3+i])+1, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � (13+atoi(Profile_source[2])*3+i)-� ������ ������� Profile_reverse (�������������� ������)
         strcpy(Profile_reverse[13+atoi(Profile_source[2])*3+i],Profile_source[13+atoi(Profile_source[2])*3+i]);
   }

   //���������� ����������� � ������������ �������
   for(i=0; i<atoi(Profile_source[1]); i++)//i - ���������� ����� ����������� ������� � "0"
   {
      	R1=atof(Profile_source[14+(atoi(Profile_source[2]))*3+(atoi(Profile_source[1])-i)*4]);
         gcvt(R-R1,strlen(Profile_source[14+(atoi(Profile_source[2]))*3+(atoi(Profile_source[1])-i)*4]),buf);//������ ���������� �� ������ i-�� ����������� � �����
         Profile_reverse[17+atoi(Profile_source[2])*3+i*4]= (char *) farcalloc(strlen(buf)+4, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � (17+atoi(Profile_source[2])*3+i*4)-� ������ ������� Profile_reverse (���������� �� ������ i-�� �����������)
         strcat(Profile_reverse[17+atoi(Profile_source[2])*3+i*4]," ");
         strcat(Profile_reverse[17+atoi(Profile_source[2])*3+i*4],buf);
         strcat(Profile_reverse[17+atoi(Profile_source[2])*3+i*4]," \n");
         R1=atof(Profile_source[13+(atoi(Profile_source[2]))*3+(atoi(Profile_source[1])-i)*4]);
         gcvt(R-R1,strlen(Profile_source[13+(atoi(Profile_source[2]))*3+(atoi(Profile_source[1])-i)*4]),buf);//������ ���������� �� ����� i-�� ����������� � �����
         Profile_reverse[18+atoi(Profile_source[2])*3+i*4]= (char *) farcalloc(strlen(buf)+4, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � (18+atoi(Profile_source[2])*3+i*4)-� ������ ������� Profile_reverse (���������� �� ����� i-�� �����������)
         strcat(Profile_reverse[18+atoi(Profile_source[2])*3+i*4]," ");
         strcat(Profile_reverse[18+atoi(Profile_source[2])*3+i*4],buf);
         strcat(Profile_reverse[18+atoi(Profile_source[2])*3+i*4]," \n");
         Profile_reverse[19+atoi(Profile_source[2])*3+i*4]= (char *) farcalloc(strlen(Profile_source[15+(atoi(Profile_source[2]))*3+(atoi(Profile_source[1])-i)*4])+1, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � (19+atoi(Profile_source[2])*3+i*4)-� ������ ������� Profile_reverse (������ i-�� �����������)
         strcpy(Profile_reverse[19+atoi(Profile_source[2])*3+i*4],Profile_source[15+(atoi(Profile_source[2]))*3+(atoi(Profile_source[1])-i)*4]);//������ ������ i-�� �����������
         Profile_reverse[20+atoi(Profile_source[2])*3+i*4]= (char *) farcalloc(strlen(Profile_source[16+(atoi(Profile_source[2]))*3+(atoi(Profile_source[1])-i)*4])+1, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � (20+atoi(Profile_source[2])*3+i*4)-� ������ ������� Profile_reverse (��� i-�� �����������)
         strcpy(Profile_reverse[20+atoi(Profile_source[2])*3+i*4],Profile_source[16+(atoi(Profile_source[2]))*3+(atoi(Profile_source[1])-i)*4]);//������ ���� i-�� �����������
   }

   //��������� ������������ ����� ������������� � ������� ���������
   for(i=0; i<2; i++)//2 �����������
   {
			Profile_reverse[17+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+i]= (char *) farcalloc(strlen(Profile_source[17+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+i])+1, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � (17+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+i)-� ������ ������� Profile_reverse (�������������� ������)
         strcpy(Profile_reverse[17+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+i],Profile_source[17+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+i]);
   }

   //���������� ������ �������� � ������������ �������
   for(i=0; i<atoi(Profile_source[5]); i++)//i - ���������� ����� ������� ������� ������� � "0"
   {
      	R1=atof(Profile_source[18+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+(atoi(Profile_source[5])-i)*2]);
         gcvt(R-R1,strlen(Profile_source[18+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+(atoi(Profile_source[5])-i)*2]), buf);//������ ���������� �� ������ i-�� ������� ������� � �����
         Profile_reverse[19+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+i*2]= (char *) farcalloc(strlen(buf)+4, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � (19+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+i*2)-� ������ ������� Profile_reverse (���������� �� ������ i-�� ������� �������)
         strcat(Profile_reverse[19+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+i*2]," ");
         strcat(Profile_reverse[19+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+i*2],buf);
         strcat(Profile_reverse[19+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+i*2]," \n");
         R1=atof(Profile_source[17+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+(atoi(Profile_source[5])-i)*2]);
         gcvt(R-R1,strlen(Profile_source[17+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+(atoi(Profile_source[5])-i)*2]), buf);//������ ���������� �� ����� i-�� ������� ������� � �����
         Profile_reverse[20+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+i*2]= (char *) farcalloc(strlen(buf)+4, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � (19+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+i*2)-� ������ ������� Profile_reverse (���������� �� ����� i-�� ������� �������)
         strcat(Profile_reverse[20+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+i*2]," ");
         strcat(Profile_reverse[20+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+i*2],buf);
         strcat(Profile_reverse[20+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+i*2]," \n");
   }

   //��������� �������� �������
   Profile_reverse[19+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2]= (char *) farcalloc(strlen(Profile_source[20+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2])+1, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � (19+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2)-� ������ ������� Profile_reverse (�������� ������� �����)
   strcpy(Profile_reverse[19+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2],Profile_source[20+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2]);
   Profile_reverse[20+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2]= (char *) farcalloc(strlen(Profile_source[19+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2])+1, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � (20+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2)-� ������ ������� Profile_reverse (�������� ������� ������)
   strcpy(Profile_reverse[20+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2],Profile_source[19+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2]);

   //���������� ��������
   strcpy(buf,Profile_source[21+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2]);
   Obr_grad=strtok(buf, " ");
   Obr_min=strtok(NULL, " ");
   Pryam_grad=strtok(NULL, " ");
   Pryam_min=strtok(NULL, " ");
   Obr_min[strlen(Obr_min)+1]=0;
   Obr_min[strlen(Obr_min)]='\n';
   Pryam_min[strlen(Pryam_min)-1]=0;
   Profile_reverse[21+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2]=(char *) farcalloc(strlen(Obr_grad)+strlen(Obr_min)+strlen(Pryam_grad)+strlen(Pryam_min)+7, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � (21+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2)-� ������ ������� Profile_reverse (�������)
   strcat(Profile_reverse[21+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2],Pryam_grad);
   strcat(Profile_reverse[21+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2]," ");
   strcat(Profile_reverse[21+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2],Pryam_min);
   strcat(Profile_reverse[21+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2],"    ");
   strcat(Profile_reverse[21+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2],Obr_grad);
   strcat(Profile_reverse[21+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2]," ");
   strcat(Profile_reverse[21+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2],Obr_min);

   //���������� ��������� ������
   Profile_reverse[22+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2]=(char *) farcalloc(strlen(Profile_source[22+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2])+1, sizeof(char));//�������������� ������ ��� �������� ����������� ������ � (22+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2)-� ������ ������� Profile_reverse (��������� ������)
   strcpy(Profile_reverse[22+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2],Profile_source[22+atoi(Profile_source[2])*3+atoi(Profile_source[1])*4+atoi(Profile_source[5])*2]);

   return (Profile_reverse);

}// konec Reverse_Profile

int Write_Profile(FILE *stream, char **Profile)// ���������� ������ ����� ������� Profile � ����, �� ������� ��������� stream, �������� � text mode (t).
															  // ��� ������ ������ � ���� ���������� EOF
{
    	int i, number_of_strings, flag;


      /* ����������� ���������� ����� � ������� �������
         [0] - ��������� "�-�"
         [1] - ���������� ����������� �� �������
         [2] - ���������� �������� ������� ������� �� �������
         [3] - ���������������
         [4] - ������� ���������
         [5] - ���������� ������ �������� */


      number_of_strings=23+3*atoi(Profile[2])+4*atoi(Profile[1])+2*atoi(Profile[5]);//���������� ��������� �����  � ����� �������

      fseek(stream, 0, SEEK_SET); //����������� ������� ���������� � ������ �����

      for(i=0; i<number_of_strings; i++)
      {
      	flag=fputs(Profile[i], stream);//������ ����� ������ � ����
         if (flag==EOF)
         	break;
      }

      return (flag);


}// konec Write_Profile

/*       ���� �������������. ����������� ����� �������������� ��� ���������� �������� �� ������ *.hgt

#define a_WGS84 6378137 		//����� ������� ������� ���������� ��� WGS84
#define b_WGS84 6356752.3142  //����� ����� ������� ���������� ��� WGS84

#define a_SK42 6378245 			//����� ������� ������� ���������� ��� ��42
#define b_SK42 6356863.0188	//����� ����� ������� ���������� ��� ��42

BOOL hgt3s2prof001(void)//������� ���� ������� � ������� *.001 �� 3-� ��������� SRTM ���� ������
							 //������� ���������(����� *.hgt). ���������� ������� - WGS84
                      //���������� TRUE ���� ���� ������� ������,
                      //           FALSE ���� �� ������
{
	int Names_and_koordinates_entered_flag,
   	 N_points,//���������� ����� �� �������
       N_DataFiles=0,//���������� ������ ������, ����������� ��� ���������� �������
       *IndexDataFile,//������ ��������, ��������������� ������� ������ ������, ���������� � ������� HGTFileNamesForLine
       i,//�������
       p_p_km=100;//���������� ����� ������������� ���������� �� 1 �� ������ (���=1/p_p_km)
   static TCHAR Names_koordinates[14][32],// 0 - �������� ������� �����
   										// 1 - ����. ������ ������� �����
                                 // 2 - ���. ������ ������� �����
                                 // 3 - � ������ ������� �����
                                 // 4 - ����. ������� ������� �����
                                 // 5 - ���. ������� ������� �����
                                 // 6 - � ������� ������� �����
                                 // 7 - �������� ������� ������
   										// 8 - ����. ������ ������� ������
                                 // 9 - ���. ������ ������� ������
                                 // 10 - � ������ ������� ������
                                 // 11 - ����. ������� ������� ������
                                 // 12 - ���. ������� ������� ������
                                 // 13 - � ������� ������� ������
   		 lat_long[4],//����. ������ �������� ������ � ������� ������� ������ (���-���������) ���� �
          			  	 //����� �����
          buf[130];
   		 HGTFileNamesForPoint[12]="N00E000.hgt";//������ � ������ ����� ���� ������� ��� ������� �����
   LPTSTR Path,//���� � ����� � ������� ������� *.hgt ���� SRTM3
          HGTFileNamesForLine[361][12];//������ ����� � ������� ����������� ������ ���� ������� ���
          							  	  //���� �����
   double l_lat_deg, l_lat_min, l_lat_s, l_long_deg, l_long_min, l_long_s,
          r_lat_deg, r_lat_min, r_lat_s, r_long_deg, r_long_min, r_long_s,//���������� ������� � ������� ���
          teta1, teta2, fi1, fi2,//���������� ������� � ��������
          a,b,//������� � ����� ������� ���������� �����
          L, f, U1,U2, sinalfa, cos2alfa, sinsigma, cossigma, cos2sigmam, lambda, lambda_, sigma, C,
   		 u2, A_, B, deltasigma,//���������� ��� ����������� ���������� � �������� ����� ���������
          d, A, Aobr,//���������� ����� ��������� (��), ������ � ���. ������� (����)
          *otmetki_rel[4],//0 - ��������� �� ������ ����� ��������� ����� �� �������, ����.
                          //1 - ��������� �� ������ ������ ��������� ����� �� �������, ����.
   							  //2 - ��������� �� ������ ���������� �� ������� ����� �� �����. ����� �� �������, ��
                          //3 - ��������� �� ������ ������� ����� �� �������
          sigma1,cosalfa,E;//���������� ��� ����������� ��������� ������������� ����� �� �������

//******������ ����� �������� ������� � ���������*****

	Names_and_koordinates_entered_flag=DialogBoxParam( //1 - ������ ������ "OK", ���������� �������
   																	//    (�������� � ������� Names_koordinates),
   																	//0 - ������ ������ "Cancel", ���������� �������� ��������
         	g_hInst,	// handle of application instance
    			"IDD_DIALOG2",	// identifies dialog box template name
    			GetForegroundWindow(),	// handle of owner window
    			NK_DialogFunc, 	// address of dialog box procedure
            (LPARAM)&(Names_koordinates[0][0])//��������� �� ������� ������� ������� �������� � ��������� �������
   			);

//****************************************************

	if (Names_and_koordinates_entered_flag==0)//���� ������ "Cancel"
   	return (FALSE);//���� ������� �� ������

   l_lat_deg=atol(Names_koordinates[1]);
   l_lat_min=atol(Names_koordinates[2]);
   l_lat_s=atol(Names_koordinates[3]);
   l_long_deg=atol(Names_koordinates[4]);
   l_long_min=atol(Names_koordinates[5]);
   l_long_s=atol(Names_koordinates[6]);
   r_lat_deg=atol(Names_koordinates[8]);
   r_lat_min=atol(Names_koordinates[9]);
   r_lat_s=atol(Names_koordinates[10]);
   r_long_deg=atol(Names_koordinates[11]);
   r_long_min=atol(Names_koordinates[12]);
   r_long_s=atol(Names_koordinates[13]);

//*********����������� ����� ��������� � ��������***********

   teta1=l_lat_deg+l_lat_min/60.+l_lat_s/3600.;
   fi1=l_long_deg+l_long_min/60.+l_long_s/3600.;
   teta2=r_lat_deg+r_lat_min/60.+r_lat_s/3600.;
   fi2=r_long_deg+r_long_min/60.+r_long_s/3600.;
   a=a_WGS84;//��� ��42 ��������� a_SK42 � b_SK42
   b=b_WGS84;
   L=(fi2-fi1)*M_PI/180.;
   f=(a-b)/a;
   U1=atan((1.-f)*tan(teta1*M_PI/180.));
   U2=atan((1.-f)*tan(teta2*M_PI/180.));
   lambda=L;
   lambda_=M_PI*2.;
   while(fabs(lambda-lambda_)>1.e-12)
   {
   	sinsigma=sqrt(cos(U2)*cos(U2)*sin(lambda)*sin(lambda)+(cos(U1)*sin(U2)-sin(U1)*cos(U2)*cos(lambda))*(cos(U1)*sin(U2)-sin(U1)*cos(U2)*cos(lambda)));
      cossigma=sin(U1)*sin(U2)+cos(U1)*cos(U2)*cos(lambda);
      sigma=atan2(sinsigma,cossigma);
      sinalfa=cos(U1)*cos(U2)*sin(lambda)/sinsigma;
      cos2alfa=1.-sinalfa*sinalfa;
      cos2sigmam=cossigma-2.*sin(U1)*sin(U2)/cos2alfa;
      C=f/16.*cos2alfa*(4.+f*(4.-3.*cos2alfa));
      lambda_=lambda;
      lambda=L+(1.-C)*f*sinalfa*(sigma+C*sinsigma*(cos2sigmam+C*cossigma*(-1.+2.*cos2sigmam*cos2sigmam)));
   }
   u2=cos2alfa*(a*a-b*b)/b/b;
   A_=1.+u2/16384.*(4096.+u2*(-768.+u2*(320.-175.*u2)));
   B=u2/1024.*(256.+u2*(-128.+u2*(74.-47.*u2)));
   deltasigma=B*sinsigma*(cos2sigmam+B/4.*(cossigma*(-1.+2*cos2sigmam)-B/6.*cos2sigmam*(-3.+4.*sinsigma*sinsigma)*(-3.+4.*cos2sigmam*cos2sigmam)));
   d=b*A_*(sigma-deltasigma)/1000.;//���������� ����� ��������� (��)
   A=atan2(cos(U2)*sin(lambda),cos(U1)*sin(U2)-sin(U1)*cos(U2)*cos(lambda))*180./M_PI;
   if(A<0.)
   	A=A+360.;//������ ������ (����)
   Aobr=atan2(cos(U1)*sin(lambda),-cos(U2)*sin(U1)+sin(U2)*cos(U1)*cos(lambda))*180./M_PI+180.;
   if(Aobr>=360.)
   	Aobr=Aobr-360.;//�������� ������ (����)

//**********************************************************

	Path=BrowseForDir(NULL, "�������� ����� � ������� ������� *.hgt ���� SRTM3");
   if (Path==0)//���� ����� � ����� �� �������
   	return (FALSE);//�����

   
//*����������� ������� ����������� ������ ������ ������� ��� ����� ������������ �������**
  //************************������ �������******************************************
  	if(teta1==(int)teta1 && A<=270. && A>=90.)//���� ����� ��������� �� ������������� ��������� (��
   														//�������������� ������� ����� ������) � ������ ����� ���� (�� ��)
   	sprintf(lat_long, "%03u", int(teta1-1));
   else sprintf(lat_long, "%03u", int(teta1));
      HGTFileNamesForPoint[2]=lat_long[2];//������� � ��� ����� ������� ������ ������
      HGTFileNamesForPoint[1]=lat_long[1];//������� � ��� ����� ������� ������ ������
   if(fi1==(int)fi1 && A>180. && A<360.)//���� ����� ��������� �� ������������� ��������� (��
   												 //������������ ������� ����� ������) � ������ ����� ����� (�� �����)
		sprintf(lat_long, "%03u", int(fi1-1));
   else sprintf(lat_long, "%03u", int(fi1));
      HGTFileNamesForPoint[6]=lat_long[2];//������� � ��� ����� ������� ������ �������
      HGTFileNamesForPoint[5]=lat_long[1];//������� � ��� ����� ������� ������ �������
      HGTFileNamesForPoint[4]=lat_long[0];//������� � ��� ����� ������� ������ �������

  	if(SearchPath(
    	Path,	// address of search path
    	HGTFileNamesForPoint,	// address of filename
    	NULL,	// address of extension
    	0,	// size, in characters, of buffer
    	NULL,	// address of buffer for found filename
    	NULL 	// address of pointer to file component
   	)==0)                    									//���� ��������� ���� �� ������
      {
         strcpy(buf,"��������� ���� ");
         strcat(buf,HGTFileNamesForPoint);
         strcat(buf," �� ������.\n\n��� ����� ������� �� ������: \nftp://e0srp01u.ecs.nasa.gov/srtm/version2/SRTM3.");
   		MessageBox(NULL,buf,"hgt3s2prof001(void)",MB_ICONSTOP);
         return(FALSE);//�����
      }

   strcpy(HGTFileNamesForLine[0],HGTFileNamesForPoint);//������ ���� ������ ������
   N_DataFiles++;//��������� �������� ������
  //********************************************************************************


	N_points=(((int)(d*(double)p_p_km)+1)==(d*(double)p_p_km+1.))?((int)(d*(double)p_p_km)+1):((int)(d*(double)p_p_km)+2);//���=1/p_p_km ��
                                                                                 //���� ���������� ������ 1/p_p_km ��
                                                                                 //���-�� ����� == �����. 1/p_p_km ��
                                                                                 //���������� + 1,
                                                                                 //���� �� ������, �� + 2
   otmetki_rel[0]=(double*)VirtualAlloc(NULL,N_points*sizeof(double),MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
   otmetki_rel[1]=(double*)VirtualAlloc(NULL,N_points*sizeof(double),MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
   otmetki_rel[2]=(double*)VirtualAlloc(NULL,N_points*sizeof(double),MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
   otmetki_rel[3]=(double*)VirtualAlloc(NULL,N_points*sizeof(double),MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
   IndexDataFile=(int*)VirtualAlloc(NULL,N_points*sizeof(int),MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);

   otmetki_rel[0][0]=teta1;//������ ������� �����, ����
   otmetki_rel[1][0]=fi1;//������� ������� �����, ����
   otmetki_rel[2][0]=0.;//����������
   IndexDataFile[0]=0;//������ ����� ������ � ������� HGTFileNamesForLine ��� ������� �����

  //************************������������� �����*************************************

   sigma1=atan2(tan(U1),cos(A*M_PI/180.));
   sinalfa=cos(U1)*sin(A*M_PI/180.);
   cosalfa=sqrt(1.-sinalfa*sinalfa);
   u2=cosalfa*cosalfa*(a*a-b*b)/b/b;
   A_=1.+u2/16384.*(4096.+u2*(-768.+u2*(320.-175.*u2)));
   B=u2/1024.*(256.+u2*(-128.+u2*(74.-47.*u2)));
   for(i=1;i<N_points-1;i++)
   {
      //*****����������� ��������� i-��� �����********************************************************
      otmetki_rel[2][i]=i*1./p_p_km;//���������� �� ������� ����� �� �������������� �����, ��
      E=otmetki_rel[2][i]*1000./b/A_;
      sigma=E;
      cos2sigmam=cos(2.*sigma1+sigma);
      deltasigma=B*sin(sigma)*(cos2sigmam+0.25*B*(cos(sigma)*(-1.+2.*cos2sigmam*cos2sigmam)-B/6.*cos2sigmam*(-3.+4.*sin(sigma)*sin(sigma))*(-3.+4.*cos2sigmam*cos2sigmam)));
      sigma_=sigma;
      sigma=E+deltasigma;
      while(fabs(sigma-sigma_)>0.000000000001)
      {
       	cos2sigmam=cos(2.*sigma1+sigma);
      	deltasigma=B*sin(sigma)*(cos2sigmam+0.25*B*(cos(sigma)*(-1.+2.*cos2sigmam*cos2sigmam)-B/6.*cos2sigmam*(-3.+4.*sin(sigma)*sin(sigma))*(-3.+4.*cos2sigmam*cos2sigmam)));
      	sigma_=sigma;
      	sigma=E+deltasigma;
      }

      //������ i-��� �����
      otmetki_rel[0][i]=atan2(sin(U1)*cos(sigma)+cos(U1)*sin(sigma)*cos(A*M_PI/180.),(1.-f)*sqrt(sinalfa*sinalfa+(sin(U1)*sin(sigma)-cos(U1)*cos(sigma)*cos(A*M_PI/180.))*(sin(U1)*sin(sigma)-cos(U1)*cos(sigma)*cos(A*M_PI/180.))))*180./M_PI;

      lambda=atan2(sin(sigma)*sin(A*M_PI/180.),cos(U1)*cos(sigma)-sin(U1)*sin(sigma)*cos(A*M_PI/180.));
      C=f/16.*cosalfa*cosalfa*(4.+f*(4.-3.*cosalfa*cosalfa));
      L=lambda-(1.-C)*f*sinalfa*(sigma+C*sin(sigma)*(cos2sigmam+C*cos(sigma)*(-1.+2.*cos2sigmam*cos2sigmam)));

		//������� i-��� �����
      otmetki_rel[1][i]=fi1+L*180./M_PI;
   }
//***************************************************************************************
   VirtualFree(otmetki_rel[0],0,MEM_RELEASE);
   VirtualFree(otmetki_rel[1],0,MEM_RELEASE);
   VirtualFree(otmetki_rel[2],0,MEM_RELEASE);
   VirtualFree(otmetki_rel[3],0,MEM_RELEASE);
   VirtualFree(IndexDataFile,0,MEM_RELEASE);

}//����� hgt2prof001



BOOL CALLBACK NK_DialogFunc(

    	HWND  hwndDlg,	// handle of dialog box
    	UINT  uMsg,	// message
    	WPARAM  wParam,	// first message parameter
    	LPARAM  lParam 	// second message parameter
   	)
    {
    	static char (*buf)[14][32];

    	switch (uMsg)
      {

        case WM_INITDIALOG:
        //����������� ���� ������������ � ��������� ��������
			buf=(char*)lparam;

         SetDlgItemText(
    			hwndDlg,	// handle of dialog box
    			104,	// identifier of control ������������ ������� �����
    			&(buf[0][0]) 	// text to set
   			);

         SetDlgItemText(
    			hwndDlg,	// handle of dialog box
    			102,	// identifier of control ����. ������ ������� �����
    			&(buf[1][0]) 	// text to set
   			);

         SetDlgItemText(
    			hwndDlg,	// handle of dialog box
    			107,	// identifier of control ���. ������ ������� �����
    			&(buf[2][0]) 	// text to set
   			);

         SetDlgItemText(
    			hwndDlg,	// handle of dialog box
    			110,	// identifier of control � ������ ������� �����
    			&(buf[3][0]) 	// text to set
   			);

         SetDlgItemText(
    			hwndDlg,	// handle of dialog box
    			112,	// identifier of control ����. ������� ������� �����
    			&(buf[4][0]) 	// text to set
   			);

         SetDlgItemText(
    			hwndDlg,	// handle of dialog box
    			114,	// identifier of control ���. ������� ������� �����
    			&(buf[5][0]) 	// text to set
   			);

         SetDlgItemText(
    			hwndDlg,	// handle of dialog box
    			116,	// identifier of control � ������� ������� �����
    			&(buf[6][0]) 	// text to set
   			);

         SetDlgItemText(
    			hwndDlg,	// handle of dialog box
    			122,	// identifier of control ������������ ������� ������
    			&(buf[7][0]) 	// text to set
   			);

         SetDlgItemText(
    			hwndDlg,	// handle of dialog box
    			120,	// identifier of control ����. ������ ������� ������
    			&(buf[8][0]) 	// text to set
   			);

         SetDlgItemText(
    			hwndDlg,	// handle of dialog box
    			124,	// identifier of control ���. ������ ������� ������
    			&(buf[9][0]) 	// text to set
   			);

         SetDlgItemText(
    			hwndDlg,	// handle of dialog box
    			127,	// identifier of control � ������ ������� ������
    			&(buf[10][0]) 	// text to set
   			);

         SetDlgItemText(
    			hwndDlg,	// handle of dialog box
    			129,	// identifier of control ����. ������� ������� ������
    			&(buf[11][0]) 	// text to set
   			);

         SetDlgItemText(
    			hwndDlg,	// handle of dialog box
    			131,	// identifier of control ���. ������� ������� ������
    			&(buf[12][0]) 	// text to set
   			);

         SetDlgItemText(
    			hwndDlg,	// handle of dialog box
    			133,	// identifier of control � ������� ������� ������
    			&(buf[13][0]) 	// text to set
   			);

        	return TRUE;

        case WM_COMMAND:
        	switch (LOWORD(wParam))
         {
                case 1://������ ������ ����

                	  GetDlgItemText(
    						hwndDlg,	// handle of dialog box
    						104,	// identifier of control ������������ ������� �����
    						&(buf[0][0]), 	// address of buffer for text
                     31 	// maximum size of string
   						);

                    GetDlgItemText(
    						hwndDlg,	// handle of dialog box
    						102,	// identifier of control ����. ������ ������� �����
    						&(buf[1][0]), 	// address of buffer for text
                     31 	// maximum size of string
   						);

                    GetDlgItemText(
    						hwndDlg,	// handle of dialog box
    						107,	// identifier of control ���. ������ ������� �����
    						&(buf[2][0]), 	// address of buffer for text
                     31 	// maximum size of string
   						);

                    GetDlgItemText(
    						hwndDlg,	// handle of dialog box
    						110,	// identifier of control � ������ ������� �����
    						&(buf[3][0]), 	// address of buffer for text
                     31 	// maximum size of string
   						);

                    GetDlgItemText(
    						hwndDlg,	// handle of dialog box
    						112,	// identifier of control ����. ������� ������� �����
    						&(buf[4][0]), 	// address of buffer for text
                     31 	// maximum size of string
   						);

                    GetDlgItemText(
    						hwndDlg,	// handle of dialog box
    						114,	// identifier of control ���. ������� ������� �����
    						&(buf[5][0]), 	// address of buffer for text
                     31 	// maximum size of string
   						);

                    GetDlgItemText(
    						hwndDlg,	// handle of dialog box
    						116,	// identifier of control � ������� ������� �����
    						&(buf[6][0]), 	// address of buffer for text
                     31 	// maximum size of string
   						);

                    GetDlgItemText(
    						hwndDlg,	// handle of dialog box
    						122,	// identifier of control ������������ ������� ������
    						&(buf[7][0]), 	// address of buffer for text
                     31 	// maximum size of string
   						);

                    GetDlgItemText(
    						hwndDlg,	// handle of dialog box
    						120,	// identifier of control ����. ������ ������� ������
    						&(buf[8][0]), 	// address of buffer for text
                     31 	// maximum size of string
   						);

                    GetDlgItemText(
    						hwndDlg,	// handle of dialog box
    						124,	// identifier of control ���. ������ ������� ������
    						&(buf[9][0]), 	// address of buffer for text
                     31 	// maximum size of string
   						);

                    GetDlgItemText(
    						hwndDlg,	// handle of dialog box
    						127,	// identifier of control � ������ ������� ������
    						&(buf[10][0]), 	// address of buffer for text
                     31 	// maximum size of string
   						);

                    GetDlgItemText(
    						hwndDlg,	// handle of dialog box
    						129,	// identifier of control ����. ������� ������� ������
    						&(buf[11][0]), 	// address of buffer for text
                     31 	// maximum size of string
   						);

                    GetDlgItemText(
    						hwndDlg,	// handle of dialog box
    						131,	// identifier of control ���. ������� ������� ������
    						&(buf[12][0]), 	// address of buffer for text
                     31 	// maximum size of string
   						);

                    GetDlgItemText(
    						hwndDlg,	// handle of dialog box
    						133,	// identifier of control � ������� ������� ������
    						&(buf[13][0]), 	// address of buffer for text
                     31 	// maximum size of string
   						);
                    EndDialog(hwndDlg, 1);
                    return TRUE;


                case 2://������ ������ ������
                    EndDialog(hwndDlg, 0);
                    return TRUE;
         }

        default:
        	return FALSE;
    	}

    }// konec NK_DialogFunc


#pragma argsused
LPTSTR BrowseForDir(HWND hwnd, LPTSTR zagolovok)//���������� ��������� �� ������ �� ������� SHBrowseForFolder
																				//��������� ����� � �������.
                                                            //���� ����� ��
                                                            //������� - ���������� NULL.
                                                            //hwnd - ����������� ���� ��� ������� SHBrowseForFolder
                                                            //�.�. NULL.
                                                            //zagolovok - ��������� ������� SHBrowseForFolder
{
    BROWSEINFO  bi;
    LPTSTR chFolderBuff;
    static TCHAR Path[MAX_PATH];


    // Get the shell's allocator.
    if (!SUCCEEDED(SHGetMalloc(&g_pMalloc)))
        return (NULL);

    // Allocate a buffer to receive browse information.
        if ((chFolderBuff = (LPTSTR) g_pMalloc->lpVtbl->Alloc(
            g_pMalloc, MAX_PATH)) == NULL)
        return (NULL);
    ZeroMemory(&bi, sizeof(bi));
    bi.hwndOwner=hwnd;
    bi.pidlRoot=NULL;
    bi.pszDisplayName=chFolderBuff;
    bi.lpszTitle=zagolovok;
    bi.ulFlags=BIF_RETURNONLYFSDIRS;
    bi.lpfn=NULL;
    bi.lParam=NULL;
    bi.iImage=NULL;



    // Browse for a folder.
    if (SHBrowseForFolder(&bi) != NULL)
    {
      	strcpy(Path,chFolderBuff);

         // Clean up.
      	g_pMalloc->lpVtbl->Free(g_pMalloc, chFolderBuff);

         // Release the shell's allocator.
    		g_pMalloc->lpVtbl->Release(g_pMalloc);

    		return (Path);
    }
    // Clean up.
    g_pMalloc->lpVtbl->Free(g_pMalloc, chFolderBuff);


    // Release the shell's allocator.
    g_pMalloc->lpVtbl->Release(g_pMalloc);


    return (NULL);

}//����� BrowseForDir

*/
