#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../arbre_de_prediction/arbre_pred.h"
#include "../sequence/sequence.h"
#include "../sequence/hash.h"
#include "../entree_sortie/entree_sortie.h"

#define UI_FILE "interface.glade"

GtkBuilder *builder_global;
GtkWidget *main_window;
GtkWidget *quit_window;
GtkWidget *about_window;
GtkTextBuffer *bufferTextEntry;
GtkTextBuffer *prediction_buffer;
struct strhash_table *ht;
int entryWordCount = 0; // Variable permettant de compter le nombre de mot entré dans le champ de saisie

int initialize_main_window();
void initialize_quit_window();
void initialize_about_window();
void on_quit_menu_item_activate();
int on_save_file_menu_item_activate();
void on_open_file_menu_item_activate();
void on_about_menu_item_activate();
int on_save_database_menu_item_activate();
void on_open_database_menu_item_activate();
void on_text_buffer_changed(GtkTextBuffer *buffer);
void on_main_window_destroy();
void on_quit_without_saving_button_activate();
void on_cancel_button_quit_activate();
void on_ok_button_about_activate();
void on_file_button_clicked();
void on_database_button_clicked();

#define COLOR_RED "\033[31m"
#define COLOR_ORANGE "\033[38;5;214m" // Code ANSI pour une couleur orange approximative
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

    prediction_buffer = GTK_TEXT_BUFFER(gtk_builder_get_object(builder_global, "prediction_buffer_main_window"));
    if (!prediction_buffer)
    {
        printf(COLOR_RED "Erreur : Le buffer prediction_buffer_main_window n'a pas été trouvé\n" COLOR_RESET);
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

    GtkWidget *button_quit_without_saving = GTK_WIDGET(gtk_builder_get_object(builder_global, "button_quit_without_saving"));
    if (!button_quit_without_saving)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le widget 'button_quit_without_saving'\n" COLOR_RESET);
        return;
    }
    g_signal_connect(button_quit_without_saving, "clicked", G_CALLBACK(on_quit_without_saving_button_activate), NULL);

    GtkWidget *button_cancel_quit_window = GTK_WIDGET(gtk_builder_get_object(builder_global, "quit_windows_cancel_button"));
    if (!button_cancel_quit_window)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le widget 'button_cancel_quit_window'\n" COLOR_RESET);
        return;
    }
    g_signal_connect(button_cancel_quit_window, "clicked", G_CALLBACK(on_cancel_button_quit_activate), NULL);

    GtkWidget *quit_window_file_button = GTK_WIDGET(gtk_builder_get_object(builder_global, "quit_window_file_button"));
    if (!quit_window_file_button)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le widget 'quit_window_file_button'\n" COLOR_RESET);
        return;
    }
    g_signal_connect(quit_window_file_button, "clicked", G_CALLBACK(on_file_button_clicked), NULL);

    GtkWidget *quit_window_database_button = GTK_WIDGET(gtk_builder_get_object(builder_global, "quit_window_database_button"));
    if (!quit_window_database_button)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le widget 'quit_window_database_button'\n" COLOR_RESET);
        return;
    }
    g_signal_connect(quit_window_database_button, "clicked", G_CALLBACK(on_database_button_clicked), NULL);
}

void initialize_about_window()
{
    about_window = GTK_WIDGET(gtk_builder_get_object(builder_global, "about_window"));
    if (!about_window)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le widget 'about_window'\n" COLOR_RESET);
        return;
    }

    GtkWidget *button_ok_about_window = GTK_WIDGET(gtk_builder_get_object(builder_global, "ok_about_button"));
    if (!button_ok_about_window)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le widget 'button_ok_about_window'\n" COLOR_RESET);
        return;
    }

    g_signal_connect(button_ok_about_window, "clicked", G_CALLBACK(on_ok_button_about_activate), NULL);
}

void on_quit_menu_item_activate()
{
    printf(COLOR_ORANGE "Log : Menu item quit\n" COLOR_RESET);

    initialize_quit_window();

    // Bloque la page principale
    gtk_window_set_modal(GTK_WINDOW(quit_window), TRUE);
    gtk_window_set_transient_for(GTK_WINDOW(quit_window), GTK_WINDOW(main_window));

    if (!quit_window)
    {
        printf(COLOR_RED "Erreur : La fenêtre quit_window n'est pas initialisée\n" COLOR_RESET);
        return;
    }

    gtk_widget_show_all(quit_window);
}

