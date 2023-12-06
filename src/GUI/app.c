#include <gtk/gtk.h>
#include <stdlib.h>
#include <ctype.h>
#include "preProcessing/Rotate/rotate.h"
#include "preProcessing/SDL_Function/sdlFunction.h"
#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "neuralNetwork/network_Utils/struct.h"
#include "sudokuSolver/sudokuAux/sudokuUtil.h"
#include "preProcessing/preProcessingAux/preProcessingHandle.h"
#include "neuralNetwork/network_Utils/networkUtils.h"
#include "GUI/handleUtils.h"
#include "sudokuSolver/sudokuSolver/sudoku_solver.h"
#include "sudokuSolver/outputGrid/createOutputGrid.h"
#include "neuralNetwork/network_Utils/networkHandle.h"




#define NB_FLAGS 5
#define GRID_DIM 9
#define DEFAULT_GRID_DIR "sudoku_grid"

#define EDIT_GRID_SIZE 9

typedef struct DataApp {
    char* originalImgPath;
    gint stepProcess;
    gdouble rotateAngle;
    gint currentPage;

    AllStepResult* allStepResult;
    SudokuGrid sG;
    Flag* flags;

    GtkStack* pageContainer;

    GtkImage* logoImg;
    GtkImage* inputImageResult;
    GtkImage* rotateImg;
    GtkImage* matrixGrid;
    GtkImage* intermediateImg;
    GtkImage* outImg;

    GtkScale* rotateSlider;
    GtkScale* pageSlider;

    GtkFileChooserButton* fileChooser;

    GtkEntry*** editGridMat;
} DataApp;

typedef struct {
    int row;
    int col;
} EntryCoordinates;

void pageManager(DataApp* dataApp, gint newNbPage);
void resetApp(DataApp* dataApp);

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
    //SDL_FreeSurface(sdlSurface);

    GdkPixbuf *resize = gdk_pixbuf_scale_simple(pixbuf, new_width, new_height, GDK_INTERP_BILINEAR);
    g_object_unref(pixbuf);
    return resize;
}



void pageChanger(DataApp* dataApp, gint newNbPage){

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
    DataApp* dataApp = user_data;

    if(dataApp->originalImgPath == NULL){
        dataApp->originalImgPath =
            gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooserbutton));
    }else{
        resetApp(dataApp);
        dataApp->originalImgPath =
            gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooserbutton));
    }
}



//reset app for clean to load a new image
void resetApp(DataApp* dataApp){
    dataApp->stepProcess = 1;
    dataApp->stepProcess = 1;
    dataApp->originalImgPath = NULL;
    dataApp->rotateAngle = 0;

    for (int i = 0; i < EDIT_GRID_SIZE; i++) {
        for (int j = 0; j < EDIT_GRID_SIZE; j++) {
            gtk_entry_set_text(dataApp->editGridMat[i][j], "");
        }
    }

    pageChanger(dataApp, 1);
}





