#include <windows.h>    // includes basic windows functionality
#include <stdio.h>
#include<io.h>

BOOL BrowseForProfile(char *file_pathname, HANDLE hInstance);
char ** Read_Profile(FILE *stream);			// ������ ���� ������� �� ������� ��������� stream, �������� � text mode (t).
														// ���������� ��������� �� ������ ����� - ����� ����� ������� (������ ��� ������� ������������� ����� ��).
char ** Reverse_Profile(char ** Profile_source);			// �������������� ������� Profile_source �� �����������.
																		// ���������� ��������� �� ������ ����� - ����� ����� ������������� ������� (������ ��� ������� ������������� ����� ��).
int Write_Profile(FILE *stream, char **Profile);// ���������� ������ ����� ������� Profile � ����, �� ������� ��������� stream, �������� � text mode (t).
															  // ��� ������ ������ � ���� ���������� EOF

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
    	return (FALSE);                                                             														//
    }                                                                           													 		//
                                                                                												  			//
    //-----------------------------------------------------------------------------------------------------------------------


    stream=fopen(file_pathname, "wt");

    Write_Profile(stream, Profile_rev);

    fclose(stream);


	return (TRUE);
}//����� WinMain