int on_save_file_menu_item_activate()
{
    printf(COLOR_ORANGE "Log : Menu item save file\n" COLOR_RESET);

    const char *extension = ".txt";

    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new("Save File", NULL, GTK_FILE_CHOOSER_ACTION_SAVE, "_Cancel", GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, NULL);

    // Ajouter un filtre n'afficher que les fichier au bon format
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.txt");
    gtk_file_filter_set_name(filter, "Text Files (*.txt)");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    int response = gtk_dialog_run(GTK_DIALOG(dialog));

    // Si l'utilisateur clique sur "Cancel"
    if (response == GTK_RESPONSE_CANCEL)
    {
        gtk_widget_destroy(dialog);
        return 0;
    }

    // Si l'utilisateur clique sur "Save"
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

        // Si le fichier n'a pas d'extension, l'ajoute
        if (!strchr(filename, '.'))
        {
            char *new_filename = g_strconcat(filename, extension, NULL);
            g_free(filename);
            filename = new_filename;
        }
        // Si l'extension est incorrecte
        else if (!g_str_has_suffix(filename, extension))
        {
            printf(COLOR_RED "Erreur : L'extension du fichier doit être %s\n" COLOR_RESET, extension);
            GtkWidget *error_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Erreur : L'extension du fichier doit être .txt.");
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
            g_free(filename);
            gtk_widget_destroy(dialog);
            return 0;
        }

        // Vérification si le fichier existe déjà
        if (g_file_test(filename, G_FILE_TEST_EXISTS))
        {
            GtkWidget *confirm_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_YES_NO, "Le fichier \"%s\" existe déjà.\nVoulez-vous le remplacer ?", filename);

            // Si l'utilisateur dit non
            if (gtk_dialog_run(GTK_DIALOG(confirm_dialog)) != GTK_RESPONSE_YES)
            {
                gtk_widget_destroy(confirm_dialog);
                g_free(filename);
                gtk_widget_destroy(dialog);
                return 0;
            }

            gtk_widget_destroy(confirm_dialog);
        }

        // Récupérer le contenu du buffer GTK et l'écrire dans le fichier de sauvegarde
        GtkTextIter start, end;
        gtk_text_buffer_get_bounds(bufferTextEntry, &start, &end);
        char *text = gtk_text_buffer_get_text(bufferTextEntry, &start, &end, FALSE);

        FILE *file = fopen(filename, "w");
        if (file)
        {
            fprintf(file, "%s", text);
            fclose(file);
            printf(COLOR_ORANGE "Log : Fichier enregistré avec succès : %s\n" COLOR_RESET, filename);
        }
        else
            printf(COLOR_RED "Erreur : Ouverture du fichier impossible : %s\n" COLOR_RESET, filename);

        g_free(text);
        g_free(filename);
    }

    gtk_widget_destroy(dialog);

    return 1;
}

