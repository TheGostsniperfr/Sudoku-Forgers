#include <gtk/gtk.h>
#include <stdlib.h>
#include <ctype.h>
#include "preProcessing/Rotate/rotate.h"
#include "preProcessing/SDL_Function/sdlFunction.h"
#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"


#define EDIT_GRID_SIZE 9

typedef struct DataApp {
    char* originalImgPath;
    gint stepProcess;
    gdouble rotateAngle;
    gint currentPage;

    GtkStack* pageContainer;

    GtkImage* logoImg;
    GtkImage* inputImageResult;
    GtkImage* rotateImg;
    GtkImage* matrixGrid;
    GtkScale* rotateSlider;
    GtkScale* pageSlider;

    GtkEntry*** editGridMat;
} DataApp;

typedef struct {
    int row;
    int col;
} EntryCoordinates;

void pageManager(DataApp* dataApp, gint newNbPage);

gboolean is_digit(const gchar *text) {
    return (text != NULL && *text != '\0' && isdigit(*text));
}

GdkPixbuf *convertSurfaceToPixbuf(SDL_Surface *sdlSurface, int sizeX, int sizeY) {
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_data(
        (const guchar *)sdlSurface->pixels,
        GDK_COLORSPACE_RGB,
        TRUE,
        8,
        sdlSurface->w,
        sdlSurface->h,
        sdlSurface->pitch,
        NULL,
        NULL
    );

    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);

    double aspect_ratio = (double)width / (double)height;

    int new_width, new_height;

    if (width > height)
    {
        new_width = sizeX;
        new_height = (int)(sizeX / aspect_ratio);
    }
    else
    {
        new_height = sizeY;
        new_width = (int)(sizeY * aspect_ratio);
    }

    GdkPixbuf *resize = gdk_pixbuf_scale_simple(pixbuf, new_width, new_height, GDK_INTERP_BILINEAR);
    g_object_unref(pixbuf);
    return resize;
}

void pageChanger(DataApp* dataApp, gint newNbPage){
    g_print("New page : %d\n", newNbPage);

    dataApp->currentPage = newNbPage;

    gtk_range_set_value(GTK_RANGE(dataApp->pageSlider), CLAMP(newNbPage, 1, 6));

    gtk_stack_set_visible_child_name
    (
        dataApp->pageContainer,
        g_strdup_printf("page%d", newNbPage)
    );
}

void load_and_resize_image(const char *filename, int sizeX, int sizeY, GtkImage *img)
{
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);

    if (pixbuf == NULL)
    {
        g_printerr("Error loading image: %s\n", filename);
        return;
    }

    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);

    double aspect_ratio = (double)width / (double)height;

    int new_width, new_height;

    if (width > height)
    {
        new_width = sizeX;
        new_height = (int)(sizeX / aspect_ratio);
    }
    else
    {
        new_height = sizeY;
        new_width = (int)(sizeY * aspect_ratio);
    }

    GdkPixbuf *resize = gdk_pixbuf_scale_simple(pixbuf, new_width, new_height, GDK_INTERP_BILINEAR);

    gtk_image_set_from_pixbuf(img, resize);

    g_object_unref(pixbuf);
    g_object_unref(resize);
}


void on_file_selected(GtkFileChooserButton *filechooserbutton,
    gpointer user_data)
{
    DataApp* appData = user_data;

    appData->originalImgPath =
        gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooserbutton));
}



//reset app for clean to load a new image
void resetApp(DataApp* dataApp){
    dataApp->stepProcess = 1;

    if (dataApp->originalImgPath != NULL) {
        g_free((gpointer)dataApp->originalImgPath);
    }

    for (int i = 0; i < EDIT_GRID_SIZE; i++) {
        free(dataApp->editGridMat[i]);
    }
    free(dataApp->editGridMat);
}

