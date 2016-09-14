#include "d:\work\bc\include\mcadincl.h"
#include <math.h>
#include <winuser.h>
#include <alloc.h>
#include <commdlg.h>
#include<io.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>




   //Сохраняет профиль интервала РРЛ в графическом виде в формате DXF
	BOOL DrawProfileDXF(
   					 char *pr001_p_f_n,//Путь и имя файла профиля формата 001
						 char *prDXF_p_f_n,//Путь и имя выходного графического DXF файла профиля
                   double Gradient_d,//градиент диэлектрической проницаемости воздуха, 1/мХ10-8
                   double Chast_d,//Рабочая частота, ГГц
                   double Pogr_d,//Погрешность отметок земли, м
                   double Hosnleft_d,//Высота центра раскрыва основной антенны слева, м
                   double Hosnright_d,//Высота центра раскрыва основной антенны справа, м
                   int PRP,//Флаг наличия пространственно-разнесенного приема: 0 - ПРП нет, 1 - ПРП есть
                   double HdopLeft_d,//Высота центра раскрыва дополнительной антенны слева, м
                   double HdopRight_d//Высота центра раскрыва дополнительной антенны справа, м
                   );


	double linterp(int N,                   // Линейная интерполяция функции по точкам.
    						double* X,               // X - вектор абсцисс,
                     double* Y,               // Y - вектор ординат,
                     double x);               // x - значение абсциссы точки, в которой производится вычисление
                                              // N - количество точек

	double parab_approx(int N,              // Параболическая аппроксимация функции по точкам полиномом 2-го порядка,
    														 // по методу наименьших квадратов. Возвращает значение аппроксимированной функции
                                              //
    						double* X,               // X - вектор абсцисс,
                     double* Y,               // Y - вектор ординат,
                     double x,
                     double far *A);          // x - значение абсциссы точки, в которой производится вычисление
                                              // N - количество точек
                                              // A - указатель на массив с коэффициентами полинома. Элементу с большим индексом соответствует более старший коэффициент.

    double minerr(double (*func)(double,double*),    // Определяет значение первого аргумента функции *func, при
    							double niz,         // котором функция принимает значение val с точностью prec
                        double verh,        // в пределах изменения аргумента niz - verh.
                        double prec,        // если на заданном интервале не найден аргумент, удовлетворяющий
                        double val,
                        double *Args);      // требуемой точности возвращается значение аргумента, при котором
                                            // функция максимально приближается к значению val.
                                            //Остальные аргументы функции *func должны быть записаны в массиве Args

    double Vd(double I, double *Args);// Промежуточная ф-ция для Тд



    LRESULT FRelyefFunction(COMPLEXARRAY * const Relyef,							// zapolnyaet massiv Relyef znacheniyami iz
                            COMPLEXARRAY * const profile,						// massiva profile, prichem 1-j element stroki
                            COMPLEXSCALAR * const N_relyef); 					// massiva Relyef - rasstoyanye do prepyatstviya,
                             																// a 2-j - visotnaya otmetka. Kolichestvo strok
                                                                           // sootvetstvuet kolichestvu tochek

    LRESULT FN_prep_cFunction(COMPLEXSCALAR * const N_prep_c,					// opredelyaet kolichestvo prepyatstviy bez "perelomov"
                              COMPLEXSCALAR * const N_prep,
                              COMPLEXSCALAR * const N_relyef,
                            	COMPLEXARRAY * const profile,
                            	COMPLEXARRAY * const Relyef);

    LRESULT FPrepyatstvieFunction(COMPLEXARRAY * const Prepyatstvie,				 // zapolnyaet massiv prepyatstviy
    										 COMPLEXSCALAR * const N_prep,
	                               COMPLEXSCALAR * const N_relyef,
    	                          	 COMPLEXARRAY * const profile,
                            		 COMPLEXARRAY * const Relyef);

    LRESULT FVoda_uchFunction(COMPLEXARRAY * const Voda_uch,				 // zapolnyaet massiv vodnyh uchastkov
                              COMPLEXSCALAR * const N_voda_c,
                              COMPLEXSCALAR * const N_voda,
                              COMPLEXSCALAR * const N_relyef,
                              COMPLEXSCALAR * const N_prep,
                              COMPLEXARRAY * const profile);

    LRESULT Fa_eFunction(COMPLEXSCALAR * const a_e,				 // vozvrashaet ekvivalentnyi radius zemli
    							 COMPLEXSCALAR * const g0);

    LRESULT Fy_xFunction(COMPLEXSCALAR * const y_x,				 // vozvrashaet vysotu zemnogo segmenta
    							 COMPLEXSCALAR * const x,
                         COMPLEXSCALAR * const R,
                         COMPLEXSCALAR * const a_e,
                         COMPLEXSCALAR * const Hor);

    LRESULT FPrepFunction(COMPLEXARRAY * const Prep,				 // Preobrazovyvaet massiv Prepyatstvie k udobnomu vidu
    							 COMPLEXSCALAR * const N_prep_c,			 // Prep dlya interpolyacii
                         COMPLEXSCALAR * const R,
                         COMPLEXARRAY * const Prepyatstvie);

    LRESULT FrFunction(COMPLEXSCALAR * const r,				 // Vichislyaet visotnuyu otmetky relyefa putem lineynoy interpolyacii
    						  COMPLEXSCALAR * const x,
                       COMPLEXARRAY * const Relyef,
                       COMPLEXSCALAR * const Hor,
                       COMPLEXSCALAR * const a_e,
                       COMPLEXSCALAR * const R,
                       COMPLEXSCALAR * const N_relyef);

    LRESULT FpFunction(COMPLEXSCALAR * const p,				 // Vichislyaet visotu prepyatstvya putem lineynoy interpolyacii
                       COMPLEXSCALAR * const x,
    						  COMPLEXARRAY * const Prep,
                       COMPLEXSCALAR * const N_prep_c);

    LRESULT FsurfFunction(COMPLEXSCALAR * const surf,				 // Vichislyaet visotu poverhnosti profilya s uchetom pogreshnosti putem lineynoy interpolyacii
    							  COMPLEXSCALAR * const x,
                          COMPLEXARRAY * const Prep,
                       	  COMPLEXSCALAR * const N_prep_c,
                          COMPLEXARRAY * const Relyef,
                          COMPLEXSCALAR * const Hor,
                          COMPLEXSCALAR * const a_e,
                          COMPLEXSCALAR * const R,
                          COMPLEXSCALAR * const N_relyef,
                          COMPLEXSCALAR * const Delta_H);

      LRESULT FluchFunction(COMPLEXSCALAR * const luch,				 // Vichislyaet vysotu lucha
                            COMPLEXSCALAR * const x,
    							 	 COMPLEXSCALAR * const H1,
                            COMPLEXSCALAR * const H2,
                            COMPLEXSCALAR * const R,
                            COMPLEXSCALAR * const r_ot_0,
                            COMPLEXSCALAR * const r_ot_R);



      LRESULT FR_prFunction(COMPLEXSCALAR * const R_pr,				 // Vichislyaet rasstoyanye do prepyatstvya
                            COMPLEXARRAY * const Prep,
    							 	 COMPLEXARRAY * const Relyef,
                            COMPLEXARRAY * const Args);


 
		LRESULT FHorda_Krit_PrepFunction(COMPLEXARRAY * const R1_R2_DY,			// Vichislyaet parametry hordy
    							 COMPLEXARRAY * const Prep,                           // R1_R2_DY[0] - km, (parametr_r) dlina hordy
                         COMPLEXARRAY * const Relyef,									// R1_R2_DY[1] - km, (R1h) rasstoyanie do levoy granicy hordy
                       	 COMPLEXARRAY * const Args);									// R1_R2_DY[2] - km, (R2h) rasstoyanie do pravoy granicy hordy
                         																		// R1_R2_DY[3] - m, (H1h) vysotnaya otmetka levoy granicy hordy
                                                                              // R1_R2_DY[4] - m, (H2h) vysotnaya otmetka pravoy granicy hordy
                                                                              // R1_R2_DY[5] - m, (Hkph) vysotnaya otmetka hordy v tochke kriticheskogo prepyatstviya
                                                                              // R1_R2_DY[6] - m, (DY) vysota segmenta approksimiruyuschey sfery
                                                                              // R1_R2_DY[7] - km, (R1kas) rasstoyanie do tochki kasaniya luchem profilya sleva
                                                                              // R1_R2_DY[8] - km, (R2kas) rasstoyanie do tochki kasaniya luchem profilya sprava
                                                                              // R1_R2_DY[9] - m, (H1kas) vysotnaya otmetka tochki kasaniya luchem profilya sleva
                                                                              // R1_R2_DY[10] - m, (H2kas) vysotnaya otmetka tochki kasaniya luchem profilya sprava
                                                                              // R1_R2_DY[11] - km, (R1ellipse) rasstoyanie do tochki peresecheniya ellipsom profilya sleva
                                                                              // R1_R2_DY[12] - km, (R2ellipse) rasstoyanie do tochki peresecheniya ellipsom profilya sprava
                                                                              // R1_R2_DY[13] - m, (H1ellipse) vysotnaya otmetka tochki peresecheniya ellipsom profilya sleva
                                                                              // R1_R2_DY[14] - m, (H2ellipse) vysotnaya otmetka tochki peresecheniya ellipsom profilya sprava
                                                                              // R1_R2_DY[15] - km, (R1hordaH0) rasstoyanie do tochki peresecheniya hordy(H0) profilya sleva
                                                                              // R1_R2_DY[16] - km, (R2hordaH0) rasstoyanie do tochki peresecheniya hordy(H0) profilya sprava
                                                                              // R1_R2_DY[17] - m, (H1hordaH0) vysotnaya otmetka tochki peresecheniya hordy(H0) profilya sleva
                                                                              // R1_R2_DY[18] - m, (H2hordaH0) vysotnaya otmetka tochki peresecheniya hordy(H0) profilya sprava
																										// R1_R2_DY[19] - km, (R_kross_kasat) rasstoyanie do tochki peresecheniya kasatelnyh
                                                                              // R1_R2_DY[20] - m, (H_kross_kasat) vysotnaya otmetka tochki peresecheniya kasatelnyh
                                                                              // R1_R2_DY[21] - m, (Prosvet_kross_kasat) prosvet v tochke peresecheniya kasatelnyh
                                                                              // R1_R2_DY[22] - km, (R_kr) rasstoyanie do kriticheskogo prepyatstviya
                                                                              // R1_R2_DY[23] - m, (H_kr) prosvet v tochke kriticheskogo prepyatstviya s uchetom pogreshnosti
                                                                              // R1_R2_DY[24] - m, (H0) optimalny prosvet v tochke kriticheskogo prepyatstviya
                                                                              // R1_R2_DY[25] - m, (p_ot_g) otnositelny prosvet v tochke kriticheskogo prepyatstviya s uchetom pogreshnosti
                                                                              // Prep - массив расстояний и высотных отметок препятствий
                                               											// Relyef - massiv rasstoyaniy i vysotnyh otmetok izlomov relyefa
                                                      								// Args[0] - N_prep_c - kolichestvo prepyatstviy
                                                										// Args[1] - Hor - m, uroven uslovnogo gorizonta
                         	                            								// Args[2] - a_e - m, ekvivalentny radius zemli
                                                                              // Args[3] - N_relyef - kolichestvo tochek relyefa v massive Relyef
                                                                              // Args[4] - Delta_H - m, pogreshnost otmetok relyefa
                                                                              // Args[5] - H1 - m, vysota podvesa antenny sleva
                                                                              // Args[6] - H2 - m, vysota podvesa antenny sprava
                                                                              // Args[7] - f - GGz, rabochaya chastota
                                                                              // Args[8] - lambda - m, rabochaya dlina volny
                                                                              // Args[9] - R_pr - km, rasstoyanie do prepyatstviya s minimalnym otnositelnym prosvetom mejdu luchom i poverhnostyu profilya
                                                                              // Args[10] - H_pr - m, prosvet s uchetom pogreshnosti v tochke s minimalnym otnositelnym prosvetom mejdu luchom i poverhnostyu profilya



 
      LRESULT FSlinearFunction(COMPLEXARRAY * const Korny,			 // Opredelyaet korny systemy lineynyh uravneniy
                           COMPLEXARRAY * const M,          		 // M - matrica, v strokah kotoroy zapisany
                           COMPLEXSCALAR * const N);               // koefficienty pri peremennyh, a v poslednem
                           	             			                // stolbce - znachenya funkcii.
                           								                // N - kolichestvo peremennyh (kolichestvo uravnenyi)
                                                                   // Korny - korny systemy

      LRESULT FInterpFunction(COMPLEXSCALAR * const F,			 // Opredelyaet znachenie funkcii polinomialnoy interpolyaciey
                           COMPLEXARRAY * const X,
                           COMPLEXARRAY * const Y,
                           COMPLEXSCALAR * const x);          		 // X - vector znachenyi argumenta v tochkah
                           	             			                // Y - vector znachenyi funkcii v tochkah
                                                                   // x - argument
                                                                   // F - znachenie funkcii v tochke x.

      LRESULT FInterp1Function(COMPLEXSCALAR * const F,			 // Opredelyaet znachenie funkcii interpolyaciey polinomom
                           COMPLEXARRAY * const X,					 // 2-j stepeni na intervalah mejdu dvumya sosednimi
                           COMPLEXARRAY * const Y,					 // tochkami (doljno byt ne menee 3-h tochek)
                           COMPLEXSCALAR * const x);          		 // X - vector znachenyi argumenta v tochkah
                           	             			                // Y - vector znachenyi funkcii v tochkah
                                                                   // x - argument
                                                                   // F - znachenie funkcii v tochke x.
                                                                   // Proizvodnaya v tochke nahoditsya kak tg(ugol) naklona linii
                                                                   // soedinyaushey predydushuyu i sled. tochki


      LRESULT FInterp2Function(COMPLEXSCALAR * const F,			 // Opredelyaet znachenie funkcii interpolyaciey polinomom
                           COMPLEXARRAY * const X,					 // 2-j stepeni na intervalah mejdu dvumya sosednimi
                           COMPLEXARRAY * const Y,					 // tochkami (doljno byt ne menee 3-h tochek)
                           COMPLEXSCALAR * const x);          		 // X - vector znachenyi argumenta v tochkah
                           	             			                // Y - vector znachenyi funkcii v tochkah
                                                                   // x - argument
                                                                   // F - znachenie funkcii v tochke x.
                                                                   // Proizvodnaya v tochke nahoditsya kak dlya paraboly
                                                                   // prohodyashey cherez etu i dve sosednih tochki


      LRESULT FQFunction(COMPLEXARRAY * const Q,			 // Opredelyaet znachenie Aотр.,F(p(g),A),Q
                           COMPLEXSCALAR * const f,
                           COMPLEXSCALAR * const R,
                           COMPLEXSCALAR * const sigma,
                           COMPLEXSCALAR * const Suh,
                           COMPLEXSCALAR * const Peres,
                           COMPLEXSCALAR * const R_otr,
                           COMPLEXSCALAR * const p_ot_g_sr,
                           COMPLEXARRAY * const K_vp);          	 // f - chastota (GHz)
                           	             			                // R - Dlina intervala (km)
                                                                   // sigma - standartnoe otklonenie g (1/m)
                                                                   // Suh - 1 esli trassa suhoputnaya, 0 - esli primorskaya
                                                                   // Peres - 1 esli trassa peresechennaya, 0 - slaboperesechennaya
                                                                   // R_otr - rasstoyanie do tochki otragenya (km) (esli Peres==1, ignoriruetsya)
                                                                   // p_ot_g_sr - относительный prosvet pri sredney refrakcii (m)
                                                                   // K_vp[0] - koefficient vodnoi poverhnosti (%)
                                                                   // K_vp[1] - h_sr, vysota trassy nad urovnem morya (m)


      LRESULT FChrpFunction(COMPLEXARRAY * const SESR_Kng,			 // Opredelyaet znachenie SESR i Kng na uchastke
                           COMPLEXSCALAR * const K,                // rezervirovaniya pri chrp.
                           COMPLEXSCALAR * const N,
                           COMPLEXSCALAR * const N_ChRP,
                           COMPLEXSCALAR * const df,
                           COMPLEXSCALAR * const f,
                           COMPLEXARRAY * const Kint,
                           COMPLEXARRAY * const Tint_refr_d,
                           COMPLEXARRAY * const Suh_Peres,
                           COMPLEXARRAY * const Alfaint_refr,
                           COMPLEXARRAY * const p_ot_g);     // K - Kol-vo rabochih stvolov ne uchityvaya rezervnogo
                           	             			                // N - Kol-vo proletov na uchastke rezervirovaniya
                                                                   // N_ChRP -  Kol-vo proletov bez PRP na uchastke rezervirovaniya
                                                                   // df - Raznos po chastote megdu rezervnym i blijaishim
                                                                   //      k nemu rabochim (MHz)
                                                                   // f - Chastota rabochego stvola (MHz)
                                                                   // Kint - massiv K interferencii na intervalah
                                                                   // Tint_refr_d - massiv znacheniy Tint, T0 i Td na intervalah (%)
                                                                   // Suh_Peres - massiv priznakov suhoputnosti i peresechennosti trass (1/0)
                                                                   // Alfaint_refr - massiv znacheniy koefficientov alfaint i alfarefr.
                                                                   // p_ot_g - massiv otnositelnyh prosvetov v tochkah otrajeniya
                                                                   //          na slaboperesechennyh intervalah

      LRESULT FDeltaVdegrFunction(COMPLEXARRAY * const DeltaVdegr,			 // Opredelyaet znachenie degradacii porogovogo urovnya
                           COMPLEXSCALAR * const Tip,                // Tip - 1-2ОФМ;2-4ОФМ;3-8ОФМ;4-16КАМ;5-32КАМ;6 - 64КАМ,
                           COMPLEXSCALAR * const Vmin0,              //       7-128КАМ(ТСМ);8-256КАМ;9-512КАМ.
                           COMPLEXSCALAR * const Nmesh,              // Vmin0 - dB минимально допустимый ый множитель ослабления без
                           COMPLEXSCALAR * const R,                  //         учета деградации порогоавого уровня и
                           COMPLEXSCALAR * const Vsr,                //         ограничения усиления
                           COMPLEXARRAY * const Mmesh,               // Nmesh - Количество мешающих интервалов
                           COMPLEXSCALAR * const Zpordops,
                           COMPLEXARRAY * const MCochannel,
                           COMPLEXSCALAR * const C);          // R - км, протяженность рассчитываемого интервала
                           												      // Vsr - среднее ослабление на рассчитываемом интервале
                           									               // Mmesh - матрица параметров мешающих интервалов
                                                                     //			  0 столбец - км, протяженности трасс мешающих интервалов
                                                                     //         1 столбец - дБ, уменьшение уровня принимаемого сигнала
                                                                     //							  из-за ДНА (со знаком "-")
                                                                     //         2 столбец - дБ, разница в мощностях передатчиков мешающего и
                                                                     //                     полезного сигналов
                                                                     //         3 столбец - дБ, диапазон автоматической регулировки мощности
                                                                     //                     передатчика мешающего интервала
                                                                     //         4 столбец - дБ, значения множителяослабления мешающих сигналов
                                                                     //							  при среднем градиенте диэлектрической проницаемости
                                                                     //                     среды (со знаком "-")
                                                                     //         5 столбец - дБ, разница в коэффициентах усиления передающих
                                                                     //							  антенн мешающего и полезного сигналов
                                                                     //	        6 столбец - дБ, разница потерь в передающих фидерных трактах
                                                                     //                     шающего и  полезного сигналов.
                                                                     // Zpordops - отношение мощностей мешающего сигнала
                                                                     //            соседнего ствола и полезного сигнала,
                                                                     //            вызывающее в канале Рош_макс при
                                                                     //            деградации порогового уровня на 3 дБ.
                                                                     // MCochannel[0] - Cochannel - 1 - есть Co-channel, 0 - нет,
                                                                     // MCochannel[1] - Dp0 - дБ, кроссполяризационная защита приемной антенны
                                                                     // MCochannel[2] - Ixpic - дБ, выигрыш компенсатора помех,
                                                                     // MCochannel[3] - f - ГГц, рабочая частота,
                                                                     // С - Мбит/с - скорость передачи системы,
                                                                     // DeltaVdegr[0] - DeltaVdegr - дБ, деградация порогового уровня
                                                                     // DeltaVdegr[1] - Zпордоп - дБ, пороговое защ. отношение



      LRESULT FTdFunction(COMPLEXARRAY * const Td,			 		 // Opredelyaet znachenie Неустойчивости из-за влияния осадков за наихудший месяц
                           COMPLEXSCALAR * const f,                // f - ГГц, рабочая частота;
                           COMPLEXSCALAR * const R,              	 // R - км, длина интервала;
                           COMPLEXSCALAR * const Pol,              // Pol - поляризация, 0 - горизонтальная, 1 - вертикальная;
                           COMPLEXSCALAR * const Vmin,             // Vmin - дБ, мин. доп. множитель ослабления для расчета Тд;
                           COMPLEXSCALAR * const Region);          // Region - номер региона по карте районирования по интенсивности дождей;
                           			                               // Td - 0-й эленмент - мм/ч,I_max
                                                                   //      1-й элемент - %, неустойчивость из-за влияния осадков за наихудший месяц

    LRESULT FNegotovnostFunction(COMPLEXARRAY * const Negotovnost,  // Opredelyaet коэффициенты готовности, SESR, коэффициент неготовности
                           COMPLEXARRAY * const Args);             // Args[0] - T0 - %, рефракционная составляющая неустойчивости;
                                            								  // Args[1] - Td - %, дождевая составляющая неустойчивости за год;
                                                                    // Args[2] - Tint - %, интерференционная составляющая неустойчивости;
                                                                    // Args[3] - Vmin_ef - дБ, мин. доп. множитель ослабления для расчета Тint;
                                                                    // Args[4] - Vmin - дБ, мин. доп. множитель ослабления для расчета Т0;
                                                                    // Args[5] - f - ГГц, рабочая частота;
                                                                    // Args[6] - R - км, длина интервала;
                                                                    // Args[7] - Peres - 0 - слабопересеченная, 1 - пересеченная;
                                                                    // Args[8] - Suh - 0 - приморская, 1 - сухопутная;
                                                                    // Args[9] - p_ot_g0 - относительный просвет в критической точке при средней рефракции;
                                                                    // Args[10] - p_ot_gint - относительный просвет в точке отражения при средней рефракции (если нет точки отражения, то p_ot_gint=p_ot_g0);
                           			                                // Negotovnost - 0-й элемент - интерференционный коэффициент готовности;
                                                                    //               1-й элемент - рефракционный коэффициент готовности;
                                                                    //               2-й элемент - коэффициент интерференции;
                                                                    //               3-й элемент - %, SESR;
                                                                    //               4-й элемент - %, коэффициент неготовности.



    LRESULT FVgazFunction(COMPLEXSCALAR * const Vgaz,  // Opredelyaet ослабление сигнала в газах атмосферы
                           COMPLEXARRAY * const Args);             // Args[0] - f - ГГц, рабочая частота;
                                            								  // Args[1] - R - км, длина интервала;
                                                                    // Args[2] - w - г/м3, абсолютная влажность водяного пара;
                                                                    // Vgaz - дБ, ослабление сигнала в газах атмосферы;


    LRESULT FBERPpmFunction(COMPLEXARRAY * const BERPpm,  // Opredelyaet значения коэффициента ошибок и порогового уровня
                           COMPLEXARRAY * const Args);             // Args[0] - Pпм_пор при BER=0.001 - дБм, пороговый уровень при коэффициенте ошибок 0.001;
                                            								  // Args[1] - С - Мбит/с, скорость передачи.
                                                                    // BERPpm[0] - BER - коэффициент ошибок;
                                                                    // BERPpm[1] - Pпм_пор - дБм, пороговый уровень при соответствующем скорости передачи коэффициенте ошибок.


    LRESULT FVodaFunction(COMPLEXARRAY * const Voda,  // Заполняет массив водных участков
                           COMPLEXARRAY * const Voda_uch,
                           COMPLEXARRAY * const Args);              // Voda_uch - исходный массив водных участков;
                                            								  // Args[0] - Hor - м, условный уровень горизонта;
                                                                    // Args[1] - N_voda_c - количество водных участков;
                                                                    // Voda - результирующий массив водных участков.


    LRESULT FPrep_LesFunction(COMPLEXARRAY * const Prep,				 // Preobrazovyvaet massiv Prepyatstvie k udobnomu vidu
    							 COMPLEXSCALAR * const N_prep_c,			 			 // Prep_Les dlya interpolyacii лесных массивов
                         COMPLEXSCALAR * const R,
                         COMPLEXARRAY * const Prepyatstvie,
                         COMPLEXSCALAR * const Hor,
                         COMPLEXARRAY * const Relyef,
                         COMPLEXSCALAR * const a_e,
                         COMPLEXSCALAR * const N_relyef);





    LRESULT FPrep_DomFunction(COMPLEXARRAY * const Prep,				 // Preobrazovyvaet massiv Prepyatstvie k udobnomu vidu
    							 COMPLEXSCALAR * const N_prep_c,			 			 // Prep_Dom dlya interpolyacii застройки
                         COMPLEXSCALAR * const R,
                         COMPLEXARRAY * const Prepyatstvie,
                         COMPLEXSCALAR * const Hor,
                         COMPLEXARRAY * const Relyef,
                         COMPLEXSCALAR * const a_e,
                         COMPLEXSCALAR * const N_relyef);


    LRESULT FK_vpFunction(COMPLEXSCALAR * const K_vp,				 // Определяет коэффициент водной поверхности
    							 COMPLEXSCALAR * const N_voda,
                         COMPLEXSCALAR * const R,
                         COMPLEXARRAY * const Voda_uch);


    LRESULT Fg_sr_sigmaFunction(COMPLEXARRAY * const g_sr_sigma,				 // Определяет средний градиент диэлектрической проницаемости воздуха и его СКО
    							 COMPLEXSCALAR * const Raion);                      // Raion - номер климатического района по распределению
                                                                            //			эффективных градиентов диэлектрической
                                                                            //			проницаемости воздуха.


    LRESULT Fsigma_ot_RFunction(COMPLEXSCALAR * const sigma_ot_R,				 // Определяет СКО градиента диэлектрической проницаемости воздуха
    							 COMPLEXARRAY * const Args);                      // Args[0] - длина интервала
                                                                            //Args[1] - среднее значение градиента
                                                                            //Args[2] - СКО табличное


    LRESULT FTdyFunction(COMPLEXARRAY * const Tdy,				 // Определяет коэффициент Qd и неустойчивость по дождям в год
    							 COMPLEXARRAY * const Args);         // Args[0] - Tdm - неустойчивость по дождям в месяц
                         												 //Args[1] - Region - номер района по карте распределения Qd
                                                             //Tdy[0] - Qd
                                                             //Tdy[1] - Tdy


    LRESULT FP_Vd_V0_teta_WFunction(COMPLEXARRAY * const P_Vd_V0_teta_W,				 // Определяет средний уровень мощности принимаемого сигнала,
    							 COMPLEXARRAY * const Args);         								 // минимально-допустимый множитель ослабления по замираниям в осадках (без учета деградации),
                         																				 // минимально-допустимый множитель ослабления по интерференционным и субрефракционным замираниям (без учета деградации),
                                                                                     // затухание в АФТ,
                                                                                     // ослабление сигнала в свободном пространстве.
                                                             								 // Args[0] - R - км, длина интервала,
                         												 								 // Args[1] - f - ГГц, рабочая частота,
                                                                                     // Args[2] - Pпд - дБм, мощность передатчика,
                                                                                     // Args[3] - Pпм_пор - дБм, пороговая чувствительность приемника,
                                                                                     // Args[4] - Gпд - дБи, коэффициент усиления передающей антенны,
                                                                                     // Args[5] - Gпм - дБи, коэффициент усиления приемной антенны,
                                                                                     // Args[6] - alfa_пд - дБ/м, удельное затухание передающего фидера,
                                                                                     // Args[7] - Lпд - м, длина передающего фидера,
                                                                                     // Args[8] - teta_post_пд - дБ, постоянные потери передающего АФТ,
                                                                                     // Args[9] - alfa_пм - дБ/м, удельное затухание приемного фидера,
                                                                                     // Args[10] - Lпм - м, длина приемного фидера,
                                                                                     // Args[11] - teta_post_пм - дБ, постоянные потери приемного АФТ,
                                                                                     // Args[12] - Vg - дБ, поглощение в газах атмосферы,
                                                                                     // Args[13] - Vdifr - дБ, дифракционное ослабление сигнала при средней рефракции,
                                                                                     // Args[14] - periskop_pd - 1/0, 1 - перископическая передающая антенна, 0 - нет,
                                                                                     // Args[15] - periskop_pm - 1/0, 1 - перископическая приемная антенна, 0 - нет,
                                                                                     // Args[16] - truba_pd - 1/0, 1 - передающая антенна устанавливается на трубчатой опоре, 0 - нет,
                                                                                     // Args[17] - truba_pm - 1/0, 1 - приемная антенна устанавливается на трубчатой опоре, 0 - нет,
                                                                                     // Args[18] - pass - 1/0, 1 - интервал с пассивным ретранслятором, 0 - нет,
                                                                                     // Args[19] - Otm_pd - м, отметка земли передающей антенны,
                                                                                     // Args[20] - Otm_pm - м, отметка земли приемной антенны,
                                                                                     // P_Vd_V0_teta_W[0] - Pпм - дБм, средний уровень мощности принимаемого сигнала,
                                                                                     // P_Vd_V0_teta_W[1] - Vmin0_d - дБ, минимально-допустимый множитель ослабления по замираниям в осадках (без учета деградации и среднего ослабления),
                                                                                     // P_Vd_V0_teta_W[2] - Vmin0_int_0 - дБ, минимально-допустимый множитель ослабления по интерференционным и субрефракционным замираниям (без учета деградации и среднего ослабления),
                                                                                     // P_Vd_V0_teta_W[3] - teta_AFT - дБ, затухание в АФТ,
                                                                                     // P_Vd_V0_teta_W[4] - W0 - дБ, ослабление в свободном пространстве.
                                                                                     // P_Vd_V0_teta_W[5] - Gpd_0_int - дБи, коэффициент усиления передающей антенны для расчета неустойчивости из-за рефракции и интерференции.
                                                                                     // P_Vd_V0_teta_W[6] - Gpm_0_int - дБи, коэффициент усиления приемной антенны для расчета неустойчивости из-за рефракции и интерференции.


    LRESULT FV_min_effFunction(COMPLEXARRAY * const V_min_eff,				 // Определяет эффективное значение запаса на замирания, предельное значение запаса на замирания и предельное значение запаса на замирания с учетом эквалайзера.
    							 COMPLEXARRAY * const Args);         								// Args[0] - C - Мбит/с, емкость системы,
                         												 								 // Args[1] - f - ГГц, рабочая частота,
                                                                                     // Args[2] - R - км, длина интервала,
                                                                                     // Args[3] - Vmin - дБ, запас на замирания для расчета интерференционной составляющей,
                                                                                     // Args[4] - Peres - 1/0, 1 - трасса пересеченная, 0 - трасса слабопересеченная,
                                                                                     // Args[5] - Suh - 1/0, 1 - трасса сухопутная, 0 - трасса над водными массивами или приморская
                                                                                     // Args[6] - Ekv - 1/0, 1 - есть эквалайзер, 0 - нет,
                                                                                     // Args[7] - hc - дБ, высота сигнатуры,
                                                                                     // Args[8] - delta_fc - МГц, ширина сигнатуры,
                                                                                     // Args[9] - Tip -  тип системы: 1 - 2ОФМ, 2 - 4ОФМ, 3 - 8ОФМ, 4 - 16КАМ, 5 - 32КАМ, 6 - 64КАМ,7 - 128КАМ (ТСМ), 8 - 256КАМ, 9 - 512КАМ,
                                                                                     // V_min_eff[0] - Vmin_eff - дБ, эффективное значение запаса на замирания,
                                                                                     // V_min_eff[1] - Vmin_eff_pred - дБ, предельное эффективное значение запаса на замирания,
                                                                                     // V_min_eff[2] - Vmin_eff_pred_ekv - дБ, предельное эффективное значение запаса на замирания с учетом эквалайзера,



    LRESULT FT_intFunction(COMPLEXARRAY * const T_int,				 // Определяет интерференционную составляющую неустойчивости.
    							 COMPLEXARRAY * const Args);         								// Args[0] - R - км, длина интервала,
                         												 								 // Args[1] - Q - , климатический коэффициент с учетом топографической особенности местности,
                                                                                     // Args[2] - f - ГГц, рабочая частота,
                                                                                     // Args[3] - Vmin_ef - дБ, эффективный запас на замирания для расчета интерференционной составляющей,
                                                                                     // Args[4] - Vmin - дБ, запас на плоские замирания для расчета интерференционной составляющей,
                                                                                     // Args[5] - V_50 - дБ, среднее ослабление из-за наклона трассы,
                                                                                     // Args[6] - Peres - 1 - трасса пересеченная, 0 - трасса слабопересеченная,
                                                                                     // Args[7] - Otm_z_pd - м, отметка земли станции слева,
                                                                                     // Args[8] - Otm_z_pm - м, отметка земли станции справа,
                                                                                     // Args[9] - H1 - м, высота центра раскрыва антенны станции слева,
                                                                                     // Args[10] - H2 - м, высота центра раскрыва антенны станции справа,
                                                                                     // T_int[0] - T_delta_epsilon - %, параметр, учитывающий вероятность возникновения многолучевых замираний,
                                                                                     //                              обусловленных отражениями радиоволн от слоистых неоднородностей тропосферы
                                                                                     //                              с перепадом диэлектрической проницаемости воздуха delta_epsilon,
                                                                                     // T_int[1] - T_int - %, интерференционная составляющая неустойчивости.
                                                                                     // T_int[2] - h_sr - м, средняя высота трассы над уровнем моря.
                                                                                     // T_int[3] - sigma0 - параметр сигма0.
                                                                                     // T_int[4] - sigma1 - параметр сигма1.
                                                                                     // T_int[5] - T_int_p - %, интерференционная составляющая неустойчивости для приземных трасс.
                                                                                     // T_int[6] - T_int_vg - %, интерференционная составляющая неустойчивости для высокогорных трасс.



    LRESULT FRek_raznFunction(COMPLEXARRAY * const Rek_razn,				 // Определяет рекомендуемый разнос слева и справа для ПРП.
    							 COMPLEXARRAY * const Args);         				 // Args[0] - Peres - 1 - пересеченная трасса, 0 - слабопересеченная,
                         																 // Args[1] - f - ГГц, рабочая частота,
                                                                         // Args[2] - R - км, протяженность интервала,
                                                                         // Args[3] - R_otr - км, расстояние до точки отражения,
                                                                         // Rek_razn[0] - м, рекомендуемый разнос антенн слева,
                                                                         // Rek_razn[1] - м, рекомендуемый разнос антенн справа.


    LRESULT FPRP_peresFunction(COMPLEXARRAY * const PRP,				 // Определяет выигрыш за счет ПРП, Тинт при ПРП, SESR при ПРП, Кнг при ПРП для пересеченных интервалов.
    							 COMPLEXARRAY * const Args);         				 // Args[0] - Suh - 1 - сухопутная трасса, 0 - приморская,
                         																 // Args[1] - f - ГГц, рабочая частота,
                                                                         // Args[2] - Delta_h - м, разнос между основной и дополнительной приемными антеннами,
                                                                         // Args[3] - alfa_tau_int - коэффициент для расчета неготовности по интерференции для основной приемной антенны,
                                                                         // Args[4] - alfa_tau_0 - коэффициент для расчета неготовности по рефракции,
                                                                         // Args[5] - T_int - %,неустойчивость из-за интерференции без учета резерва,
                                                                         // Args[6] - T_0 - %,неустойчивость из-за рефракции без учета резерва,
                                                                         // Args[7] - T_d - %,неустойчивость из-за осадков за год,
                                                                         // Args[8] - nu - раз, разница в усилениях основной и разнесенной антенн,
                                                                         // Args[9] - alfa_tau_int_d - коэффициент для расчета неготовности по интерференции для дополнительной приемной антенны,
                                                                         // PRP[0] - I_PRP раз, выигрыш за счет ПРП,
                                                                         // PRP[1] - T_int_PRP - %,неустойчивость из-за интерференции при ПРП,
                                                                         // PRP[2] - SESR_PRP - %,SESR при ПРП,
                                                                         // PRP[3] - K_ng - %,Кнг при ПРП.
                                                                         // PRP[4] - fi_tau_int - ,коэффициент готовности по интерференции при ПРП.


    LRESULT FPRP_slperesFunction(COMPLEXARRAY * const PRP,				 // Определяет SESR при ПРП, Кнг при ПРП для слабопересеченных интервалов.
    							 COMPLEXARRAY * const Args);         				 // Args[0] - Suh - 1 - сухопутная трасса, 0 - приморская,
                         																 // Args[1] - f - ГГц, рабочая частота,
                                                                         // Args[2] - R - км, длина интервала,
                                                                         // Args[3] - R_otr - км, расстояние до точки отражения,
                                                                         // Args[4] - f_p_g_a_u - , параметр для основной приемной антенны,
                                                                         // Args[5] - f_p_g_a_d - , параметр для дополнительной приемной антенны,
                                                                         // Args[6] - Delta_h - м, разнос между основной и дополнительной приемными антеннами,
                                                                         // Args[7] - alfa_tau_int_u - коэффициент для расчета неготовности по интерференции для основной приемной антенны,
                                                                         // Args[8] - alfa_tau_int_d - коэффициент для расчета неготовности по интерференции для дополнительной приемной антенны,
                                                                         // Args[9] - alfa_tau_0 - коэффициент для расчета неготовности по рефракции для основной приемной антенны,
                                                                         // Args[10] - Vmin_ef_u - дБ, эффективный запас на замирания для основной приемной антенны,
                                                                         // Args[11] - Vmin_ef_d - дБ, эффективный запас на замирания для дополнительной приемной антенны,
                                                                         // Args[12] - p_g_u - , относительный просвет в точке отражения при средней рефракции для основной приемной антенны,
                                                                         // Args[13] - p_g_d - , относительный просвет в точке отражения при средней рефракции для дополнительной приемной антенны,
                                                                         // Args[14] - T_int_u - %,неустойчивость из-за интерференции без учета резерва для основной приемной антенны,
                                                                         // Args[15] - T_int_d - %,неустойчивость из-за интерференции без учета резерва для дополнительной приемной антенны,
                                                                         // Args[16] - T_0 - %,неустойчивость из-за рефракции без учета резерва для основной приемной антенны,
                                                                         // Args[17] - T_d - %,неустойчивость из-за осадков за год для основной приемной антенны,
                                                                         // PRP[0] - SESR_PRP - %,SESR при ПРП,
                                                                         // PRP[1] - K_ng - %,Кнг при ПРП.
                                                                         // PRP[2] - fi_tau_int - , интерференционный коэффициент готовности при ПРП.

    LRESULT FNormyFunction(COMPLEXARRAY * const Normy,				 // Определяет норму на SESR и Кнг на интервал ЦРРЛ.
    							 COMPLEXARRAY * const Args);         				 // Args[0] - Dlina_RRL - км, суммарная длина всех интервалов ЦРРЛ,
                         																 // Args[1] - Naznachenye - назначение ЦРРЛ, 0 - сеть доступа, 1 - местная сеть, 2 - внутризоновая сеть, 3 - магистральная сеть, 4 - международный участок,
                                                                         // Args[2] - R - км, длина расчитываемого интервала ЦРРЛ,
                                                                         // Args[3] - Reconstr - 0 - линия вновь проектируемая (на проектируемых опорах), 1 - линия реконструируемая (на существующих опорах АРРЛ)
                                                                         // Normy[0] - %, норма на SESR на интервал,
                                                                         // Normy[1] - %, норма на Кнг на интервал.

    LRESULT FReadProfileFunction(COMPLEXARRAY * const Profile,
    										COMPLEXSCALAR * const flag);			 // Читает файл профиля.
                                                                      // flag - 1 - читать профиль, 0 - не читать профиль.


    BOOL BrowseForProfile(char *file_pathname, HANDLE hInstance);//заполняет массив на который указывает file_pathname путем
																				//к выбранному файлу профиля, возвращвет TRUE. Если профиль не
                                                            //выбран - возвращает FALSE

    LRESULT FZigzagoobraznostFunction(COMPLEXARRAY * const Zigzagoobraznost,
    										COMPLEXARRAY * const Args);				 // Вычисляет неустойчивость связи из-за помех прямого прохождения.
                                                                      // Args[0] - Tip, тип системы: 1 - 2ОФМ,
                                                                      //													  2 - 4ОФМ,
                                                                      //													  3 - 8ОФМ,
                                                                      //                                         4 - 16КАМ,
                                                                      //                                         5 - 32КАМ,
                                                                      //                                         6 - 64КАМ,
                                                                      //                                         7 - 128КАМ (ТСМ),
                                                                      //                                         8 - 256КАМ,
                                                                      //                                         9 - 512КАМ;
                                                                      // Args[1] - C, скорость передачи, МБит/с;
                                                                      // Args[2] - f, частота, ГГц;
                                                                      // Args[3] - Shiroty, широты: 0 - умеренные (50 - 65 сш),
                                                                      //													 1 - полярные (севернее 65 сш),
                                                                      //													 2 - южные (юнее  50 сш);
                                                                      // Args[4] - Mor, характер трассы: 0 - сухопутная,
                                                                      //                                 1 - приморская,
                                                                      //                                 2 - морская,
                                                                      //											3 - смешанная;
                                                                      // Args[5] - F_pd_1, дБ, ослабление меш. сигн. из-за ДНА меш. ант.;
                                                                      // Args[6] - F_pm_4, дБ, ослабление меш. сигн. из-за ДНА ПМ ант.;
                                                                      // Args[7] - delta_P_pd, дБ, P_pd_mesh-P_pd_pol;
                                                                      // Args[8] - ARM, дБ;
                                                                      // Args[9] - delta_G_pd, дБ, G_pd_mesh-G_pd_pol;
                                                                      // Args[10] - delta_eta_pd, дБ, eta_pd_mesh("+")-eta_pd_pol("+");
                                                                      // Args[11] - R_j, км, протяженность интервала на котором оценивается помеха;
                                                                      // Args[12] - V_j, дБ, средний множитель ослабления полезного сигнала;
                                                                      // Args[13] - V_g_m, дБ, множитель ослабления в газах мешающего сигнала;
                                                                      // Args[14] - K_f, климатический фактор;
                                                                      // Args[15] - R_m_i, км, длина интервала мешающего сигнала;
                                                                      // Args[16] - R_1, км, расстояние до критического преп. на трассе мешающего сигнала;
                                                                      // Args[17] - H, м, просвет в т. критического преп. на трассе мешающего сигнала;
                                                                      // Args[18] - R_m_suh, км, Протяженность сухопутного участка смешанной трассы мешающего сигнала
                                                                      // Args[19] - R_m_prim, км, Протяженность приморского участка смешанной трассы мешающего сигнала
                                                                      // Args[20] - R_m_mor, км, Протяженность морского участка смешанной трассы мешающего сигнала
                                                                      // Zigzagoobraznost[0] - Z_max, дБ;
                                                                      // Zigzagoobraznost[1] - F_sum, дБ, F_pd_1+F_pm_4;
                                                                      // Zigzagoobraznost[2] - Z_R_V_P_G_eta_sum, дБ;
                                                                      // Zigzagoobraznost[3] - Z_por_dop, дБ;
                                                                      // Zigzagoobraznost[4] - V_m_dop, дБ;
                                                                      // Zigzagoobraznost[5] - Tetta, рад;
                                                                      // Zigzagoobraznost[6] - R_e_m, км, эквивалентная длина трассы мешающего сигнала;
                                                                      // Zigzagoobraznost[7] - L;
                                                                      // Zigzagoobraznost[8] - F_ot_f;
                                                                      // Zigzagoobraznost[9] - ksi;
                                                                      // Zigzagoobraznost[10] - A_m;
                                                                      // Zigzagoobraznost[11] - B_m;
                                                                      // Zigzagoobraznost[12] - ksi_suh;
                                                                      // Zigzagoobraznost[13] - F_ot_f_suh, дБ, для приморских и смешанных трасс;
                                                                      // Zigzagoobraznost[14] - V_m_suh, дБ, для приморских и смешанных трасс;
                                                                      // Zigzagoobraznost[15] - ksi_mor, для приморских и смешанных трасс;
                                                                      // Zigzagoobraznost[16] - F_ot_f_mor, дБ, для приморских и смешанных трасс;
                                                                      // Zigzagoobraznost[17] - V_m_mor, дБ;
                                                                      // Zigzagoobraznost[18] - V_m_prim, дБ;
                                                                      // Zigzagoobraznost[19] - V_m_smesh, дБ;
                                                                      // Zigzagoobraznost[20] - T_1_4, %;





	 double V_m_suh_mor_prim(double T,double *Args);//Вычисляет значение V_m для сухопутных, морских, приморских и смешанных трасс,
    															  //при расчете помех прямого прохождения на приморских трассах
                                                  //T - %, неустойчивость связи за счет помех прямого прохождения
                                                  //Args[0] - R_e_m, км, эквивалентная длина трассы мешающего сигнала;
                                                  //Args[1] - K_f, климатический фактор;
                                                  //Args[2] - Mor - характер трассы: 0 - сухопутная;
                                                  //                       			 1 - приморская;
                                                  //                       			 2 - морская;
                                                  //                                 3 - смешанная
                                                  //Args[3] - F_ot_f_suh, для сухопутных трасс;
                                                  //Args[4] - F_ot_f_mor, для приморских трасс;
                                                  //Args[5] - Shiroty, широты: 0 - умеренные (50 - 65 сш),
                                                  //									 2 - южные (юнее  50 сш).
                                                  // Args[6] - R_m_suh, км, Протяженность сухопутного участка смешанной трассы мешающего сигнала
                                                  // Args[7] - R_m_mor, км, Протяженность морского участка смешанной трассы мешающего сигнала
                                                  // Args[8] - R_m, км, Протяженность трассы мешающего сигнала


    LRESULT FSaveDXFProfileFunction(COMPLEXSCALAR * const Flag,
    										COMPLEXARRAY * const Args);//Предлагает сохранить и сохраняет чертеж профиля в файл DXF
    															  //Flag - 1 - Файл сохранен, 0 - файл не сохранен (нажата кнопка отмены)
                                                  //Args[0] - Количество символов в пути и имени файла профиля 001
                                                  //Args[1] - Строка пути и имени файла профиля 001
                                                  //Args[2] - Значение градиента диэлектрической проницаемости воздуха (1/м)
                                                  //Args[3] - Значение рабочей частоты (ГГц)
                                                  //Args[4] - Значение погрешности отметок земли (м)
                                                  //Args[5] - Значение высоты центра раскрыва основной антенны слева (м)
                                                  //Args[6] - Значение высоты центра раскрыва основной антенны справа (м)
                                                  //Args[7] - Флаг наличия пространственно-разнесенного приема: 0 - ПРП нет, 1 - ПРП есть
                                                  //Args[8] - Значение высоты центра раскрыва дополнительной антенны слева (м)
                                                  //Args[9] - Значение высоты центра раскрыва дополнительной антенны справа (м)


    LRESULT FReflection_pointFunction(COMPLEXARRAY * const Reflection_point, // определяет точку отражения на профиле
                                       COMPLEXARRAY * const Prepyatstvie,
                                       COMPLEXARRAY * const Relyef,
     												COMPLEXARRAY * const Args);
																				//Reflection_point[0] - R_otr - км, расстояние до точки отражения
                                                            //Reflection_point[1] - H_otr - м, абсолютный просвет в точке отражения
                                                            //Reflection_point[2] - H0_otr - м, оптимальный просвет в точке отражения
                                                            //Reflection_point[3] - p_otr - , относительный просвет в точке отражения
                                                            //Reflection_point[4] - R_beg_appr - км, расстояние до начала области аппроксимации
                                                            //Reflection_point[5] - R_end_appr - км, расстояние до конца области аппроксимации
                                                            //Reflection_point[6] - X - км, продольный размер области отражения
                                                            //Reflection_point[7] - Delta_h_max - м, максимальная высота неровностей рельефа, при которой участок трассы можно считать плоским
                                                            //Reflection_point[8] - A0 - , свободный член аппроксимирующего полинома 2-й степени
                                                            //Reflection_point[9] - A1 - , коэффициент аппроксимирующего полинома 2-й степени при x в первой степени
                                                            //Reflection_point[10] - A2 - , коэффициент аппроксимирующего полинома 2-й степени при x во второй степени
                                                            //Reflection_point[11] - l_under_profile - км, суммарная протяженность участков пересечения падающего и отраженного лучей с профилем с учетом минусовой погрешности рельефа в месте пересечения.
                                                            //Prepyatstvie - массив начала, конца, высоты и типа препятствий
                                                            //Relyef - массив расстояний и отметок рельефа
                                                            //Args[0] - N_prep - количество высотных препятствий на профиле
                                                            //Args[1] - N_relyef - количество высотных отметок рельефа на профиле
                                                            //Args[2] - a_e - км, эквивалентный радиус земли
                                                            //Args[3] - Delta_H - м, погрешность отметок рельефа
                                                            //Args[4] - H1 - м, высота центра раскрыва антенны слева
                                                            //Args[5] - H2 - м, высота центра раскрыва антенны справа
                                                            //Args[6] - f - ГГц, рабочая частота


	BOOL parab_reflect(double far *A, double R, double Y1, double Y2, double far * otr);//Определяет параметры отражения от параболы.
   																												 //Возвращает true - если есть точка отражения, false - если нет
                                                                                        //A - указатель на массив с коэффициентами полинома. Элементу с большим индексом соответствует более старший коэффициент.
                                                                                        //R - м, длина анализируемого интервала.
                                                                                        //Y1 - м, абсолютная отметка излучателя (слева).
                                                                                        //Y2 - м, абсолютная отметка приемника (справа).
                                                                                        //otr[0] - D - Дискрминант для отыскания величины a при А[2]<=0 или величины b при А[2]>=0.
                                                                                        //otr[1] - a - Левая граница участка поиска точки отражения.
                                                                                        //otr[2] - b - Правая граница участка поиска точки отражения.
                                                                                        //otr[3] - Xотр - Координата Х точки отражения.
                                                                                        //otr[4] - Tg_alfa - Тангенс угла падения (отражения) к касательной.
                                                                                        //otr[5] - k_kas_parab - Угловой коэффициент касательной к параболе в точке отражения.
                                                                                        //otr[6] - k_lev - Угловой коэффициент падающего (слева) луча.
                                                                                        //otr[7] - k_prav - Угловой коэффициент отраженного (справа) луча.


   double Razn_tg_otr(double x,double far *Args);//Определяет разницу между тангенсами углов падения слева и справа на параболу в точке х
                                                                               //A - указатель на массив с коэффициентами полинома. Элементу с большим индексом соответствует более старший коэффициент.
                                                                               //R - м, длина анализируемого интервала.
                                                                               //Y1 - м, абсолютная отметка излучателя (слева).
                                                                               //Y2 - м, абсолютная отметка приемника (справа).
                                                                               //х - м, координата точки падения





   BOOL CALLBACK MPR_DialogFunc(

    	HWND  hwndDlg,	// handle of dialog box
    	UINT  uMsg,	// message
    	WPARAM  wParam,	// first message parameter
    	LPARAM  lParam 	// second message parameter
   	);














/*
//Константы для диалога изменения расстояния до точки отражения
   #define ID_Obyasnenie 101
   #define ID_Imya_peremennoy 103
   #define ID_Peremennaya 107
*/







    FUNCTIONINFO    FRelyef =
    {
    "FRelyef",                          // Name by which mathcad will recognize the function
    "profile,N_relyef",                              // FRelyef will be called as FRelyef(profile,N_relyef)
    "Vozvrashaet massiv vysotnyh otmetok relyefa",      // description of FRelyef(profile,N_relyef)
    (LPCFUNCTION)FRelyefFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    2,                                  //  the function takes on 2 arguments
    { COMPLEX_ARRAY, COMPLEX_SCALAR}   // arguments are complex array and scalar
    };

    FUNCTIONINFO    FN_prep_c =
    {
    "FN_prep_c",                          // Name by which mathcad will recognize the function
    "N_prep,N_relyef,profile,Relyef",     // FN_prep_c will be called as FN_prep_c(N_prep,N_relyef,profile,Relyef)
    "Vozvrashaet kolichestvo prepyatstviy bez perelomov",      // description of FN_prep_c(N_prep,N_relyef,profile,Relyef)
    (LPCFUNCTION)FN_prep_cFunction,       // pointer to the executible code
    COMPLEX_SCALAR,                     // the return type is also a complex scalar
    4,                                  //  the function takes on 4 arguments
    { COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_ARRAY,COMPLEX_ARRAY }   // types of arguments
    };

    FUNCTIONINFO    FPrepyatstvie =
    {
    "FPrepyatstvie",                          // Name by which mathcad will recognize the function
    "N_prep,N_relyef,profile,Relyef",     // FPrepyatstvie will be called as FPrepyatstvie(N_prep,N_relyef,profile,Relyef)
    "Vozvrashaet massiv prepyatstviy",      // description of FPrepyatstvie(N_prep,N_relyef,profile,Relyef)
    (LPCFUNCTION)FPrepyatstvieFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    4,                                  //  the function takes on 4 arguments
    { COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_ARRAY,COMPLEX_ARRAY }   // types of arguments
    };

    FUNCTIONINFO    FVoda_uch =
    {
    "FVoda_uch",                          // Name by which mathcad will recognize the function
    "N_voda_c,N_voda,N_relyef,N_prep,profile",     // FVoda_uch will be called as FVoda_uch(N_voda_c,N_voda,N_relyef,N_prep,profile)
    "Vozvrashaet massiv vodnyh uchastkov",      // description of FVoda_uch(N_voda_c,N_voda,N_relyef,N_prep,profile)
    (LPCFUNCTION)FVoda_uchFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    5,                                  //  the function takes on 5 arguments
    { COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_ARRAY }   // types of arguments
    };

    FUNCTIONINFO    Fa_e =
    {
    "Fa_e",                          // Name by which mathcad will recognize the function
    "g0",     // Fa_e will be called as Fa_e(g0)
    "Vozvrashaet ekvivalentnyi radius zemli",      // description of Fa_e(g0)
    (LPCFUNCTION)Fa_eFunction,       // pointer to the executible code
    COMPLEX_SCALAR,                     // the return type is also a complex scalar
    1,                                  //  the function takes on 1 argument
    { COMPLEX_SCALAR}   // types of arguments
    };

    FUNCTIONINFO    Fy_x =
    {
    "Fy_x",                          // Name by which mathcad will recognize the function
    "x,R,a_e,Hor",     // Fa_e will be called as Fy_x(x,R,a_e,Hor)
    "Vozvrashaet vysotu zemnogo segmenta",      // description of Fy_x(x,R,a_e,Hor)
    (LPCFUNCTION)Fy_xFunction,       // pointer to the executible code
    COMPLEX_SCALAR,                     // the return type is also a complex scalar
    4,                                  //  the function takes on 4 arguments
    { COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR}   // types of arguments
    };

    FUNCTIONINFO    FPrep =
    {
    "FPrep",                          // Name by which mathcad will recognize the function
    "N_prep_c,R,Prepyatstvie",     // FPrep will be called as FPrep(N_prep_c,R,Prepyatstvie)
    "Preobrazovyvaet massiv Prepyatstvie k udobnomu vidu Prep dlya interpolyacii",      // description of FPrep(N_prep_c,R,Prepyatstvie)
    (LPCFUNCTION)FPrepFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    3,                                  //  the function takes on 3 arguments
    { COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_ARRAY}   // types of arguments
    };

    FUNCTIONINFO    Fr =
    {
    "Fr",                          // Name by which mathcad will recognize the function
    "x,Relyef,Hor,a_e,R,N_relyef",     // Fr will be called as Fr(x,Relyef,Hor,a_e,R,N_relyef)
    "Vichislyaet visotnuyu otmetky relyefa putem lineynoy interpolyacii",      // description of Fr(x,Relyef,Hor,a_e,R,N_relyef)
    (LPCFUNCTION)FrFunction,       // pointer to the executible code
    COMPLEX_SCALAR,                     // the return type is also a complex scalar
    6,                                  //  the function takes on 6 arguments
    { COMPLEX_SCALAR,COMPLEX_ARRAY,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR}   // types of arguments
    };

    FUNCTIONINFO    Fp =
    {
    "Fp",                          // Name by which mathcad will recognize the function
    "x,Prep,N_prep_c",     // Fp will be called as Fp(x,Prep,N_prep_c)
    "Vichislyaet visotu prepyatstvya putem lineynoy interpolyacii",      // description of Fp(x,Prep,N_prep_c)
    (LPCFUNCTION)FpFunction,       // pointer to the executible code
    COMPLEX_SCALAR,                     // the return type is also a complex scalar
    3,                                  //  the function takes on 3 arguments
    { COMPLEX_SCALAR,COMPLEX_ARRAY,COMPLEX_SCALAR}   // types of arguments
    };

    FUNCTIONINFO    Fsurf =
    {
    "Fsurf",                          // Name by which mathcad will recognize the function
    "x,Prep,N_prep_c,Relyef,Hor,a_e,R,N_relyef,Delta_H",     // Fsurf will be called as Fsurf(x,Prep,N_prep_c,Relyef,Hor,a_e,R,N_relyef,Delta_H)
    "Vichislyaet visotu poverhnosti profilya s uchetom pogreshnosti putem lineynoy interpolyacii",      // description of Fsurf(x,Prep,N_prep_c,Relyef,Hor,a_e,R,N_relyef,Delta_H)
    (LPCFUNCTION)FsurfFunction,       // pointer to the executible code
    COMPLEX_SCALAR,                     // the return type is also a complex scalar
    9,                                  //  the function takes on 9 arguments
    { COMPLEX_SCALAR,COMPLEX_ARRAY,COMPLEX_SCALAR,COMPLEX_ARRAY,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR}   // types of arguments
    };

    FUNCTIONINFO    Fluch =
    {
    "Fluch",                          // Name by which mathcad will recognize the function
    "x,H1,H2,R,r_ot_0,r_ot_R",     // Fluch will be called as Fluch(x,H1,H2,R,r_ot_0,r_ot_R)
    "Vichislyaet vysotu lucha",      // description of Fluch(x,H1,H2,R,r_ot_0,r_ot_R)
    (LPCFUNCTION)FluchFunction,       // pointer to the executible code
    COMPLEX_SCALAR,                     // the return type is also a complex scalar
    6,                                  //  the function takes on 6 arguments
    { COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR}   // types of arguments
    };

    FUNCTIONINFO    FR_pr =
    {
    "FR_pr",                          // Name by which mathcad will recognize the function
    "Prep,Relyef,Args",     // FR_pr will be called as FR_pr(Prep,Relyef,Args)
    "Vichislyaet rasstoyanye do prepyatstvya",      // description of FR_pr(Prep,Relyef,Args)
    (LPCFUNCTION)FR_prFunction,       // pointer to the executible code
    COMPLEX_SCALAR,                     // the return type is also a complex scalar
    3,                                  //  the function takes on 3 arguments
    { COMPLEX_ARRAY,COMPLEX_ARRAY,COMPLEX_ARRAY}   // types of arguments
    };




     FUNCTIONINFO    FHorda_Krit_Prep =
    {
    "FHorda_Krit_Prep",                          // Name by which mathcad will recognize the function
    "Prep,Relyef,Args",     // FHorda_Krit_Prep will be called as FHorda_Krit_Prep(Prep,Relyef,Args)
    "Vichislyaet parametry hordy",      // description of FHorda_Krit_Prep(Prep,Relyef,Args)
    (LPCFUNCTION)FHorda_Krit_PrepFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    3,                                  //  the function takes on 3 arguments
    { COMPLEX_ARRAY,COMPLEX_ARRAY,COMPLEX_ARRAY}   // types of arguments
    };

 

    FUNCTIONINFO    FSlinear =
    {
    "FSlinear",                          // Name by which mathcad will recognize the function
    "M,N",     // FSlinear will be called as FSlinear(M,N)
    "Reshaet systemu lineynyh uravnenyi",      // description of FSlinear(M,N)
    (LPCFUNCTION)FSlinearFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    2,                                  //  the function takes on 2 arguments
    { COMPLEX_ARRAY,COMPLEX_SCALAR}   // types of arguments
    };

    FUNCTIONINFO    FInterp =
    {
    "FInterp",                          // Name by which mathcad will recognize the function
    "X,Y,x",     // FInterp will be called as FInterp(X,Y,x)
    "Opredelyaet znachenie funkcii polinomialnoy interpolyaciey",      // description of FInterp(X,Y,x)
    (LPCFUNCTION)FInterpFunction,       // pointer to the executible code
    COMPLEX_SCALAR,                     // the return type is also a complex scalar
    3,                                  //  the function takes on 3 arguments
    { COMPLEX_ARRAY,COMPLEX_ARRAY,COMPLEX_SCALAR}   // types of arguments
    };

    FUNCTIONINFO    FInterp1 =
    {
    "FInterp1",                          // Name by which mathcad will recognize the function
    "X,Y,x",     // FInterp will be called as FInterp1(X,Y,x)
    "Opredelyaet znachenie funkcii interpolyaciey polinomom 2-j stepeni na kajdom intervale",      // description of FInterp1(X,Y,x)
    (LPCFUNCTION)FInterp1Function,       // pointer to the executible code
    COMPLEX_SCALAR,                     // the return type is also a complex scalar
    3,                                  //  the function takes on 3 arguments
    { COMPLEX_ARRAY,COMPLEX_ARRAY,COMPLEX_SCALAR}   // types of arguments
    };

    FUNCTIONINFO    FInterp2 =
    {
    "FInterp2",                          // Name by which mathcad will recognize the function
    "X,Y,x",     // FInterp will be called as FInterp2(X,Y,x)
    "Opredelyaet znachenie funkcii interpolyaciey polinomom 2-j stepeni na kajdom intervale",      // description of FInterp2(X,Y,x)
    (LPCFUNCTION)FInterp2Function,       // pointer to the executible code
    COMPLEX_SCALAR,                     // the return type is also a complex scalar
    3,                                  //  the function takes on 3 arguments
    { COMPLEX_ARRAY,COMPLEX_ARRAY,COMPLEX_SCALAR}   // types of arguments
    };


    FUNCTIONINFO    FQ =
    {
    "FQ",                          // Name by which mathcad will recognize the function
    "f,R,g,sigma,Suh,Peres,R_otr,H,K_vp",     // FQ will be called as FQ(f,R,g,sigma,Suh,Peres,R_otr,H,K_vp)
    "Opredelyaet znachenie Aотр.,F(p(g),A),Q",      // description of FQ(f,R,g,sigma,Suh,Peres,R_otr,H,K_vp)
    (LPCFUNCTION)FQFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    8,                                  //  the function takes on 8 arguments
    { COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_ARRAY}   // types of arguments
    };



    FUNCTIONINFO    FChrp =
    {
    "FChrp",                          // Name by which mathcad will recognize the function
    "K,N,N_ChRP,df,f,Kint,Tint_refr_d,Suh_Peres,Alfaint_refr,p_ot_g",     // FChrp will be called as FChrp(K,N,N_ChRP,df,f,Kint,Tint_refr_d,Suh_Peres,Alfaint_refr,p_ot_g)
    "Opredelyaet znachenie SESR i Kng na uchastke rezervirovaniya pri chrp.",      // description of FChrp(K,N,N_ChRP,df,f,Kint,Tint_refr_d,Suh_Peres,Alfaint_refr,p_ot_g)
    (LPCFUNCTION)FChrpFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    10,                                  //  the function takes on 10 arguments
    { COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_ARRAY,COMPLEX_ARRAY,COMPLEX_ARRAY,COMPLEX_ARRAY,COMPLEX_ARRAY}   // types of arguments
    };


    FUNCTIONINFO    FDeltaVdegr =
    {
    "FDeltaVdegr",                          // Name by which mathcad will recognize the function
    "Tip,Vmin0,Nmesh,R,Vsr,Mmesh,Zpordops,MCochannel,C",     // FDeltaVdegr will be called as FDeltaVdegr(Tip,Vmin0,Nmesh,R,Vsr,Mmesh,Zpordops,MCochannel,C)
    "Opredelyaet znachenie DeltaVdegr",      // description of FDeltaVdegr(Tip,Vmin0,Nmesh,R,Vsr,Mmesh,Zpordops,MCochannel,C)
    (LPCFUNCTION)FDeltaVdegrFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex ARRAY
    9,                                  //  the function takes on 7 arguments
    { COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_ARRAY,COMPLEX_SCALAR,COMPLEX_ARRAY,COMPLEX_SCALAR }   // types of arguments
    };



    FUNCTIONINFO    FTd =
    {
    "FTd",                          // Name by which mathcad will recognize the function
    "f,R,Pol,Vmin,Region",     // FTd will be called as FTd(f,R,Pol,Vmin,Region)
    "Opredelyaet znachenie Неустойчивости из-за влияния осадков за наихудший месяц",      // description of FTd(f,R,Pol,Vmin,Region)
    (LPCFUNCTION)FTdFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    5,                                  //  the function takes on 5 arguments
    { COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_SCALAR}   // types of arguments
    };



    FUNCTIONINFO    FNegotovnost =
    {
    "FNegotovnost",                          // Name by which mathcad will recognize the function
    "Args",     // FNegotovnost will be called as FNegotovnost(Args)
    "Opredelyaet коэффициенты готовности, SESR, коэффициент неготовности",      // description of FNegotovnost(Args)
    (LPCFUNCTION)FNegotovnostFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    1,                                  //  the function takes on 1 argument
    { COMPLEX_ARRAY}   // types of arguments
    };


    FUNCTIONINFO    FVgaz =
    {
    "FVgaz",                          // Name by which mathcad will recognize the function
    "Args",     // FVgaz will be called as FVgaz(Args)
    "Opredelyaet ослабление сигнала в газах атмосферы",      // description of FVgaz(Args)
    (LPCFUNCTION)FVgazFunction,       // pointer to the executible code
    COMPLEX_SCALAR,                     // the return type is also a complex scalar
    1,                                  //  the function takes on 1 argument
    { COMPLEX_ARRAY}   // types of arguments
    };


    FUNCTIONINFO    FBERPpm =
    {
    "FBERPpm",                          // Name by which mathcad will recognize the function
    "Args",     // FBERPpm will be called as FBERPpm(Args)
    "Opredelyaet значения коэффициента ошибок и порогового уровня",      // description of FBERPpm(Args)
    (LPCFUNCTION)FBERPpmFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    1,                                  //  the function takes on 1 argument
    { COMPLEX_ARRAY}   // types of arguments
    };


    FUNCTIONINFO    FVoda =
    {
    "FVoda",                          // Name by which mathcad will recognize the function
    "voda,Args",     // FVoda will be called as FVoda(voda,Args)
    "Заполняет массив водных участков",      // description of FVoda(voda,Args)
    (LPCFUNCTION)FVodaFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    2,                                  //  the function takes on 2 argument
    { COMPLEX_ARRAY,COMPLEX_ARRAY}   // types of arguments
    };


    FUNCTIONINFO    FPrep_Les =
    {
    "FPrep_Les",                          // Name by which mathcad will recognize the function
    "N_prep_c,R,Prepyatstvie,Hor,Relyef,a_e,N_relyef,N_prep",     // FPrep_Les will be called as FPrep_Les(N_prep_c,R,Prepyatstvie,Hor,Relyef,a_e,N_relyef,N_prep)
    "Preobrazovyvaet massiv Prepyatstvie k udobnomu vidu Prep_Les dlya interpolyacii лесных массивов",      // description of FPrep_Les(N_prep_c,R,Prepyatstvie,Hor,Relyef,a_e,N_relyef,N_prep)
    (LPCFUNCTION)FPrep_LesFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    7,                                  //  the function takes on 7 argument
    { COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_ARRAY,COMPLEX_SCALAR,COMPLEX_ARRAY,COMPLEX_SCALAR,COMPLEX_SCALAR}   // types of arguments
    };


    FUNCTIONINFO    FPrep_Dom =
    {
    "FPrep_Dom",                          // Name by which mathcad will recognize the function
    "N_prep_c,R,Prepyatstvie,Hor,Relyef,a_e,N_relyef,N_prep",     // FPrep_Dom will be called as FPrep_Dom(N_prep_c,R,Prepyatstvie,Hor,Relyef,a_e,N_relyef,N_prep)
    "Preobrazovyvaet massiv Prepyatstvie k udobnomu vidu Prep_Dom dlya interpolyacii застройки",      // description of FPrep_Dom(N_prep_c,R,Prepyatstvie,Hor,Relyef,a_e,N_relyef,N_prep)
    (LPCFUNCTION)FPrep_DomFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    7,                                  //  the function takes on 7 argument
    { COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_ARRAY,COMPLEX_SCALAR,COMPLEX_ARRAY,COMPLEX_SCALAR,COMPLEX_SCALAR}   // types of arguments
    };


    FUNCTIONINFO    FK_vp =
    {
    "FK_vp",                          // Name by which mathcad will recognize the function
    "N_voda,R,Voda_uch",     // FK_vp will be called as FK_vp(N_voda,R,Voda_uch)
    "Определяет коэффициент водной поверхности",      // description of FK_vp(N_voda,R,Voda_uch)
    (LPCFUNCTION)FK_vpFunction,       // pointer to the executible code
    COMPLEX_SCALAR,                     // the return type is also a complex scalar
    3,                                  //  the function takes on 3 argument
    { COMPLEX_SCALAR,COMPLEX_SCALAR,COMPLEX_ARRAY}   // types of arguments
    };



    FUNCTIONINFO    Fg_sr_sigma =
    {
    "Fg_sr_sigma",                          // Name by which mathcad will recognize the function
    "Raion",     // Fg_sr_sigma will be called as Fg_sr_sigma(Raion)
    "Определяет средний градиент диэлектрической проницаемости воздуха и его СКО",      // description of Fg_sr_sigma(Raion)
    (LPCFUNCTION)Fg_sr_sigmaFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    1,                                  //  the function takes on 1 argument
    { COMPLEX_SCALAR}   // types of arguments
    };


    FUNCTIONINFO    Fsigma_ot_R =
    {
    "Fsigma_ot_R",                          // Name by which mathcad will recognize the function
    "Args",     // Fsigma_ot_R will be called as Fsigma_ot_R(Args)
    "Определяет СКО градиента диэлектрической проницаемости воздуха",      // description of Fsigma_ot_R(Args)
    (LPCFUNCTION)Fsigma_ot_RFunction,       // pointer to the executible code
    COMPLEX_SCALAR,                     // the return type is also a complex scalar
    1,                                  //  the function takes on 1 argument
    { COMPLEX_ARRAY}   // types of arguments
    };


    FUNCTIONINFO    FTdy =
    {
    "FTdy",                          // Name by which mathcad will recognize the function
    "Args",     // FTdy will be called as FTdy(Args)
    "Определяет коэффициент Qd и неустойчивость по дождям в год",      // description of FTdy(Args)
    (LPCFUNCTION)FTdyFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    1,                                  //  the function takes on 1 argument
    { COMPLEX_ARRAY}   // types of arguments
    };


    FUNCTIONINFO    FP_Vd_V0_teta_W =
    {
    "FP_Vd_V0_teta_W",                          // Name by which mathcad will recognize the function
    "Args",     // FP_Vd_V0_teta_W will be called as FP_Vd_V0_teta_W(Args)
    "Определяет средний уровень мощности принимаемого сигнала,\
    минимально-допустимый множитель ослабления по замираниям в осадках (без учета деградации),\
    минимально-допустимый множитель ослабления по интерференционным и субрефракционным замираниям (без учета деградации),\
    затухание в АФТ,\
    ослабление сигнала в свободном пространстве",// description of FP_Vd_V0_teta_W(Args)
    (LPCFUNCTION)FP_Vd_V0_teta_WFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    1,                                  //  the function takes on 1 argument
    { COMPLEX_ARRAY}   // types of arguments
    };


    FUNCTIONINFO    FV_min_eff =
    {
    "FV_min_eff",                          // Name by which mathcad will recognize the function
    "Args",     // FV_min_eff will be called as FV_min_eff(Args)
    "Определяет эффективное значение запаса на замирания,\
     предельное значение запаса на замирания и предельное\
     значение запаса на замирания с учетом эквалайзера.",// description of FV_min_eff(Args)
    (LPCFUNCTION)FV_min_effFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    1,                                  //  the function takes on 1 argument
    { COMPLEX_ARRAY}   // types of arguments
    };



    FUNCTIONINFO    FT_int =
    {
    "FT_int",                          // Name by which mathcad will recognize the function
    "Args",     // FT_int will be called as FT_int(Args)
    "Определяет интерференционную составляющую неустойчивости.",// description of FT_int(Args)
    (LPCFUNCTION)FT_intFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    1,                                  //  the function takes on 1 argument
    { COMPLEX_ARRAY}   // types of arguments
    };



    FUNCTIONINFO    FRek_razn =
    {
    "FRek_razn",                          // Name by which mathcad will recognize the function
    "Args",     // FRek_razn will be called as FRek_razn(Args)
    "Определяет рекомендуемый разнос слева и справа для ПРП.",// description of FRek_razn(Args)
    (LPCFUNCTION)FRek_raznFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    1,                                  //  the function takes on 1 argument
    { COMPLEX_ARRAY}   // types of arguments
    };


    FUNCTIONINFO    FPRP_peres =
    {
    "FPRP_peres",                          // Name by which mathcad will recognize the function
    "Args",     // FPRP_peres will be called as FPRP_peres(Args)
    "Определяет выигрыш за счет ПРП, Тинт при ПРП, SESR при ПРП, Кнг при ПРП.",// description of FPRP_peres(Args)
    (LPCFUNCTION)FPRP_peresFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    1,                                  //  the function takes on 1 argument
    { COMPLEX_ARRAY}   // types of arguments
    };


    FUNCTIONINFO    FPRP_slperes =
    {
    "FPRP_slperes",                          // Name by which mathcad will recognize the function
    "Args",     // FPRP_slperes will be called as FPRP_slperes(Args)
    "Определяет SESR при ПРП, Кнг при ПРП для слабопересеченных интервалов.",// description of FPRP_slperes(Args)
    (LPCFUNCTION)FPRP_slperesFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    1,                                  //  the function takes on 1 argument
    { COMPLEX_ARRAY}   // types of arguments
    };


    FUNCTIONINFO    FNormy =
    {
    "FNormy",                          // Name by which mathcad will recognize the function
    "Args",     // FNormy will be called as FNormy(Args)
    "Определяет норму на SESR и Кнг на интервал ЦРРЛ.",// description of FNormy(Args)
    (LPCFUNCTION)FNormyFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    1,                                  //  the function takes on 1 argument
    { COMPLEX_ARRAY}   // types of arguments
    };


    FUNCTIONINFO    FReadProfile =
    {
    "FReadProfile",                          // Name by which mathcad will recognize the function
    "flag",     // FReadProfile will be called as FReadProfile(flag)
    "Читает файл профиля.",// description of FReadProfile()
    (LPCFUNCTION)FReadProfileFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    1,                                  //  the function takes on 0 argument
    {COMPLEX_SCALAR }   // types of arguments
    };


    FUNCTIONINFO    FZigzagoobraznost =
    {
    "FZigzagoobraznost",                          // Name by which mathcad will recognize the function
    "Args",     // FZigzagoobraznost will be called as FZigzagoobraznost(Args)
    "Вычисляет неустойчивость из-за помех прямого прохождения",// description of FZigzagoobraznost()
    (LPCFUNCTION)FZigzagoobraznostFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is also a complex array
    1,                                  //  the function takes on 1 argument
    {COMPLEX_ARRAY }   // types of arguments
    };


    FUNCTIONINFO    FSaveDXFProfile =
    {
    "FSaveDXFProfile",                          // Name by which mathcad will recognize the function
    "Args",     // FSaveDXFProfile will be called as FSaveDXFProfile(Args)
    "Предлагает сохранить и сохраняет чертеж профиля в файл DXF",// description of FSaveDXFProfile(Args)
    (LPCFUNCTION)FSaveDXFProfileFunction,       // pointer to the executible code
    COMPLEX_SCALAR,                     // the return type is a complex scalar
    1,                                  //  the function takes on 1 argument
    {COMPLEX_ARRAY }   // types of arguments
    };


    FUNCTIONINFO    FReflection_point =
    {
    "FReflection_point",                          // Name by which mathcad will recognize the function
    "Prepyatstvie,Relyef,Args",     // FReflection_point will be called as FReflection_point(Prepyatstvie,Relyef,Args)
    "Определяет точку отражения на профиле",// description of FReflection_point(Prepyatstvie,Relyef,Args)
    (LPCFUNCTION)FReflection_pointFunction,       // pointer to the executible code
    COMPLEX_ARRAY,                     // the return type is a complex array
    3,                                  //  the function takes on 3 arguments
    {COMPLEX_ARRAY,COMPLEX_ARRAY,COMPLEX_ARRAY}   // types of arguments
    };


  







	    double linterp(int N,                   // Линейная интерполяция функции по точкам.
    						double* X,               // X - вектор абсцисс,
                     double* Y,               // Y - вектор ординат,
                     double x)               // x - значение абсциссы точки, в которой производится вычисление
                                              // N - количество точек
    {
    	double y;
      int i;

      if(x<X[1])
        	y=Y[0]+(Y[1]-Y[0])/(X[1]-X[0])*(x-X[0]);
      else
      	if(x>=X[N-2])
          	y=Y[N-2]+(Y[N-1]-Y[N-2])/(X[N-1]-X[N-2])*(x-X[N-2]);
      else
      {
      	for(i=1;i<=N-3;i++)
         if(X[i]<=x && x<X[i+1])
         	break;
         y=Y[i]+(Y[i+1]-Y[i])/(X[i+1]-X[i])*(x-X[i]);
      }

      return y;
    }// konec linterp


    double parab_approx(int N,              // Параболическая аппроксимация линейно-ломаной функции по точкам полиномом 2-го порядка,
    														 // по методу наименьших квадратов. Возвращает значение аппроксимированной функции
                                              //
    						double* X,               // X - вектор абсцисс,
                     double* Y,               // Y - вектор ординат,
                     double x,
                     double far *A)            // x - значение абсциссы точки, в которой производится вычисление
                                              // N - количество точек
                                              // A - указатель на массив с коэффициентами полинома. Элементу с большим индексом соответствует более старший коэффициент.
    {
      double *alfa, *beta, *t_up, *t_down, gamma, delta, a0=0., a1=0., a2=0.;
      int i;

      alfa=(double*)farcalloc(N-1,sizeof(double));
      beta=(double*)farcalloc(N-1,sizeof(double));
      t_up=(double*)farcalloc(N-1,sizeof(double));
      t_down=(double*)farcalloc(N-1,sizeof(double));

      gamma=(X[0]+X[N-1])/2.;
      delta=(X[N-1]-X[0])/2.;

      for(i=0;i<=N-2;i++)
      {
       	beta[i]=(Y[i+1]-Y[i])/(X[i+1]-X[i]);
         alfa[i]=Y[i]-X[i]*beta[i];
         t_up[i]=(X[i+1]-gamma)/delta;
         t_down[i]=(X[i]-gamma)/delta;
      }

      for(i=0;i<=N-2;i++)
      {
       	a0=a0+(alfa[i]+beta[i]*gamma)*(t_up[i]-t_down[i])+delta*beta[i]/2.*(t_up[i]*t_up[i]-t_down[i]*t_down[i]);
      }
      a0=a0*0.5;

      for(i=0;i<=N-2;i++)
      {
       	a1=a1+(alfa[i]+beta[i]*gamma)/2.*(t_up[i]*t_up[i]-t_down[i]*t_down[i])+delta*beta[i]/3.*(t_up[i]*t_up[i]*t_up[i]-t_down[i]*t_down[i]*t_down[i]);
      }
      a1=a1*1.5;

      for(i=0;i<=N-2;i++)
      {
       	a2=a2+(alfa[i]+beta[i]*gamma)/2.*(t_up[i]*t_up[i]*t_up[i]-t_down[i]*t_down[i]*t_down[i])+3.*delta*beta[i]/8.*(t_up[i]*t_up[i]*t_up[i]*t_up[i]-t_down[i]*t_down[i]*t_down[i]*t_down[i])-(alfa[i]+beta[i]*gamma)/2.*(t_up[i]-t_down[i])-delta*beta[i]/4.*(t_up[i]*t_up[i]-t_down[i]*t_down[i]);
      }
      a2=a2*2.5;

      A[0]=a0-a1*(X[N-1]+X[0])/(X[N-1]-X[0])+a2/2.*(3.*(X[N-1]+X[0])*(X[N-1]+X[0])/(X[N-1]-X[0])/(X[N-1]-X[0])-1.);
      A[1]=2.*a1/(X[N-1]-X[0])-a2*6*(X[N-1]+X[0])/(X[N-1]-X[0])/(X[N-1]-X[0]);
      A[2]=a2*6./(X[N-1]-X[0])/(X[N-1]-X[0]);


      return A[0]+x*A[1]+x*x*A[2];

    }// konec parab_approx



    double minerr(double (*func)(double,double*),    // Определяет значение первого аргумента функции *func, при
    							double niz,         // котором функция принимает значение val с точностью не хуже prec
                        double verh,        // в пределах изменения аргумента niz - verh.
                        double prec,        // если на заданном интервале не найден аргумент, удовлетворяющий
                        double val,
                        double *Args)       // требуемой точности возвращается значение аргумента, при котором
    {                                       // функция максимально приближается к значению val.
    	double shag,shag_porog, arg, arg_minerr,val_minerr, prev_val,cur_val,prec10; //Остальные аргументы функции *func должны быть записаны в массиве Args

      arg=niz;
      shag_porog=(verh-niz)/5000.;
      shag=shag_porog*10.;
      arg_minerr=arg;
      val_minerr=func(arg_minerr,Args);
      prec10=prec/10.;
      cur_val=func(arg,Args);

      while(arg<=verh)
      {
      	if((cur_val-val)<=prec && (cur_val-val)>=-prec)
         	break;
         prev_val=cur_val;
         arg=arg+shag;
         cur_val=func(arg,Args);
         if((cur_val-prev_val)<prec10 && (cur_val-prev_val)>-prec10)
         	shag=shag*5.;
         while((cur_val>val && prev_val<val) || (cur_val<val && prev_val>val))
         {
            if(shag>shag_porog)
            {
         		shag=shag/2.;
            	arg=arg-shag;
            	cur_val=func(arg,Args);
               if((cur_val-val)<=prec && (cur_val-val)>=-prec)
         			break;
               if(fabs(cur_val-val)<fabs(val_minerr-val))
         		{
         			arg_minerr=arg;
            		val_minerr=func(arg_minerr,Args);
         		}
            }
            else
            {
               arg=arg-shag+shag_porog*10.;
            	prev_val=func(arg,Args);
            	shag=shag_porog*10.;
               arg=arg+shag;
            	cur_val=func(arg,Args);
            }
         }
         if(fabs(cur_val-val)<fabs(val_minerr-val))
         {
         	arg_minerr=arg;
            val_minerr=func(arg_minerr,Args);
         }
      }

      if(arg>verh)
      	return arg_minerr;

      return arg;

    }// konec minerr

    double Vd(double I, double *Args)// Промежуточная ф-ция для Тд
    {
      double kdv, kdg, alfa_dv, alfa_dg, pol, R;

      kdv=Args[0];
      kdg=Args[1];
      alfa_dv=Args[2];
      alfa_dg=Args[3];
      pol=Args[4];
      R=Args[5];

     	if(pol==1)
      	return -kdv*pow(I,alfa_dv)*R*exp(-0.000214*pow(R,0.657)*pow(log(I),4)+0.0000004*R*R*I);

      return -kdg*pow(I,alfa_dg)*R*exp(-0.000214*pow(R,0.657)*pow(log(I),4)+0.0000004*R*R*I);


    }// konec V_d



    LRESULT FRelyefFunction(COMPLEXARRAY * const Relyef,
                            COMPLEXARRAY * const profile,
                            COMPLEXSCALAR * const N_relyef)
    {
		unsigned int i, j;

        // allocate space for the return array Relyef
        if ( !MathcadArrayAllocate( Relyef,  // allocate space for Relyef
                    (unsigned int)(N_relyef->real),    //with N_relyef rows
                    2,    //and 2 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
            return 2;           // if allocation is insufficient
                                // return the error code



        for ( i = 0; i <= N_relyef->real-1; i++ )
        {
            if ( isUserInterrupted( ) )
            {
                MathcadArrayFree( Relyef );
                return  3;      // user interrupted
            }

            for ( j = 0; j <= 1; j++ )
            {

                Relyef->hReal[j][i] = profile->hReal[0][12+i*3+j];   // 1-j index - colonka, 2-j - stroka

            }
        }

        return 0;               // return 0 to indicate there was no error

    }// konec FRelyefFunction




    LRESULT FN_prep_cFunction(COMPLEXSCALAR * const N_prep_c,					// opredelyaet kolichestvo prepyatstviy bez "perelomov"
                              COMPLEXSCALAR * const N_prep,
                              COMPLEXSCALAR * const N_relyef,
                            	COMPLEXARRAY * const profile,
                            	COMPLEXARRAY * const Relyef)
    {
		unsigned int i, j, n;
      n=0;


        for ( i = 0; i <= N_prep->real-1; i++ )
        {
            for ( j = 0; j <=N_relyef->real-1; j++ )
            {
                if (profile->hReal[0][16+N_relyef->real*3+i*4] < Relyef->hReal[0][j] && Relyef->hReal[0][j] < profile->hReal[0][17+N_relyef->real*3+i*4])
                n++;   // 1-j index - colonka, 2-j - stroka

            }
        }

        N_prep_c->real=N_prep->real ? N_prep->real+n : 1;

        return 0;               // return 0 to indicate there was no error

    }// konec FN_prep_cFunction


    LRESULT FPrepyatstvieFunction(COMPLEXARRAY * const Prepyatstvie,				 // zapolnyaet massiv prepyatstviy,
    										 COMPLEXSCALAR * const N_prep,					 // prigodnyi dlya interpolyacii
	                               COMPLEXSCALAR * const N_relyef,
    	                          	 COMPLEXARRAY * const profile,
                            		 COMPLEXARRAY * const Relyef)
    {
		unsigned int i, j, k, prep_rows;

        i=0;
        j=0;
        k=0;


        while(i<=N_prep->real-1)	// opredelenye strok v massive prepyatstviy
        {
        	if ( isUserInterrupted( ) )
            {
                MathcadArrayFree( Relyef );
                return  3;      // user interrupted
            }
         while(Relyef->hReal[0][k] <= profile->hReal[0][16+N_relyef->real*3+i*4])
         	k++;
         while(Relyef->hReal[0][k] < profile->hReal[0][17+N_relyef->real*3+i*4])
         {
         	k++;
            j++;
         }
         j++;
         i++;
        }

        prep_rows=j;  // strok v massive prepyatstviy
        if (N_prep->real==0)
        	prep_rows=1;

        // allocate space for the return array Prepyatstvie
        if ( !MathcadArrayAllocate( Prepyatstvie,  // allocate space for Prepyatstvie
                    prep_rows,    //with N_relyef rows
                    4,    //and 4 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
            return 2;           // if allocation is insufficient
                                // return the error code

        i=0;  //номер препятствия в массиве profile
        j=0;  //номер препятствия в массиве Prepyatstvie
        k=0;  //номер точки высотных отметок рельефа

        while(i<=N_prep->real-1)	// zapolnenie massiva
        {
        	if ( isUserInterrupted( ) )
            {
                MathcadArrayFree( Relyef );
                return  3;      // user interrupted
            }
        	Prepyatstvie->hReal[0][j]=profile->hReal[0][16+N_relyef->real*3+i*4];  //расстояние до начала препятствия
         Prepyatstvie->hReal[2][j]=profile->hReal[0][18+N_relyef->real*3+i*4];  //высота препятствия
         Prepyatstvie->hReal[3][j]=profile->hReal[0][19+N_relyef->real*3+i*4];  //тип препятствия
         while(Relyef->hReal[0][k] <= profile->hReal[0][16+N_relyef->real*3+i*4]) //найти первую отметку рельефа за началом препятствия
         	k++;
         while(Relyef->hReal[0][k] < profile->hReal[0][17+N_relyef->real*3+i*4]) //пока отметки рельефа находятся внутри препятствия
         {
            Prepyatstvie->hReal[1][j]=Relyef->hReal[0][k];  //расстояние до конца препятствия
            Prepyatstvie->hReal[0][j+1]=Relyef->hReal[0][k]; //расстояние до начала следующего препятствия
            Prepyatstvie->hReal[2][j]=profile->hReal[0][18+N_relyef->real*3+i*4];  //высота препятствия
            Prepyatstvie->hReal[3][j]=profile->hReal[0][19+N_relyef->real*3+i*4];  //тип препятствия
            Prepyatstvie->hReal[2][j+1]=profile->hReal[0][18+N_relyef->real*3+i*4]; //высота следующего препятствия
            Prepyatstvie->hReal[3][j+1]=profile->hReal[0][19+N_relyef->real*3+i*4]; //тип следующего препятствия
         	k++;
            j++;
         }
         Prepyatstvie->hReal[1][j]=(profile->hReal[0][17+N_relyef->real*3+i*4]<=Prepyatstvie->hReal[0][j])?profile->hReal[0][17+N_relyef->real*3+i*4]+0.00000007:profile->hReal[0][17+N_relyef->real*3+i*4];//если расстояние до конца преп. не превосходит расстояния до начала
         j++;
         i++;
        }
        if (N_prep->real==0)
        	{
          	Prepyatstvie->hReal[0][0]=-0.00002;
          	Prepyatstvie->hReal[1][0]=-0.00001;
            Prepyatstvie->hReal[2][0]=0;
            Prepyatstvie->hReal[3][0]=0;
         }



        return 0;               // return 0 to indicate there was no error

    }// konec FPrepyatstvieFunction


    LRESULT FVoda_uchFunction(COMPLEXARRAY * const Voda_uch,				 // zapolnyaet massiv vodnyh uchastkov
                              COMPLEXSCALAR * const N_voda_c,
                              COMPLEXSCALAR * const N_voda,
                              COMPLEXSCALAR * const N_relyef,
                              COMPLEXSCALAR * const N_prep,
                              COMPLEXARRAY * const profile)
    {
		unsigned int i, j;

        // allocate space for the return array Voda_uch
        if ( !MathcadArrayAllocate( Voda_uch,  // allocate space for Voda_uch
                    (unsigned int)(N_voda_c->real),    //with N_voda_c rows
                    2,    //and 2 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
            return 2;           // if allocation is insufficient
                                // return the error code

        for(i=0;i<=N_voda_c->real-1;i++)
        {
        	if ( isUserInterrupted( ) )
            {
                MathcadArrayFree( Voda_uch );
                return  3;      // user interrupted
            }
         for (j=0;j<=1;j++)
         	{
            	if(N_voda->real!=0)
               	Voda_uch->hReal[j][i]=profile->hReal[0][18+N_relyef->real*3+N_prep->real*4+i*2+j];
               else
               	{
                  	Voda_uch->hReal[0][i]=-0.00002;
                  	Voda_uch->hReal[1][i]=-0.00001;
                  }
            }
        }

        return 0;               // return 0 to indicate there was no error

    }// konec FVoda_uchFunction

    LRESULT Fa_eFunction(COMPLEXSCALAR * const a_e,				 // vozvrashaet ekvivalentnyi radius zemli
    							 COMPLEXSCALAR * const g0)
    {

        a_e->real=6370000./(1.+6370000.*g0->real/2.);

        return 0;               // return 0 to indicate there was no error

    }// konec Fa_eFunction

    LRESULT Fy_xFunction(COMPLEXSCALAR * const y_x,				 // vozvrashaet vysotu zemnogo segmenta
    							 COMPLEXSCALAR * const x,
                         COMPLEXSCALAR * const R,
                         COMPLEXSCALAR * const a_e,
                         COMPLEXSCALAR * const Hor)
    {

        y_x->real=R->real*1000000*x->real*(1-x->real/R->real)/2/a_e->real+Hor->real;

        return 0;               // return 0 to indicate there was no error

    }// konec Fy_xFunction





    LRESULT FPrepFunction(COMPLEXARRAY * const Prep,				 // Preobrazovyvaet massiv Prepyatstvie k udobnomu vidu
    							 COMPLEXSCALAR * const N_prep_c,			 // Prep dlya interpolyacii
                         COMPLEXSCALAR * const R,
                         COMPLEXARRAY * const Prepyatstvie)
    {
		unsigned int i, j;

        // allocate space for the return array Prep
        if ( !MathcadArrayAllocate( Prep,  // allocate space for Prep
                    (unsigned int)(N_prep_c->real*4+2),    //with N_prep_c rows
                    2,    //and 2 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
            return 2;           // if allocation is insufficient
                                // return the error code

        Prep->hReal[0][0]=-0.001;
        Prep->hReal[1][0]=0;
        Prep->hReal[0][N_prep_c->real*4+1]=(Prepyatstvie->hReal[1][N_prep_c->real-1]<R->real+0.001)?R->real+0.001:Prepyatstvie->hReal[1][N_prep_c->real-1];
        Prep->hReal[1][N_prep_c->real*4+1]=0;

        for(i=1;i<=N_prep_c->real*4;i++)
        {
        	if ( isUserInterrupted( ) )
            {
                MathcadArrayFree( Prep );
                return  3;      // user interrupted
            }
         for (j=0;j<=1;j++)
         	{
            	if(fmod(i-2,4)==0 && j==1)//отметки верхних левых точек препятствий
               	Prep->hReal[j][i]=Prepyatstvie->hReal[2][(i-2)/4];
               if(fmod(i-3,4)==0 && j==1)//отметки верхних правых точек препятствий
               	Prep->hReal[j][i]=Prepyatstvie->hReal[2][(i-3)/4];
               if(fmod(i-2,4)==0 && j==0)//расстояния до верхних левых точек препятствий
               	Prep->hReal[j][i]=Prepyatstvie->hReal[0][(i-2)/4]+0.00000002;
               if(fmod(i-3,4)==0 && j==0)//расстояния до верхних правых точек препятствий
               	Prep->hReal[j][i]=Prepyatstvie->hReal[1][(i-3)/4]-0.00000002;
               if(fmod(i-1,4)==0 && j==1 || fmod(i-4,4)==0 && j==1)//отметки нижних левых и правых точек препятствий
               	Prep->hReal[j][i]=0;
               if(fmod(i-1,4)==0 && j==0)//расстояния до нижних левых точек препятствий
               	Prep->hReal[j][i]=Prepyatstvie->hReal[0][(i-1)/4]+0.00000001;
               if(fmod(i-4,4)==0 && j==0)//расстояния до нижних правых точек препятствий
               	Prep->hReal[j][i]=Prepyatstvie->hReal[1][(i-4)/4]-0.00000001;
            }
        }

        return 0;               // return 0 to indicate there was no error

    }// konec FPrepFunction



    LRESULT FrFunction(COMPLEXSCALAR * const r,				 // Vichislyaet visotnuyu otmetky relyefa putem lineynoy interpolyacii
    						  COMPLEXSCALAR * const x,
                       COMPLEXARRAY * const Relyef,
                       COMPLEXSCALAR * const Hor,
                       COMPLEXSCALAR * const a_e,
                       COMPLEXSCALAR * const R,
                       COMPLEXSCALAR * const N_relyef)
    {
		double y_x;

        y_x=R->real*1000000*x->real*(1-x->real/R->real)/2/a_e->real+Hor->real;

 

        r->real=linterp(N_relyef->real,Relyef->hReal[0],Relyef->hReal[1],x->real)+y_x-Hor->real;

        return 0;               // return 0 to indicate there was no error

    }// konec FrFunction



    LRESULT FpFunction(COMPLEXSCALAR * const p,				 // Vichislyaet visotu prepyatstvya putem lineynoy interpolyacii
                       COMPLEXSCALAR * const x,
    						  COMPLEXARRAY * const Prep,
                       COMPLEXSCALAR * const N_prep_c)
    {
		unsigned int i;

        for(i=0;i<=N_prep_c->real*4+1;i++)
        {
        	if (x->real==Prep->hReal[0][i])
         	{
            	p->real=Prep->hReal[1][i];
               break;
            }
         else if(x->real<Prep->hReal[0][i])
         	{
            	p->real=(x->real-Prep->hReal[0][i-1])*(Prep->hReal[1][i]-Prep->hReal[1][i-1])/(Prep->hReal[0][i]-Prep->hReal[0][i-1])+Prep->hReal[1][i-1];
               break;
            }
        }

        return 0;               // return 0 to indicate there was no error

    }// konec FpFunction



    LRESULT FsurfFunction(COMPLEXSCALAR * const surf,				 // Vichislyaet visotu poverhnosti profilya s uchetom pogreshnosti putem lineynoy interpolyacii
    							  COMPLEXSCALAR * const x,
                          COMPLEXARRAY * const Prep,
                       	  COMPLEXSCALAR * const N_prep_c,
                          COMPLEXARRAY * const Relyef,
                          COMPLEXSCALAR * const Hor,
                          COMPLEXSCALAR * const a_e,
                          COMPLEXSCALAR * const R,
                          COMPLEXSCALAR * const N_relyef,
                          COMPLEXSCALAR * const Delta_H)
    {
		COMPLEXSCALAR p,r;


        if (x->real>=0 && x->real<=R->real)
        	{
            FpFunction(&p,x,Prep,N_prep_c);
            FrFunction(&r,x,Relyef,Hor,a_e,R,N_relyef);
        		surf->real=p.real+r.real+Delta_H->real;
         }
        else
        	surf->real=-100000;

        return 0;               // return 0 to indicate there was no error

    }// konec FsurfFunction




    LRESULT FluchFunction(COMPLEXSCALAR * const luch,				 // Vichislyaet vysotu lucha
                            COMPLEXSCALAR * const x,
    							 	 COMPLEXSCALAR * const H1,
                            COMPLEXSCALAR * const H2,
                            COMPLEXSCALAR * const R,
                            COMPLEXSCALAR * const r_ot_0,
                            COMPLEXSCALAR * const r_ot_R)
    {
		  if (x->real==0)
         	luch->real=H1->real+r_ot_0->real;
        else if(x->real==R->real)
         	luch->real=H2->real+r_ot_R->real;
        else luch->real=(H1->real+r_ot_0->real)+x->real*(H2->real+r_ot_R->real-H1->real-r_ot_0->real)/R->real;

        return 0;               // return 0 to indicate there was no error

    }// konec FluchFunction





    LRESULT FR_prFunction(COMPLEXSCALAR * const R_pr,				// Vichislyaet rasstoyanye do prepyatstvya
                          COMPLEXARRAY * const Prep,           //Args[0] - N_prep_c;
    							  COMPLEXARRAY * const Relyef,       	//Args[1] - Hor;
                          COMPLEXARRAY * const Args)          	//Args[2] - a_e;
                          													//Args[3] - R;
                                                               //Args[4] - N_relyef;
                                                               //Args[5] - Delta_H;
                                                               //Args[6] - H1;
                                											//Args[7] - H2;
                                                               //Args[8] - f;

    {
        COMPLEXSCALAR x,x_min,luch,luch_min,r_ot_0,r_ot_R,surf,surf_min,H1,H2,R,N_prep_c,Hor,a_e,N_relyef,Delta_H;
        double H0,H0_min,lambda,f,shag;

        N_prep_c.real=Args->hReal[0][0];
        Hor.real=Args->hReal[0][1];
        a_e.real=Args->hReal[0][2];
        R.real=Args->hReal[0][3];
        N_relyef.real=Args->hReal[0][4];
        Delta_H.real=Args->hReal[0][5];
        H1.real=Args->hReal[0][6];
        H2.real=Args->hReal[0][7];
        f=Args->hReal[0][8];

        x.real=R.real*0.9999;
        x_min.real=R.real*0.99999;
        lambda=0.3/f;
        shag=R.real*0.000005;

		  while(x.real>=R.real*0.00001)
        	{
         	r_ot_0.real=Relyef->hReal[1][0];
            r_ot_R.real=Relyef->hReal[1][N_relyef.real-1];
            FluchFunction(&luch,&x,&H1,&H2,&R,&r_ot_0,&r_ot_R);
            FluchFunction(&luch_min,&x_min,&H1,&H2,&R,&r_ot_0,&r_ot_R);
            FsurfFunction(&surf,&x,Prep,&N_prep_c,Relyef,&Hor,&a_e,&R,&N_relyef,&Delta_H);
            FsurfFunction(&surf_min,&x_min,Prep,&N_prep_c,Relyef,&Hor,&a_e,&R,&N_relyef,&Delta_H);
            H0=sqrt(1./3.*lambda*1000.*x.real*(1.-x.real/R.real));
            H0_min=sqrt(1./3.*lambda*1000.*x_min.real*(1.-x_min.real/R.real));
            if(((luch.real-surf.real)/H0)<((luch_min.real-surf_min.real)/H0_min))
            	x_min.real=x.real;
            x.real=x.real-shag;
         }
        R_pr->real=x_min.real;

        return 0;               // return 0 to indicate there was no error

    }// konec FR_prFunction



 

    LRESULT FHorda_Krit_PrepFunction(COMPLEXARRAY * const R1_R2_DY,				// Vichislyaet parametry hordy
    							 COMPLEXARRAY * const Prep,                           // R1_R2_DY[0] - km, (parametr_r) dlina hordy
                         COMPLEXARRAY * const Relyef,									// R1_R2_DY[1] - km, (R1h) rasstoyanie do levoy granicy hordy
                       	 COMPLEXARRAY * const Args)									// R1_R2_DY[2] - km, (R2h) rasstoyanie do pravoy granicy hordy
                         																		// R1_R2_DY[3] - m, (H1h) vysotnaya otmetka levoy granicy hordy
                                                                              // R1_R2_DY[4] - m, (H2h) vysotnaya otmetka pravoy granicy hordy
                                                                              // R1_R2_DY[5] - m, (Hkph) vysotnaya otmetka hordy v tochke kriticheskogo prepyatstviya
                                                                              // R1_R2_DY[6] - m, (DY) vysota segmenta approksimiruyuschey sfery
                                                                              // R1_R2_DY[7] - km, (R1kas) rasstoyanie do tochki kasaniya luchem profilya sleva
                                                                              // R1_R2_DY[8] - km, (R2kas) rasstoyanie do tochki kasaniya luchem profilya sprava
                                                                              // R1_R2_DY[9] - m, (H1kas) vysotnaya otmetka tochki kasaniya luchem profilya sleva
                                                                              // R1_R2_DY[10] - m, (H2kas) vysotnaya otmetka tochki kasaniya luchem profilya sprava
                                                                              // R1_R2_DY[11] - km, (R1ellipse) rasstoyanie do tochki peresecheniya ellipsom profilya sleva
                                                                              // R1_R2_DY[12] - km, (R2ellipse) rasstoyanie do tochki peresecheniya ellipsom profilya sprava
                                                                              // R1_R2_DY[13] - m, (H1ellipse) vysotnaya otmetka tochki peresecheniya ellipsom profilya sleva
                                                                              // R1_R2_DY[14] - m, (H2ellipse) vysotnaya otmetka tochki peresecheniya ellipsom profilya sprava
                                                                              // R1_R2_DY[15] - km, (R1hordaH0) rasstoyanie do tochki peresecheniya hordy(H0) profilya sleva
                                                                              // R1_R2_DY[16] - km, (R2hordaH0) rasstoyanie do tochki peresecheniya hordy(H0) profilya sprava
                                                                              // R1_R2_DY[17] - m, (H1hordaH0) vysotnaya otmetka tochki peresecheniya hordy(H0) profilya sleva
                                                                              // R1_R2_DY[18] - m, (H2hordaH0) vysotnaya otmetka tochki peresecheniya hordy(H0) profilya sprava
																										// R1_R2_DY[19] - km, (R_kross_kasat) rasstoyanie do tochki peresecheniya kasatelnyh
                                                                              // R1_R2_DY[20] - m, (H_kross_kasat) vysotnaya otmetka tochki peresecheniya kasatelnyh
                                                                              // R1_R2_DY[21] - m, (Prosvet_kross_kasat) prosvet v tochke peresecheniya kasatelnyh
                                                                              // R1_R2_DY[22] - km, (R_kr) rasstoyanie do kriticheskogo prepyatstviya
                                                                              // R1_R2_DY[23] - m, (H_kr) prosvet v tochke kriticheskogo prepyatstviya s uchetom pogreshnosti
                                                                              // R1_R2_DY[24] - m, (H0) optimalny prosvet v tochke kriticheskogo prepyatstviya
                                                                              // R1_R2_DY[25] - m, (p_ot_g) otnositelny prosvet v tochke kriticheskogo prepyatstviya s uchetom pogreshnosti
                                                                              // Prep - массив расстояний и высотных отметок препятствий
                                               											// Relyef - massiv rasstoyaniy i vysotnyh otmetok izlomov relyefa
                                                      								// Args[0] - N_prep_c - kolichestvo prepyatstviy
                                                										// Args[1] - Hor - m, uroven uslovnogo gorizonta
                         	                            								// Args[2] - a_e - m, ekvivalentny radius zemli
                                                                              // Args[3] - N_relyef - kolichestvo tochek relyefa v massive Relyef
                                                                              // Args[4] - Delta_H - m, pogreshnost otmetok relyefa
                                                                              // Args[5] - H1 - m, vysota podvesa antenny sleva
                                                                              // Args[6] - H2 - m, vysota podvesa antenny sprava
                                                                              // Args[7] - f - GGz, rabochaya chastota
                                                                              // Args[8] - lambda - m, rabochaya dlina volny
                                                                              // Args[9] - R_pr - km, rasstoyanie do prepyatstviya s minimalnym otnositelnym prosvetom mejdu luchom i poverhnostyu profilya
                                                                              // Args[10] - H_pr - m, prosvet s uchetom pogreshnosti v tochke s minimalnym otnositelnym prosvetom mejdu luchom i poverhnostyu profilya
    {
        double x_kas1,x_kas2,tg_k1,tg_k2,tg_k_max1,tg_k_min2,
        			H1kas,H2kas,H0,k,R1ellipse,R2ellipse,H1ellipse,H2ellipse,
               R1hordaH0,R2hordaH0,H1hordaH0,H2hordaH0,shag,
               R_far,H_far,R_near,H_near,h_near_H0,h_near_e,tg_left_H0,tg_left_e;
        COMPLEXSCALAR surf1,surf2,surf_kp,x1,x2,R,H_luch1,H_luch2,otm_zem1,otm_zem2,
        					 N_prep_c,Hor,a_e,N_relyef,Delta_H,H1,H2,f,lambda,R_pr,H_pr;

		  // allocate space for the return array
        if ( !MathcadArrayAllocate( R1_R2_DY,  // allocate space for R1_R2_DY
                    26,    //with 19 rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
            return 2;           // if allocation is insufficient
                                // return the error code

        N_prep_c.real=Args->hReal[0][0];
        Hor.real=Args->hReal[0][1];
        a_e.real=Args->hReal[0][2];
        N_relyef.real=Args->hReal[0][3];
        Delta_H.real=Args->hReal[0][4];
        H1.real=Args->hReal[0][5];
        H2.real=Args->hReal[0][6];
        f.real=Args->hReal[0][7];
        lambda.real=Args->hReal[0][8];
        R_pr.real=Args->hReal[0][9];
        H_pr.real=Args->hReal[0][10];

        otm_zem1.real=Relyef->hReal[1][0];//м, отметка земли слева
        otm_zem2.real=Relyef->hReal[1][N_relyef.real-1];//м, отметка земли справа

        R.real=Relyef->hReal[0][N_relyef.real-1];//км, протяженность профиля

        FsurfFunction(&surf_kp,&R_pr,Prep,&N_prep_c,Relyef,&Hor,&a_e,&R,&N_relyef,&Delta_H);//вычисление отметки рельефа в точке критического препятствия

        shag=R.real*0.000005;

        x1.real=R.real*0.0001;  //км, начальная точка для поиска касательной справа
        x2.real=R.real-x1.real;  //км, начальная точка для поиска касательной слева

        R1ellipse=0;//км, начальная точка 0 для поиска левой границы хорды в ДМВ диапазоне на полузакрытых, а также закрытых трассах
        R2ellipse=R.real;//км, начальная точка R для поиска правой границы хорды в ДМВ диапазоне на полузакрытых, а также закрытых трассах
        H1ellipse=-1000.;//м, начальная высотная отметка для поиска левой границы хорды в ДМВ диапазоне на полузакрытых, а также закрытых трассах
        H2ellipse=-1000.;//м, начальная высотная отметка для поиска правой границы хорды в ДМВ диапазоне на полузакрытых, а также закрытых трассах

        R1hordaH0=0;//км, начальная точка 0 для поиска левой границы хорды, соответствующей линии параллельной лучу, отстоящей на Н0 от критического препятствия
        R2hordaH0=R.real;//км, начальная точка R для поиска правой границы хорды, соответствующей линии параллельной лучу, отстоящей на Н0 от критического препятствия
        H1hordaH0=-1000.;//м, начальная высотная отметка для поиска левой границы хорды=-1000.;//м, начальная высотная отметка для поиска левой границы хорды в ДМВ диапазоне на полузакрытых и закрытых трассах
        H2hordaH0=-1000.;//м, начальная высотная отметка для поиска правой границы хорды, соответствующей линии параллельной лучу, отстоящей на Н0 от критического препятствия

        tg_k_max1=-1000.;//м/км, начальное максимальное значение тангенса угла наклона касательной к профилю слева
        tg_k_min2=1000.;//м/км, начальное минимальное значение тангенса угла наклона касательной к профилю справа

        k=R_pr.real/R.real;//относительная координата препятствия с минимальным относительным просветом между лучом и поверхностью профиля
        H0=sqrt(1000./3.*R.real*lambda.real*k*(1.-k));//м, оптимальный просвет в точке с минимальным относительным просветом между лучом и поверхностью профиля

        while(x1.real<=R.real-0.001)
        {
        	if ( isUserInterrupted( ) )
            {
                MathcadArrayFree( R1_R2_DY );
                return  3;      // user interrupted
            }
         FsurfFunction(&surf1,&x1,Prep,&N_prep_c,Relyef,&Hor,&a_e,&R,&N_relyef,&Delta_H);//вычисление отметки рельефа в текущей точке x1
         FsurfFunction(&surf2,&x2,Prep,&N_prep_c,Relyef,&Hor,&a_e,&R,&N_relyef,&Delta_H);//вычисление отметки рельефа в текущей точке x2

         FluchFunction(&H_luch1,&x1,&H1,&H2,&R,&otm_zem1,&otm_zem2);//м, высота луча в текущей точке x1
         FluchFunction(&H_luch2,&x2,&H1,&H2,&R,&otm_zem1,&otm_zem2);//м, высота луча в текущей точке x2

         //---Определение координат точек пересечения контура минимальной существенной зоны с поверхностью профиля в диапазоне ДМВ---
         if(f.real<=0.6 && H_pr.real<H0)//если ДМВ диапазон и плузакрытый или закрытый интервал                   						|
         {                                                                                                          						//
            if(H1ellipse==-1000.)//если левая граница хорды еще не определена                                       						|
            	if(surf1.real>H_luch1.real-sqrt(1000./3.*R.real*lambda.real*(x1.real/R.real)*(1.-x1.real/R.real)))//если поверхность профиля в т. х1 выше эллипса
         		{                                                                                                    						//
               	R1ellipse=x1.real;//км, координата левой границы хорды в ДМВ диапазоне на плузакрытом или закрытом интервале
                  H1ellipse=H_luch1.real-sqrt(1000./3.*R.real*lambda.real*(x1.real/R.real)*(1.-x1.real/R.real));//м, высотная отметка левой границы хорды в ДМВ диапазоне на плузакрытом или закрытом интервале
               }                                                                                                    						//
            if(H2ellipse==-1000.)//если правая граница хорды еще не определена                                      						|
            	if(surf2.real>H_luch2.real-sqrt(1000./3.*R.real*lambda.real*(x2.real/R.real)*(1.-x2.real/R.real)))//если поверхность профиля в т. х2 выше луча
         		{                                                                                                    						//
               	R2ellipse=x2.real;//км, координата правой границы хорды в ДМВ диапазоне на плузакрытом или закрытом интервале
                  H2ellipse=H_luch2.real-sqrt(1000./3.*R.real*lambda.real*(x2.real/R.real)*(1.-x2.real/R.real));//м, высотная отметка правой границы хорды в ДМВ диапазоне на плузакрытом или закрытом интервале
               }                                                                                                   						//
         }                                                                                                          						//
         //--------------------------------------------------------------------------------------------------------------------------

         else//иначе (либо диапазон не ДМВ либо профиль открытый)
         {
            //---Определение координат точек пересечения линии отстоящей на H0 от крит. преп. с поверхностью профиля----
            if(H1hordaH0==-1000.)//если левая граница хорды еще не определена                                          |
               if(surf1.real>H_luch1.real-H_pr.real-H0)//если поверхность профиля в т. х1 выше линии, отстоящей на H0 от крит. преп.
               {                                                                                                    	  //
               	R1hordaH0=x1.real;//км, координата левой границы хорды, соответствующей линии отстоящей на H0 от крит. преп.
                  H1hordaH0=H_luch1.real-H_pr.real-H0;//м, высотная отметка левой границы хорды, соответствующей линии отстоящей на H0 от крит. преп.
               }                                                                                                       //
            if(H2hordaH0==-1000.)//если правая граница хорды еще не определена                                         |
               if(surf2.real>H_luch2.real-H_pr.real-H0)//если поверхность профиля в т. х2 выше линии, отстоящей на H0 от крит. преп.
               {                                                                                                    	  //
               	R2hordaH0=x2.real;//км, координата правой границы хорды, соответствующей линии отстоящей на H0 от крит. преп.
                  H2hordaH0=H_luch2.real-H_pr.real-H0;//м, высотная отметка правой границы хорды, соответствующей линии отстоящей на H0 от крит. преп.
               }                                                                                                       //
            //-----------------------------------------------------------------------------------------------------------

         	if(H_pr.real<=0.)//если профиль закрытый не ДМВ диапазон
         	{
         		//---Определение координат точек касания к профилю------------------------------------
         		tg_k1=(surf2.real-Relyef->hReal[1][0]-H1.real)/x2.real;                            	//м/км, текущий тангенс угла наклона касательной к профилю слева
         		tg_k2=(Relyef->hReal[1][N_relyef.real-1]+H2.real-surf1.real)/(R.real-x1.real);   	//м/км, текущий тангенс угла наклона касательной к профилю справа
         		if(tg_k1>tg_k_max1)                                                                	//Поиск максимального тангенса наклона секущей к профилю слеава
         		{                                                                             		//
         			x_kas1=x2.real;                                                               	//
               	tg_k_max1=tg_k1;                                                              	//
               	H1kas=surf2.real;                                                             	//
            	}                                                                                	//
         		if(tg_k2<tg_k_min2)                                                                	//Поиск минимального тангенса наклона секущей к профилю справа
         		{                                                                             		//
         			x_kas2=x1.real;                                                               	//
               	tg_k_min2=tg_k2;                                                              	//
               	H2kas=surf1.real;                                                             	//
            	}                                                                                	//
         		//------------------------------------------------------------------------------------
         	}
         }

         	x1.real=x1.real+shag;//шаг вперед по профилю
         	x2.real=x2.real-shag;//шаг назад по профилю
        }

        if(f.real<=0.6 && H_pr.real<H0)//если ДМВ диапазон и плузакрытый или закрытый интервал
        {
        	R1_R2_DY->hReal[0][0]=R2ellipse-R1ellipse;//km, (parametr_r) dlina hordy
        	R1_R2_DY->hReal[0][1]=R1ellipse;//km, (R1h) rasstoyanie do levoy granicy hordy
        	R1_R2_DY->hReal[0][2]=R2ellipse;//km, (R2h) rasstoyanie do pravoy granicy hordy
        	R1_R2_DY->hReal[0][3]=H1ellipse;//m, (H1h) vysotnaya otmetka levoy granicy hordy
        	R1_R2_DY->hReal[0][4]=H2ellipse;//m, (H2h) vysotnaya otmetka pravoy granicy hordy
        	R1_R2_DY->hReal[0][5]=H1ellipse+(H2ellipse-H1ellipse)/(R2ellipse-R1ellipse)*(R_pr.real-R1ellipse);//m, (Hkph) vysotnaya otmetka hordy v tochke kriticheskogo prepyatstviya
        	R1_R2_DY->hReal[0][6]=surf_kp.real-R1_R2_DY->hReal[0][5];//m, (DY) vysota segmenta approksimiruyuschey sfery
        	R1_R2_DY->hReal[0][11]=R1ellipse;//км, координата левой границы хорды в ДМВ диапазоне на плузакрытом или закрытом интервале
        	R1_R2_DY->hReal[0][12]=R2ellipse;//км, координата правой границы хорды в ДМВ диапазоне на плузакрытом или закрытом интервале
        	R1_R2_DY->hReal[0][13]=H1ellipse;//м, высотная отметка левой границы хорды в ДМВ диапазоне на плузакрытом или закрытом интервале
        	R1_R2_DY->hReal[0][14]=H2ellipse;//м, высотная отметка правой границы хорды в ДМВ диапазоне на плузакрытом или закрытом интервале
         R1_R2_DY->hReal[0][22]=R_pr.real;//km, (R_kr) rasstoyanie do kriticheskogo prepyatstviya
         R1_R2_DY->hReal[0][23]=H_pr.real;// m, (H_kr) prosvet v tochke kriticheskogo prepyatstviya s uchetom pogreshnosti
         R1_R2_DY->hReal[0][24]=H0;//m, (H0) optimalny prosvet v tochke kriticheskogo prepyatstviya
         R1_R2_DY->hReal[0][25]=H_pr.real/H0;//m, (p_ot_g) otnositelny prosvet v tochke kriticheskogo prepyatstviya s uchetom pogreshnosti
        }
        else//иначе (либо диапазон не ДМВ либо профиль открытый)
        {
         	if(H_pr.real>0.)//открытая трасса
            {
             	R1_R2_DY->hReal[0][0]=R2hordaH0-R1hordaH0;//km, (parametr_r) dlina hordy
        			R1_R2_DY->hReal[0][1]=R1hordaH0;//km, (R1h) rasstoyanie do levoy granicy hordy
        			R1_R2_DY->hReal[0][2]=R2hordaH0;//km, (R2h) rasstoyanie do pravoy granicy hordy
        			R1_R2_DY->hReal[0][3]=H1hordaH0;//m, (H1h) vysotnaya otmetka levoy granicy hordy
        			R1_R2_DY->hReal[0][4]=H2hordaH0;//m, (H2h) vysotnaya otmetka pravoy granicy hordy
        			R1_R2_DY->hReal[0][5]=H1hordaH0+(H2hordaH0-H1hordaH0)/(R2hordaH0-R1hordaH0)*(R_pr.real-R1hordaH0);//m, (Hkph) vysotnaya otmetka hordy v tochke kriticheskogo prepyatstviya
        			R1_R2_DY->hReal[0][6]=surf_kp.real-R1_R2_DY->hReal[0][5];//m, (DY) vysota segmenta approksimiruyuschey sfery
        			R1_R2_DY->hReal[0][22]=R_pr.real;//km, (R_kr) rasstoyanie do kriticheskogo prepyatstviya
         		R1_R2_DY->hReal[0][23]=H_pr.real;// m, (H_kr) prosvet v tochke kriticheskogo prepyatstviya s uchetom pogreshnosti
         		R1_R2_DY->hReal[0][24]=H0;//m, (H0) optimalny prosvet v tochke kriticheskogo prepyatstviya
         		R1_R2_DY->hReal[0][25]=H_pr.real/H0;//m, (p_ot_g) otnositelny prosvet v tochke kriticheskogo prepyatstviya s uchetom pogreshnosti
         	}
            else//иначе, закрытая трасса не ДМВ
            {
               x1.real=R.real*0.0001;  //км, начальная точка для поиска касательной справа и границы хорды слева
        			x2.real=R.real-x1.real;  //км, начальная точка для поиска касательной слева и границы хорды справа

        			R1ellipse=x_kas1;//км, начальная точка для поиска левой границы хорды
        			R2ellipse=x_kas2;//км, начальная точка для поиска правой границы хорды
        			H1ellipse=H1kas;//м, начальная высотная отметка для поиска левой границы хорды
        			H2ellipse=H2kas;//м, начальная высотная отметка для поиска правой границы хорды

               while(x1.real<=x_kas1)//поиск пересечения эллипса с поверхностью профиля слева
        			{
        				if ( isUserInterrupted( ) )
            		{
                		MathcadArrayFree( R1_R2_DY );
                		return  3;      // user interrupted
            		}

         			FsurfFunction(&surf1,&x1,Prep,&N_prep_c,Relyef,&Hor,&a_e,&R,&N_relyef,&Delta_H);//вычисление отметки рельефа в текущей точке x1
                  H_luch1.real=Relyef->hReal[1][0]+H1.real+tg_k_max1*x1.real;//м, высотная отметка касательного луча слева в точке x1
                  if(R1ellipse==x_kas1)//если левая граница хорды еще не определена
            			if(surf1.real>H_luch1.real-sqrt(1000./3.*x_kas1*lambda.real*(x1.real/x_kas1)*(1.-x1.real/x_kas1)))//если поверхность профиля в т. х1 выше эллипса
         				{
               			R1ellipse=x1.real;//км, координата левой границы хорды
                  		H1ellipse=H_luch1.real-sqrt(1000./3.*x_kas1*lambda.real*(x1.real/x_kas1)*(1.-x1.real/x_kas1));//м, высотная отметка левой границы хорды
                        break;
               		}
                  x1.real=x1.real+shag;//шаг вперед по профилю
         		}
               while(x2.real>=x_kas2)//поиск пересечения эллипса с поверхностью профиля справа
        			{
        				if ( isUserInterrupted( ) )
            		{
                		MathcadArrayFree( R1_R2_DY );
                		return  3;      // user interrupted
            		}

         			FsurfFunction(&surf2,&x2,Prep,&N_prep_c,Relyef,&Hor,&a_e,&R,&N_relyef,&Delta_H);//вычисление отметки рельефа в текущей точке x2
         			H_luch2.real=Relyef->hReal[1][N_relyef.real-1]+H2.real+tg_k_min2*(x2.real-R.real);//м, высотная отметка касательного луча справа в точке x2
                  if(R2ellipse==x_kas2)//если правая граница хорды еще не определена
            			if(surf2.real>H_luch2.real-sqrt(1000./3.*(R.real-x_kas2)*lambda.real*(R.real-x2.real)/(R.real-x_kas2)*(1.-(R.real-x2.real)/(R.real-x_kas2))))//если поверхность профиля в т. х2 выше эллипса
         				{
               			R2ellipse=x2.real;//км, координата правой границы хорды
                  		H2ellipse=H_luch2.real-sqrt(1000./3.*(R.real-x_kas2)*lambda.real*(R.real-x2.real)/(R.real-x_kas2)*(1.-(R.real-x2.real)/(R.real-x_kas2)));//м, высотная отметка правой границы хорды
                        break;
               		}
                  x2.real=x2.real-shag;//шаг назад по профилю
               }

               //Определение хорды по критерию минимальной высоты и отсутствия пересечений другими хордами из вариантов:------//
               //- H0-H0 - левая и правая границы соответствуют хореде, построенной по критерию H0;                           //
               //- H0-e - левая граница соответствует хореде, построенной по критерию H0, а правая - пересечение эллипса      //
               //				с поверхностью профиля;                                                                            //
               //- e-H0 - левая граница  - пересечение эллипса с поверхностью профиля, а правая соответствует хореде,         //
               //				построенной по критерию H0;                                                                        //
               //- е-е - левая и правая  - пересечения эллипсов с поверхностью профиля;                                       //
               //Определение более удаленной(far) и менее удаленной (near) левой границы хорды от станции слева-//            //
               if(R1ellipse<R1hordaH0)//far - HO, near - e                                                      //            //
               {                                                                                                //            //
               	R_far=R1hordaH0;                                                                              //            //
                  H_far=H1hordaH0;                                                                              //            //
                  R_near=R1ellipse;                                                                             //            //
                  H_near=H1ellipse;                                                                             //            //
               }                                                                                                //            //
               else//far - e, near - H0                                                                         //            //
               {                                                                                                //            //
               	R_far=R1ellipse;                                                                              //            //
                  H_far=H1ellipse;                                                                              //            //
                  R_near=R1hordaH0;                                                                             //            //
                  H_near=H1hordaH0;                                                                             //            //
               }//----------------------------------------------------------------------------------------------//            //
               //Высотная отметка отрезка near-H0 в точке с координатой R_far                                                 //
               h_near_H0=H_near+(H2hordaH0-H_near)/(R2hordaH0-R_near)*(R_far-R_near);                                         //
               //Высотная отметка отрезка near-e в точке с координатой R_far                                                  //
               h_near_e=H_near+(H2ellipse-H_near)/(R2ellipse-R_near)*(R_far-R_near);                                          //
               //Если высотная отметка отрезков near-H0 или near-e в точке far ниже H_far,-------------//                     //
               //то near - левая граница хорды                                                         //                     //
               if(h_near_H0<H_far || h_near_e<H_far)                                                   //                     //
               {                                                                                       //                     //
               	R1_R2_DY->hReal[0][1]=R_near;//km, (R1h) rasstoyanie do levoy granicy hordy          //                     //
                  R1_R2_DY->hReal[0][3]=H_near;//m, (H1h) vysotnaya otmetka levoy granicy hordy        //                     //
                  //Определение тангенсов углов наклона хорд near-H0 и near-e                          //                     //
                  tg_left_H0=(H2hordaH0-H_near)/(R2hordaH0-R_near);                                    //                     //
                  tg_left_e=(H2ellipse-H_near)/(R2ellipse-R_near);                                     //                     //
                  //Если угол наклона хоры near-H0 меньше угла наклона хорды near-e,                   //                     //
                  //то H0 - правая граница хорды                                                       //                     //
                  if(tg_left_H0<tg_left_e)                                                             //                     //
                  {                                                                                    //                     //
                  	R1_R2_DY->hReal[0][2]=R2hordaH0;//km, (R2h) rasstoyanie do правой granicy hordy   //                     //
                  	R1_R2_DY->hReal[0][4]=H2hordaH0;//m, (H2h) vysotnaya otmetka правой granicy hordy //                     //
                  }                                                                                    //                     //
                  else//Если угол наклона хоры near-e меньше угла наклона хорды near-H0,               //                     //
                  		//то e - правая граница хорды                                                  //                     //
                  {                                                                                    //                     //
                  	R1_R2_DY->hReal[0][2]=R2ellipse;//km, (R2h) rasstoyanie do правой granicy hordy   //                     //
                  	R1_R2_DY->hReal[0][4]=H2ellipse;//m, (H2h) vysotnaya otmetka правой granicy hordy //                     //
                  }                                                                                    //                     //
               }//-------------------------------------------------------------------------------------//                     //
               else//если far находится ниже хорд, проведенных из near, far - левая граница хорды------//                     //
               {                                                                                       //                     //
               	R1_R2_DY->hReal[0][1]=R_far;//km, (R1h) rasstoyanie do levoy granicy hordy           //                     //
                  R1_R2_DY->hReal[0][3]=H_far;//m, (H1h) vysotnaya otmetka levoy granicy hordy         //                     //
                  //Определение тангенсов углов наклона хорд far-H0 и far-e                            //                     //
                  tg_left_H0=(H2hordaH0-H_far)/(R2hordaH0-R_far);                                      //                     //
                  tg_left_e=(H2ellipse-H_far)/(R2ellipse-R_far);                                       //                     //
                  //Если угол наклона хоры far-H0 меньше угла наклона хорды far-e,                     //                     //
                  //то H0 - правая граница хорды                                                       //                     //
                  if(tg_left_H0<tg_left_e)                                                             //                     //
                  {                                                                                    //                     //
                  	R1_R2_DY->hReal[0][2]=R2hordaH0;//km, (R2h) rasstoyanie do правой granicy hordy   //                     //
                  	R1_R2_DY->hReal[0][4]=H2hordaH0;//m, (H2h) vysotnaya otmetka правой granicy hordy //                     //
                  }                                                                                    //                     //
                  else//Если угол наклона хоры far-e меньше угла наклона хорды far-H0,                 //                     //
                  		//то e - правая граница хорды                                                  //                     //
                  {                                                                                    //                     //
                  	R1_R2_DY->hReal[0][2]=R2ellipse;//km, (R2h) rasstoyanie do правой granicy hordy   //                     //
                  	R1_R2_DY->hReal[0][4]=H2ellipse;//m, (H2h) vysotnaya otmetka правой granicy hordy //                     //
                  }                                                                                    //                     //
               }//-------------------------------------------------------------------------------------//---------------------//

               R1_R2_DY->hReal[0][0]=R1_R2_DY->hReal[0][2]-R1_R2_DY->hReal[0][1];//km, (parametr_r) dlina hordy

               //m, (Hkph) vysotnaya otmetka hordy v tochke kriticheskogo prepyatstviya
               R1_R2_DY->hReal[0][5]=R1_R2_DY->hReal[0][3]+(R1_R2_DY->hReal[0][4]-R1_R2_DY->hReal[0][3])/(R1_R2_DY->hReal[0][2]-R1_R2_DY->hReal[0][1])*(R_pr.real-R1_R2_DY->hReal[0][1]);

               R1_R2_DY->hReal[0][6]=surf_kp.real-R1_R2_DY->hReal[0][5];//m, (DY) vysota segmenta approksimiruyuschey sfery
               R1_R2_DY->hReal[0][7]=x_kas1;//км, расстояние до точки касания слева
        			R1_R2_DY->hReal[0][8]=x_kas2;//км, расстояние до точки касания справа
        			R1_R2_DY->hReal[0][9]=H1kas; //м, vysotnaya otmetka tochki kasaniya luchem profilya sleva
        			R1_R2_DY->hReal[0][10]=H2kas; //м, vysotnaya otmetka tochki kasaniya luchem profilya sprava
        			R1_R2_DY->hReal[0][11]=R1ellipse;//км, координата пересечения эллипсом поверхности профиля слева
        			R1_R2_DY->hReal[0][12]=R2ellipse;//км, координата пересечения эллипсом поверхности профиля справа
        			R1_R2_DY->hReal[0][13]=H1ellipse;//м, высотная отметка точки пересечения эллипсом поверхности профиля слева
        			R1_R2_DY->hReal[0][14]=H2ellipse;//м, высотная отметка точки пересечения эллипсом поверхности профиля справа
               R1_R2_DY->hReal[0][15]=R1hordaH0;//km, (R1hordaH0) rasstoyanie do tochki peresecheniya hordy(H0) profilya sleva
               R1_R2_DY->hReal[0][16]=R2hordaH0;//km, (R2hordaH0) rasstoyanie do tochki peresecheniya hordy(H0) profilya sprava
               R1_R2_DY->hReal[0][17]=H1hordaH0;//m, (H1hordaH0) vysotnaya otmetka tochki peresecheniya hordy(H0) profilya sleva
               R1_R2_DY->hReal[0][18]=H2hordaH0;//m, (H2hordaH0) vysotnaya otmetka tochki peresecheniya hordy(H0) profilya sprava
					R1_R2_DY->hReal[0][19]=(otm_zem2.real+H2.real-tg_k_min2*R.real-otm_zem1.real-H1.real)/(tg_k_max1-tg_k_min2);//km, (R_kross_kasat) rasstoyanie do tochki peresecheniya kasatelnyh
               R1_R2_DY->hReal[0][20]=Relyef->hReal[1][0]+H1.real+tg_k_max1*R1_R2_DY->hReal[0][19];//m, (H_kross_kasat) vysotnaya otmetka tochki peresecheniya kasatelnyh
               R1_R2_DY->hReal[0][21]=-R1_R2_DY->hReal[0][20]+((H1.real+otm_zem1.real)+R1_R2_DY->hReal[0][19]*(H2.real+otm_zem2.real-H1.real-otm_zem1.real)/R.real);//m, (Prosvet_kross_kasat) prosvet v tochke peresecheniya kasatelnyh
         		R1_R2_DY->hReal[0][22]=R1_R2_DY->hReal[0][19];//km, (R_kr) rasstoyanie do kriticheskogo prepyatstviya
         		R1_R2_DY->hReal[0][23]=R1_R2_DY->hReal[0][21];// m, (H_kr) prosvet v tochke kriticheskogo prepyatstviya s uchetom pogreshnosti
         		R1_R2_DY->hReal[0][24]=sqrt(1000./3.*R.real*lambda.real*(R1_R2_DY->hReal[0][19]/R.real)*(1.-R1_R2_DY->hReal[0][19]/R.real));//m, (H0) optimalny prosvet v tochke kriticheskogo prepyatstviya
         		R1_R2_DY->hReal[0][25]=R1_R2_DY->hReal[0][23]/R1_R2_DY->hReal[0][24];//m, (p_ot_g) otnositelny prosvet v tochke kriticheskogo prepyatstviya s uchetom pogreshnosti
            }
        }

        return 0;               // return 0 to indicate there was no error

    }// konec FHorda_Krit_PrepFunction



 
    LRESULT FSlinearFunction(COMPLEXARRAY * const Korny,			 // Opredelyaet korny systemy lineynyh uravneniy
                           COMPLEXARRAY * const M,          		 // M - matrica, v strokah kotoroy zapisany
                           COMPLEXSCALAR * const N)                // koefficienty pri peremennyh, a v poslednem
                           	             			                // stolbce - znachenya funkcii.
                           								                // N - kolichestvo peremennyh (kolichestvo uravnenyi)
                                                                   // Korny - korny systemy
    {
      COMPLEXARRAY P[100],b;
      long double c;
      unsigned i,j,k,l,m,n,N1,str;

      N1=N->real-1;



      for (i=0;i<N->real;i++)
      {
          	if ( !MathcadArrayAllocate( &P[i],  // allocate space for P[i]
                    (unsigned int)(N->real),    //with N rows
                    (unsigned int)(N->real+1),    //and N+1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
            return 2;           // if allocation is insufficient
                                // return the error code

      }
      if ( !MathcadArrayAllocate( &b,  // allocate space for b
                    (unsigned)(N->real-1),    //with N-1 rows
                    (unsigned)(N->real),    //and N cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
            return 2;           // if allocation is insufficient
                                // return the error code






      k=N1-1;
      for (i=0;i<=N1+1;i++)
      {
      	for(j=0;j<=N1;j++)
            P[0].hReal[i][j]=M->hReal[i][j];

      }

      for(i=1;i<=N1;i++)
      {

      	for(j=0;j<=k;j++)
         {
            str=-1;
            for(m=1;m<=k+1;m++)
            	if(P[i-1].hReal[0][m]!=0)
               	str=m;
         	for(l=0;l<=k+1;l++)
            {

            	if(str!=-1)
               	b.hReal[l][j]=(long double)P[i-1].hReal[l+1][j]-(long double)P[i-1].hReal[l+1][str]*(long double)P[i-1].hReal[0][j]/(long double)P[i-1].hReal[0][str];
                  else
                  	b.hReal[l][j]=P[i-1].hReal[l+1][j+1];
            }
         }
         for (m=0;m<=k+1;m++)
      	{

         	for(n=0;n<=k;n++)
         		P[i].hReal[m][n]=b.hReal[m][n];
      	}
         k--;
         for (m=0;m<N1+1;m++)
      	{

         	for(n=0;n<N1;n++)
              	b.hReal[m][n]=0;

      	}


      }

      if ( !MathcadArrayAllocate( Korny,  // allocate space for Korny
                    (unsigned int)(N->real),    //with N rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
            return 2;           // if allocation is insufficient
                                // return the error code



      Korny->hReal[0][N1]=0;
      for(i=0;i<=N1;i++)
      {

         c=0;
         for(k=0;k<=i;k++)
         	if(P[N1-i].hReal[0][k]!=0)
            	break;
         for(j=1;j<=i;j++)
         {

         	if(i!=0)
            	c=c+(long double)P[N1-i].hReal[j][k]*(long double)Korny->hReal[0][j+N1-i];
         }
         if (P[N1-i].hReal[0][k]!=0)
         	Korny->hReal[0][N1-i]=((long double)P[N1-i].hReal[i+1][k]-c)/(long double)P[N1-i].hReal[0][k];
         else
         	Korny->hReal[0][N1-i]=0;
      }
      return 0;               // return 0 to indicate there was no error

    }// konec FSlinearFunction

    LRESULT FInterp1Function(COMPLEXSCALAR * const F,			 // Opredelyaet znachenie funkcii interpolyaciey polinomom
    																			 // 2-j stepeni na intervalah mejdu dvumya sosednimi
                           					 						 // tochkami (doljno byt ne menee 3-h tochek)
                           COMPLEXARRAY * const X,
                           COMPLEXARRAY * const Y,
                           COMPLEXSCALAR * const x)          		 // X - vector znachenyi argumenta v tochkah
                           	             			                // Y - vector znachenyi funkcii v tochkah
                                                                   // x - argument
                                                                   // F - znachenie funkcii v tochke x.

    {
        unsigned i;
        double a,b,c,x1,x2,y1,y2,dy1,dy2;

        for(i=0;i<X->rows;i++)
        {
        	if(X->hReal[0][i]>=x->real)
         	break;
        }

        if(i<=1)
        {
        	x1=X->hReal[0][0];
         x2=X->hReal[0][1];
         y1=Y->hReal[0][0];
         y2=Y->hReal[0][1];
         dy1=2*(Y->hReal[0][1]-Y->hReal[0][0])/(X->hReal[0][1]-X->hReal[0][0])-(Y->hReal[0][2]-Y->hReal[0][0])/(X->hReal[0][2]-X->hReal[0][0]);
         dy2=(Y->hReal[0][2]-Y->hReal[0][0])/(X->hReal[0][2]-X->hReal[0][0]);
        }
        else if(i>X->rows-2)
        {
        	x1=X->hReal[0][X->rows-2];
         x2=X->hReal[0][X->rows-1];
         y1=Y->hReal[0][X->rows-2];
         y2=Y->hReal[0][X->rows-1];
         dy2=2*(Y->hReal[0][X->rows-1]-Y->hReal[0][X->rows-2])/(X->hReal[0][X->rows-1]-X->hReal[0][X->rows-2])-(Y->hReal[0][X->rows-1]-Y->hReal[0][X->rows-3])/(X->hReal[0][X->rows-1]-X->hReal[0][X->rows-3]);
         dy1=(Y->hReal[0][X->rows-1]-Y->hReal[0][X->rows-3])/(X->hReal[0][X->rows-1]-X->hReal[0][X->rows-3]);
        }
        else
        {
      	x1=X->hReal[0][i-1];
         x2=X->hReal[0][i];
         y1=Y->hReal[0][i-1];
         y2=Y->hReal[0][i];
         dy1=(Y->hReal[0][i]-Y->hReal[0][i-2])/(X->hReal[0][i]-X->hReal[0][i-2]);
         dy2=(Y->hReal[0][i+1]-Y->hReal[0][i-1])/(X->hReal[0][i+1]-X->hReal[0][i-1]);
        }

        a=(dy1-dy2)/2/(x1-x2);
        b=(y2-y1+a*x1*x1-a*x2*x2)/(x2-x1);
        c=y2-a*x2*x2-b*x2;

        F->real=a*x->real*x->real+b*x->real+c;

        return 0;               // return 0 to indicate there was no error


    }// konec FInterp1Function



    LRESULT FInterpFunction(COMPLEXSCALAR * const F,			 // Opredelyaet znachenie funkcii polinomialnoy interpolyaciey
                           COMPLEXARRAY * const X,
                           COMPLEXARRAY * const Y,
                           COMPLEXSCALAR * const x)          		 // X - vector znachenyi argumenta v tochkah
                           	             			                // Y - vector znachenyi funkcii v tochkah
                                                                   // x - argument
                                                                   // F - znachenie funkcii v tochke x.

    {
        COMPLEXARRAY M,Korny;
        COMPLEXSCALAR N;
        unsigned i,j;

        if ( !MathcadArrayAllocate( &M,  // allocate space for M
                    X->rows,    //with X->rows rows
                    X->rows+1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
            return 2;           // if allocation is insufficient
                                // return the error code

        for(i=0;i<X->rows;i++)
        {
         M.hReal[X->rows][i]=Y->hReal[0][i];
         M.hReal[X->rows-1][i]=1;
        	for(j=0;j<X->rows-1;j++)
        		M.hReal[j][i]=powl(X->hReal[0][i],(unsigned)(X->rows-1-j));
        }
        N.real=X->rows;
        FSlinearFunction(&Korny,&M,&N);
        F->real=0;
        for(i=0;i<X->rows;i++)
        {
        	if(i!=X->rows-1)
         	F->real=(long double)F->real+(long double)Korny.hReal[0][i]*powl(x->real,(unsigned)(X->rows-1-i));
         else
            F->real=(long double)F->real+(long double)Korny.hReal[0][i];
        }

        return 0;               // return 0 to indicate there was no error

    }// konec FInterpFunction

        LRESULT FInterp2Function(COMPLEXSCALAR * const F,			 // Opredelyaet znachenie funkcii interpolyaciey polinomom
    																			 // 2-j stepeni na intervalah mejdu dvumya sosednimi
                           					 						 // tochkami (doljno byt ne menee 3-h tochek)
                           COMPLEXARRAY * const X,
                           COMPLEXARRAY * const Y,
                           COMPLEXSCALAR * const x)          		 // X - vector znachenyi argumenta v tochkah
                           	             			                // Y - vector znachenyi funkcii v tochkah
                                                                   // x - argument
                                                                   // F - znachenie funkcii v tochke x.

    {
        unsigned i;
        double a,b,c,x1,x2,x3,y1,y2,y3,dy1,dy2;

        for(i=0;i<X->rows;i++)
        {
        	if(X->hReal[0][i]>=x->real)
         	break;
        }

        if(i<=1)
        {
        	x1=X->hReal[0][0];
         x2=X->hReal[0][1];
         x3=X->hReal[0][2];
         y1=Y->hReal[0][0];
         y2=Y->hReal[0][1];
         y3=Y->hReal[0][2];
         dy1=2*(y2-y1)/(x2-x1)-(y3-y1)/(x3-x1);
         a=(y3*(x2-x1)-x3*(y2-y1)-y1*x2+x1*y2)/(x3*x3*x2-x3*x3*x1+x1*x1*x3-x2*x2*x3-x1*x1*x2-x2*x2*x1);
         b=(y2-y1+a*x1*x1-a*x2*x2)/(x2-x1);
         dy2=2*a*x2+b;
        }
        else if(i>X->rows-2)
        {
        	x1=X->hReal[0][X->rows-2];
         x2=X->hReal[0][X->rows-1];
         x3=X->hReal[0][X->rows-3];
         y1=Y->hReal[0][X->rows-2];
         y2=Y->hReal[0][X->rows-1];
         y3=Y->hReal[0][X->rows-3];
         dy2=2*(y2-y1)/(x2-x1)-(y2-y3)/(x2-x3);
         a=(y2*(x1-x3)-x2*(y1-y3)-y3*x1+x3*y1)/(x2*x2*x1-x2*x2*x3+x3*x3*x2-x1*x1*x2-x3*x3*x1-x1*x1*x3);
         b=(y1-y3+a*x3*x3-a*x1*x1)/(x1-x3);
         dy1=2*a*x2+b;
        }
        else
        {
      	x1=X->hReal[0][i-1];
         x2=X->hReal[0][i];
         y1=Y->hReal[0][i-1];
         y2=Y->hReal[0][i];
         x3=X->hReal[0][i-2];
         y3=Y->hReal[0][i-2];
         a=(y2*(x1-x3)-x2*(y1-y3)-y3*x1+x3*y1)/(x2*x2*x1-x2*x2*x3+x3*x3*x2-x1*x1*x2-x3*x3*x1-x1*x1*x3);
         b=(y1-y3+a*x3*x3-a*x1*x1)/(x1-x3);
         dy1=2*a*x2+b;
         x3=X->hReal[0][i+1];
         y3=Y->hReal[0][i+1];
         a=(y3*(x2-x1)-x3*(y2-y1)-y1*x2+x1*y2)/(x3*x3*x2-x3*x3*x1+x1*x1*x3-x2*x2*x3-x1*x1*x2-x2*x2*x1);
         b=(y2-y1+a*x1*x1-a*x2*x2)/(x2-x1);
         dy2=2*a*x2+b;
        }

        a=(dy1-dy2)/2/(x1-x2);
        b=(y2-y1+a*x1*x1-a*x2*x2)/(x2-x1);
        c=y2-a*x2*x2-b*x2;

        F->real=a*x->real*x->real+b*x->real+c;

        return 0;               // return 0 to indicate there was no error


    }// konec FInterp2Function






	LRESULT FQFunction(COMPLEXARRAY * const Q,			 // Opredelyaet znachenie Aотр.,F(p(g),A),Q
                           COMPLEXSCALAR * const f,
                           COMPLEXSCALAR * const R,
                           COMPLEXSCALAR * const sigma,
                           COMPLEXSCALAR * const Suh,
                           COMPLEXSCALAR * const Peres,
                           COMPLEXSCALAR * const R_otr,
                           COMPLEXSCALAR * const p_ot_g_sr,
                           COMPLEXARRAY * const K_vp)          	 // f - chastota (GHz)
                           	             			                // R - Dlina intervala (km)
                                                                   // sigma - standartnoe otklonenie g (1/m)
                                                                   // Suh - 1 esli trassa suhoputnaya, 0 - esli primorskaya
                                                                   // Peres - 1 esli trassa peresechennaya, 0 - slaboperesechennaya
                                                                   // R_otr - rasstoyanie do tochki otragenya (km) (esli Peres==1, ignoriruetsya)
                                                                   // p_ot_g_sr - относительный prosvet pri средней refrakcii (m)
                                                                   // K_vp[0] - koefficient vodnoi poverhnosti (%)
                                                                   // K_vp[1] - h_sr, vysota trassy nad urovnem morya (m)
{
        double k,lambda,p_ot_g,right,left,A,f_ot_p_ot_g_ot_A=0;
        char minimum_mes[250],buf[10];
        unsigned i=1;
        COMPLEXARRAY f_array,q_suh,q_mor;
        COMPLEXSCALAR Q_suh,Q_mor,F_ot_p_ot_g_ot_A;

        if ( !MathcadArrayAllocate( Q,  // allocate space for Q
                    3,    //with 3 rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
        return 2;           // if allocation is insufficient
                                // return the error code


        if(Peres->real==0)
        {
        	k=R_otr->real/R->real;
        	lambda=0.3/f->real;
        	p_ot_g=p_ot_g_sr->real;
         do
         {
          	left=sqrt(6.*i-1.);//левая граница i-го минимума
            right=sqrt(6.*i+1.);//правая граница i-го минимума
            i++;
         }while(p_ot_g>=right);
         i--;//nomer minimuma v kotorom mojet raspolagatsya p_ot_g
         if(p_ot_g>=left && p_ot_g<=right)//prosvet nahoditsya v minimume
         {
            strcpy(minimum_mes,"Приемная антенна находится в минимуме:\np(g)=");
         	gcvt(p_ot_g,4,buf);
            strcat(minimum_mes,buf);
            strcat(minimum_mes,"\n");
            gcvt(i,3,buf);
            strcat(minimum_mes,buf);
            strcat(minimum_mes,"-й минимум ");
            gcvt(left,4,buf);
            strcat(minimum_mes,buf);
            strcat(minimum_mes,"<=p(g)<=");
            gcvt(right,4,buf);
            strcat(minimum_mes,buf);

           	MessageBox(NULL,minimum_mes,"!!!Минимум!!!",MB_ICONEXCLAMATION);
         }

         A=sqrt(lambda/(R->real*1000.)/(R->real*1000.)/(R->real*1000.)/k/(1.-k))/sigma->real;
         for(i=1;i<=5000;i++)
         	f_ot_p_ot_g_ot_A=f_ot_p_ot_g_ot_A+exp(-2.67*A*A*(p_ot_g-sqrt(6*i))*(p_ot_g-sqrt(6.*i)))/sqrt(i);

         F_ot_p_ot_g_ot_A.real=f_ot_p_ot_g_ot_A*A*0.36;
         F_ot_p_ot_g_ot_A.real=(F_ot_p_ot_g_ot_A.real<0.32)?F_ot_p_ot_g_ot_A.real:0.32;

         Q->hReal[0][0]=A;
         Q->hReal[0][1]=F_ot_p_ot_g_ot_A.real;

        	if ( !MathcadArrayAllocate( &f_array,  // allocate space for f_array
                    5,    //with 15 rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
            return 2;           // if allocation is insufficient
                                // return the error code

        	if ( !MathcadArrayAllocate( &q_suh,  // allocate space for q_suh
                    5,    //with 15 rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
            return 2;           // if allocation is insufficient
                                // return the error code

        	if ( !MathcadArrayAllocate( &q_mor,  // allocate space for q_mor
                    5,    //with 15 rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
            return 2;           // if allocation is insufficient
                                // return the error code

        	f_array.hReal[0][0]=0.04;
        	f_array.hReal[0][1]=0.14;
        	f_array.hReal[0][2]=0.22;
        	f_array.hReal[0][3]=0.28;
        	f_array.hReal[0][4]=0.32;

        	q_suh.hReal[0][0]=1.825;
        	q_suh.hReal[0][1]=1.925;
        	q_suh.hReal[0][2]=2.15;
        	q_suh.hReal[0][3]=2.7;
        	q_suh.hReal[0][4]=5;

        	q_mor.hReal[0][0]=5.25;
        	q_mor.hReal[0][1]=6.7;
        	q_mor.hReal[0][2]=8;
        	q_mor.hReal[0][3]=9.47;
        	q_mor.hReal[0][4]=12;

        	if(Suh->real!=0)
         {

            if(0.04<=F_ot_p_ot_g_ot_A.real && F_ot_p_ot_g_ot_A.real<=0.32)
            {
            	FInterp1Function(&Q_suh,&f_array,&q_suh,&F_ot_p_ot_g_ot_A);
        			Q->hReal[0][2]=Q_suh.real;
            }
            else if(F_ot_p_ot_g_ot_A.real<0.04)
               Q->hReal[0][2]=1.825;
            else
               Q->hReal[0][2]=5.;
         }
         else
         {
         	if(0.04<F_ot_p_ot_g_ot_A.real && F_ot_p_ot_g_ot_A.real<0.32)
            {
            	FInterp1Function(&Q_mor,&f_array,&q_mor,&F_ot_p_ot_g_ot_A);
        			Q->hReal[0][2]=Q_mor.real;
            }
            else if(F_ot_p_ot_g_ot_A.real<=0.04)
               Q->hReal[0][2]=5.;
            else
               Q->hReal[0][2]=12.;
         }
        }
        else if(Suh->real!=0 && K_vp->hReal[0][0]<20)
        	Q->hReal[0][2]=5.*(K_vp->hReal[0][0]/25.+0.2);
        else
         Q->hReal[0][2]=(K_vp->hReal[0][1]<500.)?5.:((K_vp->hReal[0][1]>=800.)?2.:2.5);

        return 0;               // return 0 to indicate there was no error

    }// konec FQFunction

      LRESULT FChrpFunction(COMPLEXARRAY * const SESR_Kng,			 // Opredelyaet znachenie SESR i Kng na uchastke
                           COMPLEXSCALAR * const K,                // rezervirovaniya pri chrp.
                           COMPLEXSCALAR * const N,
                           COMPLEXSCALAR * const N_ChRP,
                           COMPLEXSCALAR * const df,
                           COMPLEXSCALAR * const f,
                           COMPLEXARRAY * const Kint,
                           COMPLEXARRAY * const Tint_refr_d,
                           COMPLEXARRAY * const Suh_Peres,
                           COMPLEXARRAY * const Alfaint_refr,
                           COMPLEXARRAY * const p_ot_g)     // K - Kol-vo rabochih stvolov ne uchityvaya rezervnogo
                           	             			                // N - Kol-vo proletov na uchastke rezervirovaniya
                                                                   // N_ChRP -  Kol-vo proletov bez PRP na uchastke rezervirovaniya
                                                                   // df - Raznos po chastote megdu rezervnym i blijaishim
                                                                   //      k nemu rabochim (MHz)
                                                                   // f - Chastota rabochego stvola (MHz)
                                                                   // Kint - massiv K interferencii na intervalah
                                                                   // Tint_refr_d - massiv znacheniy Tint, T0, Tdg, Tdm, Tint_p,Tint_vg na intervalah (%)
                                                                   // Suh_Peres - massiv priznakov suhoputnosti, peresechennosti trass (1/0), h_sr srednyaya visota trassy, F(p(g),A), perepad vysot
                                                                   // Alfaint_refr - massiv znacheniy koefficientov alfaint i alfarefr.

                                                                  // p_ot_g - massiv otnositelnyh prosvetov v tochkah otrajeniya
                                                                   //          na slaboperesechennyh intervalah
                                                                   //SESR_Kng[0][0] - SESRчрп, %
                                                                   //SESR_Kng[0][1] - Кнг_чрп, %
                                                                   //SESR_Kng[0][2] - df/f,%
                                                                   //SESR_Kng[i][3] - Cdf_priz, параметр С_дельта_f для приземных трасс
                                                                   //SESR_Kng[0][4] - q, Параметр
                                                                   //SESR_Kng[0][5] - alfa_int, Параметр альфа_тау_инт_ЧРП
                                                                   //SESR_Kng[i][6] - Cf_priz, параметр С_f для приземных трасс
                                                                   //SESR_Kng[0][7] - Kng_month, Коэффициент неготовности при ЧРП за худший месяц
                                                                   //SESR_Kng[0][8] - T_chrp, Суммарная неустойчивость при ЧРП
                                                                   //SESR_Kng[i][9] - Tint_p_chrp, Интерференционная неустойчивость при ЧРП для приземной трассы горного интервала
                                                                   //SESR_Kng[i][10] - Cdf, параметр С_дельта_f для горных и высокогорных трасс
                                                                   //SESR_Kng[i][11] - Cf, параметр С_f для горных и высокогорных трасс
    {
      double *df_f_p, **Cdf_p,*Cdf_s_p, **Cdf_s, *p_g_s, *df_f_s, df_f, *Cdf,*Cdf_priz, *Cdf_s_1, q, alfa_int=0, *Cf,
      		 *Cf_priz,*beta_int,
      		 *beta_0,*hi, T1=0, T2=0, T3=0, T4=0, T_chrp, Kng_month, T1_month=0, T2_month=0, T3_month=0, T4_month=0,
             T1_vg=0,T1_month_vg=0, *Tint_p_chrp;
      int i,*Gorn;
      char raznos[110]="Недостаточный относительный разнос по частоте\n", buf[10];


      df_f_p=(double*)farcalloc(16,sizeof(double));

      Cdf_p=(double**)farcalloc(2,sizeof(double*));
      Cdf_p[0]=(double*)farcalloc(16,sizeof(double));
      Cdf_p[1]=(double*)farcalloc(16,sizeof(double));

      Cdf_s_p=(double*)farcalloc(16,sizeof(double));

      Cdf_s=(double**)farcalloc(5,sizeof(double*));
      Cdf_s[0]=(double*)farcalloc(11,sizeof(double));
      Cdf_s[1]=(double*)farcalloc(11,sizeof(double));
      Cdf_s[2]=(double*)farcalloc(11,sizeof(double));
      Cdf_s[3]=(double*)farcalloc(11,sizeof(double));
      Cdf_s[4]=(double*)farcalloc(11,sizeof(double));

      p_g_s=(double*)farcalloc(11,sizeof(double));

      df_f_s=(double*)farcalloc(5,sizeof(double));

      df_f_p[0]=0.75;
      df_f_p[1]=1;
      df_f_p[2]=1.25;
      df_f_p[3]=1.5;
      df_f_p[4]=1.75;
      df_f_p[5]=2;
      df_f_p[6]=2.25;
      df_f_p[7]=2.5;
      df_f_p[8]=2.75;
      df_f_p[9]=3;
      df_f_p[10]=3.25;
      df_f_p[11]=3.5;
      df_f_p[12]=3.75;
      df_f_p[13]=4;
      df_f_p[14]=4.25;
      df_f_p[15]=4.5;

      Cdf_p[0][0]=2267;     //сухопутные пересеченные
      Cdf_p[0][1]=1700;
      Cdf_p[0][2]=1600;
      Cdf_p[0][3]=1400;
      Cdf_p[0][4]=1100;
      Cdf_p[0][5]=900;
      Cdf_p[0][6]=780;
      Cdf_p[0][7]=690;
      Cdf_p[0][8]=610;
      Cdf_p[0][9]=550;
      Cdf_p[0][10]=500;
      Cdf_p[0][11]=460;
      Cdf_p[0][12]=430;
      Cdf_p[0][13]=420;
      Cdf_p[0][14]=405;
      Cdf_p[0][15]=400;

      Cdf_p[1][0]=1000.95;     //приморские пересеченные
      Cdf_p[1][1]=700;
      Cdf_p[1][2]=510;
      Cdf_p[1][3]=400;
      Cdf_p[1][4]=340;
      Cdf_p[1][5]=290;
      Cdf_p[1][6]=250;
      Cdf_p[1][7]=230;
      Cdf_p[1][8]=200;
      Cdf_p[1][9]=185;
      Cdf_p[1][10]=175;
      Cdf_p[1][11]=160;
      Cdf_p[1][12]=145;
      Cdf_p[1][13]=130;
      Cdf_p[1][14]=120;
      Cdf_p[1][15]=110;

      Cdf_s_p[0]=182.374;     //слабопересеченные приземные с перепадом высот
      Cdf_s_p[1]=144.05;
      Cdf_s_p[2]=120.;
      Cdf_s_p[3]=100;
      Cdf_s_p[4]=90;
      Cdf_s_p[5]=81;
      Cdf_s_p[6]=76;
      Cdf_s_p[7]=70;
      Cdf_s_p[8]=66;
      Cdf_s_p[9]=60;
      Cdf_s_p[10]=56;
      Cdf_s_p[11]=52;
      Cdf_s_p[12]=48;
      Cdf_s_p[13]=46;
      Cdf_s_p[14]=44;
      Cdf_s_p[15]=42;


      p_g_s[0]=1;
      p_g_s[1]=1.5;
      p_g_s[2]=2;
      p_g_s[3]=2.5;
      p_g_s[4]=3;
      p_g_s[5]=3.5;
      p_g_s[6]=4;
      p_g_s[7]=4.5;
      p_g_s[8]=5;
      p_g_s[9]=5.5;
      p_g_s[10]=6;

      df_f_s[0]=1;
      df_f_s[1]=1.5;
      df_f_s[2]=2;
      df_f_s[3]=3;
      df_f_s[4]=4.5;

      Cdf_s[0][0]=1000;     //слабопересеченные для отн. частотного разнесения 1%
      Cdf_s[0][1]=580;
      Cdf_s[0][2]=360;
      Cdf_s[0][3]=260;
      Cdf_s[0][4]=200;
      Cdf_s[0][5]=175;
      Cdf_s[0][6]=165;
      Cdf_s[0][7]=155;
      Cdf_s[0][8]=150;
      Cdf_s[0][9]=145;
      Cdf_s[0][10]=140;

      Cdf_s[1][0]=550;      //слабопересеченные для отн. частотного разнесения 1.5%
      Cdf_s[1][1]=300;
      Cdf_s[1][2]=175;
      Cdf_s[1][3]=112;
      Cdf_s[1][4]=82;
      Cdf_s[1][5]=68;
      Cdf_s[1][6]=60;
      Cdf_s[1][7]=55;
      Cdf_s[1][8]=53;
      Cdf_s[1][9]=51;
      Cdf_s[1][10]=50;

      Cdf_s[2][0]=350;      //слабопересеченные для отн. частотного разнесения 2%
      Cdf_s[2][1]=190;
      Cdf_s[2][2]=105;
      Cdf_s[2][3]=59;
      Cdf_s[2][4]=39;
      Cdf_s[2][5]=31;
      Cdf_s[2][6]=27;
      Cdf_s[2][7]=25;
      Cdf_s[2][8]=23.5;
      Cdf_s[2][9]=23;
      Cdf_s[2][10]=23;

      Cdf_s[3][0]=190;      //слабопересеченные для отн. частотного разнесения 3%
      Cdf_s[3][1]=90;
      Cdf_s[3][2]=45;
      Cdf_s[3][3]=27;
      Cdf_s[3][4]=17.5;
      Cdf_s[3][5]=13;
      Cdf_s[3][6]=11.5;
      Cdf_s[3][7]=10;
      Cdf_s[3][8]=9.2;
      Cdf_s[3][9]=8.7;
      Cdf_s[3][10]=8.5;

      Cdf_s[4][0]=100;      //слабопересеченные для отн. частотного разнесения 4.5%
      Cdf_s[4][1]=46.5;
      Cdf_s[4][2]=23;
      Cdf_s[4][3]=13;
      Cdf_s[4][4]=8;
      Cdf_s[4][5]=6;
      Cdf_s[4][6]=5.2;
      Cdf_s[4][7]=4.6;
      Cdf_s[4][8]=4.3;
      Cdf_s[4][9]=4;
      Cdf_s[4][10]=3.75;



      df_f=df->real/f->real*100.;

      Cdf=(double*)farcalloc(N_ChRP->real,sizeof(double));
      Cdf_priz=(double*)farcalloc(N_ChRP->real,sizeof(double));
      Tint_p_chrp=(double*)farcalloc(N_ChRP->real,sizeof(double));
      Gorn=(int*)farcalloc(N_ChRP->real,sizeof(int));
      if ( !MathcadArrayAllocate( SESR_Kng,  // allocate space for SESR_Kng
                    12,    //with 12 rows
                    N_ChRP->real,    //and N_ChRP->real cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
      return 2;           // if allocation is insufficient
                                // return the error code
      for(i=0;i<=N_ChRP->real-1;i++)
      {
      	if(Suh_Peres->hReal[2][i]<=800.)
      		Gorn[i]=0;
      	else if(Suh_Peres->hReal[2][i]>800. && Suh_Peres->hReal[2][i]<1200.)
      		Gorn[i]=1;
      	else
      		Gorn[i]=2;
      	if(Suh_Peres->hReal[1][i]==1)//если интервал пересеченный
      	{
      		if(Suh_Peres->hReal[0][i]==1)//если интервал сухопутный
            {
            	if(df_f<1.0)//недостаточный разнос по частоте
         		{
                  strcat(raznos,"для пересеченных сухопутных трасс\n");
            		gcvt(df_f,3,buf);
            		strcat(raznos,buf);
            		strcat(raznos,"%.");

           			MessageBox(NULL,raznos,"!!!ЧРП!!!",MB_ICONEXCLAMATION);
         		}
         		Cdf_priz[i]=(df_f<1.0)?17./(df_f/100.):linterp(16,df_f_p,Cdf_p[0],df_f);
               Cdf_priz[i]=(Cdf_priz[i]<400.)?400.:Cdf_priz[i];
               if(Suh_Peres->hReal[2][i]<800.)//приземная
               	Cdf[i]=Cdf_priz[i];
               else if(Suh_Peres->hReal[2][i]<1196.)//горная до 1196 м
                  Cdf[i]=Cdf_priz[i]*400./(1200.-Suh_Peres->hReal[2][i]);
               else//горные свыше 1196 м и высокогорные
                  Cdf[i]=Cdf_priz[i]*100.;
            }
         	else//приморский
            {
            	if(df_f<0.75)//недостаточный разнос по частоте
         		{
                  strcat(raznos,"для пересеченных приморских трасс\n");
            		gcvt(df_f,3,buf);
            		strcat(raznos,buf);
            		strcat(raznos,"%.");

           			MessageBox(NULL,raznos,"!!!ЧРП!!!",MB_ICONEXCLAMATION);
         		}
            	Cdf_priz[i]=(df_f<0.75)?2.32/pow(df_f/100.,1.24):linterp(16,df_f_p,Cdf_p[1],df_f);
               Cdf_priz[i]=(Cdf_priz[i]<110.)?110.:Cdf_priz[i];
               if(Suh_Peres->hReal[2][i]<800.)//приземная
               	Cdf[i]=Cdf_priz[i];
               else if(Suh_Peres->hReal[2][i]<1196.)//горная до 1196 м
                  Cdf[i]=Cdf_priz[i]*400./(1200.-Suh_Peres->hReal[2][i]);
               else//горные свыше 1196 м и высокогорные
                  Cdf[i]=Cdf_priz[i]*100.;
            }
      	}
      	else  //слабопересеченная трасса
      	{
         	if(df_f<1.0)//недостаточный разнос по частоте
            {
                  strcat(raznos,"для слабопересеченных трасс\n");
            		gcvt(df_f,3,buf);
            		strcat(raznos,buf);
            		strcat(raznos,"%.");

           			MessageBox(NULL,raznos,"!!!ЧРП!!!",MB_ICONEXCLAMATION);
            }
            if(Suh_Peres->hReal[2][i]<=800. && Suh_Peres->hReal[3][i]<=0.31 && Suh_Peres->hReal[3][i]>=0.3 && Suh_Peres->hReal[4][i]>=800.) //если трасса приземная, 0.3<=F(p(g),A)<=0.31, перепад высот>=800 м
            {
             	Cdf_priz[i]=(df_f<1.)?3.3/pow(df_f/100.,0.82):linterp(16,df_f_p,Cdf_s_p,df_f);
               Cdf_priz[i]=(Cdf_priz[i]<42.)?42.:Cdf_priz[i];
               Cdf[i]=Cdf_priz[i];
            }
            else
            {
      			Cdf_s_1=(double*)farcalloc(5,sizeof(double));
         		Cdf_s_1[0]=linterp(11,p_g_s,Cdf_s[0],p_ot_g->hReal[0][i]);
            	Cdf_s_1[0]=(Cdf_s_1[0]<125.)?125.:Cdf_s_1[0];
            	Cdf_s_1[1]=linterp(11,p_g_s,Cdf_s[1],p_ot_g->hReal[0][i]);
            	Cdf_s_1[1]=(Cdf_s_1[1]<40.)?40.:Cdf_s_1[1];
            	Cdf_s_1[2]=linterp(11,p_g_s,Cdf_s[2],p_ot_g->hReal[0][i]);
            	Cdf_s_1[2]=(Cdf_s_1[2]<20.)?20.:Cdf_s_1[2];
            	Cdf_s_1[3]=linterp(11,p_g_s,Cdf_s[3],p_ot_g->hReal[0][i]);
            	Cdf_s_1[3]=(Cdf_s_1[3]<8.)?8.:Cdf_s_1[3];
            	Cdf_s_1[4]=linterp(11,p_g_s,Cdf_s[4],p_ot_g->hReal[0][i]);
            	Cdf_s_1[4]=(Cdf_s_1[4]<3.5)?3.5:Cdf_s_1[4];
            	Cdf_priz[i]=linterp(5,df_f_s,Cdf_s_1,df_f);
            	Cdf_priz[i]=(Cdf_priz[i]<3.5)?3.5:Cdf_priz[i];
            	farfree(Cdf_s_1);
               if(Suh_Peres->hReal[2][i]<800.)//приземная  (см. Особенности программы DISAP, п. 4)
               	Cdf[i]=Cdf_priz[i];
               else if(Suh_Peres->hReal[2][i]<1196.)//горная до 1196 м
                  Cdf[i]=Cdf_priz[i]*400./(1200.-Suh_Peres->hReal[2][i]);
               else//горные свыше 1196 м и высокогорные
                  Cdf[i]=Cdf_priz[i]*100.;
            }
         }
      }

      q=0.0003*N->real*(K->real+1.);

      for(i=0;i<=N_ChRP->real-1;i++)
      	alfa_int=alfa_int+Alfaint_refr->hReal[0][i];
      alfa_int=alfa_int/N_ChRP->real;

      Cf=(double*)farcalloc(N_ChRP->real,sizeof(double));
      Cf_priz=(double*)farcalloc(N_ChRP->real,sizeof(double));
      for(i=0;i<=N_ChRP->real-1;i++)
      {
      	Cf_priz[i]=(Cdf_priz[i]+N_ChRP->real-1.)/N_ChRP->real;
         Cf[i]=(Cdf[i]+N_ChRP->real-1.)/N_ChRP->real;
      }

      beta_int=(double*)farcalloc(N_ChRP->real,sizeof(double));
      for(i=0;i<=N_ChRP->real-1;i++)
      {
         if(Gorn[i]==0 || Suh_Peres->hReal[1][i]==0 && Gorn[i]==1)//если приземная или слабопересеченная горная
         {
      		if(q*Tint_refr_d->hReal[0][i]*Alfaint_refr->hReal[0][i]+(K->real+1.)/2.*alfa_int*0.01*(1.-q)*Tint_refr_d->hReal[0][i]*Cf[i]*(1.-q)*Tint_refr_d->hReal[0][i]>=0.0004)
      			beta_int[i]=1;
         	else
            	beta_int[i]=0;
         }
         else if(Gorn[i]==1 && Suh_Peres->hReal[1][i]==1)//если горная пересеченная
         {
            Tint_p_chrp[i]=(K->real+1.)/2.*0.01*(1.-q)*Tint_refr_d->hReal[4][i]*Cf_priz[i]*(1.-q)*Tint_refr_d->hReal[4][i];
      		if(q*Tint_refr_d->hReal[0][i]*Alfaint_refr->hReal[0][i]+alfa_int*(Tint_p_chrp[i]+(Tint_refr_d->hReal[5][i]-Tint_p_chrp[i])/400.*(Suh_Peres->hReal[2][i]-800.))>=0.0004)
      			beta_int[i]=1;
         	else
            	beta_int[i]=0;
         }
         else //если высокогорная
            {
      		if(Tint_refr_d->hReal[0][i]*Alfaint_refr->hReal[0][i]>=0.0004)
      			beta_int[i]=1;
         	else
            	beta_int[i]=0;
         	}

      }

      beta_0=(double*)farcalloc(N_ChRP->real,sizeof(double));
      for(i=0;i<=N_ChRP->real-1;i++)
      {
      	if(Tint_refr_d->hReal[1][i]*Alfaint_refr->hReal[1][i]>=0.0004)
      		beta_0[i]=1;
         else
            beta_0[i]=0;
      }

      hi=(double*)farcalloc(N_ChRP->real,sizeof(double));
      for(i=0;i<=N_ChRP->real-1;i++)
      {
      	if(Suh_Peres->hReal[0][i]==0)
      		hi[i]=3;
         else
            hi[i]=4;
      }



      for(i=0;i<=N_ChRP->real-1;i++)
      {
         if(Gorn[i]==0 || Suh_Peres->hReal[1][i]==0 && Gorn[i]==1)//если приземная или слабопересеченная горная
         {
      		T1=T1+q*(Tint_refr_d->hReal[0][i]*Alfaint_refr->hReal[0][i]*beta_int[i]/hi[i]);//1-я часть интерференционной составляющей Кнг за год
         	T1_month=T1_month+q*(Tint_refr_d->hReal[0][i]*Alfaint_refr->hReal[0][i]*beta_int[i]);//1-я часть интерференционной составляющей Кнг за х. месяц

         	T2=T2+Tint_refr_d->hReal[0][i]*sqrt(Cf[i]/hi[i])*beta_int[i];//2-я часть интерференционной составляющей Кнг за год
         	T2_month=T2_month+Tint_refr_d->hReal[0][i]*sqrt(Cf[i])*beta_int[i];//2-я часть интерференционной составляющей Кнг за х. месяц
         }
         else if(Gorn[i]==1 && Suh_Peres->hReal[1][i]==1)//если горная пересеченная
         {
         	T1=T1+q*(Tint_refr_d->hReal[0][i]*Alfaint_refr->hReal[0][i]*beta_int[i]/hi[i]);//1-я часть интерференционной составляющей Кнг за год
            T1_month=T1_month+q*(Tint_refr_d->hReal[0][i]*Alfaint_refr->hReal[0][i]*beta_int[i]);//1-я часть интерференционной составляющей Кнг за х. месяц

            T2=T2+(Tint_refr_d->hReal[4][i]*sqrt(Cf[i])+(Tint_refr_d->hReal[5][i]-Tint_refr_d->hReal[4][i]*sqrt(Cf[i])/400.*(Suh_Peres->hReal[2][i]-800.)))*beta_int[i]/sqrt(hi[i]);//2-я часть интерференционной составляющей Кнг за год
            T2_month=T2_month+(Tint_refr_d->hReal[4][i]*sqrt(Cf[i])+(Tint_refr_d->hReal[5][i]-Tint_refr_d->hReal[4][i]*sqrt(Cf[i])/400.*(Suh_Peres->hReal[2][i]-800.)))*beta_int[i];//2-я часть интерференционной составляющей Кнг за х. месяц
         }
         else//если высокогорная
         {
         	T1_vg=T1_vg+Tint_refr_d->hReal[0][i]*Alfaint_refr->hReal[0][i]*beta_int[i]/hi[i];//интерференционная составляющая Кнг за год
            T1_month_vg=T1_month_vg+Tint_refr_d->hReal[0][i]*Alfaint_refr->hReal[0][i]*beta_int[i];//интерференционная составляющая Кнг за х. месяц
         }
         T3=T3+Tint_refr_d->hReal[1][i]*Alfaint_refr->hReal[1][i]*beta_0[i]/hi[i];//Субрефракционная составляющая Кнг за год
         T3_month=T3_month+Tint_refr_d->hReal[1][i]*Alfaint_refr->hReal[1][i]*beta_0[i];//Субрефракционная составляющая Кнг за х. месяц
         T4=T4+Tint_refr_d->hReal[2][i];//Дождевая составляющая Кнг за год
         T4_month=T4_month+Tint_refr_d->hReal[3][i];//Дождевая составляющая Кнг за х. месяц
      }
      Kng_month=T1_month+(K->real+1.)/2.*alfa_int*0.01*(1.-q)*(1.-q)*T2_month*T2_month+T3_month+T4_month+T1_month_vg;//Kнг_чрп за худший месяц
      SESR_Kng->hReal[0][1]=(f->real<1427.)?0.:T1+(K->real+1.)/2.*alfa_int*0.01*(1.-q)*(1.-q)*T2*T2+T3+T4+T1_vg;//Кнг_чрп

      T1=0;
      T2=0;
      T3=0;
      T1_month_vg=0;

      for(i=0;i<=N_ChRP->real-1;i++)
      {
      	if(Gorn[i]==0 || Suh_Peres->hReal[1][i]==0 && Gorn[i]==1)//если приземная или слабопересеченная горная
         {
      		T1=T1+Tint_refr_d->hReal[0][i]*Kint->hReal[0][i];
         	T2=T2+Tint_refr_d->hReal[0][i]*sqrt(Cf[i])*Kint->hReal[0][i];
         }
         else if(Gorn[i]==1 && Suh_Peres->hReal[1][i]==1)//если горная пересеченная
         {
            T1=T1+Tint_refr_d->hReal[0][i]*Kint->hReal[0][i];
            T2=T2+(Tint_refr_d->hReal[4][i]*sqrt(Cf[i])*Kint->hReal[0][i]+(Tint_refr_d->hReal[5][i]*Kint->hReal[0][i]-Tint_refr_d->hReal[4][i]*sqrt(Cf[i])*Kint->hReal[0][i]/400.*(Suh_Peres->hReal[2][i]-800.)));
         }
         else//если высокогорная
         {
            T1_month_vg=T1_month_vg+Tint_refr_d->hReal[0][i]*Kint->hReal[0][i];
         }
         T3=T3+Tint_refr_d->hReal[1][i];
      }
      T_chrp=T3+T4_month+q*T1+(K->real+1.)/2.*0.01*(1.-q)*(1.-q)*T2*T2+T1_month_vg;

      SESR_Kng->hReal[0][2]=df_f;
      for(i=0;i<=N_ChRP->real-1;i++)
      {
      	SESR_Kng->hReal[i][3]=Cdf_priz[i];
         SESR_Kng->hReal[i][6]=Cf_priz[i];
         SESR_Kng->hReal[i][9]=Tint_p_chrp[i];
         SESR_Kng->hReal[i][10]=Cdf[i];
         SESR_Kng->hReal[i][11]=Cf[i];
      }
      SESR_Kng->hReal[0][4]=q;
      SESR_Kng->hReal[0][5]=alfa_int;
      SESR_Kng->hReal[0][7]=Kng_month;
      SESR_Kng->hReal[0][8]=T_chrp;

      SESR_Kng->hReal[0][0]=T_chrp-Kng_month;//SESRчрп

      if(f->real<1427)//для диапазонов частот ниже 1427 МГц вся неустойчивость относится к периоду готовности (из разговора с Святогором В.В.)
      {
         SESR_Kng->hReal[0][0]=T_chrp;//SESRчрп
         SESR_Kng->hReal[0][1]=0;//Кнг_чрп
      	return 0;
      }





      return 0;               // return 0 to indicate there was no error

    }// konec FChrpFunction






      LRESULT FDeltaVdegrFunction(COMPLEXARRAY * const DeltaVdegr,			 // Opredelyaet znachenie degradacii porogovogo urovnya
                           COMPLEXSCALAR * const Tip,                // Tip - 1-2ОФМ;2-4ОФМ;3-8ОФМ;4-16КАМ;5-32КАМ;6 - 64КАМ,
                           COMPLEXSCALAR * const Vmin0,              //       7-128КАМ(ТСМ);8-256КАМ;9-512КАМ.
                           COMPLEXSCALAR * const Nmesh,              // Vmin0 - dB минимально допустимый ый множитель ослабления без
                           COMPLEXSCALAR * const R,                  //         учета деградации порогового уровня и
                           COMPLEXSCALAR * const Vsr,                //         среднего ослабления
                           COMPLEXARRAY * const Mmesh,               // Nmesh - Количество мешающих интервалов
                           COMPLEXSCALAR * const Zpordops,
                           COMPLEXARRAY * const MCochannel,
                           COMPLEXSCALAR * const C)           // R - км, протяженность рассчитываемого интервала
                           												      // Vsr - среднее ослабление на рассчитываемом интервале
                           									               // Mmesh - матрица параметров мешающих интервалов
                                                                     //			  0 столбец - км, протяженности трасс мешающих интервалов, а в Nmesh строке - признак реконструкции
                                                                     //         1 столбец - дБ, уменьшение уровня принимаемого сигнала
                                                                     //							  из-за ДНА (со знаком "-")
                                                                     //         2 столбец - дБ, разница в мощностях передатчиков мешающего и
                                                                     //                     полезного сигналов
                                                                     //         3 столбец - дБ, диапазон автоматической регулировки мощности
                                                                     //                     передатчика мешающего интервала
                                                                     //         4 столбец - дБ, значения множителяослабления мешающих сигналов
                                                                     //							  при среднем градиенте диэлектрической проницаемости
                                                                     //                     среды (со знаком "-")
                                                                     //         5 столбец - дБ, разница в коэффициентах усиления передающих
                                                                     //							  антенн мешающего и полезного сигналов
                                                                     //	        6 столбец - дБ, разница потерь в передающих фидерных трактах
                                                                     //                     шающего и  полезного сигналов.
                                                                     // Zpordops - отношение мощностей мешающего сигнала
                                                                     //            соседнего ствола и полезного сигнала,
                                                                     //            вызывающее в канале Рош_макс при
                                                                     //            деградации порогового уровня на 3 дБ.
                                                                     // MCochannel[0] - Cochannel - 1 - есть Co-channel, 0 - нет,
                                                                     // MCochannel[1] - Dp0 - дБ, кроссполяризационная защита приемной антенны
                                                                     // MCochannel[2] - Ixpic - дБ, выигрыш компенсатора помех,
                                                                     // MCochannel[3] - f - ГГц, рабочая частота,
                                                                     // С - Мбит/с - скорость передачи системы,
                                                                     // DeltaVdegr[0] - DeltaVdegr_int - дБ, деградация порогового уровня для
                                                                     //                      	интерференционной составляющей неустойчивости
                                                                     // DeltaVdegr[1] - DeltaVdegr_subr - дБ, деградация порогового уровня для
                                                                     //                      	субрефракционной составляющей неустойчивости
                                                                     // DeltaVdegr[2] - DeltaVdegr_dozhd - дБ, деградация порогового уровня для
                                                                     //                      	дождевой составляющей неустойчивости
                                                                     // DeltaVdegr[3] - Zpor_dop - дБ, пороговое защ. отношение
                                                                     // DeltaVdegr[4] - Zuzl - дБ, запас на интервале по помехе,
                                                                     //                            возникающей из-за узлообразования,
                                                                     //										относительно Zpor_dop,
                                                                     // DeltaVdegr[5] - Zc - дБ, отношение мощности мешающего сигнала,
                                                                     //                            возникающего в системах "co-channel",
                                                                     //										к пороговой мощности полезного сигнала,
                                                                     // DeltaVdegr[6] - Zc_dozhd - дБ, отношение мощности мешающего сигнала,
                                                                     //                            возникающего в системах "co-channel",
                                                                     //										к пороговой мощности полезного сигнала для
                                                                     //                      	дождевой составляющей неустойчивости,
                                                                     // DeltaVdegr[7] - Zs - дБ, отношение мощности мешающего сигнала от соседнего
                                                                     //									 канала к пороговой мощности полезного сигнала,
                                                                     // DeltaVdegr[8] - DVuzl - дБ, значение деградации порогового уровня, обусловленной
                                                                     //                         узлобразованием,
                                                                     // DeltaVdegr[9] - DVc - дБ, значение деградации порогового уровня в системах
                                                                     //                       "co-channel",
                                                                     // DeltaVdegr[10] - DVc_dozhd - дБ, значение деградации порогового уровня в системах
                                                                     //                       "co-channel" для дождевой составляющей неустойчивости,
                                                                     // DeltaVdegr[11] - DVs - дБ, значение деградации порогового уровня, обусловленной
                                                                     //                         помехами от соседнего ствола,
                                                                     // DeltaVdegr[12] - Zpor - дБ, отношение мощности мешающего сигнала,
                                                                     //                            возникающего из-за узлообразования,
                                                                     //										к пороговой мощности полезного сигнала,
                                                                     // DeltaVdegr[13] - Dp - дБ, отношение мощности полезного сигнала к мощности
                                                                     //                          кроссполяризованного мешающего сигнала для
                                                                     //                      интерференционной и субрефракционной составляющих неустойчивости
                                                                     // DeltaVdegr[14] - Dp_dozhd - дБ, отношение мощности полезного сигнала к мощности
                                                                     //                          кроссполяризованного мешающего сигнала для
                                                                     //                      	дождевой составляющей неустойчивости
                                                                     // DeltaVdegr[15] - Q - дБ, коэффициент, учитывающий наклон кроссполяризационной
                                                                     //                        диаграммы направленности антенны

    {
    	double Zpor_dop, Zpor=0, Zuzl=0, Zs, Zc=0, Zc_dozhd=0, DVuzl, DVs, Cochannel, Dp0, Ixpic, Cap, DVc, DVc_dozhd=0, Dp=0, Dp_dozhd=0, Q=0, f,
      		Reconstr;
      int i;
      char Dp_messge[500]="", str_buf[10];
      extern HINSTANCE g_hInst;		// the current instance определена в userpack.c

      Cap=C->real;
      switch ((unsigned)Tip->real)
      {
      	case 1: Zpor_dop=-10.6; break;
         case 2: Zpor_dop=-13; break;
         case 3: Zpor_dop=-17.5; break;
         case 4: Zpor_dop=-18.8; break;
         case 5: Zpor_dop=-25; break;
         case 6: Zpor_dop=-25; break;
         case 7: Zpor_dop=-29; break;
         case 8: Zpor_dop=-31; break;
         case 9: Zpor_dop=-33; break;
      }


      Reconstr=Mmesh->hReal[0][Nmesh->real];

      if(Reconstr==0)
      {
      	if(Cap<=5)
      		Zpor_dop=Zpor_dop-0.5;
         	else if(Cap>5 && Cap<=55)
         		Zpor_dop=Zpor_dop-1.0;
         		else Zpor_dop=Zpor_dop-2.0;
      }

      g_hInst=GetModuleHandle("drrl.dll");

      //******Ручное изменение Zпор.доп.*****

         DialogBoxParam(
         	g_hInst,	// handle of application instance
    			"IDD_DIALOG3",	// identifies dialog box template name
    			GetForegroundWindow(),	// handle of owner window
    			MPR_DialogFunc, 	// address of dialog box procedure
            (LPARAM)&(Zpor_dop)//дБ, Zpor_dop
   			);
      //***********************************************

      if(Nmesh->real>0)
      {
      	for(i=0;i<=Nmesh->real-1;i++)
      		Zpor=Zpor+pow(10.,0.1*(-Vmin0->real+Mmesh->hReal[1][i]-20.*log10(Mmesh->hReal[0][i]/R->real)+Mmesh->hReal[2][i]-Mmesh->hReal[3][i]+Mmesh->hReal[5][i]+Mmesh->hReal[4][i]-Vsr->real+Mmesh->hReal[6][i]));
      	Zpor=10.*log10(Zpor);

      	Zuzl=Zpor-Zpor_dop;
      	DVuzl=10.*log10(1.+pow(10.,0.1*Zuzl));
      }
      else DVuzl=0.;

      Zs=Zpor_dop-Zpordops->real;
      DVs=10.*log10(1.+pow(10.,0.1*Zs));

      Cochannel=MCochannel->hReal[0][0];
      if(Cochannel!=0)
      {
         Dp0=fabs(MCochannel->hReal[0][1]);
         Ixpic=fabs(MCochannel->hReal[0][2]);
         f=MCochannel->hReal[0][3];

         if(Dp0<=30.)
         	Q=11.;
            else if(Dp0>30. && Dp0<=35.)
            	Q=17.-0.2*Dp0;
            	else Q=45.-Dp0;

         Dp=Vmin0->real+Dp0+Q;
         Dp_dozhd=30.*log10(f)-12.8*pow(f,0.19)*log10(fabs(Vmin0->real))+15.;//методика стр. 142, ф.8.1
         if(Dp<=5.)
         {
            strcat(Dp_messge,"Необходимо использование ПРП, т.к.\n Dп=");
         	strcat(Dp_messge,gcvt(Dp,3,str_buf));
      		strcat(Dp_messge," дБ,\n что не превосходит 5 дБ");
            MessageBox(NULL,Dp_messge,"Xpic Внимание!!!",MB_ICONEXCLAMATION);
         }
         Zc=-(Dp+Ixpic)-Zpor_dop;
         Zc_dozhd=-(Dp_dozhd+Ixpic)-Zpor_dop;
         DVc=10.*log10(1.+pow(10.,0.1*Zc));
         DVc_dozhd=10.*log10(1.+pow(10.,0.1*Zc_dozhd));
      }
      else DVc=0.;

      if ( !MathcadArrayAllocate( DeltaVdegr,  // allocate space for DeltaVdegr
                    16,    //with 16 rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
      return 2;           // if allocation is insufficient
                                // return the error code

      DeltaVdegr->hReal[0][0]=10.*log10(pow(10.,0.1*DVuzl)+pow(10.,0.1*DVs)+pow(10.,0.1*DVc)-2.);//деградация для интерференционной неустойчивости
      DeltaVdegr->hReal[0][1]=10.*log10(pow(10.,0.1*DVuzl)+pow(10.,0.1*DVs)-1.);//деградация для субрефракционной неустойчивости (методика стр.96, дельтаVп=0)
      DeltaVdegr->hReal[0][2]=10.*log10(pow(10.,0.1*DVuzl)+pow(10.,0.1*DVs)+pow(10.,0.1*DVc_dozhd)-2.);//деградация для дождевой неустойчивости
      DeltaVdegr->hReal[0][3]=Zpor_dop;
      DeltaVdegr->hReal[0][4]=Zuzl;
      DeltaVdegr->hReal[0][5]=Zc;
      DeltaVdegr->hReal[0][6]=Zc_dozhd;
      DeltaVdegr->hReal[0][7]=Zs;
      DeltaVdegr->hReal[0][8]=DVuzl;
      DeltaVdegr->hReal[0][9]=DVc;
      DeltaVdegr->hReal[0][10]=DVc_dozhd;
      DeltaVdegr->hReal[0][11]=DVs;
      DeltaVdegr->hReal[0][12]=Zpor;
      DeltaVdegr->hReal[0][13]=Dp;
      DeltaVdegr->hReal[0][14]=Dp_dozhd;
      DeltaVdegr->hReal[0][15]=Q;

      return 0;               // return 0 to indicate there was no error

    }// konec FDeltaVdegrFunction



      LRESULT FTdFunction(COMPLEXARRAY * const Td,			 		 // Opredelyaet znachenie Неустойчивости из-за влияния осадков за наихудший месяц
                           COMPLEXSCALAR * const f,                // f - ГГц, рабочая частота;
                           COMPLEXSCALAR * const R,              	 // R - км, длина интервала;
                           COMPLEXSCALAR * const Pol,              // Pol - поляризация, 0 - горизонтальная, 1 - вертикальная;
                           COMPLEXSCALAR * const Vmin,             // Vmin - дБ, мин. доп. множитель ослабления для расчета Тд;
                           COMPLEXSCALAR * const Region)           // Region - номер региона по карте районирования по интенсивности дождей;
                           			                               // Td - 0-й эленмент - мм/ч,I_max
                                                                   //      1-й элемент - %, неустойчивость из-за влияния осадков за наихудший месяц
    {
     	double kdg, kdv, alfa_dg, alfa_dv, I_max, Args[6];
      double I1[17]={10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170},
      		 TD1[17]={0.085,0.038,0.026,0.0175,0.0124,0.0085,0.006,0.0043,0.0031,0.0022,0.00165,0.00115,0.00082,0.00055,0.000375,0.00025,0.00015},
             I2[21]={5,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200},
             TD2[21]={0.2,0.085,0.04,0.023,0.015,0.0095,0.0065,0.0045,0.00325,0.0025,0.00185,0.0016,0.0013,0.0011,0.00086,0.000725,0.00059,0.0005,0.00042,0.00036,0.0003},
             I3[14]={20,30,40,50,60,70,80,90,100,110,120,130,140,150},
             TD3[14]={0.035,0.022,0.013,0.0082,0.00525,0.0035,0.00225,0.0015,0.00095,0.00063,0.0004,0.000275,0.000175,0.000115},
             I4[19]={20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200},
             TD4[19]={0.055,0.03,0.019,0.0135,0.01,0.0078,0.006,0.00475,0.00375,0.003,0.00235,0.0019,0.0016,0.0013,0.0011,0.00088,0.00075,0.000625,0.000525},
             I5[20]={10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200},
             TD5[20]={0.15,0.0675,0.041,0.0275,0.02,0.015,0.0115,0.0086,0.007,0.0055,0.0045,0.0037,0.003,0.0024,0.00195,0.0017,0.0013,0.0011,0.00087,0.00069},
             I6[20]={10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200},
             TD6[20]={0.11,0.062,0.039,0.027,0.02,0.0145,0.011,0.008,0.006,0.00475,0.0037,0.00275,0.0022,0.0017,0.00128,0.000975,0.00077,0.0006,0.00048,0.00039},
             I7[20]={10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200},
             TD7[20]={0.2,0.055,0.038,0.0275,0.021,0.0165,0.013,0.0105,0.0085,0.0069,0.0055,0.0045,0.0037,0.00295,0.0024,0.00195,0.00165,0.00135,0.0012,0.00094},
             I8[19]={20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200},
             TD8[19]={0.07,0.045,0.028,0.0185,0.0135,0.01,0.008,0.0063,0.005,0.004,0.00315,0.0025,0.002,0.0016,0.0013,0.00095,0.00075,0.00056,0.00044},
             I9[20]={10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200},
             TD9[20]={0.11,0.0625,0.043,0.031,0.023,0.017,0.012,0.0082,0.0055,0.0039,0.00273,0.0019,0.00145,0.00105,0.0008,0.0006,0.00045,0.00035,0.00026,0.00018},
             I10[20]={10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200},
             TD10[20]={0.2,0.1,0.068,0.0475,0.035,0.0275,0.0205,0.017,0.013,0.01,0.00775,0.0058,0.0046,0.0036,0.00285,0.00225,0.00177,0.0014,0.00115,0.00085},
             I11[19]={20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200},
             TD11[19]={0.2,0.123,0.077,0.053,0.038,0.028,0.021,0.017,0.0135,0.012,0.01,0.0086,0.0074,0.006,0.0049,0.00385,0.0029,0.0022,0.0015},
             I12[20]={10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200},
             TD12[20]={0.0675,0.0335,0.023,0.017,0.013,0.01,0.008,0.0065,0.0054,0.00445,0.00375,0.00315,0.0027,0.0023,0.00193,0.0017,0.00147,0.0013,0.00115,0.00095},
             I13[11]={10,20,30,40,50,60,70,80,90,100,110},
             TD13[11]={0.095,0.037,0.017,0.0095,0.006,0.0042,0.003,0.00225,0.0017,0.0013,0.001},
             I14[20]={10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200},
             TD14[20]={0.13,0.054,0.03,0.018,0.0125,0.009,0.0065,0.005,0.0039,0.003,0.00245,0.0019,0.00155,0.00125,0.00094,0.00075,0.00059,0.00047,0.000385,0.00031},
             I15[20]={10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200},
             TD15[20]={0.13,0.048,0.028,0.0175,0.0125,0.0085,0.0059,0.0043,0.0032,0.0025,0.002,0.0017,0.0014,0.00125,0.0011,0.00094,0.00081,0.000725,0.00064,0.00055},
             I16[18]={10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180},
             TD16[18]={0.13,0.058,0.0325,0.019,0.0125,0.008,0.00525,0.0037,0.0027,0.0019,0.0015,0.0011,0.00079,0.00056,0.00041,0.00029,0.0002,0.00014},
             I17[15]={10,20,30,40,50,60,70,80,90,100,110,120,130,140,150},
             TD17[15]={0.13,0.048,0.028,0.0165,0.009,0.0054,0.0033,0.0021,0.00135,0.0008,0.000525,0.000335,0.000225,0.00015,0.0001},
             I18[20]={10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200},
             TD18[20]={0.17,0.065,0.0375,0.0225,0.0135,0.0085,0.0055,0.004,0.0028,0.002,0.0014,0.0011,0.00082,0.00065,0.00053,0.000445,0.00038,0.000325,0.00028,0.000245},
             I19[20]={10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200},
             TD19[20]={0.23,0.1,0.06,0.039,0.0275,0.019,0.0135,0.0095,0.00675,0.00495,0.0036,0.0027,0.002,0.00115,0.00085,0.000625,0.00046,0.00034,0.00025},
             I20[13]={10,20,30,40,50,60,70,80,90,100,110,120,130},
             TD20[13]={0.11,0.0425,0.0225,0.0125,0.0073,0.0044,0.0025,0.00145,0.00087,0.000525,0.00032,0.00019,0.00012},
             I21[7]={10,20,30,40,50,60,70},
             TD21[7]={0.08,0.019,0.0092,0.00485,0.00285,0.00173,0.00103},
             I22[11]={10,20,30,40,50,60,70,80,90,100,110},
             TD22[11]={0.0525,0.0225,0.0135,0.0081,0.0054,0.00375,0.00215,0.00115,0.00062,0.0003,0.00013};

      if ( !MathcadArrayAllocate( Td,  // allocate space for Td
                    2,    //with 2 rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
      return 2;           // if allocation is insufficient
                                // return the error code

      if(f->real<8.)
      {
      	Td->hReal[0][0]=200.;
         Td->hReal[0][1]=0.;
         return 0;
      }

      if(f->real<10.)
      {
      	kdg=0.000353+0.0000015*pow(f->real,3.815);
         kdv=0.000326+0.00000142*pow(f->real,3.773);
         alfa_dg=1.32-0.018*(f->real-7.4)*(f->real-7.4)+162/(12100-f->real*f->real*f->real*f->real);
         alfa_dv=1.262-0.0195*(f->real-7.4)*(f->real-7.4)+515/(13800-f->real*f->real*f->real*f->real);
      }
      else
      {
       	kdg=0.000614*pow(f->real-5.2,1.785);
         kdv=0.000789*pow(f->real-5.8,1.686);
         alfa_dg=1.195-0.0066*f->real+0.45/(f->real-7.);
         alfa_dv=1.172-0.0066*f->real+0.45/(f->real-7.);
      }

      Args[0]=kdv;
      Args[1]=kdg;
      Args[2]=alfa_dv;
      Args[3]=alfa_dg;
      Args[4]=Pol->real;
      Args[5]=R->real;


      I_max=minerr(Vd,1.,200.,0.005,Vmin->real,Args);
      Td->hReal[0][0]=I_max;

      switch((int)Region->real)
      {
      	case 1: Td->hReal[0][1]=linterp(17,I1,TD1,I_max); break;
         case 2: Td->hReal[0][1]=linterp(21,I2,TD2,I_max); break;
         case 3: Td->hReal[0][1]=linterp(14,I3,TD3,I_max); break;
         case 4: Td->hReal[0][1]=linterp(19,I4,TD4,I_max); break;
         case 5: Td->hReal[0][1]=linterp(20,I5,TD5,I_max); break;
         case 6: Td->hReal[0][1]=linterp(20,I6,TD6,I_max); break;
         case 7: Td->hReal[0][1]=linterp(20,I7,TD7,I_max); break;
         case 8: Td->hReal[0][1]=linterp(19,I8,TD8,I_max); break;
         case 9: Td->hReal[0][1]=linterp(20,I9,TD9,I_max); break;
         case 10: Td->hReal[0][1]=linterp(20,I10,TD10,I_max); break;
         case 11: Td->hReal[0][1]=linterp(19,I11,TD11,I_max); break;
         case 12: Td->hReal[0][1]=linterp(20,I12,TD12,I_max); break;
         case 13: Td->hReal[0][1]=linterp(11,I13,TD13,I_max); break;
         case 14: Td->hReal[0][1]=linterp(20,I14,TD14,I_max); break;
         case 15: Td->hReal[0][1]=linterp(20,I15,TD15,I_max); break;
         case 16: Td->hReal[0][1]=linterp(18,I16,TD16,I_max); break;
         case 17: Td->hReal[0][1]=linterp(15,I17,TD17,I_max); break;
         case 18: Td->hReal[0][1]=linterp(20,I18,TD18,I_max); break;
         case 19: Td->hReal[0][1]=linterp(20,I19,TD19,I_max); break;
         case 20: Td->hReal[0][1]=linterp(13,I20,TD20,I_max); break;
         case 21: Td->hReal[0][1]=linterp(7,I21,TD21,I_max); break;
         case 22: Td->hReal[0][1]=linterp(11,I22,TD22,I_max); break;
      }
      Td->hReal[0][1]=(Td->hReal[0][1]>0)?Td->hReal[0][1]:0.;

      return 0;

    }// konec FTdFunction

    LRESULT FNegotovnostFunction(COMPLEXARRAY * const Negotovnost,  // Opredelyaet коэффициенты готовности, SESR, коэффициент неготовности
                           COMPLEXARRAY * const Args)              // Args[0] - T0 - %, рефракционная составляющая неустойчивости;
                                            								  // Args[1] - Td - %, дождевая составляющая неустойчивости за год;
                                                                    // Args[2] - Tint - %, интерференционная составляющая неустойчивости;
                                                                    // Args[3] - Vmin_ef - дБ, мин. доп. множитель ослабления для расчета Тint;
                                                                    // Args[4] - Vmin - дБ, мин. доп. множитель ослабления для расчета Т0;
                                                                    // Args[5] - f - ГГц, рабочая частота;
                                                                    // Args[6] - R - км, длина интервала;
                                                                    // Args[7] - Peres - 0 - слабопересеченная, 1 - пересеченная;
                                                                    // Args[8] - Suh - 0 - приморская, 1 - сухопутная;
                                                                    // Args[9] - p_ot_g0 - относительный просвет в критической точке при средней рефракции;
                                                                    // Args[10] - p_ot_gint - относительный просвет в точке отражения при средней рефракции (если нет точки отражения, то p_ot_gint=p_ot_g0);
                                                                    // Args[11] - Otm_z_pd - м, отметка земли станции слева,
                                                                    // Args[12] - Otm_z_pm - м, отметка земли станции справа,
                                                                    // Args[13] - H1 - м, высота центра раскрыва антенны станции слева,
                                                                    // Args[14] - H2 - м, высота центра раскрыва антенны станции справа,
                           			                                // Negotovnost - 0-й элемент - интерференционный коэффициент готовности;
                                                                    //               1-й элемент - рефракционный коэффициент готовности;
                                                                    //               2-й элемент - коэффициент интерференции;
                                                                    //               3-й элемент - %, SESR;
                                                                    //               4-й элемент - %, коэффициент неготовности.
                                                                    //               5-й элемент - psi_tau_0, Параметр пси_тау при субрефракции.
                                                                    //               6-й элемент - Cm_0, Параметр Ц_м_0 при субрефракции.
                                                                    //               7-й элемент - tau_m_refr, Параметр тау_м_0 при субрефракции.
                                                                    //               8-й элемент - sigma_tau_0, Параметр сигма_тау_0 при субрефракции.
                                                                    //               9-й элемент - psi_tau_int, Параметр пси_тау при интерференции.
                                                                    //               10-й элемент - Cm_int, Параметр Ц_м_0 при интерференции.
                                                                    //               11-й элемент - tau_m_int, Параметр тау_м_0 при интерференции.
                                                                    //               12-й элемент - sigma_tau_int, Параметр сигма_тау_0 при интерференции.
    {
    	double Vmin_ef,Vmin,f,R,Peres,Suh,p_ot_g0,p_ot_gint,psi_tau_0,psi_tau_int, Cm_0,Cm_int_suh_peres,Cm_int_mor,Cm_int,
      		 tau_m_int, tau_m_refr,sigma_tau_0_1,sigma_tau_int_1,sigma_tau_0_2,sigma_tau_int_2,sigma_tau_0,sigma_tau_int,
             fv[21]={-40,-39,-38,-37,-36,-35,-34,-33,-32,-31,-30,-29,-28,-27,-26,-25,-24,-23,-22,-21,-20},
             f_sigma_tau[21]={4.7,4.74,4.77,4.82,4.86,4.9,4.95,5.03,5.1,5.19,5.28,5.38,5.47,5.57,5.68,5.8,5.94,6.07,6.23,6.37,6.53},
             delta_sigma_tau,Xn_0,Xn_int,t_0,t_int,alfa_tau_int,alfa_tau_refr,K_int,fi_tau_int,fi_tau_0,T0,Td,Tint,
             beta_int,beta_0,hi,Otm_z_pd,Otm_z_pm,H1,H2,Perepad;
      char prosvet_message[128]="";

      T0=Args->hReal[0][0];
      Td=Args->hReal[0][1];
      Tint=Args->hReal[0][2];
      Vmin_ef=Args->hReal[0][3];
      Vmin=Args->hReal[0][4];
      f=Args->hReal[0][5];
      R=Args->hReal[0][6];
      Peres=Args->hReal[0][7];
      Suh=Args->hReal[0][8];
      p_ot_g0=Args->hReal[0][9];
      p_ot_gint=Args->hReal[0][10];
      Otm_z_pd=Args->hReal[0][11];
      Otm_z_pm=Args->hReal[0][12];
      H1=Args->hReal[0][13];
      H2=Args->hReal[0][14];

      Perepad=fabs(Otm_z_pd-Otm_z_pm+H1-H2);

      if(p_ot_g0<0.)
      {
      	strcat(prosvet_message,"Для основной или, дополнительной (при наличии ПРП) антенны \n нет прямой видимости при средней рефракции");
      	MessageBox(NULL,prosvet_message,"Отрицательный просвет",MB_ICONEXCLAMATION	);
      }

      psi_tau_0=R*R*p_ot_g0*0.0001;
      psi_tau_int=R*R*p_ot_gint*0.0001;

      Cm_0=(psi_tau_0>0.1)?1./(0.00426*sqrt(psi_tau_0)-0.00046):1127.23015;
      Cm_int_suh_peres=(psi_tau_int>0.)?((psi_tau_int<=1.2)?1./(0.001+0.0141*pow(psi_tau_int,1.85)):40.):1000.;
      Cm_int_mor=(Perepad<200.)?((psi_tau_int>0.)?((psi_tau_int<=1.2)?1./(0.00222+0.0692*pow(psi_tau_int,2.15)):7.):450.45):Cm_int_suh_peres;
      Cm_int=(Peres+Suh==2)?Cm_int_suh_peres:Cm_int_mor;

      tau_m_int=1.924*Cm_int*pow(10.,Vmin_ef*0.05)/sqrt(f);
      tau_m_refr=1.924*Cm_0*pow(10.,Vmin*0.05)/sqrt(f);

      sigma_tau_0_1=linterp(21,fv,f_sigma_tau,Vmin);


      sigma_tau_int_1=linterp(21,fv,f_sigma_tau,Vmin_ef);
      

      delta_sigma_tau=5*log10(f/3.7);

      sigma_tau_0_2=sigma_tau_0_1+delta_sigma_tau;
      sigma_tau_int_2=sigma_tau_int_1+delta_sigma_tau;

      sigma_tau_0=(Suh==1)?sigma_tau_0_1:sigma_tau_0_2;
      sigma_tau_int=(Suh==1)?sigma_tau_int_1:sigma_tau_int_2;

      Xn_0=1./(0.3256*sigma_tau_0)*log(10./tau_m_refr)-0.1628*sigma_tau_0;
      Xn_int=1./(0.3256*sigma_tau_int)*log(10./tau_m_int)-0.1628*sigma_tau_int;
      t_0=1/(1+fabs(Xn_0)*0.47047);
      t_int=1/(1+fabs(Xn_int)*0.47047);

      if ( !MathcadArrayAllocate( Negotovnost,  // allocate space for Negotovnost
                    13,    //with 5 rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
      return 2;           // if allocation is insufficient
                                // return the error code

      Negotovnost->hReal[0][0]=0.5*(1.+(1.-(0.3480242*t_int-0.0958798*t_int*t_int+0.7478556*t_int*t_int*t_int)*exp(-Xn_int*Xn_int))*(Xn_int/fabs(Xn_int)));
      Negotovnost->hReal[0][1]=0.5*(1.+(1.-(0.3480242*t_0-0.0958798*t_0*t_0+0.7478556*t_0*t_0*t_0)*exp(-Xn_0*Xn_0))*(Xn_0/fabs(Xn_0)));

      alfa_tau_int=1-Negotovnost->hReal[0][0];
      alfa_tau_refr=1-Negotovnost->hReal[0][1];

      if(4.5>p_ot_gint && p_ot_gint>=3.6 && Peres==0 && -30<Vmin_ef)
      	Negotovnost->hReal[0][2]=1.5;
      else
      	if((4.5<=p_ot_gint && Peres==0) || (4.5>p_ot_gint && p_ot_gint>=3.6 && -30>=Vmin_ef && Peres==0))
      		Negotovnost->hReal[0][2]=2.;
         else
         	Negotovnost->hReal[0][2]=1.;

      K_int=Negotovnost->hReal[0][2];

      if(f<1.427)//для диапазонов частот ниже 1.427 ГГц вся неустойчивость относится к периоду готовности (из разговора с Святогором В.В.)
      {
         Negotovnost->hReal[0][3]=K_int*Tint+T0;
         Negotovnost->hReal[0][4]=0;
      	return 0;
      }

      if(Tint*alfa_tau_int>=0.0004)
      	fi_tau_int=Negotovnost->hReal[0][0];
      else
         fi_tau_int=1.;

      if(T0*alfa_tau_refr>=0.0004)
      	fi_tau_0=Negotovnost->hReal[0][1];
      else
         fi_tau_0=1.;

      Negotovnost->hReal[0][3]=(K_int==1 && fi_tau_int==0)?T0*fi_tau_0:((K_int-1.)/fi_tau_int+1.)*Tint*fi_tau_int+T0*fi_tau_0;

      beta_int=(Tint*alfa_tau_int>=0.0004)?1.:0.;
      beta_0=(T0*alfa_tau_refr>=0.0004)?1.:0.;
      hi=(Suh==0)?3.:4.;

      Negotovnost->hReal[0][4]=(Tint*alfa_tau_int*beta_int+T0*alfa_tau_refr*beta_0)/hi+Td;

      Negotovnost->hReal[0][5]=psi_tau_0;// Параметр пси_тау при субрефракции.
      Negotovnost->hReal[0][6]=Cm_0;//Параметр Ц_м_0 при субрефракции.
      Negotovnost->hReal[0][7]=tau_m_refr;//Параметр тау_м_0 при субрефракции.
      Negotovnost->hReal[0][8]=sigma_tau_0;// Параметр сигма_тау_0 при субрефракции.
      Negotovnost->hReal[0][9]=psi_tau_int;//Параметр пси_тау при интерференции.
      Negotovnost->hReal[0][10]=Cm_int;//Параметр Ц_м_0 при интерференции.
      Negotovnost->hReal[0][11]=tau_m_int;//Параметр тау_м_0 при интерференции.
      Negotovnost->hReal[0][12]=sigma_tau_int;//Параметр сигма_тау_0 при интерференции.

      return 0;

    }// konec FNegotovnostFunction




    LRESULT FVgazFunction(COMPLEXSCALAR * const Vgaz,  // Opredelyaet ослабление сигнала в газах атмосферы
                           COMPLEXARRAY * const Args)             // Args[0] - f - ГГц, рабочая частота;
                                            								  // Args[1] - R - км, длина интервала;
                                                                    // Args[2] - w - г/м3, абсолютная влажность водяного пара;
    {                                                               // Vgaz - дБ, ослабление сигнала в газах атмосферы;
    	double f,R,w,gamma_O2,gamma_H2O;

      f=Args->hReal[0][0];
      R=Args->hReal[0][1];
      w=Args->hReal[0][2];

      if(f<10)
      {
      	Vgaz->real=0;
         return 0;
      }

      gamma_O2=(0.00719+6.09/(f*f+0.277)+4.81/((f-57.)*(f-57.)+1.5))*f*f*0.001;
      gamma_H2O=(0.05+0.0021*w+3.6/((f-22.2)*(f-22.2)+8.5))*f*f*w*0.0001;
      Vgaz->real=-R*(gamma_O2+gamma_H2O);

      return 0;

    }// konec FVgazFunction


    LRESULT FBERPpmFunction(COMPLEXARRAY * const BERPpm,  // Opredelyaet значения коэффициента ошибок и порогового уровня
                           COMPLEXARRAY * const Args)              // Args[0] - Pпм_пор при BER=0.001 - дБм, пороговый уровень при коэффициенте ошибок 0.001;
                                            								  // Args[1] - С - Мбит/с, скорость передачи.
                                                                    // BERPpm[0] - BER - коэффициент ошибок;
                                                                    // BERPpm[1] - Pпм_пор - дБм, пороговый уровень при соответствующем скорости передачи коэффициенте ошибок.
    {
    	double P_pm_por_10_3,C,delta_P_pm_por;

      P_pm_por_10_3=Args->hReal[0][0];
      C=Args->hReal[0][1];

      if ( !MathcadArrayAllocate( BERPpm,  // allocate space for BERPpm
                    2,    //with 2 rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
      return 2;           // if allocation is insufficient
                                // return the error code

      if(C<=5.)
      {
      	BERPpm->hReal[0][0]=0.00032;
         delta_P_pm_por=0.5;
      }
      else
      	if(C>5 && C<=15)
         {
         	BERPpm->hReal[0][0]=0.0001;
            delta_P_pm_por=1;
      	}
         else
         	if(C>15 && C<=55)
            {
            	BERPpm->hReal[0][0]=0.000058;
               delta_P_pm_por=1;
      		}
            else
            {
            	BERPpm->hReal[0][0]=0.000018;
               delta_P_pm_por=2;
      		}

      BERPpm->hReal[0][1]=P_pm_por_10_3+delta_P_pm_por;


      return 0;

    }// konec FBERPpmFunction





    LRESULT FVodaFunction(COMPLEXARRAY * const Voda,  // Заполняет массив водных участков
                           COMPLEXARRAY * const Voda_uch,
                           COMPLEXARRAY * const Args)              // Voda_uch - исходный массив водных участков;
                                            								  // Args[0] - Hor - м, условный уровень горизонта;
                                                                    // Args[1] - N_voda_c - количество водных участков;
                                                                    // Voda - результирующий массив водных участков.
    {
    	double N_voda_c;
      int i,j;

      N_voda_c=Args->hReal[0][1];

      if ( !MathcadArrayAllocate( Voda,  // allocate space for Voda
                    (unsigned)(N_voda_c*4+1),    //with N_voda_c*4+1 rows
                    2,    //and 2 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
      	return 2;           // if allocation is insufficient
                                // return the error code

      Voda->hReal[0][0]=-0.001;
      Voda->hReal[1][0]=-500000.;

      for(i=1;i<=N_voda_c*4;i++)
      	for(j=0;j<=1;j++)
         {
         	if(j==1)//отметка
            {
            	if(fmod(i-2,4)==0 || fmod(i-3,4)==0)  //верхние точки
               	Voda->hReal[j][i]=0;
               else
               	Voda->hReal[j][i]=-500000.;     //нижние точки
            }
            else  //расстояние
            {
            	if(fmod(i-2,4)==0)   //ближняя верхняя точка
               	Voda->hReal[j][i]=Voda_uch->hReal[0][(i-2)/4];
               else
               	if(fmod(i-3,4)==0) //дальняя верхняя точка
                  	Voda->hReal[j][i]=Voda_uch->hReal[1][(i-3)/4];
                  else
                  	if(fmod(i-1,4)==0) //ближняя нижняя точка
                     	Voda->hReal[j][i]=Voda_uch->hReal[0][(i-1)/4]-0.000001;
                     else //дальняя нижняя точка
                     	Voda->hReal[j][i]=Voda_uch->hReal[1][(i-4)/4]+0.000001;
            }
         }

      return 0;

    }// konec FVodaFunction






    LRESULT FPrep_LesFunction(COMPLEXARRAY * const Prep,				 // Preobrazovyvaet massiv Prepyatstvie k udobnomu vidu
    							 COMPLEXSCALAR * const N_prep_c,			 			 // Prep_Les dlya interpolyacii лесных массивов
                         COMPLEXSCALAR * const R,
                         COMPLEXARRAY * const Prepyatstvie,
                         COMPLEXSCALAR * const Hor,
                         COMPLEXARRAY * const Relyef,
                         COMPLEXSCALAR * const a_e,
                         COMPLEXSCALAR * const N_relyef)
    {
		unsigned int i, j;
      COMPLEXSCALAR r,x;
      COMPLEXSCALAR y_x;

        // allocate space for the return array Prep
        if ( !MathcadArrayAllocate( Prep,  // allocate space for Prep
                    (unsigned int)(N_prep_c->real*4+2),    //with N_prep*4+2 rows
                    2,    //and 2 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
            return 2;           // if allocation is insufficient
                                // return the error code

        Prep->hReal[0][0]=-0.001;    //расстояние до нулевой точки препятствий
        Prep->hReal[1][0]=-50000.;   //отметка нулевой точки препятствий
        Prep->hReal[0][N_prep_c->real*4+1]=(Prepyatstvie->hReal[1][N_prep_c->real-1]<R->real+0.001)?R->real+0.001:Prepyatstvie->hReal[1][N_prep_c->real-1];//расстояние до последней точки препятствий
        Prep->hReal[1][N_prep_c->real*4+1]=-50000.;//отметка последней точки препятствий

        for(i=1;i<=N_prep_c->real*4;i++)
        {
        	if ( isUserInterrupted( ) )
            {
                MathcadArrayFree( Prep );
                return  3;      // user interrupted
            }
         for (j=0;j<=1;j++)
         	{
            	if(fmod(i-2,4)==0 && j==1 && Prepyatstvie->hReal[3][(i-2)/4]==0) //отметка ближней верхней точки
               																					  //одиночного препятствия
                                                                                //типа лес
               {
               	x.real=Prepyatstvie->hReal[0][(i-2)/4];//расстояние
               	FrFunction(&r,&x,Relyef,Hor,a_e,R,N_relyef);//вычисление отметки рельефа
                  Fy_xFunction(&y_x,&x,R,a_e,Hor);//вычисление кривизны
               	Prep->hReal[j][i]=r.real+Prepyatstvie->hReal[2][(i-2)/4]-y_x.real;
               }
               else
               	if(fmod(i-2,4)==0 && j==1 && Prepyatstvie->hReal[3][(i-2)/4]==1)//отметка ближней верхней точки
                  																					 //одиночного препятствия
                                                                                  //типа здание
                  {
               		Prep->hReal[j][i]=-50000.;
                  }
                  else
               		if(fmod(i-3,4)==0 && j==1 && Prepyatstvie->hReal[3][(i-3)/4]==0)//отметка дальней верхней точки
                     																					 //одиночного
                                                                                     //препятствия типа лес
                     {
                     	x.real=Prepyatstvie->hReal[1][(i-3)/4];
               			FrFunction(&r,&x,Relyef,Hor,a_e,R,N_relyef);
                        Fy_xFunction(&y_x,&x,R,a_e,Hor);
               			Prep->hReal[j][i]=r.real+Prepyatstvie->hReal[2][(i-3)/4]-y_x.real;
                     }
                     else
               			if(fmod(i-3,4)==0 && j==1 && Prepyatstvie->hReal[3][(i-3)/4]==1)//отметка дальней верхней точки
                        																					 //одиночного препятствия
                                                                                  		 //типа здание
                        {
               				Prep->hReal[j][i]=-50000.;
                        }
                        else
                        	if(fmod(i-2,4)==0 && j==0)//расстояние до верхней левой точки препятствия любого типа
                           {
               					Prep->hReal[j][i]=Prepyatstvie->hReal[0][(i-2)/4]+0.00000002;
                           }
                           else
               					if(fmod(i-3,4)==0 && j==0)//расстояние до верхней правой точки препятствия любого типа
                              {
               						Prep->hReal[j][i]=Prepyatstvie->hReal[1][(i-3)/4]-0.00000002;
                              }
                              else
               						if(fmod(i-1,4)==0 && j==1 || fmod(i-4,4)==0 && j==1)//отметки нижних точек
                        																					 //одиночного препятствия
                                                                                  		 //любого типа
                                 {
               							Prep->hReal[j][i]=-50000.;
                                 }
                                 else
               							if(fmod(i-1,4)==0 && j==0)//расстояние до нижней левой точки препятствия любого типа
                                 	{
               								Prep->hReal[j][i]=Prepyatstvie->hReal[0][(i-1)/4]+0.00000001;
                                 	}
                                    else
               								if(fmod(i-4,4)==0 && j==0)//расстояние до нижней правой точки препятствия любого типа
                                 		{
               									Prep->hReal[j][i]=Prepyatstvie->hReal[1][(i-4)/4]-0.00000001;
                                 		}
            }
        }

        return 0;               // return 0 to indicate there was no error

    }// konec FPrep_LesFunction




    LRESULT FPrep_DomFunction(COMPLEXARRAY * const Prep,				 // Preobrazovyvaet massiv Prepyatstvie k udobnomu vidu
    							 COMPLEXSCALAR * const N_prep_c,			 			 // Prep_Dom dlya interpolyacii застройки
                         COMPLEXSCALAR * const R,
                         COMPLEXARRAY * const Prepyatstvie,
                         COMPLEXSCALAR * const Hor,
                         COMPLEXARRAY * const Relyef,
                         COMPLEXSCALAR * const a_e,
                         COMPLEXSCALAR * const N_relyef)
    {
		unsigned int i, j;
      COMPLEXSCALAR r,x,y_x;

        // allocate space for the return array Prep
        if ( !MathcadArrayAllocate( Prep,  // allocate space for Prep
                    (unsigned int)(N_prep_c->real*4+2),    //with N_prep*4+2 rows
                    2,    //and 2 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
            return 2;           // if allocation is insufficient
                                // return the error code

        Prep->hReal[0][0]=-0.001;
        Prep->hReal[1][0]=-50000;
        Prep->hReal[0][N_prep_c->real*4+1]=(Prepyatstvie->hReal[1][N_prep_c->real-1]<R->real+0.001)?R->real+0.001:Prepyatstvie->hReal[1][N_prep_c->real-1];
        Prep->hReal[1][N_prep_c->real*4+1]=-50000;

        for(i=1;i<=N_prep_c->real*4;i++)
        {
        	if ( isUserInterrupted( ) )
            {
                MathcadArrayFree( Prep );
                return  3;      // user interrupted
            }
         for (j=0;j<=1;j++)
         	{
            	if(fmod(i-2,4)==0 && j==1 && Prepyatstvie->hReal[3][(i-2)/4]==1)
               {
               	x.real=Prepyatstvie->hReal[0][(i-2)/4];
               	FrFunction(&r,&x,Relyef,Hor,a_e,R,N_relyef);
                  Fy_xFunction(&y_x,&x,R,a_e,Hor);
               	Prep->hReal[j][i]=r.real+Prepyatstvie->hReal[2][(i-2)/4]-y_x.real;
               }
               else
               	if(fmod(i-2,4)==0 && j==1 && Prepyatstvie->hReal[3][(i-2)/4]==0)
                  {
               		Prep->hReal[j][i]=-50000.;
                  }
                  else
               		if(fmod(i-3,4)==0 && j==1 && Prepyatstvie->hReal[3][(i-3)/4]==1)
                     {
                     	x.real=Prepyatstvie->hReal[1][(i-3)/4];
               			FrFunction(&r,&x,Relyef,Hor,a_e,R,N_relyef);
                        Fy_xFunction(&y_x,&x,R,a_e,Hor);
               			Prep->hReal[j][i]=r.real+Prepyatstvie->hReal[2][(i-3)/4]-y_x.real;
                     }
                     else
               			if(fmod(i-3,4)==0 && j==1 && Prepyatstvie->hReal[3][(i-3)/4]==0)
                        {
               				Prep->hReal[j][i]=-50000.;
                        }
                        else
                        	if(fmod(i-2,4)==0 && j==0)
                           {
               					Prep->hReal[j][i]=Prepyatstvie->hReal[0][(i-2)/4]+0.00000002;
                           }
                           else
               					if(fmod(i-3,4)==0 && j==0)
                              {
               						Prep->hReal[j][i]=Prepyatstvie->hReal[1][(i-3)/4]-0.00000002;
                              }
                              else
               						if(fmod(i-1,4)==0 && j==1 || fmod(i-4,4)==0 && j==1)
                                 {
               							Prep->hReal[j][i]=-50000.;
                                 }
                                 else
               							if(fmod(i-1,4)==0 && j==0)
                                 	{
               								Prep->hReal[j][i]=Prepyatstvie->hReal[0][(i-1)/4]+0.00000001;
                                 	}
                                    else
               								if(fmod(i-4,4)==0 && j==0)
                                 		{
               									Prep->hReal[j][i]=Prepyatstvie->hReal[1][(i-4)/4]-0.00000001;
                                 		}
            }
        }

        return 0;               // return 0 to indicate there was no error

    }// konec FPrep_DomFunction




    LRESULT FK_vpFunction(COMPLEXSCALAR * const K_vp,				 // Определяет коэффициент водной поверхности
    							 COMPLEXSCALAR * const N_voda,
                         COMPLEXSCALAR * const R,
                         COMPLEXARRAY * const Voda_uch)
    {
    	int i=0;
      double r_v=0.;

      while(i<N_voda->real)
      {
      	r_v=r_v+Voda_uch->hReal[1][i]-Voda_uch->hReal[0][i];
         i++;
      }

      K_vp->real=r_v/R->real*100.;


        return 0;               // return 0 to indicate there was no error

    }// konec FK_vpFunction



    LRESULT Fg_sr_sigmaFunction(COMPLEXARRAY * const g_sr_sigma,				 // Определяет средний градиент диэлектрической проницаемости воздуха и его СКО
    							 COMPLEXSCALAR * const Raion)                      // Raion - номер климатического района по распределению
                                                                            //			эффективных градиентов диэлектрической
                                                                            //			проницаемости воздуха.
    {                                                                       // g_sr_sigma[0] - g_ср;
    	unsigned raion;                                                       // g_sr_sigma[1] - sigma;

      raion=(unsigned)Raion->real;

      if ( !MathcadArrayAllocate( g_sr_sigma,  // allocate space for g_sr_sigma
                    2,    //with 2 rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
      return 2;           // if allocation is insufficient
                                // return the error code


      switch(raion)
      {
      	case 1: g_sr_sigma->hReal[0][0]=-0.00000008;
                 g_sr_sigma->hReal[0][1]=0.00000007;
                 break;
         case 2: g_sr_sigma->hReal[0][0]=-0.0000001;
                 g_sr_sigma->hReal[0][1]=0.00000008;
                 break;
         case 3: g_sr_sigma->hReal[0][0]=-0.00000009;
                 g_sr_sigma->hReal[0][1]=0.000000075;
                 break;
         case 4: g_sr_sigma->hReal[0][0]=-0.00000007;
                 g_sr_sigma->hReal[0][1]=0.000000095;
                 break;
         case 5: g_sr_sigma->hReal[0][0]=0;
                 g_sr_sigma->hReal[0][1]=0.00000016;
                 break;
         case 6: g_sr_sigma->hReal[0][0]=-0.00000006;
                 g_sr_sigma->hReal[0][1]=0.00000007;
                 break;
         case 7: g_sr_sigma->hReal[0][0]=-0.000000013;
                 g_sr_sigma->hReal[0][1]=0.0000001;
                 break;
         case 8: g_sr_sigma->hReal[0][0]=-0.0000001;
                 g_sr_sigma->hReal[0][1]=0.00000009;
                 break;
         case 9: g_sr_sigma->hReal[0][0]=-0.00000007;
                 g_sr_sigma->hReal[0][1]=0.00000009;
                 break;
         case 10: g_sr_sigma->hReal[0][0]=-0.00000008;
                 g_sr_sigma->hReal[0][1]=0.00000008;
                 break;
         case 11: g_sr_sigma->hReal[0][0]=-0.00000006;
                 g_sr_sigma->hReal[0][1]=0.0000001;
                 break;
         case 12: g_sr_sigma->hReal[0][0]=-0.00000012;
                 g_sr_sigma->hReal[0][1]=0.00000009;
                 break;
         case 13: g_sr_sigma->hReal[0][0]=-0.00000007;
                 g_sr_sigma->hReal[0][1]=0.00000007;
                 break;
         case 14: g_sr_sigma->hReal[0][0]=-0.00000012;
                 g_sr_sigma->hReal[0][1]=0.0000001;
                 break;
         case 15: g_sr_sigma->hReal[0][0]=-0.0000001;
                 g_sr_sigma->hReal[0][1]=0.00000008;
                 break;
      }

        return 0;               // return 0 to indicate there was no error

    }// konec Fg_sr_sigmaFunction



    LRESULT Fsigma_ot_RFunction(COMPLEXSCALAR * const sigma_ot_R,				 // Определяет СКО градиента диэлектрической проницаемости воздуха
    							 COMPLEXARRAY * const Args)                      // Args[0] - длина интервала
                                                                            //Args[1] - среднее значение градиента
                                                                            //Args[2] - СКО табличное
    {
    	double R,g,sigma,Y;

      R=Args->hReal[0][0];
      g=Args->hReal[0][1];
      sigma=Args->hReal[0][2];

      Y=0.54+0.46*tanh(pow(R,1.5)*0.0054);

      sigma_ot_R->real=(R<=50)?((0.0000001+g/3.1)*(1./Y-1.)+sigma/Y):sigma;
      

        return 0;               // return 0 to indicate there was no error

    }// konec Fsigma_ot_RFunction


    LRESULT FTdyFunction(COMPLEXARRAY * const Tdy,				 // Определяет коэффициент Qd и неустойчивость по дождям в год
    							 COMPLEXARRAY * const Args)         // Args[0] - Tdm - неустойчивость по дождям в месяц
                         												 //Args[1] - Region - номер района по карте распределения Qd
                                                             //Tdy[0] - Qd
                                                             //Tdy[1] - Tdy
    {
    	double Tdm, AQ,BQ,CQ;
      unsigned Region;

      Tdm=Args->hReal[0][0];
      Region=(unsigned)Args->hReal[0][1];

      if ( !MathcadArrayAllocate( Tdy,  // allocate space for Tdy
                    2,    //with 2 rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
      return 2;           // if allocation is insufficient
                                // return the error code

      if(Tdm==0.)
      {
       	Tdy->hReal[0][0]=1.;
      	Tdy->hReal[0][1]=0.;
      }
      else
      {
      	switch(Region)
      	{
      		case 1: AQ=7.;
         			  BQ=0.2;
               	  CQ=1;
                    break;
	         case 2: AQ=3.5;
   	      		  BQ=0.6;
      	           CQ=0.5;
         	        break;
	         case 3: AQ=3.3;
   	      		  BQ=0.7;
      	           CQ=-0.31;
         	        break;
	         case 4: AQ=6538;
   	      		  BQ=-2.622;
      	           CQ=18.5;
         	        break;
	         case 5: AQ=0.09;
   	      		  BQ=2.;
      	           CQ=-5.4;
         	        break;
	         case 6: AQ=5.704;
   	      		  BQ=0.314;
      	           CQ=0.61;
         	        break;
	         case 7: AQ=3.5;
   	      		  BQ=0.6;
      	           CQ=0.5;
         	        break;
	         case 8: AQ=5;
   	      		  BQ=0.5;
      	           CQ=2.4;
         	        break;
	         case 9: AQ=7.;
   	      		  BQ=0.35;
      	           CQ=1.54;
         	        break;
	         case 10: AQ=10.4;
   	      		  BQ=0.0876;
      	           CQ=1.04;
         	        break;
	         case 11: AQ=7.3;
   	      		  BQ=0.253;
      	           CQ=1.2;
         	        break;
	         case 120: AQ=12.4;
   	      		  BQ=0.0657;
      	           CQ=1.4;
         	        break;
	         case 12: AQ=160.;
   	      		  BQ=-1.523;
      	           CQ=11.1;
         	        break;
	         case 21: AQ=160.;
   	      		  BQ=-1.523;
      	           CQ=11.1;
         	        break;
	         case 13: AQ=40.9;
   	      		  BQ=-0.788;
      	           CQ=9.8;
         	        break;
	         case 14: AQ=9.345;
   	      		  BQ=0.057;
      	           CQ=1.746;
         	        break;
	         case 20: AQ=9.345;
   	      		  BQ=0.057;
      	           CQ=1.746;
         	        break;
	         case 15: AQ=4.5;
   	      		  BQ=0.15;
      	           CQ=0.84;
         	        break;
	         case 16: AQ=13;
   	      		  BQ=-0.3;
      	           CQ=8.1;
         	        break;
	         case 17: AQ=1.3;
   	      		  BQ=1.;
      	           CQ=-2;
         	        break;
	         case 18: AQ=7.8;
   	      		  BQ=0.19;
      	           CQ=2.4;
         	        break;
	         case 19: AQ=1.4;
   	      		  BQ=1.;
      	           CQ=-0.33;
         	        break;
	         case 22: AQ=10.;
   	      		  BQ=0.078;
      	           CQ=1.7;
     	   }

         Tdy->hReal[0][0]=AQ*pow(fabs(log(Tdm)+CQ),BQ);
      	Tdy->hReal[0][1]=Tdm/Tdy->hReal[0][0];
      }

        return 0;               // return 0 to indicate there was no error

    }// konec FTdyFunction



    LRESULT FP_Vd_V0_teta_WFunction(COMPLEXARRAY * const P_Vd_V0_teta_W,				 // Определяет средний уровень мощности принимаемого сигнала,
    							 COMPLEXARRAY * const Args)         								 // минимально-допустимый множитель ослабления по замираниям в осадках (без учета деградации),
                         																				 // минимально-допустимый множитель ослабления по интерференционным и субрефракционным замираниям (без учета деградации),
                                                                                     // затухание в АФТ,
                                                                                     // ослабление сигнала в свободном пространстве.
                                                             								 // Args[0] - R - км, длина интервала,
                         												 								 // Args[1] - f - ГГц, рабочая частота,
                                                                                     // Args[2] - Pпд - дБм, мощность передатчика,
                                                                                     // Args[3] - Pпм_пор - дБм, пороговая чувствительность приемника,
                                                                                     // Args[4] - Gпд - дБи, коэффициент усиления передающей антенны,
                                                                                     // Args[5] - Gпм - дБи, коэффициент усиления приемной антенны,
                                                                                     // Args[6] - alfa_пд - дБ/м, удельное затухание передающего фидера,
                                                                                     // Args[7] - Lпд - м, длина передающего фидера,
                                                                                     // Args[8] - teta_post_пд - дБ, постоянные потери передающего АФТ,
                                                                                     // Args[9] - alfa_пд - дБ/м, удельное затухание приемного фидера,
                                                                                     // Args[10] - Lпд - м, длина приемного фидера,
                                                                                     // Args[11] - teta_post_пд - дБ, постоянные потери приемного АФТ,
                                                                                     // Args[12] - Vg - дБ, поглощение в газах атмосферы,
                                                                                     // Args[13] - Vdifr - дБ, дифракционное ослабление сигнала при средней рефракции,
                                                                                     // Args[14] - periskop_pd - 1/0, 1 - перископическая передающая антенна, 0 - нет,
                                                                                     // Args[15] - periskop_pm - 1/0, 1 - перископическая приемная антенна, 0 - нет,
                                                                                     // Args[16] - truba_pd - 1/0, 1 - передающая антенна устанавливается на трубчатой опоре, 0 - нет,
                                                                                     // Args[17] - truba_pm - 1/0, 1 - приемная антенна устанавливается на трубчатой опоре, 0 - нет,
                                                                                     // Args[18] - pass - 1/0, 1 - интервал с пассивным ретранслятором, 0 - нет,
                                                                                     // Args[19] - Otm_pd - м, отметка антенны передающей антенны,
                                                                                     // Args[20] - Otm_pm - м, отметка антенны приемной антенны,
                                                                                     // P_Vd_V0_teta_W[0] - Pпм - дБм, средний уровень мощности принимаемого сигнала,
                                                                                     // P_Vd_V0_teta_W[1] - Vmin0_d - дБ, минимально-допустимый множитель ослабления по замираниям в осадках (без учета деградации и среднего ослабления),
                                                                                     // P_Vd_V0_teta_W[2] - Vmin0_int_0 - дБ, минимально-допустимый множитель ослабления по интерференционным и субрефракционным замираниям (без учета деградации и среднего ослабления),
                                                                                     // P_Vd_V0_teta_W[3] - teta_AFT - дБ, затухание в АФТ,
                                                                                     // P_Vd_V0_teta_W[4] - W0 - дБ, ослабление в свободном пространстве.
                                                                                     // P_Vd_V0_teta_W[5] - Gpd_0_int - дБи, коэффициент усиления передающей антенны для расчета неустойчивости из-за рефракции и интерференции.
                                                                                     // P_Vd_V0_teta_W[6] - Gpm_0_int - дБи, коэффициент усиления приемной антенны для расчета неустойчивости из-за рефракции и интерференции.
    {
    	double R,f,Ppd,Ppm_por,Gpd,Gpm,alfa_pd,Lpd,teta_post_pd,alfa_pm,
             Lpm,teta_post_pm,Vg,Vdifr,periskop_pd,periskop_pm,truba_pd,truba_pm,
             pass,Otm_pd,Otm_pm,Ppm,Vmin0_d,Vmin0_int_0,teta_AFT,W0,hsr,Gpm_0_int,
             Gpd_0_int,lambda,teta_sum;

      R=Args->hReal[0][0];
      f=Args->hReal[0][1];
      Ppd=Args->hReal[0][2];
      Ppm_por=Args->hReal[0][3];
      Gpd=Args->hReal[0][4];
      Gpm=Args->hReal[0][5];
      alfa_pd=Args->hReal[0][6];
      Lpd=Args->hReal[0][7];
      teta_post_pd=Args->hReal[0][8];
      alfa_pm=Args->hReal[0][9];
      Lpm=Args->hReal[0][10];
      teta_post_pm=Args->hReal[0][11];
      Vg=Args->hReal[0][12];
      Vdifr=Args->hReal[0][13];
      periskop_pd=Args->hReal[0][14];
      periskop_pm=Args->hReal[0][15];
      truba_pd=Args->hReal[0][16];
      truba_pm=Args->hReal[0][17];
      pass=Args->hReal[0][18];
      Otm_pd=Args->hReal[0][19];
      Otm_pm=Args->hReal[0][20];

      hsr=(Otm_pd+Otm_pm)/2.;

      if((hsr<=800 && Gpm>43 && R>25 && truba_pm==1) || (pass==1 && Gpm>43 && R>25 && truba_pm==1) || ((hsr<=800 || pass==1) && Gpm>43 && R>25 && periskop_pm==1))
      	Gpm_0_int=43.;
      else
      	if((hsr<=800 && Gpm>44 && R>25 && periskop_pm!=1 && truba_pm!=1) || (pass==1 && Gpm>44 && R>25 && periskop_pm!=1 && truba_pm!=1))
         	Gpm_0_int=44.;
         else
         	if((hsr>800 && hsr<=1200 && pass!=1 && Gpm>45 && periskop_pm!=1) || (Gpm>45 && R<=25 && periskop_pm!=1))
            	Gpm_0_int=45.;
            else
            	Gpm_0_int=Gpm;

      if((hsr<=800 && Gpd>43 && R>25 && truba_pd==1) || (pass==1 && Gpd>43 && R>25 && truba_pd==1) || ((hsr<=800 || pass==1) && Gpd>43 && R>25 && periskop_pd==1))
      	Gpd_0_int=43.;
      else
      	if((hsr<=800 && Gpd>44 && R>25 && periskop_pd!=1 && truba_pd!=1) || (pass==1 && Gpd>44 && R>25 && periskop_pd!=1 && truba_pd!=1))
         	Gpd_0_int=44.;
         else
         	if((hsr>800 && hsr<=1200 && pass!=1 && Gpd>45 && periskop_pd!=1) || (Gpd>45 && R<=25 && periskop_pd!=1))
            	Gpd_0_int=45.;
            else
            	Gpd_0_int=Gpd;

      lambda=0.3/f;

      W0=20*log10(4.*M_PI*R*1000./lambda);

      teta_AFT=teta_post_pd+alfa_pd*Lpd+teta_post_pm+alfa_pm*Lpm;

      teta_sum=W0-Gpd_0_int-Gpm_0_int+teta_AFT-Vg-Vdifr;

      Ppm=Ppd-teta_sum;

      Vmin0_d=Ppm_por-Ppd+W0-Gpd-Gpm+teta_AFT;
      Vmin0_int_0=Ppm_por-Ppd+W0-Gpd_0_int-Gpm_0_int+teta_AFT;

      if ( !MathcadArrayAllocate( P_Vd_V0_teta_W,  // allocate space for P_Vd_V0_teta_W
                    7,    //with 7 rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
      return 2;           // if allocation is insufficient
                                // return the error code

      P_Vd_V0_teta_W->hReal[0][0]=Ppm;
      P_Vd_V0_teta_W->hReal[0][1]=Vmin0_d;
      P_Vd_V0_teta_W->hReal[0][2]=Vmin0_int_0;
      P_Vd_V0_teta_W->hReal[0][3]=teta_AFT;
      P_Vd_V0_teta_W->hReal[0][4]=W0;
      P_Vd_V0_teta_W->hReal[0][5]=Gpd_0_int;
      P_Vd_V0_teta_W->hReal[0][6]=Gpm_0_int;

        return 0;               // return 0 to indicate there was no error

    }// konec FP_Vd_V0_teta_WFunction


    LRESULT FV_min_effFunction(COMPLEXARRAY * const V_min_eff,				 // Определяет эффективное значение запаса на замирания, предельное значение запаса на замирания и предельное значение запаса на замирания с учетом эквалайзера.
    							 COMPLEXARRAY * const Args)         								// Args[0] - C - Мбит/с, емкость системы,
                         												 								 // Args[1] - f - ГГц, рабочая частота,
                                                                                     // Args[2] - R - км, длина интервала,
                                                                                     // Args[3] - Vmin - дБ, запас на замирания для расчета интерференционной составляющей,
                                                                                     // Args[4] - Peres - 1/0, 1 - трасса пересеченная, 0 - трасса слабопересеченная,
                                                                                     // Args[5] - Suh - 1/0, 1 - трасса сухопутная, 0 - трасса над водными массивами или приморская
                                                                                     // Args[6] - Ekv - 1/0, 1 - есть эквалайзер, 0 - нет,
                                                                                     // Args[7] - hc - дБ, высота сигнатуры,
                                                                                     // Args[8] - delta_fc - МГц, ширина сигнатуры,
                                                                                     // Args[9] - Tip -  тип системы: 1 - 2ОФМ, 2 - 4ОФМ, 3 - 8ОФМ, 4 - 16КАМ, 5 - 32КАМ, 6 - 64КАМ,7 - 128КАМ (ТСМ), 8 - 256КАМ, 9 - 512КАМ,
                                                                                     // V_min_eff[0] - Vmin_eff - дБ, эффективное значение запаса на замирания,
                                                                                     // V_min_eff[1] - Vmin_eff_pred - дБ, предельное эффективное значение запаса на замирания,
                                                                                     // V_min_eff[2] - Vmin_eff_pred_ekv - дБ, предельное эффективное значение запаса на замирания с учетом эквалайзера,
                                                                                     // V_min_eff[3] - Sс - МГц, Площадь сигнатуры
                                                                                     // V_min_eff[4] - Iэкв - дБ, выгрыш за счет эквалайзера

    {
    	double C,f,R,Vmin,Peres,Suh,Ekv,hc,delta_fc,Tip,Modulation,M,Sc,F_ot_M,I_ekv,Rf,FRf_suh,FRf_mor,F_ot_Rf,
      		 fRf[20]={100,120,140,160,180,200,220,240,260,280,300,320,340,360,380,400,420,440,460,480},
             fFRF_suh[20]={1,1,1,1.04,1.09,1.13,1.21,1.29,1.38,1.47,1.56,1.68,1.78,1.89,2,2.14,2.28,2.42,2.58,2.72},
             fFRF_mor[20]={1,1.08,1.17,1.31,1.48,1.63,1.8,1.98,2.15,2.31,2.49,2.65,2.81,2.99,3.15,3.32,3.49,3.67,3.83,4.},
             Vmin_ef_pred_peres,Ff_delta_V_suh,Ff_delta_V_mor,delta_Vmin_ef_pred,Vmin_ef_pred,Vmin_ef_pred_ekv,Vmin_ef,
             fRf1[18]={74,80,85,91,96,102,109,113,115,120,123,133,143,160,180,200,260,380},
             f_delta_V_suh[18]={0,0,0,0,0,0,0,0,0.25,0.3,0.5,1,1.5,2.2,2.76,3.23,3.7,3.78},
             f_delta_V_mor[18]={0,0.5,1,1.5,2,2.5,3,3.38,3.5,3.65,3.75,3.8,3.88,3.95,3.95,3.95,3.95,3.95};

      C=Args->hReal[0][0];
      f=Args->hReal[0][1];
      R=Args->hReal[0][2];
      Vmin=Args->hReal[0][3];
      Peres=Args->hReal[0][4];
      Suh=Args->hReal[0][5];
      Ekv=Args->hReal[0][6];
      hc=Args->hReal[0][7];
      delta_fc=Args->hReal[0][8];
      Tip=Args->hReal[0][9];

      if ( !MathcadArrayAllocate( V_min_eff,  // allocate space for V_min_eff
                    5,    //with 5 rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
      return 2;           // if allocation is insufficient
                                // return the error code

      if(Tip==1)
      {
       	V_min_eff->hReal[0][0]=Vmin;
      	V_min_eff->hReal[0][1]=-123456789.;
      	V_min_eff->hReal[0][2]=-123456789.;
      	V_min_eff->hReal[0][3]=0.;
      	V_min_eff->hReal[0][4]=123456789.;

        return 0;               // return 0 to indicate there was no error
      }

      Modulation=(Tip==1 || Tip==2 || Tip==3)?1.:0;

      switch((unsigned)Tip)
      {
      	case 1: M=2;break;
         case 2: M=4;break;
         case 3: M=8;break;
         case 4: M=16;break;
         case 5: M=32;break;
         case 6: M=64;break;
         case 7: M=128;break;
         case 8: M=256;break;
         case 9: M=512;break;
      }

      Sc=delta_fc*pow(10.,-fabs(hc)/20.);

      F_ot_M=(Modulation==0)?(sqrt(M)-1.)/log10(M)*log10(2.):log10(2.)/tan(M_PI/M)/log10(M);

      I_ekv=(Ekv==1)?10.*log10(45*C*C/19600.*F_ot_M/Sc):0.;

      Rf=R*f;
      if(Rf<100)
      	FRf_suh=1.;
      else
      	if(Rf>480)
         	FRf_suh=2.72;
         else
      		FRf_suh=linterp(20,fRf,fFRF_suh,Rf);

      if(Rf<100)
      	FRf_mor=1.;
      else
      	if(Rf>480)
         	FRf_mor=4;
         else
      		FRf_mor=linterp(20,fRf,fFRF_mor,Rf);

      F_ot_Rf=(Suh==1)?FRf_suh:FRf_mor;

      Vmin_ef_pred_peres=20*log10(0.000907*C/f*F_ot_M*F_ot_Rf);

      if(Rf<113)
      	Ff_delta_V_suh=0.;
      else
      	if(Rf>380)
         	Ff_delta_V_suh=3.78;
         else
         	Ff_delta_V_suh=linterp(18,fRf1,f_delta_V_suh,Rf);

      if(Rf<74)
      	Ff_delta_V_mor=0.;
      else
      	if(Rf>160)
         	Ff_delta_V_mor=3.95;
         else
         	Ff_delta_V_mor=linterp(18,fRf1,f_delta_V_mor,Rf);

      delta_Vmin_ef_pred=(Suh==1)?Ff_delta_V_suh:Ff_delta_V_mor;

      Vmin_ef_pred=(Peres==1)?Vmin_ef_pred_peres:Vmin_ef_pred_peres+delta_Vmin_ef_pred;

      Vmin_ef_pred_ekv=Vmin_ef_pred-I_ekv;

      Vmin_ef=10.*log10(pow(10.,0.1*Vmin)+pow(10.,0.1*Vmin_ef_pred_ekv));



      V_min_eff->hReal[0][0]=Vmin_ef;
      V_min_eff->hReal[0][1]=Vmin_ef_pred;
      V_min_eff->hReal[0][2]=Vmin_ef_pred_ekv;
      V_min_eff->hReal[0][3]=Sc;
      V_min_eff->hReal[0][4]=I_ekv;

        return 0;               // return 0 to indicate there was no error

    }// konec FV_min_effFunction


    LRESULT FT_intFunction(COMPLEXARRAY * const T_int,				 // Определяет интерференционную составляющую неустойчивости.
    							 COMPLEXARRAY * const Args)         								// Args[0] - R - км, длина интервала,
                         												 								 // Args[1] - Q - , климатический коэффициент с учетом топографической особенности местности,
                                                                                     // Args[2] - f - ГГц, рабочая частота,
                                                                                     // Args[3] - Vmin_ef - дБ, эффективный запас на замирания для расчета интерференционной составляющей,
                                                                                     // Args[4] - Vmin - дБ, запас на плоские замирания для расчета интерференционной составляющей,
                                                                                     // Args[5] - V_50 - дБ, среднее ослабление из-за наклона трассы,
                                                                                     // Args[6] - Peres - 1 - трасса пересеченная, 0 - трасса слабопересеченная,
                                                                                     // Args[7] - Otm_z_pd - м, отметка земли станции слева,
                                                                                     // Args[8] - Otm_z_pm - м, отметка земли станции справа,
                                                                                     // Args[9] - H1 - м, высота центра раскрыва антенны станции слева,
                                                                                     // Args[10] - H2 - м, высота центра раскрыва антенны станции справа,
                                                                                     // T_int[0] - T_delta_epsilon - %, параметр, учитывающий вероятность возникновения многолучевых замираний,
                                                                                     //                              обусловленных отражениями радиоволн от слоистых неоднородностей тропосферы
                                                                                     //                              с перепадом диэлектрической проницаемости воздуха delta_epsilon,
                                                                                     // T_int[1] - T_int - %, интерференционная составляющая неустойчивости.
                                                                                     // T_int[2] - h_sr - м, средняя высота трассы над уровнем моря.
                                                                                     // T_int[3] - sigma0 - параметр сигма0.
                                                                                     // T_int[4] - sigma1 - параметр сигма1.
                                                                                     // T_int[5] - T_int_p - %, интерференционная составляющая неустойчивости для приземных трасс.
                                                                                     // T_int[6] - T_int_vg - %, интерференционная составляющая неустойчивости для высокогорных трасс.
    {
      double R,Q,f,Vmin_ef,Vmin,V_50,Peres,Otm_z_pd,Otm_z_pm,H1,H2,
      			h_sr,sigma0,sigma1,T_int_p,T_int_vg,T_int_g,t1,
               V_sigma0_R[]={0.,10.,20.,30.},
               V_sigma0_sigma[]={0.,0.3,0.6,0.91263};
      int Gorn;

      R=Args->hReal[0][0];
      Q=Args->hReal[0][1];
      f=Args->hReal[0][2];
      Vmin_ef=Args->hReal[0][3];
      Vmin=Args->hReal[0][4];
      V_50=Args->hReal[0][5];
      Peres=Args->hReal[0][6];
      Otm_z_pd=Args->hReal[0][7];
      Otm_z_pm=Args->hReal[0][8];
      H1=Args->hReal[0][9];
      H2=Args->hReal[0][10];

    	if ( !MathcadArrayAllocate( T_int,  // allocate space for T_int
                    7,    //with 2 rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
      return 2;           // if allocation is insufficient
                                // return the error code

      h_sr=(Otm_z_pd+H1+Otm_z_pm+H2)/2.;

      if(h_sr<=800.)
      	Gorn=0;
      else if(h_sr>800. && h_sr<1200.)
      	Gorn=1;
      else
      	Gorn=2;

      sigma0=(R>=30.)?1.8*sqrt(sqrt(R))-3.3:linterp(4,V_sigma0_R,V_sigma0_sigma,R);

      sigma1=(R>6.41)?sqrt((1.-f*f/12.25)*(1.8*sqrt(sqrt(R))-3.3)*(1.8*sqrt(sqrt(R))-3.3)+f*f/12.25*(0.068*R*(1.+0.0000000000032*pow(R,5.)))*(0.068*R*(1.+0.0000000000032*pow(R,5.)))):0.43263;

      t1=(Vmin-V_50+2.326*sigma0)/1.41421356/sigma1-1.645;

      T_int_vg=(Gorn!=0 && Peres==1.)?23.5*exp(-0.9822*t1*t1)/(-t1):123456789.;

      if(Gorn==2 && Peres==1.)
         T_int->hReal[0][0]=123456789;
      else
      	T_int->hReal[0][0]=(R>=20)?0.00041*Q*pow(f,1.5)*R*R:0.0000205*Q*pow(f,1.5)*R*R*R;

      T_int_p=(Gorn==2 && Peres==1.)?123456789.:pow(10.,Vmin_ef/10.)*T_int->hReal[0][0];

      T_int_g=(Gorn==1 && Peres==1.)?T_int_p+(T_int_vg-T_int_p)/400.*(h_sr-800.):123456789.;

      if(Gorn==0 || Peres==0.)
      	T_int->hReal[0][1]=T_int_p;
      else
         T_int->hReal[0][1]=(Gorn==2)?T_int_vg:T_int_g;

      T_int->hReal[0][2]=h_sr;
      T_int->hReal[0][3]=sigma0;
      T_int->hReal[0][4]=sigma1;
      T_int->hReal[0][5]=T_int_p;
      T_int->hReal[0][6]=T_int_vg;

        return 0;               // return 0 to indicate there was no error

    }// konec FT_intFunction

    LRESULT FRek_raznFunction(COMPLEXARRAY * const Rek_razn,				 // Определяет рекомендуемый разнос слева и справа для ПРП.
    							 COMPLEXARRAY * const Args)         				 // Args[0] - Peres - 1 - пересеченная трасса, 0 - слабопересеченная,
                         																 // Args[1] - f - ГГц, рабочая частота,
                                                                         // Args[2] - R - км, протяженность интервала,
                                                                         // Args[3] - R_otr - км, расстояние до точки отражения,
                                                                         // Args[4] - Otm_z_pd - м, отметка земли станции слева,
                                                                         // Args[5] - Otm_z_pm - м, отметка земли станции справа,
                                                                         // Args[6] - H1 - м, высота центра раскрыва антенны станции слева,
                                                                         // Args[7] - H2 - м, высота центра раскрыва антенны станции справа,
                                                                         // Rek_razn[0] - м, рекомендуемый разнос антенн слева,
                                                                         // Rek_razn[1] - м, рекомендуемый разнос антенн справа.
    {
    	double Peres, f, R, R_otr, lambda, k,Perepad,Otm_z_pd,Otm_z_pm,H1,H2;


      Peres=Args->hReal[0][0];
      f=Args->hReal[0][1];
      R=Args->hReal[0][2];
      R_otr=Args->hReal[0][3];
      Otm_z_pd=Args->hReal[0][4];
      Otm_z_pm=Args->hReal[0][5];
      H1=Args->hReal[0][6];
      H2=Args->hReal[0][7];

      Perepad=fabs(Otm_z_pd+H1-Otm_z_pm-H2);

      lambda=0.3/f;

      if ( !MathcadArrayAllocate( Rek_razn,  // allocate space for Rek_razn
                    2,    //with 2 rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
      return 2;           // if allocation is insufficient
                                // return the error code


      if(Peres==1)
		{
      	Rek_razn->hReal[0][0]=(Perepad<=1000.)?160.*lambda:700.*lambda;
         Rek_razn->hReal[0][1]=(Perepad<=1000.)?160.*lambda:700.*lambda;
      }
      else
      {
      	k=R_otr/R;
         Rek_razn->hReal[0][0]=0.118*sqrt(R*lambda*1000.*k/(1.-k));
         Rek_razn->hReal[0][1]=0.118*sqrt(R*lambda*1000.*(1.-k)/k);
      }

        return 0;               // return 0 to indicate there was no error

    }// konec FRek_raznFunction



    LRESULT FPRP_peresFunction(COMPLEXARRAY * const PRP,				 // Определяет выигрыш за счет ПРП, Тинт при ПРП, SESR при ПРП, Кнг при ПРП
    							 COMPLEXARRAY * const Args)         				 // Args[0] - Suh - 1 - сухопутная трасса, 0 - приморская,
                         																 // Args[1] - f - ГГц, рабочая частота,
                                                                         // Args[2] - Delta_h - м, разнос между основной и дополнительной приемными антеннами,
                                                                         // Args[3] - alfa_tau_int - коэффициент для расчета неготовности по интерференции,
                                                                         // Args[4] - alfa_tau_0 - коэффициент для расчета неготовности по рефракции,
                                                                         // Args[5] - T_int - %,неустойчивость из-за интерференции без учета резерва,
                                                                         // Args[6] - T_0 - %,неустойчивость из-за рефракции без учета резерва,
                                                                         // Args[7] - T_d - %,неустойчивость из-за осадков за год,
                                                                         // Args[8] - nu - раз, разница в усилениях основной и разнесенной антенн,
                                                                         // Args[9] - alfa_tau_int_d - коэффициент для расчета неготовности по интерференции для дополнительной приемной антенны,
                                                                         // Args[10] - Otm_z_pd - м, отметка земли станции слева,
                                                                         // Args[11] - Otm_z_pm - м, отметка земли станции справа,
                                                                         // Args[12] - H1 - м, высота центра раскрыва антенны станции слева,
                                                                         // Args[13] - H2 - м, высота центра раскрыва антенны станции справа,
                                                                         // Args[14] - Tint_p - %, интерференционная неустойчивость как для приземных трасс на горном интервале,
                                                                         // Args[15] - Tint_vg - %, интерференционная неустойчивость как для высокогорных трасс на горном интервале,
                                                                         // PRP[0] - I_PRP раз, выигрыш за счет ПРП,
                                                                         // PRP[1] - T_int_PRP - %,неустойчивость из-за интерференции при ПРП,
                                                                         // PRP[2] - SESR_PRP - %,SESR при ПРП,
                                                                         // PRP[3] - K_ng_PRP - %,Кнг при ПРП.
                                                                         // PRP[4] - fi_tau_int_PRP - ,коэффициент готовности по интерференции при ПРП.
                                                                         // PRP[5] - C_h - ,параметр ц_дельта_аш.
    {
    	double Suh,f,Delta_h,alfa_tau_int,alfa_tau_0,T_int,T_0,T_d,nu,lambda,delta_h,C_h_2_6_suh,C_h_2_6,C_h_8_suh,C_h_8,
      		 C_h, fi_tau_int, fi_tau_0, beta_int, beta_0, kappa,alfa_tau_int_d,Otm_z_pd,Otm_z_pm,H1,H2,h_sr,Gorn, I_PRP,
             T_int_PRP,Tint_p,Tint_vg,Tint_p_PRP, Perepad, delta_h2;

      Suh=Args->hReal[0][0];
      f=Args->hReal[0][1];
      Delta_h=Args->hReal[0][2];
      alfa_tau_int=Args->hReal[0][3];
      alfa_tau_0=Args->hReal[0][4];
      T_int=Args->hReal[0][5];
      T_0=Args->hReal[0][6];
      T_d=Args->hReal[0][7];
      nu=Args->hReal[0][8];
      alfa_tau_int_d=Args->hReal[0][9];
      Otm_z_pd=Args->hReal[0][10];
      Otm_z_pm=Args->hReal[0][11];
      H1=Args->hReal[0][12];
      H2=Args->hReal[0][13];
      Tint_p=Args->hReal[0][14];
      Tint_vg=Args->hReal[0][15];

      h_sr=(Otm_z_pd+H1+Otm_z_pm+H2)/2.;
      Perepad=fabs(Otm_z_pd+H1-Otm_z_pm-H2);
      delta_h2=(Perepad<200.)?160.*0.3/f:700.*0.3/f;

      if(fabs(Delta_h-delta_h2)>0.5)//разнос антенн не соответствует рекомендуемому
      {
      	MessageBox(NULL,"Разнос основной и дополнительной приемных антенн\n не соответствует рекомендуемому","!!!Недопустимый разнос!!!",MB_ICONEXCLAMATION);
      }

      if(h_sr<=800.)
      	Gorn=0;
      else if(h_sr>800. && h_sr<1200.)
      	Gorn=1;
      else
      	Gorn=2;

      lambda=0.3/f;
      delta_h=(Delta_h<160.*lambda)?Delta_h:160.*lambda;

      C_h_2_6_suh=1.43*1000000.*(lambda/delta_h)*(lambda/delta_h);
      C_h_2_6=(Suh==1.)?C_h_2_6_suh:0.5*C_h_2_6_suh;

    	C_h_8_suh=2.86*1000000.*(lambda/delta_h)*(lambda/delta_h);
      C_h_8=(Suh==1.)?C_h_8_suh:0.5*C_h_8_suh;

      C_h=(f<=6.)?C_h_2_6:C_h_8;

      if ( !MathcadArrayAllocate( PRP,  // allocate space for PRP
                    6,    //with 6 rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
      return 2;           // if allocation is insufficient
                                // return the error code

      if(Gorn==0)
      {
      	T_int_PRP=nu*C_h*0.01*T_int*T_int;
      }
      else if(Gorn==1)
      {
       	Tint_p_PRP=nu*C_h*0.01*Tint_p*Tint_p;
         T_int_PRP=Tint_p_PRP+(Tint_vg-Tint_p_PRP)/400.*(h_sr-800.);
      }
      else
      {
       	T_int_PRP=T_int;
      }

      if(Gorn!=2)
      	fi_tau_int=(T_int_PRP*(alfa_tau_int+alfa_tau_int_d)/2.>=0.0004)?1.-(alfa_tau_int+alfa_tau_int_d)/2.:1.;
      else
         fi_tau_int=(T_int_PRP*alfa_tau_int>=0.0004)?1.-alfa_tau_int:1.;
      fi_tau_0=(T_0*alfa_tau_0>=0.0004)?1.-alfa_tau_0:1.;


      I_PRP=T_int/T_int_PRP;
      
      PRP->hReal[0][0]=I_PRP;//I_PRP
      PRP->hReal[0][1]=T_int_PRP;//T_int_PRP
      PRP->hReal[0][4]=1.-(alfa_tau_int+alfa_tau_int_d)/2.;//fi_tau_int
      PRP->hReal[0][5]=C_h;

      if(f<1.427)//для диапазонов частот ниже 1.427 ГГц вся неустойчивость относится к периоду готовности (из разговора с Святогором В.В.)
      {
         PRP->hReal[0][2]=T_int_PRP+T_0;//SESRпрп
         PRP->hReal[0][3]=0;//Кнг_прп
         return 0;
      }

    	PRP->hReal[0][2]=T_int_PRP*fi_tau_int+T_0*fi_tau_0;//SESRпрп



      beta_int=(T_int_PRP*(alfa_tau_int+alfa_tau_int_d)/2.>=0.0004)?1.:0.;
      beta_0=(T_0*alfa_tau_0>=0.0004)?1.:0.;

      kappa=(Suh==0)?3.:4;

      PRP->hReal[0][3]=(T_int_PRP*(alfa_tau_int+alfa_tau_int_d)/2.*beta_int+T_0*alfa_tau_0*beta_0)/kappa+T_d;//Кнг_прп


        return 0;               // return 0 to indicate there was no error

    }// konec FPRP_peresFunction


    LRESULT FPRP_slperesFunction(COMPLEXARRAY * const PRP,				 // Определяет SESR при ПРП, Кнг при ПРП для слабопересеченных интервалов.
    							 COMPLEXARRAY * const Args)         				 // Args[0] - Suh - 1 - сухопутная трасса, 0 - приморская,
                         																 // Args[1] - f - ГГц, рабочая частота,
                                                                         // Args[2] - R - км, длина интервала,
                                                                         // Args[3] - R_otr - км, расстояние до точки отражения для основной приемной антенны,
                                                                         // Args[4] - f_p_g_a_u - , параметр для основной приемной антенны,
                                                                         // Args[5] - f_p_g_a_d - , параметр для дополнительной приемной антенны,
                                                                         // Args[6] - Delta_h - м, разнос между основной и дополнительной приемными антеннами,
                                                                         // Args[7] - alfa_tau_int_u - коэффициент для расчета неготовности по интерференции для основной приемной антенны,
                                                                         // Args[8] - alfa_tau_int_d - коэффициент для расчета неготовности по интерференции для дополнительной приемной антенны,
                                                                         // Args[9] - alfa_tau_0 - коэффициент для расчета неготовности по рефракции для основной приемной антенны,
                                                                         // Args[10] - Vmin_ef_u - дБ, эффективный запас на замирания для основной приемной антенны,
                                                                         // Args[11] - Vmin_ef_d - дБ, эффективный запас на замирания для дополнительной приемной антенны,
                                                                         // Args[12] - p_g_u - , относительный просвет в точке отражения при средней рефракции для основной приемной антенны,
                                                                         // Args[13] - p_g_d - , относительный просвет в точке отражения при средней рефракции для дополнительной приемной антенны,
                                                                         // Args[14] - T_int_u - %,неустойчивость из-за интерференции без учета резерва для основной приемной антенны,
                                                                         // Args[15] - T_int_d - %,неустойчивость из-за интерференции без учета резерва для дополнительной приемной антенны,
                                                                         // Args[16] - T_0 - %,неустойчивость из-за рефракции без учета резерва для основной приемной антенны,
                                                                         // Args[17] - T_d - %,неустойчивость из-за осадков за год для основной приемной антенны,
                                                                         // PRP[0] - SESR_PRP - %,SESR при ПРП,
                                                                         // PRP[1] - K_ng - %,Кнг при ПРП.
                                                                         // PRP[2] - fi_tau_int - , интерференционный коэффициент готовности при ПРП.
                                                                         // PRP[3] - C_h_u - параметр C_delta_h для верхней антенны;
                                                                         // PRP[4] - C_h_d - параметр C_delta_h для нижней антенны;
    {
    	double Suh,f,R,R_otr,f_p_g_a_u,f_p_g_a_d,Delta_h,alfa_tau_int_u,alfa_tau_int_d,alfa_tau_0,Vmin_ef_u,
      		 Vmin_ef_d,p_g_u,p_g_d,T_int_u,T_int_d,T_0,T_d, lambda,k,delta_h2,C_h_u,C_h_d,
             K_int_u,K_int_d,alfa_tau_int,fi_tau_int,fi_tau_0,beta_int,beta_0,kappa,
             fc[12]={0.05,0.075,0.1,0.125,0.15,0.175,0.2,0.225,0.25,0.275,0.3,0.325},
             c_delta_h[12]={15.5,10,8,6.5,5.5,4.75,4,3.4,2.75,2.25,1.6,1.};

      Suh=Args->hReal[0][0];
      f=Args->hReal[0][1];
      R=Args->hReal[0][2];
      R_otr=Args->hReal[0][3];
      f_p_g_a_u=Args->hReal[0][4];
      f_p_g_a_d=Args->hReal[0][5];
      Delta_h=Args->hReal[0][6];
      alfa_tau_int_u=Args->hReal[0][7];
      alfa_tau_int_d=Args->hReal[0][8];
      alfa_tau_0=Args->hReal[0][9];
      Vmin_ef_u=Args->hReal[0][10];
      Vmin_ef_d=Args->hReal[0][11];
      p_g_u=Args->hReal[0][12];
      p_g_d=Args->hReal[0][13];
      T_int_u=Args->hReal[0][14];
      T_int_d=Args->hReal[0][15];
      T_0=Args->hReal[0][16];
      T_d=Args->hReal[0][17];


      lambda=0.3/f;

      k=R_otr/R;

      delta_h2=0.118*sqrt(R*lambda*1000.*(1.-k)/k);

      if(fabs(Delta_h-delta_h2)>0.5)//разнос антенн не соответствует рекомендуемому
         {
            MessageBox(NULL,"Разнос основной и дополнительной приемных антенн\n не соответствует рекомендуемому","!!!Недопустимый разнос!!!",MB_ICONEXCLAMATION);
         }

      if(f_p_g_a_u<0.05)
      	C_h_u=15.5;
      else
      	if(f_p_g_a_u>0.325)
         	C_h_u=1.;
         else
         	C_h_u=linterp(12,fc,c_delta_h,f_p_g_a_u);

    	if(f_p_g_a_d<0.05)
      	C_h_d=15.5;
      else
      	if(f_p_g_a_d>0.325)
         	C_h_d=1.;
         else
         	C_h_d=linterp(12,fc,c_delta_h,f_p_g_a_d);

      C_h_u=(f<=6.)?C_h_u:2*C_h_u;
      C_h_d=(f<=6.)?C_h_d:2*C_h_d;

      if(4.5>p_g_u && p_g_u>=3.6 && -30<Vmin_ef_u)
      	K_int_u=1.5;
      else
      	if((4.5<=p_g_u) || (4.5>p_g_u && p_g_u>=3.6 && -30>=Vmin_ef_u))
      		K_int_u=2.;
         else
         	K_int_u=1.;

      if(4.5>p_g_d && p_g_d>=3.6 && -30<Vmin_ef_d)
      	K_int_d=1.5;
      else
      	if((4.5<=p_g_d) || (4.5>p_g_d && p_g_d>=3.6 && -30>=Vmin_ef_d))
      		K_int_d=2.;
         else
         	K_int_d=1.;

      alfa_tau_int=(alfa_tau_int_u+alfa_tau_int_d)/2.;

      fi_tau_int=(0.01*sqrt(C_h_u*C_h_d)*alfa_tau_int*T_int_u*T_int_d>=0.0004)?1.-alfa_tau_int:1.;
      fi_tau_0=(T_0*alfa_tau_0>=0.0004)?1.-alfa_tau_0:1.;

      if ( !MathcadArrayAllocate( PRP,  // allocate space for PRP
                    5,    //with 3 rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
      return 2;           // if allocation is insufficient
                                // return the error code

      if(f<1.427)//для диапазонов частот ниже 1.427 ГГц вся неустойчивость относится к периоду готовности (из разговора с Святогором В.В.)
      {
         PRP->hReal[0][0]=0.01*sqrt(C_h_u*C_h_d)*(K_int_u*K_int_d)*T_int_u*T_int_d+T_0;//SESRпрп
         PRP->hReal[0][1]=0;//Кнг_прп
         PRP->hReal[0][2]=fi_tau_int;
      	return 0;
      }


      PRP->hReal[0][0]=0.01*sqrt(C_h_u*C_h_d)*(K_int_u*K_int_d-1.+fi_tau_int)*T_int_u*T_int_d+fi_tau_0*T_0;

      beta_int=(0.01*sqrt(C_h_u*C_h_d)*alfa_tau_int*T_int_u*T_int_d>=0.0004)?1.:0.;
      beta_0=(T_0*alfa_tau_0>=0.0004)?1.:0.;
      kappa=(Suh==0)?3.:4.;

      PRP->hReal[0][1]=0.01*sqrt(C_h_u*C_h_d)*alfa_tau_int*T_int_u*T_int_d*beta_int/kappa+alfa_tau_0*T_0*beta_0/kappa+T_d;

      PRP->hReal[0][2]=1.-alfa_tau_int;

      PRP->hReal[0][3]=C_h_u;
      PRP->hReal[0][4]=C_h_d;


        return 0;               // return 0 to indicate there was no error

    }// konec FPRP_slperesFunction


    LRESULT FNormyFunction(COMPLEXARRAY * const Normy,				 // Определяет норму на SESR и Кнг на интервал ЦРРЛ.
    							 COMPLEXARRAY * const Args)         				 // Args[0] - Dlina_RRL - км, суммарная длина всех интервалов ЦРРЛ,
                         																 // Args[1] - Naznachenye - назначение ЦРРЛ, 0 - сеть доступа, 1 - местная сеть, 2 - внутризоновая сеть, 3 - магистральная сеть, 4 - международный участок,
                                                                         // Args[2] - R - км, длина расчитываемого интервала ЦРРЛ,
                                                                         // Args[3] - Reconstr - 0 - линия вновь проектируемая (на проектируемых опорах), 1 - линия реконструируемая (на существующих опорах АРРЛ)
                                                                         // Normy[0] - %, норма на SESR на интервал,
                                                                         // Normy[1] - %, норма на Кнг на интервал.
    {
    	double Dlina_RRL,Naznachenye,R,Reconstr,SESRnorm,Kng_norm;

      Dlina_RRL=Args->hReal[0][0];
      Naznachenye=Args->hReal[0][1];
      R=Args->hReal[0][2];
      Reconstr=Args->hReal[0][3];

      if ( !MathcadArrayAllocate( Normy,  // allocate space for Normy
                    2,    //with 2 rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
      return 2;           // if allocation is insufficient
                                // return the error code


      if(Naznachenye==0)//сеть доступа
      {
      	SESRnorm=0.015*R/Dlina_RRL*0.89;
         Kng_norm=0.05*R/Dlina_RRL*0.89/3.;
      }
      else
      	if(Naznachenye==1)//местная сеть
         {
      		SESRnorm=(Reconstr==0)?((Dlina_RRL<100.)?0.01*R/Dlina_RRL*0.89:0.01*R/100.*0.89):((Dlina_RRL<100.)?0.015*R/Dlina_RRL*0.89:0.015*R/100.*0.89);
         	Kng_norm=(Dlina_RRL<100.)?0.05*R/Dlina_RRL*0.89/3.:0.05*R/100.*0.89/3.;
      	}
         else
      		if(Naznachenye==2)//внутризоновая сеть
         	{
            	if(Dlina_RRL<=50.)
               {
      				SESRnorm=(Reconstr==0)?0.003*R/Dlina_RRL*0.89:0.006*R/Dlina_RRL*0.89;
         			Kng_norm=0.0125*R/Dlina_RRL*0.89/3.;
               }
               else
               	if(Dlina_RRL>50. && Dlina_RRL<=200.)
               	{
      					SESRnorm=(Reconstr==0)?0.012*R/200.*0.89:0.025*R/200.*0.89;
         				Kng_norm=0.05*R/200.*0.89/3.;
               	}
                  else
               		if(Dlina_RRL>200. && Dlina_RRL<=600.)
               		{
      						SESRnorm=(Reconstr==0)?0.012*R/Dlina_RRL*0.89:0.025*R/Dlina_RRL*0.89;
         					Kng_norm=0.05*R/Dlina_RRL*0.89/3.;
                  	}
                     else
                     {
      						SESRnorm=(Reconstr==0)?0.012*R/600.*0.89:0.025*R/600.*0.89;
         					Kng_norm=0.05*R/600.*0.89/3.;
                  	}
      		}
            else
      			if(Naznachenye==3)//магистральная сеть
         		{
      				SESRnorm=(Reconstr==0)?((Dlina_RRL<50.)?0.012*50./2500.*R/Dlina_RRL*0.89:0.012*R/2500.*0.89):((Dlina_RRL<50.)?0.054*50./2500.*R/Dlina_RRL*0.89:0.054*R/2500.*0.89);
         			Kng_norm=(Dlina_RRL<50.)?0.3*50./2500.*R/Dlina_RRL*0.89/3.:0.3*R/2500.*0.89/3.;
      			}
               else//международный участок
      			{
      				SESRnorm=(Reconstr==0)?((Dlina_RRL<2500.)?0.06*2500./12500.*R/Dlina_RRL*0.89:0.06*R/12500.*0.89):((Dlina_RRL<2500.)?0.07*2500./12500.*R/Dlina_RRL*0.89:(0.05+0.02*Dlina_RRL/12500.)*R/Dlina_RRL*0.89);
         			Kng_norm=(Dlina_RRL<2500.)?1.5*2500./12500.*R/Dlina_RRL*0.89/3.:1.5*R/12500.*0.89/3.;
      			}

      Normy->hReal[0][0]=SESRnorm;
      Normy->hReal[0][1]=Kng_norm;

        return 0;               // return 0 to indicate there was no error

    }// konec FNormyFunction


    LRESULT FReadProfileFunction(COMPLEXARRAY * const Profile,
    										COMPLEXSCALAR * const flag)			 // Читает файл профиля.
                                                                      // flag - 1 - читать профиль, 0 - не читать профиль.

    {
      char file_pathname[_MAX_PATH];
    	int Profile_handle, Not_End,i,j=-1,k,l,m,len_filename,ind_len_filename,len_stancia_A,len_stancia_B,
      		ind_len_stancia_A,ind_len_stancia_B,ind_len_for_azimuth,ind_len_back_azimuth,len_for_azimuth,len_back_azimuth,
            ind_len_cur_date, len_cur_date, ind_len_file_date, len_file_date;
		double zagolovok[6];
      char simvol,str_buf[50],str_buf2[50],stancia_A[50],stancia_B[50],
            azimuty[100]="Прямой азимут: ",profile_message[500]="", current_date[30]="",file_date[30]="";
      SYSTEMTIME  SystemTime;/* SystemFileTime;
      FILETIME    FileTime,LocalFileTime,*lpLocalFileTime;
      FILE *osProfile_handle;*/
      struct ftime ftimep;
      extern HINSTANCE g_hInst;		// the current instance определена в userpack.c

      if(flag->real==0)
      	return 4;

      zagolovok[0]=0;

         //заполняет массив на который указывает file_pathname путем
         //к выбранному файлу профиля, возвращвет TRUE. Если профиль не
         //выбран - возвращает FALSE
      	if(BrowseForProfile(file_pathname, g_hInst)==FALSE)
      	{
      		return 4;
      	}

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
            	if(j<=4 && j>=0)  // чтение заголовка файла
            		{
             			zagolovok[j]=atof(str_buf);
            		}
            		else
            			if(j==5)    // заполнение массива Profile заголовком файла
               		{
               			if ( !MathcadArrayAllocate( Profile,  // allocate space for Profile
                    			18+zagolovok[1]*3+zagolovok[0]*4+zagolovok[4]*2+500,    //with  rows последние 500 элементов - для имени файла, названий станций по профилю и т.д.
                    			1,    //and 1 cols
                    			TRUE,   //  allocate the real part
                    			FALSE    //  don't allocate the imaginary part
                    			) )
      							return 2;           // if allocation is insufficient
                                // return the error code
                  		for(k=0;k<=4;k++)
                  			Profile->hReal[0][k]=zagolovok[k];
                  		Profile->hReal[0][j]=atof(str_buf);
               		}
               		else
            				if(j<18+zagolovok[1]*3+zagolovok[0]*4+zagolovok[4]*2) // j < номера последней записи в массиве Profile +1
                  			Profile->hReal[0][j]=atof(str_buf);
                        else
            					if(j==18+zagolovok[1]*3+zagolovok[0]*4+zagolovok[4]*2)
                           	strcpy(stancia_A,str_buf);
                           else
            						if(j==19+zagolovok[1]*3+zagolovok[0]*4+zagolovok[4]*2)
                           		strcpy(stancia_B,str_buf);
                              else
            							if(j==20+zagolovok[1]*3+zagolovok[0]*4+zagolovok[4]*2)   //заполнение азимутов
                                 {
                                 	k=0;//индекс символа в буфере str_buf (строка азимутов в файле профиля)
                                    m=0;//индикатор: 0 - градусы прямого азимута
                                    	 //			  1 - минуты прямого азимута
                                        //           2 - градусы обратного азимута
                                    	 //			  3 - минуты обратного азимута
                                    while(str_buf[k]!='\0')
                                    {
                                    	while(!(isdigit(str_buf[k]) || str_buf[k]=='.') && str_buf[k]!='\0')//поиск первого числа в строке азимутов
                                       	k++;
                                    	l=0;// индекс символа в буфере str_buf2 (текущее число из строки азимутов)
                                       while(isdigit(str_buf[k]) || str_buf[k]=='.')
                                       {
                                       	str_buf2[l]=str_buf[k];
                                          l++;
                                          k++;
                                       }
                                       if((m==0 || m==2) && l!=0)
                                       {
                                          str_buf2[l]='\0';
                                          strcat(str_buf2," град. ");
                                       	strcat(azimuty,str_buf2);
                                          m++;
                                       }
                                       else
                                       	if(m==1 && l!=0)
                                          {
                                           	str_buf2[l]='\0';
                                          	strcat(str_buf2," мин.\nОбратный азимут: ");
                                       		strcat(azimuty,str_buf2);
                                          	m++;
                                       	}
                                          else
                                       		if(m==3 && l!=0)
                                          	{
                                           		str_buf2[l]='\0';
                                          		strcat(str_buf2," мин.");
                                       			strcat(azimuty,str_buf2);
                                          		m++;
                                       		}

                                    }
                                 }
            j++;
         }

      }while(Not_End!=0);

      len_filename=strlen(file_pathname); //количество символов в названии файла профиля
      ind_len_filename=18+zagolovok[1]*3+zagolovok[0]*4+zagolovok[4]*2; //индекс количества символов в названии файла профиля
      Profile->hReal[0][ind_len_filename]=len_filename;
      for(k=0;k<=len_filename-1.;k++)
      	Profile->hReal[0][ind_len_filename+k+1]=(file_pathname[k]>=0)?file_pathname[k]:file_pathname[k]+256; //побуквенно имя файла профиля (256 - сдвиг для русских символов)

      ind_len_stancia_A=ind_len_filename+len_filename+1;//индекс количества символов в названии станции слева
      len_stancia_A=strlen(stancia_A); //количество символов в названии станции слева по профилю
      Profile->hReal[0][ind_len_stancia_A]=len_stancia_A;
      OemToChar(stancia_A,stancia_A);
      for(k=0;k<=len_stancia_A-1.;k++)
      	Profile->hReal[0][ind_len_stancia_A+1+k]=(stancia_A[k]>=0)?stancia_A[k]:stancia_A[k]+256; //побуквенно название станции слева по профилю

      ind_len_stancia_B=ind_len_stancia_A+len_stancia_A+1;//индекс количества символов в названии станции справа
      len_stancia_B=strlen(stancia_B); //количество символов в названии станции справа по профилю
      Profile->hReal[0][ind_len_stancia_B]=len_stancia_B;
      OemToChar(stancia_B,stancia_B);
      for(k=0;k<=len_stancia_B-1.;k++)
      	Profile->hReal[0][ind_len_stancia_B+1+k]=(stancia_B[k]>=0)?stancia_B[k]:stancia_B[k]+256; //побуквенно название станции справа по профилю

      ind_len_for_azimuth=ind_len_stancia_B+len_stancia_B+1;//индекс количества символов в прямом азимуте
      len_for_azimuth=0;
      i=0;
      while(azimuty[i]!=':') i++;
      i++; //i - начало прямого азимута в массиве azimuty
      while(azimuty[i+len_for_azimuth] !='\n' && azimuty[i+len_for_azimuth] !='\0') len_for_azimuth++; //len_for_azimuth - количество символов в прямом азимуте
      Profile->hReal[0][ind_len_for_azimuth]=len_for_azimuth;
      for(k=0;k<=len_for_azimuth-1.;k++)
      	Profile->hReal[0][ind_len_for_azimuth+1+k]=(azimuty[i+k]>=0)?azimuty[i+k]:azimuty[i+k]+256; //побуквенно прямой азимут по профилю


      ind_len_back_azimuth=ind_len_for_azimuth+len_for_azimuth+1;//индекс количества символов в обратном азимуте
      Profile->hReal[0][ind_len_back_azimuth]=1;
      Profile->hReal[0][ind_len_back_azimuth+1]='?';
      len_back_azimuth=1;
      if(m>2/*azimuty[ind_len_for_azimuth+len_for_azimuth] !='\0'*/)
      {
      	len_back_azimuth=0;
         while(azimuty[i]!=':') i++;
      	i++; //i - начало обратного азимута в массиве azimuty
      	while(azimuty[i+len_back_azimuth] !='\0') len_back_azimuth++; //len_for_azimuth - количество символов в прямом азимуте
      	Profile->hReal[0][ind_len_back_azimuth]=len_back_azimuth;
      	for(k=0;k<=len_back_azimuth-1.;k++)
      		Profile->hReal[0][ind_len_back_azimuth+1+k]=(azimuty[i+k]>=0)?azimuty[i+k]:azimuty[i+k]+256; //побуквенно обратный азимут по профилю
      }

      GetLocalTime(  &SystemTime); 	// address of system time structure
      strcat(current_date,gcvt((double) SystemTime.wDay, 2, str_buf));
      strcat(current_date,".");
      strcat(current_date,gcvt((double) SystemTime.wMonth, 2, str_buf));
      strcat(current_date,".");
      strcat(current_date,gcvt((double) SystemTime.wYear, 4, str_buf));
      strcat(current_date," г. ");
      strcat(current_date,gcvt((double) SystemTime.wHour, 2, str_buf));
      strcat(current_date,":");
      strcat(current_date,gcvt((double) SystemTime.wMinute, 2, str_buf));
      strcat(current_date," ч:мин");

      ind_len_cur_date=ind_len_back_azimuth+len_back_azimuth+1;//индекс количества символов в текущей дате
      len_cur_date=strlen(current_date); //количество символов в текущей дате
      Profile->hReal[0][ind_len_cur_date]=len_cur_date;
      for(k=0;k<=len_cur_date-1.;k++)
      	Profile->hReal[0][ind_len_cur_date+1+k]=(current_date[k]>=0)?current_date[k]:current_date[k]+256; //побуквенно текущая дата и время


      getftime(Profile_handle, &ftimep);

      strcat(file_date,gcvt((double) ftimep.ft_day, 2, str_buf));
      strcat(file_date,".");
      strcat(file_date,gcvt((double) ftimep.ft_month, 2, str_buf));
      strcat(file_date,".");
      strcat(file_date,gcvt((double) ftimep.ft_year+1980., 4, str_buf));
      strcat(file_date," г. ");
      strcat(file_date,gcvt((double) ftimep.ft_hour, 2, str_buf));
      strcat(file_date,":");
      strcat(file_date,gcvt((double) ftimep.ft_min, 2, str_buf));
      strcat(file_date," ч:мин");

      ind_len_file_date=ind_len_cur_date+len_cur_date+1;//индекс количества символов в дате последнего изменения файла профиля
      len_file_date=strlen(file_date); //количество символов в дате последнего изменения файла профиля
      Profile->hReal[0][ind_len_file_date]=len_file_date;
      for(k=0;k<=len_file_date-1.;k++)
      	Profile->hReal[0][ind_len_file_date+1+k]=(file_date[k]>=0)?file_date[k]:file_date[k]+256; //побуквенно дата и время последнего изменения файла профиля


      close(Profile_handle);

      strcat(profile_message,"Интервал РРЛ: ");
		strcat(profile_message,stancia_A);
      strcat(profile_message," - ");
      strcat(profile_message,stancia_B);
      strcat(profile_message,";\n");
      strcat(profile_message,azimuty);
      strcat(profile_message,";\nДлина интервала: ");
      strcat(profile_message,gcvt(Profile->hReal[0][12+3*(Profile->hReal[0][1]-1)],5,str_buf));
      strcat(profile_message," км;\nОтметка земли слева: ");
      strcat(profile_message,gcvt(Profile->hReal[0][13],5,str_buf));
      strcat(profile_message," м;\nОтметка земли справа: ");
      strcat(profile_message,gcvt(Profile->hReal[0][13+3*(Profile->hReal[0][1]-1)],5,str_buf));
      strcat(profile_message," м;\n");



      MessageBox(NULL,profile_message,"Данные профиля интервала РРЛ",MB_ICONINFORMATION);



      return 0;               // return 0 to indicate there was no error

    }// konec FReadProfileFunction



    LRESULT FZigzagoobraznostFunction(COMPLEXARRAY * const Zigzagoobraznost,
    										COMPLEXARRAY * const Args)				 // Вычисляет неустойчивость связи из-за помех прямого прохождения.
                                                                      // Args[0] - Tip, тип системы: 1 - 2ОФМ,
                                                                      //													  2 - 4ОФМ,
                                                                      //													  3 - 8ОФМ,
                                                                      //                                         4 - 16КАМ,
                                                                      //                                         5 - 32КАМ,
                                                                      //                                         6 - 64КАМ,
                                                                      //                                         7 - 128КАМ (ТСМ),
                                                                      //                                         8 - 256КАМ,
                                                                      //                                         9 - 512КАМ;
                                                                      // Args[1] - C, скорость передачи, МБит/с;
                                                                      // Args[2] - f, частота, ГГц;
                                                                      // Args[3] - Shiroty, широты: 0 - умеренные (50 - 65 сш),
                                                                      //													 1 - полярные (севернее 65 сш),
                                                                      //													 2 - южные (юнее  50 сш);
                                                                      // Args[4] - Mor, характер трассы: 0 - сухопутная,
                                                                      //                                 1 - приморская,
                                                                      //                                 2 - морская,
                                                                      //											3 - смешанная;
                                                                      // Args[5] - F_pd_1, дБ, ослабление меш. сигн. из-за ДНА меш. ант.;
                                                                      // Args[6] - F_pm_4, дБ, ослабление меш. сигн. из-за ДНА ПМ ант.;
                                                                      // Args[7] - delta_P_pd, дБ, P_pd_mesh-P_pd_pol;
                                                                      // Args[8] - ARM, дБ;
                                                                      // Args[9] - delta_G_pd, дБ, G_pd_mesh-G_pd_pol;
                                                                      // Args[10] - delta_eta_pd, дБ, eta_pd_mesh("+")-eta_pd_pol("+");
                                                                      // Args[11] - R_j, км, протяженность интервала на котором оценивается помеха;
                                                                      // Args[12] - V_j, дБ, средний множитель ослабления полезного сигнала;
                                                                      // Args[13] - V_g_m, дБ, множитель ослабления в газах мешающего сигнала;
                                                                      // Args[14] - K_f, климатический фактор;
                                                                      // Args[15] - R_m_i, км, длина интервала мешающего сигнала;
                                                                      // Args[16] - R_1, км, расстояние до критического преп. на трассе мешающего сигнала;
                                                                      // Args[17] - H, м, просвет в т. критического преп. на трассе мешающего сигнала;
                                                                      // Args[18] - R_m_suh, км, Протяженность сухопутного участка смешанной трассы мешающего сигнала
                                                                      // Args[19] - R_m_prim, км, Протяженность приморского участка смешанной трассы мешающего сигнала
                                                                      // Args[20] - R_m_mor, км, Протяженность морского участка смешанной трассы мешающего сигнала
                                                                      // Zigzagoobraznost[0] - Z_max, дБ;
                                                                      // Zigzagoobraznost[1] - F_sum, дБ, F_pd_1+F_pm_4;
                                                                      // Zigzagoobraznost[2] - Z_R_V_P_G_eta_sum, дБ;
                                                                      // Zigzagoobraznost[3] - Z_por_dop, дБ;
                                                                      // Zigzagoobraznost[4] - V_m_dop, дБ;
                                                                      // Zigzagoobraznost[5] - Tetta, рад;
                                                                      // Zigzagoobraznost[6] - R_e_m, км, эквивалентная длина трассы мешающего сигнала;
                                                                      // Zigzagoobraznost[7] - L;
                                                                      // Zigzagoobraznost[8] - F_ot_f;
                                                                      // Zigzagoobraznost[9] - ksi;
                                                                      // Zigzagoobraznost[10] - A_m;
                                                                      // Zigzagoobraznost[11] - B_m;
                                                                      // Zigzagoobraznost[12] - ksi_suh;
                                                                      // Zigzagoobraznost[13] - F_ot_f_suh, дБ, для приморских и смешанных трасс;
                                                                      // Zigzagoobraznost[14] - V_m_suh, дБ, для приморских и смешанных трасс;
                                                                      // Zigzagoobraznost[15] - ksi_mor, для приморских и смешанных трасс;
                                                                      // Zigzagoobraznost[16] - F_ot_f_mor, дБ, для приморских и смешанных трасс;
                                                                      // Zigzagoobraznost[17] - V_m_mor, дБ;
                                                                      // Zigzagoobraznost[18] - V_m_prim, дБ;
                                                                      // Zigzagoobraznost[19] - V_m_smesh, дБ;
                                                                      // Zigzagoobraznost[20] - T_1_4, %;
    {
    	double Tip,C,f,Shiroty,Mor,F_pd_1,F_pm_4,delta_P_pd,ARM,delta_G_pd,delta_eta_pd,R_j,V_j,V_g_m,K_f,
      			R_m_i,R_1,H,R_m_suh,R_m_prim,R_m_mor,Z_max,Z_por_dop,V_m_dop,k,Tetta_e,L,R_e_m,F_ot_f,F_ot_f_suh,F_ot_f_mor,
               ksi,A_m,B_m,T_1_4,Args_V[9],F_sum,Z_R_V_P_G_eta_sum;
      COMPLEXARRAY DeltaVdegr,caMmesh,caMCochannel;
      COMPLEXSCALAR csTip,csVmin0,csNmesh,csR,csVsr,csZpordops,csC;

      Tip=Args->hReal[0][0];
      C=Args->hReal[0][1];
      f=Args->hReal[0][2];
      Shiroty=Args->hReal[0][3];
      Mor=Args->hReal[0][4];
      F_pd_1=Args->hReal[0][5];
      F_pm_4=Args->hReal[0][6];
      delta_P_pd=Args->hReal[0][7];
      ARM=Args->hReal[0][8];
      delta_G_pd=Args->hReal[0][9];
      delta_eta_pd=Args->hReal[0][10];
      R_j=Args->hReal[0][11];
      V_j=Args->hReal[0][12];
      V_g_m=Args->hReal[0][13];
      K_f=Args->hReal[0][14];
      R_m_i=Args->hReal[0][15];
      R_1=Args->hReal[0][16];
      H=Args->hReal[0][17];
      R_m_suh=Args->hReal[0][18];
      R_m_prim=Args->hReal[0][19];
      R_m_mor=Args->hReal[0][20];

      csTip.real=Tip;
      csVmin0.real=-30;
      csNmesh.real=0;
      csR.real=R_j;
      csVsr.real=V_j;
      if ( !MathcadArrayAllocate( &caMmesh,  // allocate space for pcaMmesh
                    1,    //with 1 rows
                    7,    //and 7 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
      return 2;           // if allocation is insufficient
                                // return the error code
      caMmesh.hReal[0][0]=0;
      caMmesh.hReal[1][0]=0;
      caMmesh.hReal[2][0]=0;
      caMmesh.hReal[3][0]=0;
      caMmesh.hReal[4][0]=0;
      caMmesh.hReal[5][0]=0;
      caMmesh.hReal[6][0]=0;

      csZpordops.real=1000;
      if ( !MathcadArrayAllocate( &caMCochannel,  // allocate space for pcaMCochannel
                    4,    //with 4 rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
      return 2;           // if allocation is insufficient
                                // return the error code
      caMCochannel.hReal[0][0]=0;
      caMCochannel.hReal[0][1]=30;
      caMCochannel.hReal[0][2]=20;
      caMCochannel.hReal[0][3]=f;
      csC.real=C;

      if ( !MathcadArrayAllocate( Zigzagoobraznost,  // allocate space for Zigzagoobraznost
                    21,    //with 21 rows
                    1,    //and 1 cols
                    TRUE,   //  allocate the real part
                    FALSE    //  don't allocate the imaginary part
                    ) )
      return 2;           // if allocation is insufficient
                                // return the error code

      if(Shiroty==1 || Mor==0)             // полярные или сухопутная
      	Zigzagoobraznost->hReal[0][0]=-45; // Z_max, дБ;
      else if (Mor==1)// приморская
         Zigzagoobraznost->hReal[0][0]=-54; // Z_max, дБ;
      else if (Mor==2)// морская
         Zigzagoobraznost->hReal[0][0]=-63; // Z_max, дБ;
      else //смешанная
      	Zigzagoobraznost->hReal[0][0]=(-45.*(R_m_suh+R_m_prim/2.)-63.*(R_m_mor+R_m_prim/2.))/R_m_i; // Z_max, дБ;

      Z_max=Zigzagoobraznost->hReal[0][0];

      Zigzagoobraznost->hReal[0][1]=F_pd_1+F_pm_4;//F_sum, дБ, F_pd_1+F_pm_4;
      F_sum=Zigzagoobraznost->hReal[0][1];

      Zigzagoobraznost->hReal[0][2]=Z_max+20.*log10(R_m_i/R_j)-V_g_m+V_j-delta_P_pd+fabs(ARM)-delta_G_pd+delta_eta_pd; // Z_R_V_P_G_eta_sum, дБ;
      Z_R_V_P_G_eta_sum=Zigzagoobraznost->hReal[0][2];
      if (F_sum>Z_R_V_P_G_eta_sum)//если условие зигзагообразности не выполняется
      {
      	FDeltaVdegrFunction(&DeltaVdegr,			 // Opredelyaet znachenie degradacii porogovogo urovnya
                           &csTip,                // Tip - 1-2ОФМ;2-4ОФМ;3-8ОФМ;4-16КАМ;5-32КАМ; 6-64КАМ;
                           &csVmin0,              //       7-128КАМ(ТСМ);8-256КАМ;9-512КАМ.
                           &csNmesh,              // Vmin0 - dB минимально допустимый ый множитель ослабления без
                           &csR,                  //         учета деградации порогового уровня и
                           &csVsr,                //         среднего ослабления
                           &caMmesh,               // Nmesh - Количество мешающих интервалов
                           &csZpordops,
                           &caMCochannel,
                           &csC);
      	Zigzagoobraznost->hReal[0][3]=DeltaVdegr.hReal[0][3]; // Z_por_dop, дБ;
      	Z_por_dop=Zigzagoobraznost->hReal[0][3];

      	Zigzagoobraznost->hReal[0][4]=Z_por_dop-F_pd_1-F_pm_4+20.*log10(R_m_i/R_j)+V_j-delta_P_pd+fabs(ARM)-delta_G_pd+delta_eta_pd; // V_m_dop, дБ;
      	V_m_dop=Zigzagoobraznost->hReal[0][4];

      	k=R_1/R_m_i;
      	Zigzagoobraznost->hReal[0][5]=(H<0.)?(-H+0.02*R_m_i*R_m_i*k*(1.-k))/R_m_i/1000./k/(1.-k):0.00002*R_m_i; //Tetta, рад;
      	Tetta_e=Zigzagoobraznost->hReal[0][5];

      	Zigzagoobraznost->hReal[0][6]=8500.*Tetta_e; // R_e_m, км, эквивалентная длина трассы мешающего сигнала;
      	R_e_m=Zigzagoobraznost->hReal[0][6];



      	if(Mor==0 || Mor==2 || Shiroty==1)//если трасса сухопутная или морская
         {
      		Zigzagoobraznost->hReal[0][7]=(Mor==0 || Shiroty==1)?6.:24; // L;
      		L=Zigzagoobraznost->hReal[0][7];

            Zigzagoobraznost->hReal[0][8]=(Mor==0 || Shiroty==1)?-0.04*(R_e_m+50.)*(3.1*(pow(f,0.2)-1.)+1.4*(pow(f,-0.3)-1.))+0.6*(23.6/(1.+0.2*pow(f,1.5))-1./6./f-19.5):-0.025*(R_e_m+50.)*(0.07*(pow(f,-1.5)-1.)+0.6*(pow(f,0.6)-1.)); // F_ot_f;
      		F_ot_f=Zigzagoobraznost->hReal[0][8];

      		Zigzagoobraznost->hReal[0][9]=(pow(10.,0.05*(V_m_dop-F_ot_f-L))>2.)?123456789.:-(0.05*(V_m_dop-F_ot_f-L)-log10(2.-pow(10.,0.05*(V_m_dop-F_ot_f-L))))/0.8686; // ksi;
      		ksi=Zigzagoobraznost->hReal[0][9];

      		if(Mor==0 || Shiroty==1)//сухопутная или в северных широтах
				Zigzagoobraznost->hReal[0][10]=1.037+0.007604*R_e_m-0.000001118*R_e_m*R_e_m-50.*K_f/(R_e_m+50.); // A_m;
      		else  if(Shiroty==0)//морская в умеренных широтах
         	Zigzagoobraznost->hReal[0][10]=-0.27+0.00392*R_e_m-0.00000137*R_e_m*R_e_m; // A_m;
      		else//морская в южных широтах
         		Zigzagoobraznost->hReal[0][10]=-1.53+0.00392*R_e_m-0.00000137*R_e_m*R_e_m; // A_m;
      		A_m=Zigzagoobraznost->hReal[0][10];

      		Zigzagoobraznost->hReal[0][11]=(Mor==0 || Shiroty==1)?0.0003185*(2000.-R_e_m):1.; // B_m;
      		B_m=Zigzagoobraznost->hReal[0][11];

      		Zigzagoobraznost->hReal[0][20]=(ksi==123456789)?123456789:pow(10.,(ksi-A_m)/B_m); // T_1_4, %;
         }
      	else//приморская или смешанная не в северных широтах
      	{
       		F_ot_f_suh=-0.04*(R_e_m+50.)*(3.1*(pow(f,0.2)-1.)+1.4*(pow(f,-0.3)-1.))+0.6*(23.6/(1.+0.2*pow(f,1.5))-1./6./f-19.5);
         	Zigzagoobraznost->hReal[0][13]=F_ot_f_suh; // F_ot_f_suh;
         	F_ot_f_mor=-0.025*(R_e_m+50.)*(0.07*(pow(f,-1.5)-1.)+0.6*(pow(f,0.6)-1.));
         	Zigzagoobraznost->hReal[0][16]=F_ot_f_mor; // F_ot_f_mor;
         	Args_V[0]=R_e_m;
         	Args_V[1]=K_f;
         	Args_V[2]=Mor;
         	Args_V[3]=F_ot_f_suh;
         	Args_V[4]=F_ot_f_mor;
         	Args_V[5]=Shiroty;
         	Args_V[6]=R_m_suh;
         	Args_V[7]=R_m_mor;
         	Args_V[8]=R_m_i;

         	Zigzagoobraznost->hReal[0][20]=minerr(V_m_suh_mor_prim,    // Определяет значение первого аргумента функции *func, при
         	/*niz*/			0.000009,         // котором функция принимает значение val с точностью не хуже prec
         	/*verh*/       0.2,        // в пределах изменения аргумента niz - verh.
         	/*prec*/      	0.00001,        // если на заданном интервале не найден аргумент, удовлетворяющий
         	/*val*/        V_m_dop,
                        	Args_V);       // требуемой точности возвращается значение аргумента, при котором
                                            // функция максимально приближается к значению val.
    													  //Остальные аргументы функции *func должны быть записаны в массиве Args // T_1_4, %;

         	T_1_4=Zigzagoobraznost->hReal[0][20];
         	Zigzagoobraznost->hReal[0][12]=1.037+0.007604*R_e_m-0.000001118*R_e_m*R_e_m+0.0003185*(2000.-R_e_m)*log10(T_1_4)-50.*K_f/(R_e_m+50.); // ksi_suh;
         	Args_V[2]=0;
         	Zigzagoobraznost->hReal[0][14]=V_m_suh_mor_prim(T_1_4,Args_V); // V_m_suh, дБ;
         	Zigzagoobraznost->hReal[0][15]=(Shiroty==0)?-0.27+0.00392*R_e_m-0.00000137*R_e_m*R_e_m+log10(T_1_4):-1.53+0.00392*R_e_m-0.00000137*R_e_m*R_e_m+log10(T_1_4); // ksi_mor;
         	Args_V[2]=2;
         	Zigzagoobraznost->hReal[0][17]=V_m_suh_mor_prim(T_1_4,Args_V); // V_m_mor, дБ;
         	Args_V[2]=1;
         	Zigzagoobraznost->hReal[0][18]=V_m_suh_mor_prim(T_1_4,Args_V); // V_m_prim, дБ;
         	Args_V[2]=3;
         	Zigzagoobraznost->hReal[0][19]=V_m_suh_mor_prim(T_1_4,Args_V); // V_m_smesh, дБ;
      	}
      }



    	return 0;               // return 0 to indicate there was no error
    }// konec FZigzagoobraznostFunction



    double V_m_suh_mor_prim(double T,double *Args)//Вычисляет значение V_m для сухопутных, морских, приморских и смешанных трасс,
    															  //при расчете помех прямого прохождения на приморских трассах
                                                  //T - %, неустойчивость связи за счет помех прямого прохождения
                                                  //Args[0] - R_e_m, км, эквивалентная длина трассы мешающего сигнала;
                                                  //Args[1] - K_f, климатический фактор;
                                                  //Args[2] - Mor - характер трассы: 0 - сухопутная;
                                                  //                       			 1 - приморская;
                                                  //                       			 2 - морская;
                                                  //                                 3 - смешанная
                                                  //Args[3] - F_ot_f_suh, для сухопутных трасс;
                                                  //Args[4] - F_ot_f_mor, для приморских трасс;
                                                  //Args[5] - Shiroty, широты: 0 - умеренные (50 - 65 сш),
                                                  //									 2 - южные (юнее  50 сш).
                                                  // Args[6] - R_m_suh, км, Протяженность сухопутного участка смешанной трассы мешающего сигнала
                                                  // Args[7] - R_m_mor, км, Протяженность морского участка смешанной трассы мешающего сигнала
                                                  // Args[8] - R_m, км, Протяженность трассы мешающего сигнала


    {
     	double R_e_m,K_f,Mor,ksi_suh,ksi_mor,F_ot_f_suh,F_ot_f_mor,Shiroty,V_m_suh,V_m_mor,
      		R_m_suh,R_m_mor,R_m;

      R_e_m=Args[0];
      K_f=Args[1];
      Mor=Args[2];
      F_ot_f_suh=Args[3];
      F_ot_f_mor=Args[4];
      Shiroty=Args[5];
      R_m_suh=Args[6];
      R_m_mor=Args[7];
      R_m=Args[8];

      ksi_suh=1.037+0.007604*R_e_m-0.000001118*R_e_m*R_e_m+0.0003185*(2000.-R_e_m)*log10(T)-50.*K_f/(R_e_m+50.);
      ksi_mor=(Shiroty==0)?-0.27+0.00392*R_e_m-0.00000137*R_e_m*R_e_m+log10(T):-1.53+0.00392*R_e_m-0.00000137*R_e_m*R_e_m+log10(T);
      V_m_suh=12.+F_ot_f_suh+20.*log10(exp(-2.*ksi_suh)/(1.+exp(-2.*ksi_suh)));
      V_m_mor=30.+F_ot_f_mor+20.*log10(exp(-2.*ksi_mor)/(1.+exp(-2.*ksi_mor)));

      switch((int)Mor)
      {
      	case 0: //сухопутная
            return V_m_suh;

         case 2: //морская
         	return V_m_mor;

         case 1: //приморская
            return (V_m_suh+V_m_mor)/2.;

         default: //смешанная

            return (V_m_suh*(1.+R_m_suh/R_m-R_m_mor/R_m)+V_m_mor*(1.-R_m_suh/R_m+R_m_mor/R_m))/2.;
      }


    }//конец V_m_suh_mor_prim



    LRESULT FSaveDXFProfileFunction(COMPLEXSCALAR * const Flag,
    										COMPLEXARRAY * const Args)//Предлагает сохранить и сохраняет чертеж профиля в файл DXF
    															  //Flag - 1 - Файл сохранен, 0 - файл не сохранен (нажата кнопка отмены)
                                                  //Args[0] - Количество символов в пути и имени файла профиля 001
                                                  //Args[1] - Строка пути и имени файла профиля 001
                                                  //Args[2] - Значение градиента диэлектрической проницаемости воздуха (1/м)
                                                  //Args[3] - Значение рабочей частоты (ГГц)
                                                  //Args[4] - Значение погрешности отметок земли (м)
                                                  //Args[5] - Значение высоты центра раскрыва основной антенны слева (м)
                                                  //Args[6] - Значение высоты центра раскрыва основной антенны справа (м)
                                                  //Args[7] - Флаг наличия пространственно-разнесенного приема: 0 - ПРП нет, 1 - ПРП есть
                                                  //Args[8] - Значение высоты центра раскрыва дополнительной антенны слева (м)
                                                  //Args[9] - Значение высоты центра раскрыва дополнительной антенны справа (м)

    {
    	OPENFILENAME  ofn;
      char ProfileDXF_Name[_MAX_PATH]="", filter[64]="", DXF_PathFile_Name[_MAX_PATH]="", SRC_PathFile_Name[_MAX_PATH]="",
      		SRC_Path[_MAX_PATH]="", *Profile001_Name;
      int i;
      
      Flag->real=0;

    	if(MessageBox(NULL,"Желаете сохранить чертеж профиля в DXF-файл?","Сохранение чертежа профиля",MB_OKCANCEL|MB_ICONQUESTION)==IDCANCEL)
      	return 0;//Если нажата кнопка cancel - возврат без сохранения DXF, Flag==0

      strcpy(filter, "Чертеж формата DXF, *.dxf *.dxf Все файлы, *.* *.* ");//Строка фильтра в системном диалоге сохранения файла
      filter[25]=0;//Символ 0 после текстового описания первого фильтра *.dxf
      filter[31]=0;//Символ 0 после самого первого фильтра *.dxf
      filter[46]=0;//Символ 0 после текстового описания второго фильтра *.*
      filter[50]=0;//Символ 0 после самого второго фильтра *.* (в конце строки фильтра д.б. два нулевых символа)

      //Путь и имя исходного файла профиля 001
      for(i=0; i<Args->hReal[0][0]; i++)
      {
      	SRC_PathFile_Name[i]=Args->hReal[i][1];
      }
      SRC_PathFile_Name[i]=0;//замыкающий нулевой символ

      //Раздельно путь к папке и имя исходного файла профиля 001
      strcpy(SRC_Path, SRC_PathFile_Name);
      for(i=strlen(SRC_Path); i>=0; i--)//Начиная с конца пути и имени исходного файла профиля 001 в обратном порядке
      {
      	if(SRC_Path[i]=='\\')//Доходим до первого попавшегося обратного слэша (окончание пути к папке)
         {
            Profile001_Name=&SRC_Path[i]+1;//Имя профиля (название исходного файла 001)
         	SRC_Path[i]=0;//Путь к папке исходного файла профиля 001
            break;
         }
      }

      ofn.lpstrInitialDir=SRC_Path;//Папка в которой системный диалог сохранения файла предложит сохранить DXF

      //Имя DXF файла изначально такое же как и файла 001
      strcpy(DXF_PathFile_Name, Profile001_Name);
      for(i=strlen(DXF_PathFile_Name); i>=0; i--)//Начиная с конца имени исходного файла профиля 001 в обратном порядке
      {
      	if(DXF_PathFile_Name[i]=='.')//Доходим до первой попавшейся точки (начало расширения файла)
         {
         	DXF_PathFile_Name[i]=0;//Отрезаем расширение
            break;
         }
      }

    	ofn.lStructSize=sizeof(OPENFILENAME);
    	ofn.hwndOwner=NULL;
    	ofn.lpstrFilter=filter;
      ofn.lpstrCustomFilter=NULL;
    	ofn.nFilterIndex=1;//интекс фильтра, используемый системным диалогом сохранения для инициализации (первая пара типов файлов имеет индекс 1)
    	ofn.lpstrFile=DXF_PathFile_Name;//буфер, содержащий имя файла, используемое системным диалогом сохранения для инициализации. После возврата содержит
      										  //путь и имя файла, выбранные пользователем
    	ofn.nMaxFile=256;
    	ofn.lpstrFileTitle=ProfileDXF_Name;//буфер, получающий имя выбранного файла с расширением после возврата из системного диалога сохранения файла
    	ofn.nMaxFileTitle=_MAX_PATH;
    	ofn.lpstrTitle="Запись чертежа профиля в формате DXF";
    	ofn.Flags=OFN_EXPLORER|OFN_EXTENSIONDIFFERENT|OFN_OVERWRITEPROMPT;
    	ofn.lpstrDefExt="dxf";//функция системного диалога сохранения файла добавляет это расширение к имени файла, если пользователь забыл это сделать

      if(GetSaveFileName(&ofn)==FALSE)
      {
      	return 0;
      }

      //Сохранение DXF чертежа профиля
    	DrawProfileDXF(SRC_PathFile_Name, //Путь и имя файла профиля формата 001
						 DXF_PathFile_Name, 	//Путь и имя выходного графического DXF файла профиля
                   Args->hReal[0][2],  //градиент диэлектрической проницаемости воздуха, 1/м
                   Args->hReal[0][3],  //Рабочая частота, ГГц
                   Args->hReal[0][4],  //Погрешность отметок земли, м
                   Args->hReal[0][5],  //Высота центра раскрыва основной антенны слева, м
                   Args->hReal[0][6],  //Высота центра раскрыва основной антенны справа, м
                   Args->hReal[0][7],  //Флаг наличия пространственно-разнесенного приема: 0 - ПРП нет, 1 - ПРП есть
                   Args->hReal[0][8],  //Высота центра раскрыва дополнительной антенны слева, м
                   Args->hReal[0][9]); //Высота центра раскрыва дополнительной антенны справа, м

      Flag->real=1;

      return 0;

    }// konec FSaveDXFProfileFunction





    double Razn_tg_otr(double x,double far *Args)//Определяет разницу между тангенсами углов падения слева и справа на параболу в точке х
                                                 //Args[0,1,2] - A[0,1,2] - массив с коэффициентами полинома. Элементу с большим индексом соответствует более старший коэффициент.
                                                 //Args[3] - R - м, длина анализируемого интервала.
                                                 //Args[4] - Y1 - м, абсолютная отметка излучателя (слева).
                                                 //Args[5] - Y2 - м, абсолютная отметка приемника (справа).
                                                 //х - м, координата точки падения
    {
     	double A[3],R,Y1,Y2,k_parab,k_lev,k_prav, tg_alfa,tg_beta;

      A[0]=Args[0];
      A[1]=Args[1];
      A[2]=Args[2];
      R=Args[3];
      Y1=Args[4];
      Y2=Args[5];
      k_parab=A[1]+2.*A[2]*x;
      k_lev=(x==0.)?0.:(A[0]+A[1]*x+A[2]*x*x-Y1)/x;
      k_prav=(x==R)?0.:(Y2-(A[0]+A[1]*x+A[2]*x*x))/(R-x);
      tg_alfa=(x==0.)?-1./k_parab:(k_parab-k_lev)/(1.+k_parab*k_lev);
      tg_beta=(x==R)?1./k_parab:(k_prav-k_parab)/(1.+k_parab*k_prav);

      return (tg_alfa-tg_beta);

    }// konec Razn_tg_otr

    BOOL parab_reflect(double far *A, double R, double Y1, double Y2, double far * otr)//Определяет параметры отражения от параболы.
                                                                                        //Возвращает true - если есть точка отражения, false - если нет
                                                                                        //A - указатель на массив с коэффициентами полинома. Элементу с большим индексом соответствует более старший коэффициент.
                                                                                        //R - м, длина анализируемого интервала.
                                                                                        //Y1 - м, абсолютная отметка излучателя (слева).
                                                                                        //Y2 - м, абсолютная отметка приемника (справа).
                                                                                        //otr[0] - D - Дискрминант для отыскания величины a при А[2]<=0 или величины b при А[2]>=0.
                                                                                        //otr[1] - a - Левая граница участка поиска точки отражения.
                                                                                        //otr[2] - b - Правая граница участка поиска точки отражения.
                                                                                        //otr[3] - Xотр - Координата Х точки отражения.
                                                                                        //otr[4] - Tg_alfa - Тангенс угла падения (отражения) к касательной.
                                                                                        //otr[5] - k_kas_parab - Угловой коэффициент касательной к параболе в точке отражения.
                                                                                        //otr[6] - k_lev - Угловой коэффициент падающего (слева) луча.
                                                                                        //otr[7] - k_prav - Угловой коэффициент отраженного (справа) луча.
    {
    	double D,a,b,X_otr,k_lev,k_prav,b2, Args[6],k_parab,tg_alfa,tg_beta, alfa,beta,gamma,B0,B1;
      BOOL Otr_vozm;

      D=4.*A[2]*A[2]*R*R+4.*A[2]*(A[1]*R-Y2+A[0]);
      b2=(A[2]==0.)?R*R:(A[0]-Y1)/A[2];

      if(A[2]<0.)//рога вниз
      {
      	if(D<0. || b2<0.)
         {
         	a=0;
            b=0;
            Otr_vozm=FALSE;
         }
         else
         {
         	a=(2.*A[2]*R+sqrt(D))/2./A[2];
            b=sqrt(b2);
            if(a>b)
             	Otr_vozm=FALSE;
            else
               Otr_vozm=TRUE;
         }
      }
      else//
      	if(A[2]==0.)//прямая
      	{
         	if(Y1<A[0] || Y2<(A[0]+A[1]*R))//если отражающая прямая проходит хотя бы над одной из антенн
            	Otr_vozm=FALSE;
            else
            {
             	a=0.;
               b=R;
               Otr_vozm=TRUE;
            }
      	}
      	else//рога вверх
      	{
       		if(b2>=0.)
         		a=sqrt(b2);
         	if(D>=0.)
         		b=(2*A[2]*R-sqrt(D))/2./A[2];
         	if(D>=0. && b2>=0.)
         		if(a>b)
            		Otr_vozm=FALSE;
            	else
            		Otr_vozm=TRUE;
         	if(D<0. && b2<0.)
         	{
          		a=0.;
            	b=R;
            	Otr_vozm=TRUE;
         	}
         	if(D<0. && b2>=0.)
         		if(a>=R)
            	{
             		b=0.;
               	Otr_vozm=FALSE;
            	}
            	else
            	{
             		b=R;
               	Otr_vozm=TRUE;
            	}
         	if(D>=0. && b2<0.)
         		if(b<=0.)
            	{
             		a=0.;
               	Otr_vozm=FALSE;
            	}
            	else
            	{
             		a=0.;
               	Otr_vozm=TRUE;
            	}
      	}

      otr[0]=D;
      otr[1]=a;
      otr[2]=b;

      if(!Otr_vozm)
      {
       	return (FALSE);
      }
      if(A[2]!=0.)//не прямая
      {
      	a=(a<0.)?0.:a;
         b=(b>R)?R:b;
      	Args[0]=A[0];
			Args[1]=A[1];
      	Args[2]=A[2];
      	Args[3]=R;
      	Args[4]=Y1;
      	Args[5]=Y2;
      	X_otr=minerr(Razn_tg_otr,    // Определяет значение первого аргумента функции *func, при
    							a,         // котором функция принимает значение val с точностью не хуже prec
                        b,        // в пределах изменения аргумента niz - verh.
                        0.0005,        // если на заданном интервале не найден аргумент, удовлетворяющий
                        0.,
                        Args);       // требуемой точности возвращается значение аргумента, при котором
                                           // функция максимально приближается к значению val.
      }
      else//прямая
      {
       	alfa=A[1]*A[1]+1.;
         beta=A[1]*A[1]-1.;
         gamma=A[0]-Y2;
         B0=(A[0]-Y1)*R*beta+2.*A[1]*gamma*(A[0]-Y1);
         B1=alfa*(A[1]*R-Y2+2.*A[0]-Y1);
         X_otr=-B0/B1;
      }

      k_parab=A[1]+2.*A[2]*X_otr;
      k_lev=(X_otr==0.)?0.:(A[0]+A[1]*X_otr+A[2]*X_otr*X_otr-Y1)/X_otr;
      k_prav=(X_otr==R)?0.:(Y2-(A[0]+A[1]*X_otr+A[2]*X_otr*X_otr))/(R-X_otr);
      tg_alfa=(X_otr==0.)?-1./k_parab:(k_parab-k_lev)/(1.+k_parab*k_lev);
      tg_beta=(X_otr==R)?1./k_parab:(k_prav-k_parab)/(1.+k_parab*k_prav);

      if(X_otr>=0. && X_otr<=R && fabs(tg_alfa-tg_beta)<=0.0005)
      {
      	otr[0]=D;
         otr[1]=a;
         otr[2]=b;
         otr[3]=X_otr;
         otr[4]=tg_alfa;
         otr[5]=k_parab;
         otr[6]=k_lev;
         otr[7]=k_prav;
         return (TRUE);
      }
      else
      	return (FALSE);
      

    }// konec parab_reflect


    LRESULT FReflection_pointFunction(COMPLEXARRAY * const Reflection_point, // определяет точку отражения на профиле
                                       COMPLEXARRAY * const Prepyatstvie,
                                       COMPLEXARRAY * const Relyef,
     												COMPLEXARRAY * const Args)
																				//Reflection_point[0] - R_otr - км, расстояние до точки отражения
                                                            //Reflection_point[1] - H_otr - м, абсолютный просвет в точке отражения
                                                            //Reflection_point[2] - H0_otr - м, оптимальный просвет в точке отражения
                                                            //Reflection_point[3] - p_otr - , относительный просвет в точке отражения
                                                            //Reflection_point[4] - R_beg_appr - км, расстояние до начала области аппроксимации
                                                            //Reflection_point[5] - R_end_appr - км, расстояние до конца области аппроксимации
                                                            //Reflection_point[6] - X - км, продольный размер области отражения
                                                            //Reflection_point[7] - Delta_h_max - м, максимальная высота неровностей рельефа, при которой участок трассы можно считать плоским
                                                            //Reflection_point[8] - A0 - , свободный член аппроксимирующего полинома 2-й степени
                                                            //Reflection_point[9] - A1 - , коэффициент аппроксимирующего полинома 2-й степени при x в первой степени
                                                            //Reflection_point[10] - A2 - , коэффициент аппроксимирующего полинома 2-й степени при x во второй степени
                                                            //Reflection_point[11] - l_under_profile - м, суммарная протяженность участков пересечения падающего и отраженного лучей с профилем с учетом минусовой погрешности рельефа в месте пересечения.
                                                            //Prepyatstvie - массив начала, конца, высоты и типа препятствий
                                                            //Relyef - массив расстояний и отметок рельефа
                                                            //Args[0] - N_prep - количество высотных препятствий на профиле
                                                            //Args[1] - N_relyef - количество высотных отметок рельефа на профиле
                                                            //Args[2] - a_e - м, эквивалентный радиус земли
                                                            //Args[3] - Delta_H - м, погрешность отметок рельефа
                                                            //Args[4] - H1 - м, высота центра раскрыва антенны слева
                                                            //Args[5] - H2 - м, высота центра раскрыва антенны справа
                                                            //Args[6] - f - ГГц, рабочая частота
    {
		int N_prep, N_relyef, N_min=0, N_max=0, i, j, k, l, n,k_next=1,N_Otr_Appr_max=40,N_Otr_Appr,m,N_cycles=10/*Максимум 10*/;
      BOOL Otr_vozm,pereval;
      double a_e, k_y, Delta_H, H1, H2, f, H_otr, H0_otr, p_otr, R_beg_appr, R_end_appr,D_h, Delta_h_max,R,
      			far A[3], l_under_profile, shag, *X, *Y, otr[8],X_otr,Y_otr,X_plosk05,X_beg_plosk,X_end_plosk,X_cur,shag1,
               Y_luch,Y_rel,Y_surf,Y1,Y2,Y_prep,p_n,p_next_n,p_m,Hor,lambda,sko,next_mid,
               Reflection_point_max[13],Relyef_X_k,X_j,kritichnost_pogr=0.1,
               Reflection_point_min[13];// элементы соответствуют строкам в Reflection_point, 13-й элемент - суммарное квадратичное отклонение рельефа от параболы на участке отражения
      int Manual_changing_reflection_flag;
      extern HINSTANCE g_hInst;		// the current instance определена в userpack.c
      
      if ( !MathcadArrayAllocate( Reflection_point,  // allocate space for Reflection_point
                    			12,    //with 12 rows
                    			1,    //and 1 cols
                    			TRUE,   //  allocate the real part
                    			FALSE    //  don't allocate the imaginary part
                    			) )
      							return 2;           // if allocation is insufficient
                                // return the error code

      N_relyef=Args->hReal[0][1];
      R=Relyef->hReal[0][N_relyef-1];//км, длина интервала
      shag=R*1000./(double)N_Otr_Appr_max;//м, шаг, с которым берутся участки аппроксимации, причем первая половина текущего участка совпадает со второй половиной предыдущего

      X=(double*)farcalloc(N_relyef+(int)(0.002*shag/(1.1-0.1*N_cycles)),sizeof(double));//резервирование памяти с учетом возможных дополнительных точек на кривизну земли
      Y=(double*)farcalloc(N_relyef+(int)(0.002*shag/(1.1-0.1*N_cycles)),sizeof(double));
      if(X==NULL || Y==NULL)
      	return 2;//не хватает памяти

      N_prep=Args->hReal[0][0];
      a_e=Args->hReal[0][2]*0.001;
      Delta_H=Args->hReal[0][3];
      H1=Args->hReal[0][4];
      H2=Args->hReal[0][5];
      f=Args->hReal[0][6];

      Y1=Relyef->hReal[1][0]+H1;
      Y2=Relyef->hReal[1][N_relyef-1]+H2;

      N_Otr_Appr=N_Otr_Appr_max;

    for(m=0;m<N_cycles;m++)//повторить с N_cycles разными значениями шага аппроксимации
    {
    	N_Otr_Appr=N_Otr_Appr-(double)(N_Otr_Appr_max*m)*0.1;//уменьшение количества точек аппроксимации на 25%
      shag=R*1000./(double)N_Otr_Appr;//м, шаг, с которым берутся участки аппроксимации, причем первая половина текущего участка совпадает со второй половиной предыдущего
      k_next=1;
      for(i=1; i<N_Otr_Appr; i++)
      {
         R_beg_appr=shag*(double)(i-1);//м, абсцисса левой границы аппроксимируемого участка на i-том шаге
         R_end_appr=shag*(double)(i+1);//м, абсцисса правой границы аппроксимируемого участка на i-том шаге
         next_mid=shag*(double)i;

         //Заполнение массивов кусочно-ломаной функции: X - абсциссы; Y - ординаты
         X[0]=R_beg_appr;
         k_y=X[0]/R*0.001;
      	k=k_next;//индекс точки в массиве Relyef
         Y[0]=Relyef->hReal[1][k-1]+(Relyef->hReal[1][k]-Relyef->hReal[1][k-1])/(Relyef->hReal[0][k]-Relyef->hReal[0][k-1])*(X[0]*0.001-Relyef->hReal[0][k-1])/*linterp(N_relyef,Relyef->hReal[0],Relyef->hReal[1],X[0]*0.001)*/+R*R*1000.*0.5/a_e*k_y*(1.-k_y);
     		j=1;//индекс заполняемого элемента
         pereval=FALSE;//первая точка рельефа след. участка аппроксимации найдена
			Relyef_X_k=1000.*Relyef->hReal[0][k];
         while(R_end_appr>Relyef_X_k && k<N_relyef)//пока не достигли конца i-го участка или конца профиля
         {
         	X_j=X[j-1]+1000.;
            while(X_j<Relyef_X_k)//пока расстояние между k-той точкой i-го участка и j-1-й абсциссой превосходит 1 км
            {
            	X[j]=X_j;//добавить точки с шагом 1 км
               k_y=X[j]/R*0.001;
               Y[j]=Relyef->hReal[1][k-1]+(Relyef->hReal[1][k]-Relyef->hReal[1][k-1])/(Relyef->hReal[0][k]-Relyef->hReal[0][k-1])*(X[j]*0.001-Relyef->hReal[0][k-1])/*linterp(N_relyef,Relyef->hReal[0],Relyef->hReal[1],X[j]*0.001)*/+R*R*1000.*0.5/a_e*k_y*(1.-k_y);//к рельефу добавить кривизну земли;
               j++;
               X_j=X[j-1]+1000.;
            }
            if(pereval==FALSE && Relyef_X_k > next_mid)//если точка рельефа перевалила за середину участка аппроксимации
            {
            	k_next=k;//первая точка рельефа на следующем участке аппроксимации
               pereval=TRUE;//первая точка рельефа след. участка аппроксимации найдена
            }
          	X[j]=Relyef_X_k;
            k_y=X[j]/R*0.001;
            Y[j]=Relyef->hReal[1][k-1]+(Relyef->hReal[1][k]-Relyef->hReal[1][k-1])/(Relyef->hReal[0][k]-Relyef->hReal[0][k-1])*(X[j]*0.001-Relyef->hReal[0][k-1])/*linterp(N_relyef,Relyef->hReal[0],Relyef->hReal[1],X[j]*0.001)*/+R*R*1000.*0.5/a_e*k_y*(1.-k_y);//к рельефу добавить кривизну земли;
            j++;
            k++;
            Relyef_X_k=1000.*Relyef->hReal[0][k];
         }
         if(pereval==FALSE)
         	k_next=k;//первая точка рельефа на следующем участке аппроксимации
         X_j=X[j-1]+1000.;
         while(X_j<R_end_appr)//пока расстояние между R_end_appr i-го участка и j-1-й абсциссой превосходит 1 км
         {
         	X[j]=X[j-1]+1000.;//добавить точки с шагом 1 км
            k_y=X[j]/R*0.001;
            Y[j]=Relyef->hReal[1][k-1]+(Relyef->hReal[1][k]-Relyef->hReal[1][k-1])/(Relyef->hReal[0][k]-Relyef->hReal[0][k-1])*(X[j]*0.001-Relyef->hReal[0][k-1])/*linterp(N_relyef,Relyef->hReal[0],Relyef->hReal[1],X[j]*0.001)*/+R*R*1000.*0.5/a_e*k_y*(1.-k_y);//к рельефу добавить кривизну земли;
            j++;
            X_j=X[j-1]+1000.;
         }
         X[j]=R_end_appr;
         k_y=X[j]/R*0.001;
         Y[j]=Relyef->hReal[1][k-1]+(Relyef->hReal[1][k]-Relyef->hReal[1][k-1])/(Relyef->hReal[0][k]-Relyef->hReal[0][k-1])*(X[j]*0.001-Relyef->hReal[0][k-1])/*linterp(N_relyef,Relyef->hReal[0],Relyef->hReal[1],X[j]*0.001)*/+R*R*1000.*0.5/a_e*k_y*(1.-k_y);//к рельефу добавить кривизну земли;

       	parab_approx(j+1,              // Параболическая аппроксимация линейно-ломаной функции по точкам полиномом 2-го порядка,
    														 // по методу наименьших квадратов. Возвращает значение аппроксимированной функции
                                              //
    						X,               // м, X - вектор абсцисс,
                     Y,               // м, Y - вектор ординат,
                     0.,
                     A);            // x - значение абсциссы точки, в которой производится вычисление
                                              // N - количество точек
                                              // A - указатель на массив с коэффициентами полинома. Элементу с большим индексом соответствует более старший коэффициент.

         Otr_vozm=parab_reflect(A, R*1000., Y1, Y2, otr);//Определяет параметры отражения от параболы.
                                                                                        //Возвращает true - если есть точка отражения, false - если нет
                                                                                        //A - указатель на массив с коэффициентами полинома. Элементу с большим индексом соответствует более старший коэффициент.
                                                                                        //R - м, длина анализируемого интервала.
                                                                                        //Y1 - м, абсолютная отметка излучателя (слева).
                                                                                        //Y2 - м, абсолютная отметка приемника (справа).
                                                                                        //otr[0] - D - Дискрминант для отыскания величины a при А[2]<=0 или величины b при А[2]>=0.
                                                                                        //otr[1] - a - Левая граница участка поиска точки отражения.
                                                                                        //otr[2] - b - Правая граница участка поиска точки отражения.
                                                                                        //otr[3] - Xотр - Координата Х точки отражения.
                                                                                        //otr[4] - Tg_alfa - Тангенс угла падения (отражения) к касательной.
                                                                                        //otr[5] - k_kas_parab - Угловой коэффициент касательной к параболе в точке отражения.
                                                                                        //otr[6] - k_lev - Угловой коэффициент падающего (слева) луча.
                                                                                        //otr[7] - k_prav - Угловой коэффициент отраженного (справа) луча.
         if(!Otr_vozm)//если на параболе не найдена точка отражения
         	;//перейти к следующему участку аппроксимации
         else//если на параболе найдена точка отражения
         {
         	X_otr=otr[3];
          	if(X_otr<X[0] || X_otr>X[j])//если точка отражения не попадает в границы участка аппроксимации
             	;//перейти к следующему участку аппроксимации
            else//если точка отражения попадает в границы участка аппроксимации
            {
            	k_y=X_otr/R*0.001;//относительная координата точки отражения
               Y_otr=linterp(N_relyef,Relyef->hReal[0],Relyef->hReal[1],X_otr*0.001)+R*R*1000.*0.5/a_e*k_y*(1.-k_y);// м, высота рельефа в точке отражения с учетом кривизны земли
               H_otr=Y1+(Y2-Y1)*k_y-Y_otr;//м, абсолютный просвет в точке отражения
               lambda=0.3/f;
               H0_otr=sqrt(0.333333333*R*1000.*lambda*k_y*(1.-k_y));//м, оптимальный просвет
               p_otr=H_otr/H0_otr;//относительный просвет
               n=(int)(p_otr*p_otr*0.1666666666667);//номер ближайшего снизу интерференционного минимума в точке отражения
               n=(n<1)?1:n;//не меньше первого
               X_plosk05=(k_y==0. || k_y==1.)?0.:R*1000.*sqrt(0.333333333*(0.333333333+2.*n))/(0.66666666667+n/k_y/(1.-k_y));//м, половина минимальной протяженности плоского участка
               X_beg_plosk=((X_otr-X_plosk05)<=0.)?0.:X_otr-X_plosk05;
               X_end_plosk=((X_otr+X_plosk05)>=R*1000.)?R*1000.:X_otr+X_plosk05;
               Delta_h_max=0.5*H0_otr/sqrt(n)+Delta_H*kritichnost_pogr;//максимальное отклонение рельефа от параболы, с учетом 1/3 погрешности отметок, при котором еще можно считать рельеф плоским
               X_cur=X_beg_plosk;//текущая координата на профиле
               shag1=X_plosk05/20.;
               sko=0.;//сумма квадратов отклонения рельефа от параболы на участке отражения
               while(X_cur<=X_end_plosk)//на участке отражения
               {
                  k_y=X_cur/R*0.001;//относительная координата текущей точки
                  Y_rel=linterp(N_relyef,Relyef->hReal[0],Relyef->hReal[1],X_cur*0.001)+R*R*1000.*0.5/a_e*k_y*(1.-k_y);// м, высота рельефа в точке отражения с учетом кривизны земли
               	D_h=Y_rel-(A[0]+A[1]*X_cur+A[2]*X_cur*X_cur);//отклонение рельефа от параболы
                  if(D_h>Delta_h_max || D_h<-Delta_h_max)//если отклонение рельефа превышает допустимое значение
                  {
                  	Otr_vozm=FALSE;//отражение невозможно
                     break;
                  }
                  sko=sko+D_h*D_h;
                  X_cur=X_cur+shag1;
               }
               if(!Otr_vozm)//если участок отражения выходит за коридор
                	;//перейти к следующему участку аппроксимации
               else//если участок отражения попадает в коридор
               {
                 	X_cur=0.;
                  shag1=10.;//шаг по профилю - 10 м
                  l=0;//0-е препятствие - текущее
                  l_under_profile=0.;//суммарная протяженность участков закрытой трассы отраженного луча
               	while(X_cur<=R*1000.)//на всем профиле
                  {
                     k_y=X_cur/R*0.001;//относительная координата текущей точки
                   	Y_luch=(X_cur<X_otr)?Y1+(Y_otr-Y1)/X_otr*X_cur:Y_otr+(Y2-Y_otr)/(R*1000.01-X_otr)*(X_cur-X_otr);//абсолютная высота отраженного луча в текущей точке с предотвращением деления на 0
                     Y_rel=linterp(N_relyef,Relyef->hReal[0],Relyef->hReal[1],X_cur*0.001)+R*R*1000.*0.5/a_e*k_y*(1.-k_y);// м, высота рельефа в точке отражения с учетом кривизны земли
                     if(Y_rel-Y_luch>2.*Delta_H || l_under_profile>5000.)//если закрытие рельефом на трассе превышает 2*погрешность отметок или протяженность закрытия препятствиями превышает 5 км
                     {
                        Otr_vozm=FALSE;//отражение невозможно
                     	break;//выйти из цикла
                     }
                     Y_prep=0.;
                     for(l=l;l<N_prep;l++)//начиная с препятствия в которое попала предыдущая точка
                     {
                        if(X_cur*0.001<Prepyatstvie->hReal[0][l])//если не дошли до препятствия
                        	break;
                     	if(X_cur*0.001>=Prepyatstvie->hReal[0][l] && X_cur*0.001<=Prepyatstvie->hReal[1][l])//если текущая координата попала в границы препятствия
                        {
                        	Y_prep=Prepyatstvie->hReal[2][l];
                           break;
                        }
                     }
                     Y_surf=Y_rel+Y_prep;
                     if(Y_surf-Y_luch>0.)//если трасса закрыта
                     	l_under_profile=l_under_profile+shag1;//добавить к суммарной протяженности участков закрытой трассы отраженного луча величину шага
                     X_cur=X_cur+shag1;
                  }
                  if(!Otr_vozm)//если трасса отраженного луча закрыта рельефом или препятствиями на протяжении более 5 км
                		;//перейти к следующему участку аппроксимации
                  else
                  {
                   	p_n=sqrt((double)(6*n));//просвет в ближайшем снизу минимуме
                     p_next_n=sqrt((double)(6*(n+1)));//просвет в ближайшем сверху минимуме
                     p_m=sqrt((double)(3*(2*n+1)));//просвет в ближайшем максимуме
                     if(p_otr>(p_m+p_n)*0.5 && p_otr<(p_m+p_next_n)*0.5)//если точка отражения - в максимуме
                     {
                     	sko=sko/X_plosk05;
                        if(N_max==0 || N_max>0 && (l_under_profile<Reflection_point_max[11] || l_under_profile==Reflection_point_max[11] && sko<Reflection_point_max[12]))//если протяженность закрытия не превосходит чем у предыдущей точки отражения +100 м и sko меньше чем у предыдущей
                        {
                        	N_max++;
                      		Reflection_point_max[0]=X_otr*0.001;
                        	Reflection_point_max[1]=H_otr;
                        	Reflection_point_max[2]=H0_otr;
                        	Reflection_point_max[3]=p_otr;
                        	Reflection_point_max[4]=R_beg_appr*0.001;
                        	Reflection_point_max[5]=R_end_appr*0.001;
                        	Reflection_point_max[6]=X_plosk05*0.001*2.;
                        	Reflection_point_max[7]=(Delta_h_max-Delta_H*kritichnost_pogr)*2.;
                        	Reflection_point_max[8]=A[0];
                        	Reflection_point_max[9]=A[1];
                        	Reflection_point_max[10]=A[2];
                        	Reflection_point_max[11]=l_under_profile;
                           Reflection_point_max[12]=sko;
                        }
                     }
                     else
                     {
                        if(N_min==0 || N_min>0 && l_under_profile<=Reflection_point_min[11] && sko<Reflection_point_min[12])//если протяженность закрытия меньше чем у предыдущей точки отражения
                        {
                        	N_min++;
                      		Reflection_point_min[0]=X_otr*0.001;
                        	Reflection_point_min[1]=H_otr;
                        	Reflection_point_min[2]=H0_otr;
                        	Reflection_point_min[3]=p_otr;
                        	Reflection_point_min[4]=R_beg_appr*0.001;
                        	Reflection_point_min[5]=R_end_appr*0.001;
                        	Reflection_point_min[6]=X_plosk05*0.001*2.;
                        	Reflection_point_min[7]=(Delta_h_max-Delta_H*kritichnost_pogr)*2.;
                        	Reflection_point_min[8]=A[0];
                        	Reflection_point_min[9]=A[1];
                        	Reflection_point_min[10]=A[2];
                        	Reflection_point_min[11]=l_under_profile;
                           Reflection_point_min[12]=sko;
                        }
                     }
                  }
               }
            }
         }

      }
    }

    g_hInst=GetModuleHandle("drrl.dll");

      if(N_max==0 && N_min==0)
      {
         Hor=Relyef->hReal[1][0];
      	for(k=1;k<N_relyef;k++)//найти условный уровень горизонта
         	Hor=(Relyef->hReal[1][k]<Hor)?Relyef->hReal[1][k]:Hor;
         Reflection_point->hReal[0][0]=R*(Y1-Hor)/(Y1+Y2-2.*Hor);//км, R_otr

         //******Ручное изменение точки отражения*****

         Manual_changing_reflection_flag=DialogBoxParam(
         	g_hInst,	// handle of application instance
    			"IDD_DIALOG1",	// identifies dialog box template name
    			GetForegroundWindow(),	// handle of owner window
    			MPR_DialogFunc, 	// address of dialog box procedure
            (LPARAM)&(Reflection_point->hReal[0][0])//км, R_otr
   			);

         //*******************************************
         k_y=Reflection_point->hReal[0][0]/R;//относительная координата точки отражения
         Y_otr=linterp(N_relyef,Relyef->hReal[0],Relyef->hReal[1],Reflection_point->hReal[0][0])+R*R*1000.*0.5/a_e*k_y*(1.-k_y);// м, высота рельефа в точке отражения с учетом кривизны земли
         H_otr=Y1+(Y2-Y1)*k_y-Y_otr;//м, абсолютный просвет в точке отражения
         lambda=0.3/f;
         H0_otr=sqrt(0.33333333333*R*1000.*lambda*k_y*(1.-k_y));//м, оптимальный просвет
         p_otr=H_otr/H0_otr;//относительный просвет
         Reflection_point->hReal[0][1]=H_otr;
         Reflection_point->hReal[0][2]=H0_otr;
         Reflection_point->hReal[0][3]=p_otr;
         Reflection_point->hReal[0][4]=-1.;
         Reflection_point->hReal[0][5]=-1.;
         Reflection_point->hReal[0][6]=-1.;
         Reflection_point->hReal[0][7]=-1.;
         Reflection_point->hReal[0][8]=-1.;
         Reflection_point->hReal[0][9]=-1.;
         Reflection_point->hReal[0][10]=-1.;
         Reflection_point->hReal[0][11]=-1.;
      }
      else
      	if(N_min==0 || (N_max!=0 && N_min!=0) && Reflection_point_max[11]<Reflection_point_min[11])//если протяженность закрытия в максимуме меньше чем в минимуме
      	{

         //******Ручное изменение точки отражения*****

            Reflection_point->hReal[0][0]=Reflection_point_max[0];//км, R_otr

         	Manual_changing_reflection_flag=DialogBoxParam(
         		g_hInst,	// handle of application instance
    				"IDD_DIALOG1",	// identifies dialog box template name
    				GetForegroundWindow(),	// handle of owner window
    				MPR_DialogFunc, 	// address of dialog box procedure
            	(LPARAM)&(Reflection_point->hReal[0][0])//км, R_otr
   				);
            if(Manual_changing_reflection_flag==1)//Если расположение точки отражения изменено вручную
            {
         		k_y=Reflection_point->hReal[0][0]/R;//относительная координата точки отражения
         		Y_otr=linterp(N_relyef,Relyef->hReal[0],Relyef->hReal[1],Reflection_point->hReal[0][0])+R*R*1000.*0.5/a_e*k_y*(1.-k_y);// м, высота рельефа в точке отражения с учетом кривизны земли
         		H_otr=Y1+(Y2-Y1)*k_y-Y_otr;//м, абсолютный просвет в точке отражения
         		lambda=0.3/f;
         		H0_otr=sqrt(0.33333333333*R*1000.*lambda*k_y*(1.-k_y));//м, оптимальный просвет
         		p_otr=H_otr/H0_otr;//относительный просвет
         		Reflection_point->hReal[0][1]=H_otr;
         		Reflection_point->hReal[0][2]=H0_otr;
         		Reflection_point->hReal[0][3]=p_otr;
         		Reflection_point->hReal[0][4]=-1.;
         		Reflection_point->hReal[0][5]=-1.;
         		Reflection_point->hReal[0][6]=-1.;
         		Reflection_point->hReal[0][7]=-1.;
         		Reflection_point->hReal[0][8]=-1.;
         		Reflection_point->hReal[0][9]=-1.;
         		Reflection_point->hReal[0][10]=-1.;
         		Reflection_point->hReal[0][11]=-1.;

               return 0;
            }

         //*******************************************

         	for(k=1;k<12;k++)
         	{
      			Reflection_point->hReal[0][k]=Reflection_point_max[k];
         	}
      	}
      	else
      	{

         //******Ручное изменение точки отражения*****

            Reflection_point->hReal[0][0]=Reflection_point_min[0];//км, R_otr

         	Manual_changing_reflection_flag=DialogBoxParam(
         		g_hInst,	// handle of application instance
    				"IDD_DIALOG1",	// identifies dialog box template name
    				GetForegroundWindow(),	// handle of owner window
    				MPR_DialogFunc, 	// address of dialog box procedure
            	(LPARAM)&(Reflection_point->hReal[0][0])//км, R_otr
   				);
            if(Manual_changing_reflection_flag==1)//Если расположение точки отражения изменено вручную
            {
         		k_y=Reflection_point->hReal[0][0]/R;//относительная координата точки отражения
         		Y_otr=linterp(N_relyef,Relyef->hReal[0],Relyef->hReal[1],Reflection_point->hReal[0][0])+R*R*1000.*0.5/a_e*k_y*(1.-k_y);// м, высота рельефа в точке отражения с учетом кривизны земли
         		H_otr=Y1+(Y2-Y1)*k_y-Y_otr;//м, абсолютный просвет в точке отражения
         		lambda=0.3/f;
         		H0_otr=sqrt(0.33333333333*R*1000.*lambda*k_y*(1.-k_y));//м, оптимальный просвет
         		p_otr=H_otr/H0_otr;//относительный просвет
         		Reflection_point->hReal[0][1]=H_otr;
         		Reflection_point->hReal[0][2]=H0_otr;
         		Reflection_point->hReal[0][3]=p_otr;
         		Reflection_point->hReal[0][4]=-1.;
         		Reflection_point->hReal[0][5]=-1.;
         		Reflection_point->hReal[0][6]=-1.;
         		Reflection_point->hReal[0][7]=-1.;
         		Reflection_point->hReal[0][8]=-1.;
         		Reflection_point->hReal[0][9]=-1.;
         		Reflection_point->hReal[0][10]=-1.;
         		Reflection_point->hReal[0][11]=-1.;

               return 0;
            }

         //*******************************************

         	for(k=1;k<12;k++)
         	{
      			Reflection_point->hReal[0][k]=Reflection_point_min[k];
         	}
      	}

      return 0;

    }// konec FReflection_pointFunction


    BOOL CALLBACK MPR_DialogFunc(//функция обработки изменения одиночного параметра типа double

    	HWND  hwndDlg,	// handle of dialog box
    	UINT  uMsg,	// message
    	WPARAM  wParam,	// first message parameter
    	LPARAM  lParam 	// second message parameter
   	)
    {
    	char buf[10];
      static double *P2parametr;

    	switch (uMsg)
      {

        case WM_INITDIALOG:
        //Заполняется поле со значением параметра
         P2parametr=(double*)lParam;
        	SetDlgItemText(
    			hwndDlg,	// handle of dialog box
    			103,	// identifier of control
    			gcvt(*P2parametr,4,buf) 	// text to set
   			);
        	return TRUE;

        case WM_COMMAND:
        	switch (LOWORD(wParam))
         {
                case 1://Нажата кнопка Ввод
                	  GetDlgItemText(
    						hwndDlg,	// handle of dialog box
    						103,	// identifier of control
    						buf,	// address of buffer for text
    						9 	// maximum size of string
   						);
                    *P2parametr=atof(buf);
                    EndDialog(hwndDlg, 1);
                    return TRUE;


                case 2://Нажата кнопка Отмена
                    EndDialog(hwndDlg, 0);
                    return TRUE;
         }

        default:
        	return FALSE;
    	}

    }// konec MPR_DialogFunc

