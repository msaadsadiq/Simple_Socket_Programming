#include <stdio.h>
#include <string.h>	
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "common.h"


int main (int argc, char **argv)
{
  char  client_msg[MAX_STR_SIZE];
  char  server_msg1[MAX_STR_SIZE];
  char  server_msg2[MAX_STR_SIZE];
  int   sd;
  int   rand_num_int;
  int   added_response;
  struct sockaddr_in sin;
  struct hostent *h;

  short port;
  char  magic_str[] = MAGIC_STRING;

  char delims[] = "\t \n";	/* delimiters */
  char *token = 0x0, *addr_string = 0x0, *rand_num_str = 0x0;

  int ret_val, readbytes;

  h = gethostbyname(SERVER_ADDR);
  port = SERVER_PORT;



  sd = socket(AF_INET, SOCK_STREAM, 0);
  bzero((char *) &sin, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = *( (unsigned long *) h->h_addr);
  sin.sin_port = htons(port);

  ret_val = connect(sd, (struct sockaddr *) &sin, sizeof(sin));


  sprintf(client_msg, "%s HELLO %s \n", magic_str, argv[argc -1]);
  printf("\n %s ",client_msg);

  send(sd, client_msg, strlen(client_msg), 0);

  bzero(server_msg1, sizeof(server_msg1));
  readbytes = read(sd, server_msg1, sizeof(server_msg1)-1);


  printf("%s \n",server_msg1);


  token = strtok(server_msg1, delims);
  token = strtok(NULL, delims);

  token = strtok(NULL, delims);
  rand_num_str = token;

  token = strtok(NULL, delims);
  addr_string = token;

  added_response =   atoi(rand_num_str) + atoi(addr_string);
 
  sprintf(client_msg,"%s BYE %d\n", magic_str, added_response);
  printf(" %s ",client_msg);
  send(sd, client_msg, strlen(client_msg), 0);

  bzero(server_msg2, sizeof(server_msg2));
  readbytes = read(sd, server_msg2, sizeof(server_msg2)-1);
  printf("%s \n\n\n\n\n\n",server_msg2);
 
  close(sd);
  return 0;
}

