#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include <string.h>
#include <strings.h>
#include <netdb.h>

void cut_string_index(char * src, char * dest, int first_index, int second_index){
  int j = 0;
  for(int i = first_index ; i != second_index; i ++){
    dest[j] = src[i];
    j++;
  }
  dest[j] = '\0';
}

void cut_string_char(char * src, char * dest, int first_index, char second_index){
  int j = 0;
  for(int i = first_index ; src[i] != second_index; i ++){
    dest[j] = src[i];
    j++;
  }
  dest[j] = '\0';
}

void decouple(char * buffer){
  int index_get = 3, index_http = -1, index_host = 5;
  char host_adress[150], url[150];

  char *get_index = strstr(buffer, "GET");
  char *http_index = strstr(buffer, "HTTP/1.1");
  char *host = strstr(buffer, "Host:");

  if(get_index != NULL && http_index != NULL && host != NULL) {
    index_get = get_index - buffer + index_get;
    index_http = http_index - buffer + index_http;
    index_host = host - buffer + index_host;
    cut_string_index(buffer, url, index_get,index_http);
    cut_string_char(buffer, host_adress, index_host,'\n');
    printf("Request: %s\n",url);
    printf("Host: %s\n",host_adress);
  }
}

int main(int argc, char *argv[]) {
   int create_socket, new_socket;
   socklen_t addrlen;
   int bufsize = 1024;
   char *buffer = malloc(bufsize);
   struct sockaddr_in address;
   char c;
   int PORT = 8228;

   if(argc < 2){
     printf("Porta a ser utilizada será a default.");
   }

   while ((c = getopt (argc, argv, "p:")) != -1){
     switch (c){
       case 'p':
        PORT =  atoi(optarg);
        break;
	    }
    }


   if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) > 0){ /* AF_INET = IPV4 , SOCK_STREAM = TCP, 0 = IP*/
      printf("Socket criado\n");
   }else{
      printf("Erro ao criar socket");
   }

   address.sin_family = AF_INET; /* IPV4*/
   address.sin_port = htons(PORT); /* Port*/

   if (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) == 0){
      printf("Binding Socket\n");
   }

   while (1) {
      if (listen(create_socket, 10) < 0) {
         perror("server: listen");
         exit(1);
      }

      if ((new_socket = accept(create_socket, (struct sockaddr *) &address, &addrlen)) < 0) {
         perror("server: accept");
         exit(1);
      }

      if (new_socket > 0){
         printf("The Client is connected...\n");
      }

      recv(new_socket, buffer, bufsize, 0);
      decouple(buffer);
      printf("%s\n", buffer);
      write(new_socket, "HTTP/1.1 200 OK\n", 16);
      write(new_socket, "Content-length: 46\n", 19);
      write(new_socket, "Content-Type: text/html\n\n", 25);
      write(new_socket, "<html><body><H1>Hello world</H1></body></html>",46);
      system("wget www.google.com.br");
      close(new_socket);
   }
   close(create_socket);
   return 0;
}
