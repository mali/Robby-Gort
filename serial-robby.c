/*
 - serial-robby.c
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

#include "serial-robby.h"

#define PORT_NAME "/dev/ttyS1"

#define PAUSE 50

int InitRobby(void)
{

  Input = 0;
  Output = 0;

  if(OpenSerial (PORT_NAME) == -1)
    {
      return (-1);
    }

  if( WriteSerial(Output) == -1)	// tous les relais à zéro
    {
      return (-1);
    }

  /* hack pour ne pas rester bloqué en mode démon */
  /* si la carte n'est pas reliée au port série */
  usleep(500);
  
  if( ReadSerialNonBlock(&Input) == -1)		// on initialise avec l'état réel des entrées
    {
      return (-1);
    }

  return (0);
}

int CloseRobby(void)
{
/*
  Output = 0;

  if( WriteSerial(~Output) == -1)	// tous les relais à zéro avant de partir
    {
      return (-1);
    }
*/    
  if(CloseSerial () == -1)
    {
      return (-1);
    }
  return (0);
}

int RobbyUp(void)
{
  Output &= ~O_DOWN; // if UP then !DOWN 
  Output |= O_UP;
  
  while(1)
  {
     if( WriteSerial(Output) == -1)
      {
	return (-1);
      }
     usleep(PAUSE);
     if( ReadSerial(&Input) == -1)
      {
	return (-1);
      }
     if((~Input & I_UP) == I_UP)
       {
	 Output &= ~O_UP;
	 if( WriteSerial(Output) == -1)
	   {
	     return (-1);
	   }
	 break;
       }
     else
       {
	 usleep(50);
       }
  }
  return (0);
}


int RobbyDown(void)
{
  Output &= ~O_UP; // if UP then !DOWN ;-)
  Output |= O_DOWN;
  
  while(1)
  {
     if( WriteSerial(Output) == -1)
      {
	return (-1);
      }
     usleep(PAUSE);
     if( ReadSerial(&Input) == -1)
      {
	return (-1);
      }
     if((Input & I_DOWN) == I_DOWN)
       {
	 Output &= ~O_DOWN;
	 if( WriteSerial(Output) == -1)
	   {
	     return (-1);
	   }
	 break;
       }
     else
       {
	 usleep(50);
       }
  }
  return (0);
}

int RobbyLeft(void)
{
  Output &= ~O_RIGHT;
  Output |= O_LEFT;
  
  while(1)
  {
     if( WriteSerial(Output) == -1)
       {
	 return (-1);
       }
     usleep(PAUSE);       
     if( ReadSerial(&Input) == -1)
       {
	 return (-1);
       }
     if((~Input & I_LEFT) == I_LEFT)
       {
	 usleep(80000);		// permet de continuer le mouvement pour atteindre la butée mécanique
	 Output &= ~O_LEFT;
	 if( WriteSerial(Output) == -1)
	   {
	     return (-1);
	   }
	 break;
       }
     else
       {
	 usleep(50);
       }
  }
  return (0);
}

int RobbyRight(void)
{
  Output &= ~O_LEFT;
  Output |= O_RIGHT;

  while(1)
  {
     if( WriteSerial(Output) == -1)
       {
	return (-1);
       }
     usleep(PAUSE);       
     if( ReadSerial(&Input) == -1)
      {
	return (-1);
      }
     if((~Input & I_RIGHT) == I_RIGHT)
       {
	 usleep(140000);		// permet de continuer le mouvement pour atteindre la butée mécanique
	 Output &= ~O_RIGHT;
	 if( WriteSerial(Output) == -1)
	   {
	     return (-1);
	   }
	 break;
       }
     else
       {
	 usleep(50);
       }
  }
  return (0);
}

int RobbyOpen(void)
{
  Output |= O_OPEN;
  
  if( WriteSerial(Output) == -1)
    {
	return (-1);
    }
  usleep(PAUSE);    
  if( ReadSerial(&Input) == -1)
    {
	return (-1);
    }
  return (0);
}


int RobbyClose(void)
{
  Output &= ~O_CLOSE;
  
  if( WriteSerial(Output) == -1)
    {
	return (-1);
    }
usleep(PAUSE);    
  if( ReadSerial(&Input) == -1)
    {
	return (-1);
    }
  return (0);
}