void on_rotate_slider_changed(GtkRange *range , gpointer user_data){
    DataApp* dataApp = user_data;

    dataApp->rotateAngle = gtk_range_get_value(range);

    dataApp->stepProcess = 2;

    //rotate img
    if(dataApp->rotateImg == NULL){
        g_printerr("No rotate image load.");
        return;
    }

    saveImg(Rotated_image(loadImg(dataApp->originalImgPath), dataApp->rotateAngle), "src/GUI/tmp/rotatedImg.jpg");
    gtk_image_set_from_pixbuf(dataApp->rotateImg, convertSurfaceToPixbuf(loadImg("src/GUI/tmp/rotatedImg.jpg"), 350, 350));
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

    if(newNbPage <= dataApp->stepProcess){
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

            //apply rotate

            saveImg(Rotated_image(loadImg(dataApp->originalImgPath), dataApp->rotateAngle), "src/GUI/tmp/rotatedImg.jpg");
            gtk_image_set_from_pixbuf(dataApp->rotateImg, convertSurfaceToPixbuf(loadImg("src/GUI/tmp/rotatedImg.jpg"), 350, 350));

            //Go to rotate page
            pageChanger(dataApp, newNbPage);

            return;
        case 3:
            //Intermediate step page
            dataApp->stepProcess = 3;



            //make all step + find all digits + create grid struct
            saveImg(Rotated_image(loadImg(dataApp->originalImgPath), dataApp->rotateAngle), "src/GUI/tmp/rotatedImg.jpg");
            dataApp->allStepResult = (AllStepResult*)handleAllSteps(0, NULL, "src/GUI/tmp/rotatedImg.jpg", dataApp->flags);
            dataApp->allStepResult->gridCells = findAllDigits(dataApp->allStepResult->gridCells, GRID_DIM*GRID_DIM, dataApp->flags);

            dataApp->sG = gridCellToSudokuGrid(dataApp->allStepResult->gridCells, GRID_DIM);

            saveImg(dataApp->allStepResult->binarizedImg, "src/GUI/tmp/binarised.jpg");
            saveImg(dataApp->allStepResult->homographyImg, "src/GUI/tmp/homography.jpg");
            saveImg(dataApp->allStepResult->gridImg, "src/GUI/tmp/grid.jpg");
            gtk_image_set_from_pixbuf(dataApp->intermediateImg, convertSurfaceToPixbuf(loadImg("src/GUI/tmp/binarised.jpg"), 350, 350));




            pageChanger(dataApp, newNbPage);
            return;

        case 4:
            //Edit grid page
            dataApp->stepProcess = 4;

            //load image from page 1 (imported image)
            load_and_resize_image("src/GUI/tmp/homography.jpg", 350, 350, dataApp->inputImageResult);

            //init edit grid with grid struct
            for (int i = 0; i < EDIT_GRID_SIZE; i++) {
                for (int j = 0; j < EDIT_GRID_SIZE; j++) {
                    int val;
                    if(dataApp->allStepResult->gridCells[i*EDIT_GRID_SIZE + j].isDigit == 1){
                        val = dataApp->allStepResult->gridCells[i * EDIT_GRID_SIZE + j].label;
                        printf("I+J : %d\n", i+j);
                        char *text = g_strdup_printf("%d", val);
                        gtk_entry_set_text(dataApp->editGridMat[i][j], text);

                    }else{
                        gtk_entry_set_text(dataApp->editGridMat[i][j], "");
                    }

                }
            }





            pageChanger(dataApp, newNbPage);
            return;
        case 5:
            //solve sudoku grid :

            for (int i = 0; i < EDIT_GRID_SIZE; i++) {
                for (int j = 0; j < EDIT_GRID_SIZE; j++) {

                    int val = atoi(gtk_entry_get_text(dataApp->editGridMat[i][j]));

                    if(val == 0){
                        dataApp->sG.grid[i][j] = -1;
                    }else{
                        dataApp->sG.grid[i][j] = val;
                    }
                }
            }

            saveGrid("src/GUI/tmp/grid", dataApp->sG);

            printGrid(dataApp->sG);
            SudokuGrid sGSolved = loadGrid("src/GUI/tmp/grid");

            int valSolve = sudokuSolver(sGSolved);

            printf("val solve %d\n", valSolve);

            if(valSolve == EXIT_FAILURE){
                GtkWidget* msg = gtk_message_dialog_new_with_markup(
                    NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
                    "<b>Please correct your sudoku grid.</b>");

                gtk_window_set_title(GTK_WINDOW(msg), "Invalid Grid");
                g_signal_connect(GTK_DIALOG(msg), "response", G_CALLBACK(gtk_widget_destroy), NULL);
                gtk_dialog_run(GTK_DIALOG(msg));
                return;
            }

           // dataApp->stepProcess = 5;

            printGrid(dataApp->sG);
            printGrid(sGSolved);


            saveGrid("src/GUI/tmp/grid.result", sGSolved);

            //create and load out image
            GridPara gP = {
                .borderRatio = 0.3,
                .fontRatio = 0.7,
                .gridPxSize = 1000,
                .widthBorder = 15,
            };

            saveImg(createOutputGrid(dataApp->sG, sGSolved, gP), "src/GUI/tmp/outGrid.jpg");
            load_and_resize_image("src/GUI/tmp/outGrid.jpg", 350, 350, dataApp->outImg);
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






void on_new_btn_clicked(GtkButton *button __attribute__((unused)),
                                gpointer user_data)
{
    DataApp* dataApp = user_data;


    gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dataApp->fileChooser), "");
    resetApp(dataApp);
}





void on_switch_intermediate_img(GtkButton *button __attribute__((unused)),
                            gpointer user_data)
{
    DataApp* dataApp = user_data;

    char* idBtn = (char*)gtk_widget_get_name(GTK_WIDGET(button));

    if(strcmp(idBtn, "BinarizationBtn") == 0){
        load_and_resize_image("src/GUI/tmp/binarised.jpg", 350, 350, dataApp->intermediateImg);
    }else if (strcmp(idBtn, "HomographyBtn") == 0){
        load_and_resize_image("src/GUI/tmp/homography.jpg", 350, 350, dataApp->intermediateImg);
    }else if (strcmp(idBtn, "GridBtn") == 0){
        load_and_resize_image("src/GUI/tmp/grid.jpg", 350, 350, dataApp->intermediateImg);
    }else{
        g_printerr("Invalid image path.");
    }
}