void on_open_file_menu_item_activate()
{
    printf(COLOR_ORANGE "Log : Menu item open file\n" COLOR_RESET);

    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new("Open File", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);

    // Ajouter un filtre n'afficher que les fichier au bon format
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.txt");
    gtk_file_filter_set_name(filter, "Text Files (*.txt)");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    int response = gtk_dialog_run(GTK_DIALOG(dialog));
    char *content = 0;
    char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    long length;

    sequence_initialize(ht);
    entryWordCount = 0;

    // Si l'utilisateur clique sur "Cancel"
    if (response == GTK_RESPONSE_CANCEL)
    {
        gtk_widget_destroy(dialog);
        return;
    }

    // Si l'utilisateur clique sur "Open"
    if (response == GTK_RESPONSE_ACCEPT)
    {
        // Copie le contenu du fichier dans content
        FILE *file = fopen(filename, "r");
        if (file)
        {
            fseek(file, 0, SEEK_END);
            length = ftell(file);
            fseek(file, 0, SEEK_SET);
            content = malloc(length);

            if (content)
            {
                fread(content, 1, length, file);
                content[length] = '\0';
            }

            fclose(file);
        }

        if (content)
        {
            // Mettre le contenu dans le buffer de texte
            GtkTextIter start, end;
            gtk_text_buffer_get_bounds(bufferTextEntry, &start, &end);
            gtk_text_buffer_delete(bufferTextEntry, &start, &end);        // Effacer le texte existant
            gtk_text_buffer_insert(bufferTextEntry, &start, content, -1); // Insérer le contenu du fichier

            const char *separators = " \t\n.,;!?()[]{}:<>\"";
            char last_word[256] = "";
            int was_sep = 0;
            int pos_last_word = 0;

            // Parcours tous les caractères du contenu
            for (int i = 0; i <= length; i++)
            {
                // Si le caractère est un séparateur, ajoute le mot dans la table de hashage
                if ((strchr(separators, content[i]) && was_sep == 0) || i == length)
                {
                    sequence_addWord(last_word, ht);
                    sequence_progress();
                    searchOrCreateTreeNode();
                    strcpy(last_word, "");
                    was_sep = 1;
                    pos_last_word = 0;
                    entryWordCount++;
                }
                // Si non concatene le dernier caractère avec le mot
                else if (!strchr(separators, content[i]))
                {
                    last_word[pos_last_word++] = content[i];
                    was_sep = 0;
                }
            }

            // Vider le buffer d'entrée
            gtk_text_buffer_get_bounds(prediction_buffer, &start, &end);
            gtk_text_buffer_delete(prediction_buffer, &start, &end);
            gtk_text_buffer_insert(prediction_buffer, &start, "", -1);
        }

        // Fermer le fichier et libérer la mémoire
        g_free(content);
        printf(COLOR_ORANGE "Log : Fichier lu avec succès : %s\n" COLOR_RESET, filename);
    }
    else
    {
        printf(COLOR_RED "Erreur : Ouverture du fichier impossible : %s\n" COLOR_RESET, filename);
        return;
    }

    g_free(filename);
    gtk_widget_destroy(dialog);
}

void on_about_menu_item_activate()
{
    printf(COLOR_ORANGE "Log : Menu item about\n" COLOR_RESET);

    initialize_about_window();

    // Bloque la page principale
    gtk_window_set_modal(GTK_WINDOW(about_window), TRUE);
    gtk_window_set_transient_for(GTK_WINDOW(about_window), GTK_WINDOW(main_window));

    if (!about_window)
    {
        printf(COLOR_RED "Erreur : La fenêtre about_window n'est pas initialisée\n" COLOR_RESET);
        return;
    }

    gtk_widget_show_all(about_window);
}

int on_save_database_menu_item_activate()
{
    printf(COLOR_ORANGE "Log : Menu item save database\n" COLOR_RESET);

    const char *extension = ".tlmdb";

    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new("Save File", NULL, GTK_FILE_CHOOSER_ACTION_SAVE, "_Cancel", GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, NULL);

    // Ajouter un filtre n'afficher que les fichier au bon format
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.tlmdb");
    gtk_file_filter_set_name(filter, "Text Files (*.tlmdb)");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    int response = gtk_dialog_run(GTK_DIALOG(dialog));

    // Si l'utilisateur clique sur "Cancel"
    if (response == GTK_RESPONSE_CANCEL)
    {
        gtk_widget_destroy(dialog);
        return 0;
    }

    // Bouton save
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

        // Si le fichier n'a pas d'extension, l'ajoute
        if (!strchr(filename, '.'))
        {
            char *new_filename = g_strconcat(filename, extension, NULL);
            g_free(filename);
            filename = new_filename;
        }
        // Si l'extension est incorrecte
        else if (!g_str_has_suffix(filename, extension))
        {
            printf(COLOR_RED "Erreur : L'extension du fichier doit être %s\n" COLOR_RESET, extension);
            GtkWidget *error_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Erreur : L'extension du fichier doit être .txt.");
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
            g_free(filename);
            gtk_widget_destroy(dialog);
            return 0;
        }

        // Vérification si le fichier existe déjà
        if (g_file_test(filename, G_FILE_TEST_EXISTS))
        {
            GtkWidget *confirm_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING, GTK_BUTTONS_YES_NO, "Le fichier \"%s\" existe déjà.\nVoulez-vous le remplacer ?", filename);

            // Si l'utilisateur dit non
            if (gtk_dialog_run(GTK_DIALOG(confirm_dialog)) != GTK_RESPONSE_YES)
            {
                gtk_widget_destroy(confirm_dialog);
                g_free(filename);
                gtk_widget_destroy(dialog);
                return 0;
            }

            gtk_widget_destroy(confirm_dialog);
        }

        FILE *f = fopen(filename, "w");
        if (f == NULL)
        {
            printf(COLOR_RED "Erreur : Impossible d'écrire dans le fichier %s\n" COLOR_RESET, filename);
            return 0;
        }
        saveTreeNode(f, &root, 0);
        fclose(f);

        g_free(filename);
    }

    gtk_widget_destroy(dialog);

    return 1;
}

