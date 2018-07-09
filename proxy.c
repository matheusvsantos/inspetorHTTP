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
#include <gtk/gtk.h>


struct http_request
{
  char host[350];
  char url[350];
  char file_path[200];
  char complete_path[200];
};

#define BUFFER_SIZE 1024

static void
print_hello(GtkWindow *window){
  GtkWidget *dialog;

  GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
  dialog = gtk_message_dialog_new(window,
                                  flags,
                                  GTK_MESSAGE_ERROR,
                                  GTK_BUTTONS_CLOSE,
                                  "Error reading");
  gtk_dialog_run(GTK_DIALOG(dialog));

}

static void
activate(GtkApplication *app,
         gpointer user_data)
{
  /* Declarando os widgets utilizados na aplicaçao, o ponteiro do texto e o buffer */
  GtkWidget *window;
  GtkWidget *headerbar;
  GtkWidget *box;
  GtkWidget *gridleft, *gridright, *gridtop;
  GtkWidget *button;
  GtkWidget *text_view;
  GtkWidget *text_view_aux;
  GtkWidget *hpaned;
  GtkWidget *frame1 = gtk_frame_new(NULL);
  GtkWidget *frame2 = gtk_frame_new(NULL);
  GtkTextBuffer *buffer;
  GtkTextIter iter;

  /* Cria uma janela e seta o tamanho */
  window = gtk_application_window_new(app);
  gtk_window_set_default_size(GTK_WINDOW(window),650,400);
  gtk_window_set_title(GTK_WINDOW(window), "Window");
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  /* Cria o painel de divisao */
  hpaned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
  /* Cria os frames da esquerda e direita */
  gtk_frame_set_shadow_type(GTK_FRAME(frame1), GTK_SHADOW_IN);
  gtk_frame_set_shadow_type(GTK_FRAME(frame2), GTK_SHADOW_IN);

  /* Seta o tamanho do painel */
  gtk_widget_set_size_request(hpaned, 200, -1);

  /* Anexa o frame da esquerda ao painel e seta o tamanho */
  gtk_paned_pack1(GTK_PANED(hpaned), frame1, TRUE, TRUE);
  gtk_widget_set_size_request(frame1, 100, -1);

  /* Anexa o frame da direita ao painel e seta o tamanho */
  gtk_paned_pack2(GTK_PANED(hpaned), frame2, TRUE, FALSE);
  gtk_widget_set_size_request(frame2, 100, -1);

  /* Criando o box que será a base da aplicaçao, contendo todos os outros componentes */
  box = gtk_box_new(GTK_ORIENTATION_VERTICAL,10);

  /* Cria as grids (uma para o painel da esquerda, outra para o da direita e outra para o header */
  gridleft = gtk_grid_new();
  gridright = gtk_grid_new();
  gridtop = gtk_grid_new();

  /* Aloca o header, que conterá os botoes de spider e dump */
  headerbar = gtk_header_bar_new();
  buffer = gtk_text_buffer_new(NULL);

  /* Aloca duas views de texto, uma para o frame da esquerda e outra para o frame da direita */
  text_view = gtk_text_view_new_with_buffer(buffer);
  text_view_aux = gtk_text_view_new_with_buffer(buffer);

  /* Inicializa o iter no offset 0 */
  gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);

  /* Coloca o texto inicial no buffer */
  char source[10000] = "";
  FILE *fp = fopen("request.txt", "r");
  if (fp != NULL) {
    size_t newLen = fread(source, sizeof(char), 10000, fp);
    if ( ferror( fp ) != 0 ) {
        fputs("Error reading file", stderr);
    } else {
        source[newLen++] = '\0'; /* Just to be safe. */
    }
    fclose(fp);
  }
  gtk_text_buffer_insert(buffer, &iter, source, -1);

  /* Seta as propriedades da text view  */
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view_aux), GTK_WRAP_WORD);


  /* Adiciona o box na windows */
  gtk_container_add(GTK_CONTAINER(window), box);

  /* Adiciona o header no box */
  gtk_container_add(GTK_CONTAINER(box), headerbar);

  /* Adiciona o gridtop no header */
  gtk_container_add(GTK_CONTAINER(headerbar), gridtop);

  /* Adiciona o painel no box */
  gtk_container_add(GTK_CONTAINER(box), hpaned);

  /* Adiciona a gridleft no painel da esquerda */
  gtk_container_add(GTK_CONTAINER(frame1), gridleft);

  /* Adiciona a gridright no painel da direita */
  gtk_container_add(GTK_CONTAINER(frame2), gridright);

  /* Faz com que o painel ocupe o resto do espaço no box */
  gtk_box_set_child_packing(GTK_BOX(box),hpaned,TRUE,TRUE,10,GTK_PACK_START);

  /* Cria o botao de request e binda ele com a funçao na CALLBACK*/
  button = gtk_button_new_with_label("Request");
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(print_hello), window);

  /* Coloca o botao na celula (0, 0) da grid */
  gtk_grid_attach(GTK_GRID(gridleft), button, 0, 0, 1, 1);

  button = gtk_button_new_with_label("View Request");
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(print_hello), window);

  /* Coloca o botao na celula (0, 0) da grid */
  gtk_grid_attach(GTK_GRID(gridleft), button, 0, 1, 1, 1);


  /* Cria o botao de response e binda ele com a funçao na CALLBACK*/
  button = gtk_button_new_with_label("Response");
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(print_hello), window);

  /* Coloca o botao na celula (0, 0) da grid */
  gtk_grid_attach(GTK_GRID(gridright), button, 0, 0, 1, 1);

  /* Cria o botao de request e binda ele com a funçao na CALLBACK*/
  button = gtk_button_new_with_label("View Response");
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(print_hello), window);

  /* Coloca o botao na celula (0, 0) da grid */
  gtk_grid_attach(GTK_GRID(gridright), button, 0, 1, 1, 1);

  /* Cria o botao de spider e binda ele com a funçao na CALLBACK*/
  button = gtk_button_new_with_label("Spider");
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(print_hello), window);

  /* Coloca o botao na celula (0, 0) da grid */
  gtk_grid_attach(GTK_GRID(gridtop), button, 0, 0, 1, 1);

  /* Cria o botao de dump e binda ele com a funçao na CALLBACK*/
  button = gtk_button_new_with_label("Dump");
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(print_hello), window);

  /* Coloca o botao na celula (1, 0) da grid */
  gtk_grid_attach(GTK_GRID(gridtop), button, 1, 0, 1, 1);
  /* Now that we are done packing our widgets, we show them all
   * in one go, by calling gtk_widget_show_all() on the window.
   * This call recursively calls gtk_widget_show() on all widgets
   * that are contained in the window, directly or indirectly.
   */
  gtk_widget_show_all(window);
}

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