void on_rotate_slider_changed(GtkRange *range , gpointer user_data){
    DataApp* dataApp = user_data;

    dataApp->rotateAngle = gtk_range_get_value(range);

    g_print("Rotate slider val : %f\n", dataApp->rotateAngle);

    //rotate img
    if(dataApp->rotateImg == NULL){
        g_printerr("No rotate image load.");
        return;
    }

    gtk_image_set_from_pixbuf(dataApp->rotateImg, convertSurfaceToPixbuf(Rotated_image(loadImg(dataApp->originalImgPath), dataApp->rotateAngle), 350, 350));
}

void on_page_slider_changed(GtkRange *range , gpointer user_data) {
    DataApp* dataApp = user_data;
    gint value = gtk_range_get_value(range);
    pageManager(dataApp, value);
}


void pageManager(DataApp* dataApp, gint newNbPage){
    printf("Try to go page nb : %d\n", newNbPage);

    if(newNbPage < 1 || newNbPage > 6){
        return;
    }

    if(newNbPage < dataApp->stepProcess){
        pageChanger(dataApp, newNbPage);
        return;
    }

    printf("newNbPage = %d\n", newNbPage);
    printf("stepProcess = %d\n", dataApp->stepProcess);

    if(newNbPage >= dataApp->stepProcess){
        //check if all the condition to go the next page is fill
        switch (newNbPage)
        {
        case 2:

            if(dataApp->originalImgPath == NULL){
                return;
            }

            dataApp->stepProcess = 2;

            //Rotate page
            //load image from page 1 (imported image)

            if(dataApp->rotateImg== NULL){
                dataApp->rotateImg = GTK_IMAGE(gtk_image_new());
            }

            load_and_resize_image(dataApp->originalImgPath, 350, 350, dataApp->rotateImg);
            pageChanger(dataApp, newNbPage);

            return;
        case 3:
            //apply rotate
            SDL_Surface* sdlRotateImg = loadImg(dataApp->originalImgPath);
            gtk_image_set_from_pixbuf(dataApp->rotateImg, convertSurfaceToPixbuf(sdlRotateImg, 350, 350));

            //Intermediate step page
            //make all step + find all digits + create grid struct

            dataApp->stepProcess = 3;
            pageChanger(dataApp, newNbPage);
            return;

        case 4:
            //Edit grid page
            //load image from page 1 (imported image)
            //init edit grid with grid struct

            dataApp->stepProcess = 4;

            load_and_resize_image(dataApp->originalImgPath, 300, 300, dataApp->inputImageResult);
            pageChanger(dataApp, newNbPage);
            return;




        default:
            break;
        }
    }
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



void on_back_btn_pressed(GtkButton *button __attribute__((unused)),
                            gpointer user_data)
{
    DataApp* dataApp = user_data;
    gdouble value = gtk_range_get_value(GTK_RANGE(dataApp->pageSlider));
    pageManager(dataApp, value-1);
}

void on_forward_btn_pressed(GtkButton *button __attribute__((unused)),
                                gpointer user_data)
{
    DataApp* dataApp = user_data;
    gdouble value = gtk_range_get_value(GTK_RANGE(dataApp->pageSlider));
    pageManager(dataApp, value+1);
}


int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    gtk_init(NULL, NULL);

    GtkBuilder *builder = gtk_builder_new();

    // Load UI
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "main.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return EXIT_FAILURE;
    }

    DataApp* dataApp = calloc(1, sizeof(DataApp));


    dataApp->stepProcess = 1;
    dataApp->currentPage = 1;
    dataApp->originalImgPath = NULL;
    dataApp->rotateAngle = 0;


    dataApp->editGridMat = calloc(EDIT_GRID_SIZE, sizeof(GtkEntry**));

    for (int i = 0; i < EDIT_GRID_SIZE; i++)
    {
        dataApp->editGridMat[i] = calloc(EDIT_GRID_SIZE, sizeof(GtkEntry*));
    }

    GtkWidget *window =
        GTK_WIDGET(gtk_builder_get_object(builder, "SF_APP_WINDOW"));
    dataApp->pageContainer =
        GTK_STACK(gtk_builder_get_object(builder, "PageContainer"));
    dataApp->pageSlider =
        GTK_SCALE(gtk_builder_get_object(builder, "page_slider"));

    GtkButton* backBtn =
        GTK_BUTTON(gtk_builder_get_object(builder, "backBtn"));
    GtkButton* forwardBtn =
        GTK_BUTTON(gtk_builder_get_object(builder, "forwardBtn"));

    GtkFileChooserButton* fileChooseBtn =
        GTK_FILE_CHOOSER_BUTTON(
            gtk_builder_get_object(builder, "chooseFileBtn"));

    dataApp->logoImg =
        GTK_IMAGE(gtk_builder_get_object(builder, "logo"));

    dataApp->inputImageResult =
        GTK_IMAGE(gtk_builder_get_object(builder, "inputImageResult"));
    dataApp->rotateImg =
        GTK_IMAGE(gtk_builder_get_object(builder, "rotateImg"));
    dataApp->rotateSlider =
        GTK_SCALE(gtk_builder_get_object(builder, "rotateSlider"));

    GtkGrid* matrixGrid =
        GTK_GRID(gtk_builder_get_object(builder, "editGrid"));

    load_and_resize_image("ressources/logo.png", 300, 300, dataApp->logoImg);

    //Page Slider

    gtk_range_set_range(GTK_RANGE(dataApp->pageSlider), 1, 6);
    gtk_range_set_value(GTK_RANGE(dataApp->pageSlider), 1);
    /*
    g_signal_connect(dataApp->pageSlider, "value_changed",
        G_CALLBACK(on_page_slider_changed), dataApp);
    */

    //Rotate Slider
    gtk_range_set_range(GTK_RANGE(dataApp->rotateSlider), -180, 180);
    gtk_range_set_value(GTK_RANGE(dataApp->rotateSlider), 0);
    g_signal_connect(dataApp->rotateSlider, "value_changed",
        G_CALLBACK(on_rotate_slider_changed), dataApp);

    //Page Btn
    g_signal_connect(backBtn, "clicked",
        G_CALLBACK(on_back_btn_pressed), dataApp);
    g_signal_connect(forwardBtn, "clicked",
        G_CALLBACK(on_forward_btn_pressed), dataApp);

    //File chooser btn
    g_signal_connect(fileChooseBtn, "selection-changed",
        G_CALLBACK(on_file_selected), dataApp);

    //grid manager
    for (int i = 0; i < EDIT_GRID_SIZE; i++) {
        for (int j = 0; j < EDIT_GRID_SIZE; j++) {
            dataApp->editGridMat[i][j] = GTK_ENTRY(gtk_entry_new());
            gtk_grid_attach(matrixGrid,
                GTK_WIDGET(dataApp->editGridMat[i][j]), j, i, 1, 1);

            gtk_entry_set_input_purpose
                (dataApp->editGridMat[i][j], GTK_INPUT_PURPOSE_DIGITS);
            gtk_entry_set_max_length(dataApp->editGridMat[i][j], 1);
            gtk_entry_set_width_chars(dataApp->editGridMat[i][j], 1);
            gtk_entry_set_max_width_chars(dataApp->editGridMat[i][j], 2);

            EntryCoordinates *coords = g_new(EntryCoordinates, 1);
            coords->row = i;
            coords->col = j;

            g_signal_connect(G_OBJECT(dataApp->editGridMat[i][j]),
                "insert-text", G_CALLBACK(on_insert_text), coords);

        }
    }



    gtk_widget_show_all(GTK_WIDGET(matrixGrid));

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);


    gtk_main();

    resetApp(dataApp);

    free(dataApp);
    return EXIT_SUCCESS;
}