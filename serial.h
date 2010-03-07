/*
 - serial.h
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

#ifndef SERIAL_H
#define SERIAL_H

#include <termios.h>
#include <unistd.h>
#include <errno.h>

#define byte unsigned char

extern int errno;


int OpenSerial (char * port);
int WriteSerial (byte output);
int ReadSerial (byte *input);
int ReadSerialNonBlock (byte *input);
int CloseSerial ();

struct termios oldcfg;
struct termios currentcfg;
int fd;

#endif /* SERIAL_H */



