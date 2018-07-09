#include <gtk/gtk.h>

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
  gtk_text_buffer_insert(buffer, &iter, "This is a buffer\nThis is a buffer\nThis is a buffer\n", -1);

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
  g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

  /* Coloca o botao na celula (0, 0) da grid */
  gtk_grid_attach(GTK_GRID(gridleft), button, 0, 0, 1, 1);

  /* Coloca a view na celula (0, 1) da grid */
  gtk_grid_attach(GTK_GRID(gridleft), text_view, 0, 1, 1, 1);

  /* Cria o botao de response e binda ele com a funçao na CALLBACK*/
  button = gtk_button_new_with_label("Response");
  g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

  /* Coloca o botao na celula (0, 0) da grid */
  gtk_grid_attach(GTK_GRID(gridright), button, 0, 0, 1, 1);

  /* Coloca a view na celula (0, 1) da grid */
  gtk_grid_attach(GTK_GRID(gridright), text_view_aux, 0, 1, 1, 1);


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

int main(int argc,
         char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}