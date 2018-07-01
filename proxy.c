#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <netinet/tcp.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>  
#include <sys/stat.h>

#define TRUE 1
#define FALSE 0

struct http_request
{
  char host[350];
  char url[350];
  char file_path[200];
  char complete_path[200];
};

#define BUFFER_SIZE 1024

void cut_string_index(char *src, char *dest, int first_index, int second_index)
{
  int j = 0;
  for (int i = first_index; i != second_index; i++)
  {
    if (src[i] != ' ' && src[i] != '\r')
    {
      dest[j] = src[i];
      j++;
    }
  }
  dest[j] = '\0';
}

void cut_string_char(char *src, char *dest, int first_index, char second_index)
{
  int j = 0;
  for (int i = first_index; src[i] != second_index; i++)
  {
    if (src[i] != ' ' && src[i] != '\r')
    {
      dest[j] = src[i];
      j++;
    }
  }
  dest[j] = '\0';
}

void find_subdir(struct http_request get)
{
  char *data = get.url;
  char *p1, p2[10], subdir[200] = "";
  char buffer[200] = "";
  int i = 0, j = 0, dash_occurencies[2], n, m;

  p1 = strstr(data, get.host);
  if (p1)
  {
    while (p1[i] != '\0' && j < 2)
    {
      if (p1[i] == '/')
      {
        dash_occurencies[j] = i;
        j++;
      }
      i++;
    }
    if (j == 2)
    {
      n = dash_occurencies[0] + 1;
      m = dash_occurencies[1] - dash_occurencies[0] - 1;
      strncpy(p2, &p1[n], m);
      strcat(subdir, get.host);
      strcat(subdir, "/");
      strcat(subdir, p2);
      strcat(buffer, "mkdir -p ");
      strcat(buffer, subdir);
      system(buffer);
    }
  }
}

void getHTTPFile(struct http_request get, int new_socket)
{
  struct hostent *hp;
  struct sockaddr_in addr;
  FILE *index;
  int on = 1, sock;
  int dir_result;
  char url[200] = "", path[200] = "";
  char buffer[BUFFER_SIZE];

  if ((hp = gethostbyname(get.host)) == NULL){
    herror("gethostbyname");
  }else{
    bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
    addr.sin_port = htons(80);
    addr.sin_family = AF_INET;
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));

    if (sock == -1)
    {
      perror("setsockopt");
      exit(1);
    }

    if (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1)
    {
      perror("connect");
      exit(1);
    }
    strcat(url, "GET ");
    strcat(url, get.url);
    strcat(url, " \r\n");
    write(sock, url, strlen(url));
    bzero(buffer, BUFFER_SIZE);

    if ((dir_result = mkdir(get.host, S_IRUSR | S_IWUSR | S_IXUSR)) != 0)
    {
      if (dir_result != 0 && errno != EEXIST)
      {
        printf("mkdir error: %s", strerror(errno));
      }
    }
    find_subdir(get);
    strcat(path, get.host);
    strcat(path, get.file_path);
    index = fopen(path, "w");
    send(new_socket, "HTTP/1.0 200 OK\r\n\r\n", strlen("HTTP/1.0 200 OK\r\n\r\n"), 0);
    while (read(sock, buffer, BUFFER_SIZE - 1) != 0)
    {
      fputs(buffer,index);
      send(new_socket, buffer, strlen(buffer), 0);
      bzero(buffer, BUFFER_SIZE);
    }

    shutdown(sock, SHUT_RDWR);
    close(sock);
    fclose(index);
    printf("[Proxy] No cahced file! Downloaded and sent!");
  }
}

