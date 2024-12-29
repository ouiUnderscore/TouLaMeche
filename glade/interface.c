#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UI_FILE "interface.glade"

GtkBuilder *builder_global;
GtkWidget *main_window;
GtkWidget *quit_window;
GtkWidget *about_window;
GtkWidget *button_quit_without_saving;
GtkWidget *button_ok_about_window;

int initialize_main_window();
void initialize_quit_window();
void initialize_about_window();
void on_quit_without_saving_button_activate();
void on_quit_menu_item_activate();
void on_ok_button_about_activate();

#define COLOR_RED "\033[31m"
#define COLOR_ORANGE "\033[38;5;214m"  // Code ANSI pour une couleur orange approximative
#define COLOR_RESET "\033[0m"

int initialize_main_window()
{
    builder_global = gtk_builder_new();
    if (gtk_builder_add_from_file(builder_global, UI_FILE, NULL) == 0)
    {
        printf(COLOR_RED "Erreur : Impossible de charger le fichier %s\n" COLOR_RESET, UI_FILE);
        return 0;
    }

    main_window = GTK_WIDGET(gtk_builder_get_object(builder_global, "main_window"));
    if (!main_window)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le widget 'main_window'\n" COLOR_RESET);
        return 0;
    }

    return 1;
}

void initialize_quit_window()
{
    quit_window = GTK_WIDGET(gtk_builder_get_object(builder_global, "quit_window"));
    if (!quit_window)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le widget 'quit_window'\n" COLOR_RESET);
        return;
    }

    button_quit_without_saving = GTK_WIDGET(gtk_builder_get_object(builder_global, "button_quit_without_saving"));
    if (!button_quit_without_saving)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le widget 'button_quit_without_saving'\n" COLOR_RESET);
        return;
    }

    g_signal_connect(button_quit_without_saving, "clicked", G_CALLBACK(on_quit_without_saving_button_activate), NULL);
}

void initialize_about_window()
{
    about_window = GTK_WIDGET(gtk_builder_get_object(builder_global, "about_window"));
    if (!about_window)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le widget 'about_window'\n" COLOR_RESET);
        return;
    }

    button_ok_about_window = GTK_WIDGET(gtk_builder_get_object(builder_global, "ok_about_button"));
    if (!button_ok_about_window)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le widget 'button_ok_about_window'\n" COLOR_RESET);
        return;
    }

    g_signal_connect(button_ok_about_window, "clicked", G_CALLBACK(on_ok_button_about_activate), NULL);
}

void on_quit_without_saving_button_activate()
{
    printf(COLOR_ORANGE "Log : Bouton quit without saving\n" COLOR_RESET);
    gtk_main_quit();
}

void on_quit_menu_item_activate()
{
    printf(COLOR_ORANGE "Log : Menu item quit\n" COLOR_RESET);

    if (!quit_window)
    {
        printf(COLOR_RED "Erreur : La fenêtre quit_window n'est pas initialisée\n" COLOR_RESET);
        return;
    }

    gtk_widget_show_all(quit_window);
}

void on_ok_button_about_activate() {
    printf(COLOR_ORANGE "Log : Bouton Ok\n" COLOR_RESET);
    gtk_widget_hide(about_window);
}

void on_about_menu_item_activate()
{
    printf(COLOR_ORANGE "Log : Menu item about\n" COLOR_RESET);

    if (!about_window)
    {
        printf(COLOR_RED "Erreur : La fenêtre about_window n'est pas initialisée\n" COLOR_RESET);
        return;
    }

    gtk_widget_show_all(about_window);
}

void on_text_buffer_changed(GtkTextBuffer *buffer, gpointer user_data)
{
    static int previous_length = 0; // Stocke la longueur précédente du texte
    static int previous_was_separator = 0; // Indique si le dernier caractère était un séparateur
    GtkTextIter start, end;
    char *text;

    // Récupère text
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    int current_length = g_utf8_strlen(text, -1);

    // Vérifier si un caractère a été ajouté
    if (current_length > previous_length)
    {
        const char *last_char = g_utf8_offset_to_pointer(text, current_length - 1);

        // Liste des caractères séparateurs (espace, ponctuation, etc.)
        const char *separators = " \t.,;!?()[]{}:<>\"";

        // Vérifier si le dernier caractère est un séparateur
        if (strchr(separators, *last_char))
        {
            if (!previous_was_separator)
            {
                char **words = g_strsplit_set(text, " \t\n.,;!?()[]{}:<>\"", -1);
                int num_words = g_strv_length(words);

                // Vérifier qu'il y a un mot avant le séparateur
                if (num_words > 1)
                {
                    gchar *last_word = words[num_words - 2];
                    printf("Event : Dernier mot : %s\n", last_word);

                    GtkTextBuffer *prediction_buffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder_global, "prediction_buffer_main_window"));
                    if (!prediction_buffer)
                    {
                        printf(COLOR_RED "Erreur : Le buffer prediction_buffer_main_window n'a pas été trouvé\n" COLOR_RESET);
                        return;
                    } 

                    int len = strlen(last_word) + 15;
                    char res[len];
                    strcpy(res, "Prediction : ");
                    strcat(res, last_word);
                    gtk_text_buffer_set_text(prediction_buffer, res, -1);
                }

                g_strfreev(words);
            }
            previous_was_separator = 1;
        }
        else
            previous_was_separator = 0;
    }

    previous_length = current_length;
    g_free(text);
}

int main(int argc, char *argv[])
{
    GtkWidget *quit_menu_item;
    GtkWidget *about_menu_item;
    GtkTextBuffer *buffer;

    gtk_init(&argc, &argv);

    if (!initialize_main_window())
    {
        printf(COLOR_RED "Erreur : La fenêtre main_window n'est pas initialisée\n" COLOR_RESET);
        return 1; 
    }

    // Trouvez et connectez le menu Quit
    quit_menu_item = GTK_WIDGET(gtk_builder_get_object(builder_global, "quit_menu_item"));
    if (!quit_menu_item)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le widget 'quit_menu_item'\n" COLOR_RESET);
        return 1;
    }
    g_signal_connect(quit_menu_item, "activate", G_CALLBACK(on_quit_menu_item_activate), NULL);

    // Trouvez et connectez le menu About
    about_menu_item = GTK_WIDGET(gtk_builder_get_object(builder_global, "about_menu_item"));
    if (!about_menu_item)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le widget 'about_menu_item'\n" COLOR_RESET);
        return 1;
    }
    g_signal_connect(about_menu_item, "activate", G_CALLBACK(on_about_menu_item_activate), NULL);

    buffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder_global, "entry_buffer_main_window"));
    if (!buffer)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le buffer 'entry_buffer_main_window'\n" COLOR_RESET);
        return 1;
    }
    g_signal_connect(buffer, "changed", G_CALLBACK(on_text_buffer_changed), NULL);

    initialize_quit_window();
    initialize_about_window();

    // Affichez la fenêtre principale
    gtk_widget_show_all(main_window);
    
    gtk_main();

    return 0;
}