void on_open_database_menu_item_activate()
{
    printf(COLOR_ORANGE "Log : Menu item open database\n" COLOR_RESET);

    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new("Open Database", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);

    // Ajouter un filtre n'afficher que les fichier au bon format
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.tlmdb");
    gtk_file_filter_set_name(filter, "Text Files (*.tlmdb)");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    int response = gtk_dialog_run(GTK_DIALOG(dialog));
    char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

    // Si l'utilisateur clique sur "Cancel"
    if (response == GTK_RESPONSE_CANCEL)
    {
        gtk_widget_destroy(dialog);
        return;
    }

    // Si l'utilisateur clique sur "Open"
    if (response == GTK_RESPONSE_ACCEPT)
    {
        // Copie le contenu du fichier dans content
        FILE *file = fopen(filename, "r");
        if (file == NULL)
        {
            printf(COLOR_RED "Erreur : Impossible d'ouvrir le fichier %s\n" COLOR_RESET, filename);
            return;
        }

        loadTreeNode(file, &root);

        fclose(file);

        printf(COLOR_ORANGE "Log : Fichier lu avec succès : %s\n" COLOR_RESET, filename);
    }

    int totalMots = compterMotsArbre(&root) / Lg_N_gramme;
    entryWordCount = totalMots;

    g_free(filename);
    gtk_widget_destroy(dialog);
}