void decouple(char *buffer, struct http_request *httpGet)
{
  int index_get = 3, index_http = -1, index_host = 5;
  char host_adress[150], url[150], url_preload[200] = "", file_path[200] = "";

  char *get_index = strstr(buffer, "GET");
  char *http_index = strstr(buffer, "HTTP/1.1");
  char *host = strstr(buffer, "Host:");

  if (get_index != NULL && http_index != NULL && host != NULL)
  {
    index_get = get_index - buffer + index_get;
    index_http = http_index - buffer + index_http;
    index_host = host - buffer + index_host;
    cut_string_index(buffer, url, index_get, index_http);
    cut_string_char(buffer, host_adress, index_host, '\n');
    strcpy(httpGet->host, host_adress);
    strcpy(httpGet->url, url);
  }
  strcpy(url_preload, "http://");
  strcat(url_preload, host_adress);
  strcat(url_preload, "/");
  if (strcmp(url_preload, url) == 0)
  {
    strcat(file_path, "/index.html");
  }
  else
  {
    cut_string_index(url, file_path, strlen(url_preload) - 1, strlen(url));
  }
  strcpy(httpGet->file_path, file_path);
  strcpy(file_path, "");
  strcpy(httpGet->complete_path, "");
  strcat(httpGet->complete_path, httpGet->host);
  strcat(httpGet->complete_path, httpGet->file_path);
}

int cached(char * filepath){
  if( access( filepath, F_OK ) != -1 ) {
    return TRUE;
  } else {
    return FALSE;
  }
}

void sendCachedFile(char * filepath, int sendsocket){
  char buffer[BUFSIZ];
  FILE *arq;
  arq = fopen(filepath, "r");
  if(arq == NULL){
			printf("Erro, nao foi possivel abrir o arquivo\n");
  }else{
    send(sendsocket, "HTTP/1.0 200 OK\r\n\r\n", strlen("HTTP/1.0 200 OK\r\n\r\n"), 0);
    while(fread(&buffer, 1, BUFSIZ, arq) == BUFSIZ ){
      send(sendsocket, buffer, BUFSIZ, 0);
    }
  }
	fclose(arq);
  printf("[Proxy] Cached File! Just sent to the brower!");
}

int main(int argc, char *argv[])
{
  int create_socket, new_socket;
  socklen_t addrlen;
  int bufsize = 1024;
  char *buffer = malloc(bufsize);
  struct sockaddr_in address;
  char c;
  int PORT = 8228;
  struct http_request httpGet;

  if (argc < 2)
  {
    printf("Porta a ser utilizada será a default.");
  }

  while ((c = getopt(argc, argv, "p:")) != -1)
  {
    switch (c)
    {
    case 'p':
      PORT = atoi(optarg);
      break;
    }
  }

  /* Cria o socket para a comunicaçao */
  if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) > 0)
  { /* AF_INET = IPV4 , SOCK_STREAM = TCP, 0 = IP*/
    printf("Socket criado\n");
  }
  else
  {
    printf("Erro ao criar socket");
  }

  address.sin_family = AF_INET;   /* IPV4*/
  address.sin_port = htons(PORT); /* Port*/

  /* Binda o socket para a comunicacao */
  if (bind(create_socket, (struct sockaddr *)&address, sizeof(address)) == 0)
  {
    printf("Binding Socket\n");
  }

  while (1)
  {
    /* Colocar o servidor (local) para escutar */
    if (listen(create_socket, 10) < 0)
    {
      perror("server: listen");
      exit(1);
    }

    /* Libera o servidor (local) para aceitar uma conexão com o cliente */
    if ((new_socket = accept(create_socket, (struct sockaddr *)&address, &addrlen)) < 0)
    {
      perror("server: accept");
      exit(1);
    }

    if (new_socket > 0)
    {
      printf("[Proxy] Client connected\n");
    }

    recv(new_socket, buffer, bufsize, 0);
    /* Recebe a mensagem do cliente com as informaçoes da conexao: Host, Http request, Conexao de Proxy... */
    /* Chama funcao que faz o parse da mensagem do cliente */
    decouple(buffer, &httpGet);
    printf("[Proxy] GET %s \n\n", httpGet.complete_path);
    if(strcmp(httpGet.host,"g.symcd.com") != 0 && strcmp("sr.symcd.com",httpGet.host) != 0 && strcmp("ocsp.digicert.com",httpGet.host) != 0 ){
      if(cached(httpGet.complete_path)){
      sendCachedFile(httpGet.complete_path, new_socket);
      }else{
        getHTTPFile(httpGet, new_socket);
      }
    }else{
      printf("[Proxy] Notifiations not allowed");
    }
    
    close(new_socket);
    printf("\n\n_____________________________________\n\n");
  }
  close(create_socket);
  return 0;
}