void sendCachedFile(char * filepath, int sendsocket){
  char buffer[BUFSIZ];
  FILE *arq;
  arq = fopen(filepath, "r");
  if(arq == NULL){
			printf("Erro, nao foi possivel abrir o arquivo\n");
  }else{
    send(sendsocket, "HTTP/1.0 200 OK\r\n\r\n", strlen("HTTP/1.0 200 OK\r\n\r\n"), 0);
    if (strstr(filepath,".png") != NULL) {
      send(sendsocket, "Content-Type: image/png\r\n\r\n", strlen("Content-Type: image/png\r\n\r\n"), 0);
    }

    if (strstr(filepath,".jpg") != NULL) {
      send(sendsocket, "Content-Type: image/jpg\r\n\r\n", strlen("Content-Type: image/jpg\r\n\r\n"), 0);
    }
    while(fread(&buffer, 1, BUFSIZ, arq) == BUFSIZ ){
      send(sendsocket, buffer, BUFSIZ, 0);
    }
  }
	fclose(arq);
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

    while (read(sock, buffer, BUFFER_SIZE - 1) != 0){
      fputs(buffer,index);
      bzero(buffer, BUFFER_SIZE);
    }

    fclose(index);
    sendCachedFile(get.complete_path, new_socket);
    shutdown(sock, SHUT_RDWR);
    close(sock);
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


int main(int argc, char *argv[])
{
  int create_socket, new_socket, bufsize = 4096,PORT = 8228, pid;
  socklen_t addrlen;
  char *buffer = malloc(bufsize);
  struct sockaddr_in address;
  char c;
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

  if ((pid = fork()) == 0) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);
  }else{
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
      FILE * request = fopen("request.txt","w+");
      fprintf(request, "%s\n",buffer );
      fclose(request);
      decouple(buffer, &httpGet);
      printf("[Proxy] GET %s \n\n", httpGet.complete_path);
      if(strcmp(httpGet.host,"g.symcd.com") != 0 && strcmp("sr.symcd.com",httpGet.host) != 0 && strcmp("ocsp.digicert.com",httpGet.host) != 0 ){
        if(cached(httpGet.complete_path)){
          sendCachedFile(httpGet.complete_path, new_socket);
          printf("[Proxy] Cached File! Just sent to the brower!");
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
  }
  return 0;
}
