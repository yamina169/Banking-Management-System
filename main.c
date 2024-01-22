#include <gtk/gtk.h>
#include <pango/pango.h>
#include <windows.h>

GtkWidget *grid;

void on_login_button_clicked(GtkWidget *button, gpointer user_data) {
    const char *password = gtk_entry_get_text(GTK_ENTRY(user_data));

    if (strcmp(password, "chy") == 0) {
        g_print("Correct password!\n");
        GtkWidget *window = GTK_WIDGET(gtk_widget_get_toplevel(GTK_WIDGET(button)));
        gtk_widget_destroy(window);
        menu();
    } else {
        g_print("Incorrect password!\n");
        GtkWidget *window = GTK_WIDGET(gtk_widget_get_toplevel(GTK_WIDGET(button)));
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Incorrect password!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void on_exit_button_clicked(GtkWidget *button, gpointer user_data) {
    gtk_main_quit();
    HWND console = GetConsoleWindow();
    PostMessage(console, WM_CLOSE, 0, 0);
}


int main(int argc, char *argv[]) {
    HWND console = GetConsoleWindow();

    ShowWindow(console, SW_MAXIMIZE);
    gtk_init(&argc, &argv);

    GtkWidget *window;
    GtkWidget *login_entry;
    GtkWidget *login_button;
    GtkWidget *exit_button;
    GtkWidget *title_label;
    GtkWidget *image;
    GtkWidget *enter_password_label;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "CHY Bank");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_fullscreen(GTK_WINDOW(window));

    GdkRGBA color;
    gdk_rgba_parse(&color, "white");
    gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &color);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);

    exit_button = gtk_button_new();
    GtkWidget *exit_image = gtk_image_new_from_icon_name("window-close-symbolic", GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(exit_button), exit_image);

    gdk_rgba_parse(&color, "red");
    gtk_widget_override_background_color(exit_button, GTK_STATE_FLAG_NORMAL, &color);
    gtk_widget_override_color(exit_button, GTK_STATE_FLAG_NORMAL, &color);

    g_signal_connect(exit_button, "clicked", G_CALLBACK(on_exit_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), exit_button, 1, 0, 1, 1);
    gtk_widget_set_halign(exit_button, GTK_ALIGN_END);

    title_label = gtk_label_new("CHY Bank");

    title_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title_label), "<b><span font_weight='bold' font='Serif 16'>WELCOME TO CHY BANK</span></b>");
        gtk_widget_set_margin_top(title_label, 100);

    gtk_grid_attach(GTK_GRID(grid), title_label, 0, 1, 1, 1);
    gtk_widget_set_halign(title_label, GTK_ALIGN_CENTER);


   image = gtk_image_new_from_file("image.jfif");
gtk_grid_attach(GTK_GRID(grid), image, 0, 2, 1, 1);
gtk_widget_set_halign(image, GTK_ALIGN_CENTER);

gtk_widget_set_margin_top(image, 10);

enter_password_label = gtk_label_new("Enter your password");
gtk_grid_attach(GTK_GRID(grid), enter_password_label, 0, 3, 1, 1);
gtk_widget_set_halign(enter_password_label, GTK_ALIGN_CENTER);


    login_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(login_entry), FALSE);
    gtk_grid_attach(GTK_GRID(grid), login_entry, 0, 4, 1, 1);
    gtk_widget_set_hexpand(login_entry, TRUE);
    gtk_widget_set_size_request(login_entry, 120, 30);
    gtk_widget_set_halign(login_entry, GTK_ALIGN_CENTER);

    login_button = gtk_button_new_with_label("Login");
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_button_clicked), login_entry);
    gtk_grid_attach(GTK_GRID(grid), login_button, 0, 5, 1, 1);
    gtk_widget_set_size_request(login_button, 120, 30);
    gtk_widget_set_halign(login_button, GTK_ALIGN_CENTER);


    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
