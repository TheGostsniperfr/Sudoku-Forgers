#include <gtk/gtk.h>
#include <stdlib.h>

void on_scale_value_changed(GtkRange *range , gpointer user_data) {
    GtkStack *pageContainer = GTK_STACK(user_data);
    gdouble value = gtk_range_get_value(range);

    gtk_stack_set_visible_child_name
    (
        pageContainer,
        g_strdup_printf("page%.0f", value)
    );
}

void load_and_resize_image(const char *filename, int sizeX, int sizeY, GtkImage *img) {
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);

    if (pixbuf == NULL) {
        g_printerr("Error to load image: %s\n", filename);
        return;
    }

    GdkPixbuf *resized_pixbuf = gdk_pixbuf_scale_simple(pixbuf, sizeX, sizeY, GDK_INTERP_BILINEAR);
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


int main() {

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

    GtkImage* logoImg = GTK_IMAGE(gtk_builder_get_object(builder, "logo"));

    GtkImage* inputImageResult = GTK_IMAGE(gtk_builder_get_object(builder, "inputImageResult"));
    GtkImage* outputImageResult = GTK_IMAGE(gtk_builder_get_object(builder, "outputImageResult"));

    load_and_resize_image("ressources/logo.png", 300, 300, logoImg);
    load_and_resize_image("ressources/emptyGrid.png", 300, 300, inputImageResult);
    load_and_resize_image("ressources/emptyGrid.png", 300, 300, outputImageResult);

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

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_main();

    return EXIT_SUCCESS;
}