#include <stdio.h>
#include <conio.h>


main()
{
 	char *serial_number;
   char buffer[13];
   int i;

   serial_number=0x300+0xFEC88;
   for(i=0;i<12;i++)
   	buffer[i]=serial_number[i];
   buffer[12]='\0';
   printf(buffer);
   while(!getch());

   return 0;
}