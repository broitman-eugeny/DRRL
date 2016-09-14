#include "d:\work\bc\include\mcadincl.h"
    
    extern FUNCTIONINFO FRelyef,FN_prep_c,FPrepyatstvie,FVoda_uch,
    							Fa_e,Fy_x,FPrep,Fr,Fp,Fsurf,Fluch,FR_pr,
                        FHorda_Krit_Prep,FSlinear,FInterp,FInterp1,FInterp2,
                        FQ,FChrp,FDeltaVdegr,FTd,FNegotovnost,FVgaz,FBERPpm,FVoda,
                        FPrep_Les,FPrep_Dom,FK_vp,Fg_sr_sigma,Fsigma_ot_R,FTdy,
                        FP_Vd_V0_teta_W,FV_min_eff,FT_int,FRek_razn,FPRP_peres,
                        FPRP_slperes,FNormy,FReadProfile,FZigzagoobraznost,
                        FReflection_point, FSaveDXFProfile;


    char *ErrorMessageTable[] = {
    "argument must be real",    //  error 1 --  argument must be real
    "insufficient memory",      //  error 2 --  memory allocation error
    "interrupted",               //  error 3 --  execution interrupted
    "Не выбран файл",     //  error 4 --  Не выбран файл профиля или DMR
    "Отчет не выведен"  //error 5 --  Отрицательный просвет при средней рефракции в точке критического препятствия
    };

HINSTANCE g_hInst;		// the current instance

#pragma argsused

BOOL WINAPI DllEntryPoint (HANDLE hDLL, DWORD dwReason, LPVOID lpReserved)
{

   // Save off the handle to the current instance.
    g_hInst = hDLL;

  switch (dwReason)
  {
    case DLL_PROCESS_ATTACH:
    {

      // DLL is attaching to the address space of the current process.
      //
		  if (!CreateUserErrorMessageTable( hDLL, 4, ErrorMessageTable ) )
				break;

 		  if ( CreateUserFunction( hDLL, &FRelyef) == NULL )
 				break;
        
        if ( CreateUserFunction( hDLL, &FN_prep_c) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FPrepyatstvie) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FVoda_uch) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &Fa_e) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &Fy_x) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FPrep) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &Fr) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &Fp) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &Fsurf) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &Fluch) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FR_pr) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FHorda_Krit_Prep) == NULL )
        break;


        if ( CreateUserFunction( hDLL, &FSlinear) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FInterp) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FInterp1) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FInterp2) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FQ) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FChrp) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FDeltaVdegr) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FTd) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FNegotovnost) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FVgaz) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FBERPpm) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FVoda) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FPrep_Les) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FPrep_Dom) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FK_vp) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &Fg_sr_sigma) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &Fsigma_ot_R) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FTdy) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FP_Vd_V0_teta_W) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FV_min_eff) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FT_int) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FRek_razn) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FPRP_peres) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FPRP_slperes) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FNormy) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FReadProfile) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FZigzagoobraznost) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FReflection_point) == NULL )
        break;

        if ( CreateUserFunction( hDLL, &FSaveDXFProfile) == NULL )
        break;
    }

	 case DLL_THREAD_ATTACH:        // A new thread is being created in the current process.
	 case DLL_THREAD_DETACH:        // A thread is exiting cleanly.
	 case DLL_PROCESS_DETACH:      // The calling process is detaching the DLL from its address space.

		break;
  }
  return TRUE;
}

    

