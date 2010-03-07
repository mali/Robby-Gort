/*
 - libpiapc.c
 - by Laurent Navet <Laurent_Navet@yahoo.com>

This software is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; version 2 dated June, 1991.

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this software; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.

 */

#include <sys/types.h>    // pour open, lseek
#include <sys/stat.h>     // pour open
#include <fcntl.h>        // pour open
#include <unistd.h>       // pour lseek, read, write, close
#include <stdio.h>
#include <string.h>       // pour memset
#include <errno.h>

#include "piapc.h"


int fd;        // file descriptor
struct PIACtrl PIA;

int init_pia (struct PIACtrl * retpia, byte config)
{
  fd = 0;
  memset (&PIA, 0, sizeof(struct PIACtrl));

  /* Controle de la config demandée */
  switch (config)
    {
    case MODE_1 :
      {
	PIA.Port_A.mode = OUTPUT;
	PIA.Port_B.mode = INPUT;
	PIA.Port_C.mode = INPUT;
	break;
      }
    case MODE_2 :
      {
	PIA.Port_A.mode = OUTPUT;
	PIA.Port_B.mode = OUTPUT;
	PIA.Port_C.mode = INPUT;
	break;
      }
    case MODE_3 :
      {
	PIA.Port_A.mode = OUTPUT;
	PIA.Port_B.mode = INPUT;
	PIA.Port_C.mode = OUTPUT;
	break;
      }
    default :
      {
	perror("init_pia : unknown config mode");
	return (-1);
      }
    }

  /* affectation des addresses des ports */

  PIA.Port_A.adresse = PORT_A;
  PIA.Port_B.adresse = PORT_B;
  PIA.Port_C.adresse = PORT_C;

  /* pour avoir accès aux ports */
  if ((fd = open("/dev/port", O_RDWR)) == -1)
    {
      perror("init_pia : open failed");
      return(-1);
    }

  /* configuration du PIA 8255 */
  // positionnement du fd sur l'offset correspondant au Registre de contrôle
  if( lseek(fd, REG_CTRL, SEEK_SET) == -1)
    {
      perror("init_pia_ : lseek REG_CTRL failed");
      return(-1);
    }

  // ecriture de config
  if ( write(fd, &config, 1) == -1)
    {
      perror("init_pia : config PIA 8255 failed");
      return(-1);
    }

  /* copie de struct PIA dans retpia */
  memcpy(retpia, &PIA, sizeof(struct PIACtrl));
  
  // on dort un peu (100 ms)
  usleep(100000);

  return (fd);
}

int write_pia (int adresse, byte valeur)
{
  // contrôle qu'on ne fait pas n'importe quoi !!!
  if(( (adresse == PORT_A) && (PIA.Port_A.mode == INPUT) ) ||
     ( (adresse == PORT_B) && (PIA.Port_B.mode == INPUT) ) ||
     ( (adresse == PORT_C) && (PIA.Port_C.mode == INPUT) )) 
    {
      perror("write_pia : cannot write on INPUT !!");
      return(-1);
    }

  // on positionne fd sur l'offset correspondant au port voulu
  if( lseek(fd, adresse, SEEK_SET) == -1)
    {
      perror("write_pia : lseek failed");
      return(-1);
    }

  // écriture sur le port
  if ( write(fd, &valeur, 1) == -1)
    {
      perror("write_pia : write failed");
      return(-1);
    }

  return(0);
}


int read_pia  (int adresse, byte * valeur)
{

  // contrôle qu'on ne fait pas n'importe quoi !!!
  if(( (adresse == PORT_A) && (PIA.Port_A.mode == OUTPUT) ) ||
     ( (adresse == PORT_B) && (PIA.Port_B.mode == OUTPUT) ) ||
     ( (adresse == PORT_C) && (PIA.Port_C.mode == OUTPUT) )) 
    {
      perror("read_pia : cannot read on OUTPUT !!");
      return(-1);
    }

  // on positionne fd sur l'offset correspondant au port voulu
  if( lseek(fd, adresse, SEEK_SET) == -1)
    {
      perror("read_pia : lseek failed");
      return(-1);
    }

  // lecture sur le port
  if ( read(fd, valeur, 1) == -1)
    {
      perror("read_pia : read failed");
      return(-1);
    }

  return(0);
}

