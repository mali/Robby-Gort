/*
 - serial-robby.h
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

#ifndef SERIAL_ROBBY_H
#define SERIAL_ROBBY_H

#include "serial.h"

/* OUTPUTS */
#define O_UP    0x80
#define O_DOWN  0x40
#define O_LEFT  0x10
#define O_RIGHT 0x08
#define O_OPEN  0x20
#define O_CLOSE 0x20

/* INPUTS */
#define I_UP    0x01
#define I_DOWN  0x02
#define I_LEFT  0x10
#define I_RIGHT 0x08
#define I_OPEN  0x04
#define I_CLOSE 0x04


byte Output;
byte Input;

int InitRobby(void);
int CloseRobby(void);
int RobbyUp(void);
int RobbyDown(void);
int RobbyLeft(void);
int RobbyRight(void);
int RobbyOpen(void);
int RobbyClose(void);

#endif /*  SERIAL_ROBBY_H */
