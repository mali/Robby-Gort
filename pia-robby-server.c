/*
 - robby-server.c
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

#include "piapc.h"
#include "messages.h"


#define UP    0x01;
#define DOWN  0x02;
#define LEFT  0x04;
#define RIGHT 0x08;

byte RobbyWriteCtrl;   // Octet de controle de robby ( Port_A)
                      // A0 - up       1
                      // A1 - down     2
                      // A2 - left     4
                      // A3 - right    8

byte RobbyReadCtrl;   // Octet de controle de robby ( Port_B)
                      // B0 - up       1
                      // B1 - down     2
                      // B2 - left     4
                      // B3 - right    8

byte up_or_down;
byte left_or_right;

struct PIACtrl piactrl;

char * binary(char *chaine, byte valeur)
{
   int dep = 128;
   int i = 0;

   for(i=0; i<8; i++)
     {
       if(valeur - dep >= 0)
	 {
	   valeur -= dep;
	   strcat(chaine, "1");
	 }
       else
	 {
	   strcat(chaine, "0");
	 }
       dep = dep/2;
     }
   return(chaine);
}


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
	       up_or_down = UP;
	     }
	   if(strcmp(message, "down") == 0)
	     {
	       up_or_down = DOWN;
	     }
	   if(strcmp(message, "left") == 0)
	     {
	       left_or_right = LEFT;
	     }
	   if(strcmp(message, "right") == 0)
	     {
	       left_or_right = RIGHT;
	     }
	   
	   RobbyWriteCtrl = up_or_down | left_or_right;
	   
	   if(write_pia(piactrl.Port_A.adresse, RobbyWriteCtrl) == -1)
	     {
	       perror("write_pia failed");
	       return(-1);
	     }
	   printf ("Ecriture sur 0x%x, Valeur : %d\t%s\n", piactrl.Port_A.adresse, RobbyWriteCtrl, binary(chaine, RobbyWriteCtrl));
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

   /* concerne la carte PIAPC */
   int ret = 0;
   RobbyWriteCtrl = 0;
   RobbyReadCtrl = 0;
   up_or_down = 0;
   left_or_right = 0;

   memset(&piactrl, 0, sizeof(struct PIACtrl));

   /* initialisation ack & nack */
   strcpy(ack, "ACK");
   strcpy(nack,"NACK");

   /* Initialisation de la carte en MODE_1 */
   /* PORT_A => OUTPUT                     */
   /* PORT_B => INPUT                      */
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

   /* En attente d'un client ...(1 seul à la fois) */
   listen(server_socket, 1);
   addrlen = sizeof(struct sockaddr_in);

   do
    {
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

   CloseSocket(server_socket);

   return (0);
}

