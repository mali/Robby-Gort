/*
 * piapc.c
 * by Laurent Navet <Laurent_Navet@yahoo.com>
 * Released under GNU/LGPL license
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "piapc.h"


struct PIACtrl piactrl;

int main(int argc, char *argv[])
{
  int ret = 0;
  byte valeur = 0;

  memset(&piactrl, 0, sizeof(struct PIACtrl));

  if((ret = init_pia(&piactrl, MODE_1)) == -1)
    {
      perror("init_pia failed");
      exit(-1);
    }

  /* Affichage du résultat */
  printf ("Adressage :\n");
  printf (" - Port A : 0x%x\n", piactrl.Port_A.adresse);
  printf (" - Port B : 0x%x\n", piactrl.Port_B.adresse);
  printf (" - Port C : 0x%x\n\n", piactrl.Port_C.adresse);

  /* PORT_A à 1 */
  valeur = 255;
  if((ret = write_pia(piactrl.Port_A.adresse, valeur)) == -1)
    {
      perror("write_pia failed");
      exit(-1);
    }
  printf ("Ecriture sur 0x%x, Valeur : %d\n", piactrl.Port_A.adresse, valeur);

  /* Lecture sur PORT_B */
  valeur = 255;
  if((ret = read_pia(piactrl.Port_B.adresse, &valeur)) == -1)
    {
      perror("read_pia failed");
      exit(-1);
    }
  printf ("Lecture sur 0x%x,  Valeur : %d\n", piactrl.Port_B.adresse, valeur);

 exit(0);
}