void launchGUI() {
    gtk_init(NULL, NULL);

    GtkBuilder *builder = gtk_builder_new();

    // Load UI
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "src/GUI/main.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
    }

    DataApp* dataApp = calloc(1, sizeof(DataApp));


    dataApp->stepProcess = 1;
    dataApp->currentPage = 1;
    dataApp->originalImgPath = NULL;
    dataApp->rotateAngle = 0;

    //Init flags
    dataApp->flags = (Flag*)malloc(NB_FLAGS * sizeof(Flag));

    for (int i = 0; i < NB_FLAGS; i++)
    {
        dataApp->flags->flag = NULL;
        dataApp->flags->value = 0;
    }

    dataApp->flags[0].flag = "-verbose";
    dataApp->flags[1].flag = "-save";
    dataApp->flags[2].flag = "-p";
    dataApp->flags[3].flag = "-po";
    dataApp->flags[4].flag = "-g";


    dataApp->flags[0].value = 1;
    dataApp->flags[1].value = 1;
    dataApp->flags[2].value = 1;
    dataApp->flags[3].value = 1;
    dataApp->flags[4].value = 1;

    dataApp->editGridMat = calloc(EDIT_GRID_SIZE, sizeof(GtkEntry**));

    for (int i = 0; i < EDIT_GRID_SIZE; i++)
    {
        dataApp->editGridMat[i] = calloc(EDIT_GRID_SIZE, sizeof(GtkEntry*));
    }

    /*
    GtkWidget *window =
        GTK_WIDGET(gtk_builder_get_object(builder, "SF_APP_WINDOW"));*/
    dataApp->pageContainer =
        GTK_STACK(gtk_builder_get_object(builder, "PageContainer"));
    dataApp->pageSlider =
        GTK_SCALE(gtk_builder_get_object(builder, "page_slider"));

    GtkButton* backBtn =
        GTK_BUTTON(gtk_builder_get_object(builder, "backBtn"));
    GtkButton* forwardBtn =
        GTK_BUTTON(gtk_builder_get_object(builder, "forwardBtn"));

    GtkButton* BinarizationBtn =
        GTK_BUTTON(gtk_builder_get_object(builder, "BinarizationBtn"));
    GtkButton* HomographyBtn =
        GTK_BUTTON(gtk_builder_get_object(builder, "HomographyBtn"));
    GtkButton* GridBtn =
        GTK_BUTTON(gtk_builder_get_object(builder, "GridBtn"));
    dataApp->fileChooser =
        GTK_FILE_CHOOSER_BUTTON(
            gtk_builder_get_object(builder, "chooseFileBtn"));

    dataApp->logoImg =
        GTK_IMAGE(gtk_builder_get_object(builder, "logo"));

    dataApp->inputImageResult =
        GTK_IMAGE(gtk_builder_get_object(builder, "inputImageResult"));
    dataApp->rotateImg =
        GTK_IMAGE(gtk_builder_get_object(builder, "rotateImg"));
    dataApp->intermediateImg =
        GTK_IMAGE(gtk_builder_get_object(builder, "intermediateImg"));
    dataApp->outImg =
        GTK_IMAGE(gtk_builder_get_object(builder, "outImg"));
    dataApp->rotateSlider =
        GTK_SCALE(gtk_builder_get_object(builder, "rotateSlider"));

    GtkGrid* matrixGrid =
        GTK_GRID(gtk_builder_get_object(builder, "editGrid"));

    GtkButton* newBtn =
        GTK_BUTTON(gtk_builder_get_object(builder, "NewImgBtn"));


    GtkButton* closeBtn =
        GTK_BUTTON(gtk_builder_get_object(builder, "quitBtn"));

    load_and_resize_image("src/GUI/ressources/logo.png", 300, 300, dataApp->logoImg);

    //Page Slider

    gtk_range_set_range(GTK_RANGE(dataApp->pageSlider), 1, 5);
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
    g_signal_connect(dataApp->fileChooser, "selection-changed",
        G_CALLBACK(on_file_selected), dataApp);

    //Intermiate steps btn
    g_signal_connect(BinarizationBtn, "clicked",
        G_CALLBACK(on_switch_intermediate_img), dataApp);
    g_signal_connect(HomographyBtn, "clicked",
        G_CALLBACK(on_switch_intermediate_img), dataApp);
    g_signal_connect(GridBtn, "clicked",
        G_CALLBACK(on_switch_intermediate_img), dataApp);

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


    //result page
    g_signal_connect(newBtn, "clicked",
        G_CALLBACK(on_new_btn_clicked), dataApp);

    g_signal_connect(closeBtn, "clicked",
        G_CALLBACK(gtk_main_quit), NULL);

    //g_signal_connect(window, "clicked", G_CALLBACK(gtk_main_quit), NULL);


    gtk_main();
    if (dataApp->originalImgPath != NULL) {
        g_free((gpointer)dataApp->originalImgPath);
    }

    for (int i = 0; i < EDIT_GRID_SIZE; i++) {
        free(dataApp->editGridMat[i]);
    }
    free(dataApp->editGridMat);
    free(dataApp);
}
