/*
 - serial.c
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
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "serial.h"


int InitSerialCfg ();
int RestoreSerialCfg ();

/************************************************/
int OpenSerial (char * port)
{
  if((fd = open(port, O_RDWR|O_NOCTTY)) == -1)
    {
      perror("open failed");
      return (-1);
    }
  return (InitSerialCfg());
}

/************************************************/
int CloseSerial ()
{
  /* Restauration cfg */
  RestoreSerialCfg();
  if(close(fd) == -1)
    {
      perror("close failed");
      return (-1);
    }
  return (0);
}

/************************************************/
int WriteSerial(byte output)
{
 /* on vide le buffers kernel (INPUT)*/
  if( tcflush(fd, TCIFLUSH) == -1)
    {
      perror("tcflush failed");
      return(-1);
    }
    
  if( write(fd, &output , sizeof(output)) == -1)
    {
      perror("write failed");
      return (-1);
    }
  return (0);
}

/************************************************/
int ReadSerial(byte *input)
{
  int i=0;
 
  i = read(fd, input, 1);
  if(i >= 0)
    {
      return(0);
    }
  return (-1);
}

/************************************************/
int ReadSerialNonBlock (byte *input)
{
  int i=0;
 
  // passe en mode non bloquant
  fcntl(fd, F_SETFL, (fcntl(fd, F_GETFL) | O_NONBLOCK));

  i = read(fd, input, 1);

  // repasse en mode bloquant
  fcntl(fd, F_SETFL, (fcntl(fd, F_GETFL) & ~O_NONBLOCK));

  if(i == -1)
    {
      if (errno != EAGAIN)
	return(-1);
    }
  return (0);
}
/************************************************/
int InitSerialCfg ()
{
  /* lecture/Sauvegarde config */
  if( tcgetattr(fd, &oldcfg) == -1)
    {
      perror("tcgetattr failed");
      return (-1);
    }

  memcpy (&currentcfg, &oldcfg, sizeof(oldcfg));

  /* le mode raw convient très bien */
  cfmakeraw(&currentcfg);

  /* output/input sped */
  cfsetospeed(&currentcfg, B9600);
  cfsetispeed(&currentcfg, B9600);

  /* activation cfg */
  if( tcsetattr (fd, TCSANOW, &currentcfg) == -1)
    {
     perror("init tcsetattr failed");
     return (-1);      
    }

  /* on vide les buffers kernel */
  if( tcflush(fd, TCIOFLUSH) == -1)
    {
      perror("tcflush failed");
      return(-1);
    }
  return (0);
}

/************************************************/
int RestoreSerialCfg ()
{
  if( tcsetattr (fd, TCSANOW, &oldcfg) == -1)
    {
      perror("restore tcsetattr failed");
      return (-1);      
    }
  return (0);  
}
