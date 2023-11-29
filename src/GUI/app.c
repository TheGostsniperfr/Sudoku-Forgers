#include <gtk/gtk.h>
#include <stdlib.h>
#include <ctype.h>

#define EDIT_GRID_SIZE 9
GdkPixbuf *originalImg = NULL;

GtkEntry *editGridMat[EDIT_GRID_SIZE][EDIT_GRID_SIZE];
typedef struct {
    int row;
    int col;
} EntryCoordinates;

gboolean is_digit(const gchar *text) {
    return (text != NULL && *text != '\0' && isdigit(*text));
}

void on_file_selected(GtkFileChooserButton *filechooserbutton,
    gpointer user_data)
{
    const gchar *filename =
        gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooserbutton));

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);

    if (originalImg != NULL) {
        g_object_unref(originalImg);
    }
    originalImg = pixbuf;

    GtkWidget *image = GTK_WIDGET(user_data);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);

    g_free((gpointer)filename);
}

void on_scale_value_changed(GtkRange *range , gpointer user_data) {
    GtkStack *pageContainer = GTK_STACK(user_data);
    gdouble value = gtk_range_get_value(range);

    gtk_stack_set_visible_child_name
    (
        pageContainer,
        g_strdup_printf("page%.0f", value)
    );
}

void on_insert_text(GtkEditable *editable, const gchar *text,
    gint length __attribute__((unused)),
    gint *position __attribute__((unused)), gpointer user_data)
{
    GtkEntry *entry = GTK_ENTRY(editable);
    EntryCoordinates *coords = (EntryCoordinates *)user_data;
    if (!is_digit(text)) {
        g_signal_stop_emission_by_name(G_OBJECT(editable), "insert-text");
        return;
    }

    g_print("Input edit grid [%d][%d] : %s\n",
        coords->row, coords->col, text);

    (void)entry;
}

void load_and_resize_image(const char *filename, int sizeX,
    int sizeY, GtkImage *img)
{
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);

    if (pixbuf == NULL) {
        g_printerr("Error to load image: %s\n", filename);
        return;
    }

    GdkPixbuf *resized_pixbuf = gdk_pixbuf_scale_simple(pixbuf, sizeX,
        sizeY, GDK_INTERP_BILINEAR);

    g_object_unref(pixbuf);
    gtk_image_set_from_pixbuf(img, resized_pixbuf);
    g_object_unref(resized_pixbuf);
}

void on_back_btn_pressed(GtkButton *button __attribute__((unused)),
                            gpointer user_data)
{
    GtkScale* pageSlider = GTK_SCALE(user_data);
    gdouble value = gtk_range_get_value(GTK_RANGE(pageSlider));
    gtk_range_set_value(GTK_RANGE(pageSlider), CLAMP(value-1, 1, 6));
}

void on_forward_btn_pressed(GtkButton *button __attribute__((unused)),
                                gpointer user_data)
{
    GtkScale* pageSlider = GTK_SCALE(user_data);
    gdouble value = gtk_range_get_value(GTK_RANGE(pageSlider));
    gtk_range_set_value(GTK_RANGE(pageSlider), CLAMP(value+1, 1, 6));
}


int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    gtk_init(NULL,  NULL);

    GtkBuilder *builder = gtk_builder_new();

    // Load UI
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "main.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return EXIT_FAILURE;
    }


    GtkWidget *window =
        GTK_WIDGET(gtk_builder_get_object(builder, "SF_APP_WINDOW"));
    GtkStack* pageContainer =
        GTK_STACK(gtk_builder_get_object(builder, "PageContainer"));
    GtkScale* pageSlider =
        GTK_SCALE(gtk_builder_get_object(builder, "page_slider"));
    GtkButton* backBtn =
        GTK_BUTTON(gtk_builder_get_object(builder, "backBtn"));
    GtkButton* forwardBtn =
        GTK_BUTTON(gtk_builder_get_object(builder, "forwardBtn"));

    GtkFileChooserButton* fileChooseBtn =
        GTK_FILE_CHOOSER_BUTTON(
            gtk_builder_get_object(builder, "chooseFileBtn"));

    GtkImage* logoImg =
        GTK_IMAGE(gtk_builder_get_object(builder, "logo"));
    GtkImage* inputImageResult =
        GTK_IMAGE(gtk_builder_get_object(builder, "inputImageResult"));
    GtkGrid* matrixGrid =
        GTK_GRID(gtk_builder_get_object(builder, "editGrid"));

    load_and_resize_image("ressources/logo.png", 300, 300, logoImg);
    load_and_resize_image
        ("ressources/emptyGrid.png", 300, 300, inputImageResult);

    //Page Slider
    gtk_range_set_range(GTK_RANGE(pageSlider), 1, 6);
    gtk_range_set_value(GTK_RANGE(pageSlider), 1);
    g_signal_connect(pageSlider, "value_changed",
        G_CALLBACK(on_scale_value_changed), pageContainer);

    //Page Btn
    g_signal_connect(backBtn, "clicked",
        G_CALLBACK(on_back_btn_pressed), pageSlider);
    g_signal_connect(forwardBtn, "clicked",
        G_CALLBACK(on_forward_btn_pressed), pageSlider);

    //File chooser btn
    g_signal_connect(fileChooseBtn, "selection-changed",
        G_CALLBACK(on_file_selected), inputImageResult);

    //grid manager
    for (int i = 0; i < EDIT_GRID_SIZE; i++) {
        for (int j = 0; j < EDIT_GRID_SIZE; j++) {
            editGridMat[i][j] = GTK_ENTRY(gtk_entry_new());
            gtk_grid_attach(matrixGrid,
                GTK_WIDGET(editGridMat[i][j]), j, i, 1, 1);

            gtk_entry_set_input_purpose
                (editGridMat[i][j], GTK_INPUT_PURPOSE_DIGITS);
            gtk_entry_set_max_length(editGridMat[i][j], 1);
            gtk_entry_set_width_chars(editGridMat[i][j], 1);
            gtk_entry_set_max_width_chars(editGridMat[i][j], 2);

            EntryCoordinates *coords = g_new(EntryCoordinates, 1);
            coords->row = i;
            coords->col = j;

            g_signal_connect(G_OBJECT(editGridMat[i][j]),
                "insert-text", G_CALLBACK(on_insert_text), coords);

        }
    }



    gtk_widget_show_all(GTK_WIDGET(matrixGrid));

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_main();

    if (originalImg != NULL) {
        g_object_unref(originalImg);
    }

    return EXIT_SUCCESS;
}