void on_text_buffer_changed(GtkTextBuffer *buffer)
{
    static int previous_length = 0;        // Stocke la longueur précédente du texte
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
        const char *separators = " \t\n.,;!?()[]{}:<>\"";

        // Vérifier si le dernier caractère est un séparateur
        if (strchr(separators, *last_char))
        {
            if (!previous_was_separator)
            {
                char **words = g_strsplit_set(text, separators, -1);
                int num_words = g_strv_length(words);

                // Vérifier qu'il y a un mot avant le séparateur
                if (num_words > 1)
                {
                    gchar *last_word = words[num_words - 2];

                    if (strlen(last_word) > 0)
                    {
                        printf(COLOR_ORANGE "Event : Dernier mot : %s\n" COLOR_RESET, last_word);

                        sequence_addWord(last_word, ht);
                        sequence_progress();
                        searchOrCreateTreeNode();
                        entryWordCount++;

                        if (entryWordCount > 3)
                        {
                            TreeNode *noeudFinal = searchOrCreateLeaf(&root, last_word);
                            TreeNode *motPrevu = maxOccurrence(noeudFinal);
                            if (motPrevu == NULL)
                            {
                                printf(COLOR_ORANGE "Event : Aucun mot prédit trouvé.\n" COLOR_RESET);
                                gtk_text_buffer_set_text(prediction_buffer, "", 0);
                            }
                            else
                            {
                                printf(COLOR_ORANGE "Event : Mot prédit : %s\n" COLOR_RESET, motPrevu->mot);
                                int predLen = strlen(motPrevu->mot) + 15;
                                char resPred[predLen];
                                strcpy(resPred, "Prédiction : ");
                                strcat(resPred, motPrevu->mot);
                                gtk_text_buffer_set_text(prediction_buffer, resPred, -1);
                            }
                        }
                    }
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

void on_main_window_destroy()
{
    printf(COLOR_ORANGE "Log : Main window destroyed\n" COLOR_RESET);
    gtk_main_quit();
}

void on_quit_without_saving_button_activate()
{
    printf(COLOR_ORANGE "Log : Bouton quit without saving\n" COLOR_RESET);
    gtk_main_quit();
}

void on_cancel_button_quit_activate()
{
    printf(COLOR_ORANGE "Log : Bouton Cancel\n" COLOR_RESET);
    gtk_widget_hide(quit_window);
}

void on_ok_button_about_activate()
{
    printf(COLOR_ORANGE "Log : Bouton Ok\n" COLOR_RESET);
    gtk_widget_hide(about_window);
}

void on_file_button_clicked()
{
    printf(COLOR_ORANGE "Log : Bouton save\n" COLOR_RESET);
    if (on_save_file_menu_item_activate() == 1)
        gtk_main_quit();
    else
        gtk_widget_hide(quit_window);
}

void on_database_button_clicked()
{
    printf(COLOR_ORANGE "Log : Bouton database\n" COLOR_RESET);
    if (on_save_database_menu_item_activate() == 1)
        gtk_main_quit();
    else
        gtk_widget_hide(quit_window);

    afficherArbre(&root, 0);
}

int main(int argc, char *argv[])
{
    GtkWidget *quit_menu_item;
    GtkWidget *about_menu_item;
    GtkWidget *save_file_menu_item;
    GtkWidget *open_file_menu_item;
    GtkWidget *save_database_menu_item;
    GtkWidget *open_database_menu_item;

    // Initialisation
    initArbre();
    ht = strhash_create(Lg_N_gramme);
    sequence_initialize(ht);
    gtk_init(&argc, &argv);

    if (!initialize_main_window())
    {
        printf(COLOR_RED "Erreur : La fenêtre main_window n'est pas initialisée\n" COLOR_RESET);
        return 1;
    }

    // Trouver et connecter Les boutons
    quit_menu_item = GTK_WIDGET(gtk_builder_get_object(builder_global, "quit_menu_item"));
    if (!quit_menu_item)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le widget 'quit_menu_item'\n" COLOR_RESET);
        return 1;
    }
    g_signal_connect(quit_menu_item, "activate", G_CALLBACK(on_quit_menu_item_activate), NULL);

    about_menu_item = GTK_WIDGET(gtk_builder_get_object(builder_global, "about_menu_item"));
    if (!about_menu_item)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le widget 'about_menu_item'\n" COLOR_RESET);
        return 1;
    }
    g_signal_connect(about_menu_item, "activate", G_CALLBACK(on_about_menu_item_activate), NULL);

    bufferTextEntry = GTK_TEXT_BUFFER(gtk_builder_get_object(builder_global, "entry_buffer_main_window"));
    if (!bufferTextEntry)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le buffer 'entry_buffer_main_window'\n" COLOR_RESET);
        return 1;
    }
    g_signal_connect(bufferTextEntry, "changed", G_CALLBACK(on_text_buffer_changed), NULL);

    save_file_menu_item = GTK_WIDGET(gtk_builder_get_object(builder_global, "save_file_menu_item"));
    if (!save_file_menu_item)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le widget 'save_file_menu_item'\n" COLOR_RESET);
        return 1;
    }
    g_signal_connect(save_file_menu_item, "activate", G_CALLBACK(on_save_file_menu_item_activate), NULL);

    open_file_menu_item = GTK_WIDGET(gtk_builder_get_object(builder_global, "open_file_menu_item"));
    if (!open_file_menu_item)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le widget 'open_file_menu_item'\n" COLOR_RESET);
        return 1;
    }
    g_signal_connect(open_file_menu_item, "activate", G_CALLBACK(on_open_file_menu_item_activate), NULL);

    save_database_menu_item = GTK_WIDGET(gtk_builder_get_object(builder_global, "save_database_menu_item"));
    if (!save_database_menu_item)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le widget 'save_database_menu_item'\n" COLOR_RESET);
        return 1;
    }
    g_signal_connect(save_database_menu_item, "activate", G_CALLBACK(on_save_database_menu_item_activate), NULL);

    open_database_menu_item = GTK_WIDGET(gtk_builder_get_object(builder_global, "open_database_menu_item"));
    if (!open_database_menu_item)
    {
        printf(COLOR_RED "Erreur : Impossible de trouver le widget 'open_database_menu_item'\n" COLOR_RESET);
        return 1;
    }
    g_signal_connect(open_database_menu_item, "activate", G_CALLBACK(on_open_database_menu_item_activate), NULL);

    // Connecte la croix de la fenêtre
    g_signal_connect(main_window, "destroy", G_CALLBACK(on_main_window_destroy), NULL);

    gtk_window_set_default_size(GTK_WINDOW(main_window), 600, 400);
    gtk_widget_show_all(main_window);
    gtk_main();

    return 0;
}