/*
 - serial-robby-server.c
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

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "serial-robby.h"
#include "messages.h"



int TraiteMessage(char * message)
{
  char chaine[9];

   memset(chaine, 0, sizeof(chaine));

   if(MessageValide(message) == 0)
     {
  /* on ne traite pas 'quit' ni 'exit' */
       if( (strcmp(message, "quit") != 0) && (strcmp(message, "exit") != 0) )
	 {
	   if(strcmp(message, "up") == 0)
	     {
	       RobbyUp();
	     }
	   if(strcmp(message, "down") == 0)
	     {
	       RobbyDown();
	     }
	   if(strcmp(message, "left") == 0)
	     {
	       RobbyLeft();
	     }
	   if(strcmp(message, "right") == 0)
	     {
	       RobbyRight();
	     }
	   if(strcmp(message, "open") == 0)
	     {
	       RobbyOpen();
	     }
	   if(strcmp(message, "close") == 0)
	     {
	       RobbyClose();
	     }
	 }
     }
   else
     {
       return(-1);
     }
  return(0);
}


int CloseSocket(int socket)
{
  int ret =0;
  if((ret = close(socket)) == 0)
    {
      printf ("Socket closed sucessfully.\n");
    }
  else
    {
      perror("Close socket failed");
    }
  return(ret);
}

int main()
{
  /* concerne les sockets */
   int server_socket, client_socket,addrlen;
   int bufsize = 1024;
   char *recv_buffer = malloc(bufsize);
   struct sockaddr_in address;
   char *ack = malloc(bufsize);
   char *nack = malloc(bufsize);

   /* initialisation ack & nack */
   strcpy(ack, "ACK");
   strcpy(nack,"NACK");

   /* initialisation Robot (Port serie, etc) */
   if( InitRobby() == -1)
     {
       perror("InitRobby failed");
     }

   /* creation du serveur 'socket' */
   if ((server_socket = socket(AF_INET,SOCK_STREAM,0)) > 0)
     printf("Server socket created\n");
   else
     {
       perror("Server socket failed");
       exit(-1);
     }

   address.sin_family = AF_INET;
   address.sin_addr.s_addr = INADDR_ANY;
   address.sin_port = htons(15000);
   if (bind(server_socket,(struct sockaddr *)&address,sizeof(address)) == 0)
     printf("Server socket binded\n");
   else
     {
       perror("Server socket bind failed");
       exit(-1);
     }

#ifdef DAEMON_MODE
   if( daemon(0,0) != -1)
     {
#endif
       /* En attente d'un client ...(1 seul à la fois) */
       listen(server_socket, 1);
       addrlen = sizeof(struct sockaddr_in);

       do
	 {
	   printf("Waiting for client...\n");
	   client_socket = accept(server_socket,(struct sockaddr *)&address, &addrlen);
	   if (client_socket > 0)
	     printf("The Client %s is connected...\n",inet_ntoa(address.sin_addr));
	   do
	     {
	       memset(recv_buffer, 0, sizeof(recv_buffer));
	       
	       recv(client_socket,recv_buffer,bufsize,0);
	       printf("Message recieved: %s\n",recv_buffer);
	       
	       /* traitement du message et     */
	       /* envoi acquittement au client */
	       if(TraiteMessage(recv_buffer) == 0)
		 send(client_socket, ack,bufsize,0);
	       else
		 send(client_socket,nack,bufsize,0);
	       
	     }
	   while(strcmp(recv_buffer,"exit") && strcmp(recv_buffer,"quit"));
	   
	   CloseSocket(client_socket);
	 }
       while(strcmp(recv_buffer,"quit")); 
#ifdef DAEMON_MODE
     }
   else
     {
       perror("daemon fonction failed");
     }
#endif
   
   CloseSocket(server_socket);

   CloseRobby();

   return (0);
}

