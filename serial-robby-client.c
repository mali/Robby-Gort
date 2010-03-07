/*
 - robby-client.c
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

#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int main(int argc,char *argv[])
{
   int client_socket = 0;
   int bufsize = 1024;
   int ret = 0, i = 2;
   char *send_buffer = malloc(bufsize);
   char *recv_buffer = malloc(bufsize);
   char ip[16];
   struct sockaddr_in address;

   /* controle argc, argv */
    if(argc > 1)
      strcpy(ip, argv[1]);
    else
      strcpy(ip, "127.0.0.1");
   
   /* creation socket client */
   if ((client_socket = socket(AF_INET,SOCK_STREAM,0)) > 0)
     printf("The Socket was created\n");

   address.sin_family = AF_INET;
   address.sin_port = htons(15000);
   inet_pton(AF_INET,ip,&address.sin_addr);
   
   if (connect(client_socket, (struct sockaddr *) &address, sizeof(address)) == 0)
     printf("The connection was accepted with the server %s...\n",inet_ntoa(address.sin_addr));
   else
     {
       printf("The connection was refused by the server %s...\n",inet_ntoa(address.sin_addr));
       exit (-1);
     }

   if(argc <= 2) // mode manuel
     { 
       do
	 {
	   printf("Message to send: ");
	   scanf("%s", send_buffer);
	   send(client_socket,send_buffer,bufsize,0);
	   recv(client_socket,recv_buffer,bufsize,0);
	   printf("Message received : %s\n", recv_buffer);
	 }
       while ( strcmp(send_buffer,"quit") &&
	       strcmp(send_buffer, "exit") &&
	       strcmp(recv_buffer, "NACK"));
     }
   else // mode auto
     {
       do
	 {
	   printf("Message to send [%d/%d]: %s\n", (i-1), (argc-2), argv[i]);
	   sprintf(send_buffer, "%s", argv[i]);
	   send(client_socket,send_buffer,bufsize,0);
	   recv(client_socket,recv_buffer,bufsize,0);
	   printf("Message received : %s\n", recv_buffer);
	 }
       while((++i < argc) && strcmp(recv_buffer, "NACK"));
     }

   // Si on est ici à cause d'un NACK
   if( strcmp(recv_buffer, "NACK") == 0)
     {
       // on prévient avant de se déconnecter
       strcpy(send_buffer, "exit");
       send(client_socket,send_buffer,bufsize,0);
       ret = -1;
     }

   close(client_socket);

   return(ret);
}
