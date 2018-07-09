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
  GtkWidget *window;
  GtkWidget *headerbar;
  GtkWidget *box;
  GtkWidget *gridleft, *gridright, *gridtop;
  GtkWidget *button;
  GtkWidget *text_view;
  GtkWidget *hpaned;
  GtkWidget *frame1 = gtk_frame_new(NULL);
  GtkWidget *frame2 = gtk_frame_new(NULL);

  GtkTextBuffer *buffer;
  GtkTextIter *iter;

  /* create a new window, and set its title */
  window = gtk_application_window_new(app);
  gtk_window_set_default_size(GTK_WINDOW(window),600,400);
  gtk_window_set_title(GTK_WINDOW(window), "Window");
  gtk_container_set_border_width(GTK_CONTAINER(window), 30);

  hpaned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_frame_set_shadow_type(GTK_FRAME(frame1), GTK_SHADOW_IN);
  gtk_frame_set_shadow_type(GTK_FRAME(frame2), GTK_SHADOW_IN);

  gtk_widget_set_size_request(hpaned, 200, -1);

  gtk_paned_pack1(GTK_PANED(hpaned), frame1, TRUE, TRUE);
  gtk_widget_set_size_request(frame1, 100, -1);

  gtk_paned_pack2(GTK_PANED(hpaned), frame2, TRUE, FALSE);
  gtk_widget_set_size_request(frame2, 100, -1);


  /* Here we construct the container that is going pack our buttons */
  gridleft = gtk_grid_new();
  gridright = gtk_grid_new();
  gridtop = gtk_grid_new();
  headerbar = gtk_header_bar_new();
  buffer = gtk_text_buffer_new(NULL);
  text_view = gtk_text_view_new_with_buffer(buffer);
  box = gtk_box_new(GTK_ORIENTATION_VERTICAL,10);

  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);

  gtk_text_buffer_insert(buffer,iter,"This is a buffer",-1);

  /* Pack the container in the window */
  gtk_container_add(GTK_CONTAINER(window), box);
  
  gtk_container_add(GTK_CONTAINER(box), headerbar);
  gtk_container_add(GTK_CONTAINER(headerbar), gridtop);
  gtk_container_add(GTK_CONTAINER(box), hpaned);
  gtk_container_add(GTK_CONTAINER(frame1), gridleft);
  gtk_container_add(GTK_CONTAINER(frame2), gridright);

  gtk_box_set_child_packing(GTK_BOX(box),hpaned,TRUE,TRUE,10,GTK_PACK_START);

  button = gtk_button_new_with_label("Request");
  g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

  /* Place the first button in the grid cell (0, 0), and make it fill
   * just 1 cell horizontally and vertically (ie no spanning)
   */
  gtk_grid_attach(GTK_GRID(gridleft), button, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(gridleft), text_view, 0, 1, 1, 1);

  button = gtk_button_new_with_label("Response");
  g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

  /* Place the second button in the grid cell (1, 0), and make it fill
   * just 1 cell horizontally and vertically (ie no spanning)
   */
  gtk_grid_attach(GTK_GRID(gridright), button, 0, 0, 1, 1);

  button = gtk_button_new_with_label("Spider");
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(print_hello), window);

  /* Place the Quit button in the grid cell (0, 1), and make it
   * span 2 columns.
   */
  gtk_grid_attach(GTK_GRID(gridtop), button, 0, 0, 1, 1);

  button = gtk_button_new_with_label("Dump");
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(print_hello), window);

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