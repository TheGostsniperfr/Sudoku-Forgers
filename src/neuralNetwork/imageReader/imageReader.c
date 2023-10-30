#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <err.h>

// Fonction pour lire un fichier IDX3-UBYTE et extraire les images
void lireMNISTImages(const char *nomFichier, uint8_t **images, int *nombreImages, int *largeurImage, int *hauteurImage) {
    FILE *fichier = fopen(nomFichier, "rb");

    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", nomFichier);
        exit(1);
    }

    uint32_t magic_number, num_images, num_rows, num_cols;

    // Lire le magic number et les informations sur les images
    fread(&magic_number, sizeof(magic_number), 1, fichier);
    fread(&num_images, sizeof(num_images), 1, fichier);
    fread(&num_rows, sizeof(num_rows), 1, fichier);
    fread(&num_cols, sizeof(num_cols), 1, fichier);

    // Inverser les octets si nécessaire (MNIST utilise un format big-endian)
    magic_number = ((magic_number & 0xFF000000) >> 24) | ((magic_number & 0x00FF0000) >> 8) |
                   ((magic_number & 0x0000FF00) << 8) | ((magic_number & 0x000000FF) << 24);

    num_images = ((num_images & 0xFF000000) >> 24) | ((num_images & 0x00FF0000) >> 8) |
                 ((num_images & 0x0000FF00) << 8) | ((num_images & 0x000000FF) << 24);

    num_rows = ((num_rows & 0xFF000000) >> 24) | ((num_rows & 0x00FF0000) >> 8) |
               ((num_rows & 0x0000FF00) << 8) | ((num_rows & 0x000000FF) << 24);

    num_cols = ((num_cols & 0xFF000000) >> 24) | ((num_cols & 0x00FF0000) >> 8) |
               ((num_cols & 0x0000FF00) << 8) | ((num_cols & 0x000000FF) << 24);

    *nombreImages = num_images;
    *largeurImage = num_cols;
    *hauteurImage = num_rows;

    // Allouer de la mémoire pour stocker les images
    uint8_t *buffer = (uint8_t *)malloc(num_images * num_rows * num_cols);
    fread(buffer, sizeof(uint8_t), num_images * num_rows * num_cols, fichier);
    fclose(fichier);

    *images = buffer;
}

// Fonction pour lire un fichier IDX1-UBYTE et extraire les étiquettes
void lireMNISTLabels(const char *nomFichier, uint8_t **labels, int *nombreLabels) {
    FILE *fichier = fopen(nomFichier, "rb");

    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", nomFichier);
        exit(1);
    }

    uint32_t magic_number, num_labels;

    // Lire le magic number et le nombre d'étiquettes
    fread(&magic_number, sizeof(magic_number), 1, fichier);
    fread(&num_labels, sizeof(num_labels), 1, fichier);

    // Inverser les octets si nécessaire (MNIST utilise un format big-endian)
    magic_number = ((magic_number & 0xFF000000) >> 24) | ((magic_number & 0x00FF0000) >> 8) |
                   ((magic_number & 0x0000FF00) << 8) | ((magic_number & 0x000000FF) << 24);

    num_labels = ((num_labels & 0xFF000000) >> 24) | ((num_labels & 0x00FF0000) >> 8) |
                 ((num_labels & 0x0000FF00) << 8) | ((num_labels & 0x000000FF) << 24);

    *nombreLabels = num_labels;

    // Allouer de la mémoire pour stocker les étiquettes
    uint8_t *buffer = (uint8_t *)malloc(num_labels);
    fread(buffer, sizeof(uint8_t), num_labels, fichier);
    fclose(fichier);

    *labels = buffer;
}

/*
// Fonction pour afficher l'image n à l'aide de GTK
void afficherImageGTK(uint8_t *images, int n, int largeurImage, int hauteurImage) {
    GtkWidget *fenetre;
    GtkWidget *image;

    gtk_init(NULL, NULL);

    fenetre = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(fenetre), largeurImage, hauteurImage);
    gtk_window_set_title(GTK_WINDOW(fenetre), "Image MNIST");

    g_signal_connect(fenetre, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    image = gtk_image_new();
    gtk_container_add(GTK_CONTAINER(fenetre), image);

    // Créer une surface de type RGB pour l'image
    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, largeurImage, hauteurImage);
    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);

    // Copier les données de l'image MNIST dans la surface
    for (int i = 0; i < largeurImage * hauteurImage; i++) {
        pixels[i * 3] = 255 - images[n * largeurImage * hauteurImage + i];
        pixels[i * 3 + 1] = 255 - images[n * largeurImage * hauteurImage + i];
        pixels[i * 3 + 2] = 255 - images[n * largeurImage * hauteurImage + i];
    }

    // Mettre à jour l'image
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
    gtk_widget_show_all(fenetre);

    gtk_main();
}*/


void afficherValeursPixels(uint8_t *images, int n, int largeurImage, int hauteurImage) {
    // Obtenir l'image n à partir des données MNIST
    uint8_t *image = images + n * largeurImage * hauteurImage;

    for (int y = 0; y < hauteurImage; y++) {
        for (int x = 0; x < largeurImage; x++) {
            // Obtenir la valeur du pixel à la position (x, y)
            uint8_t valeurPixel = image[y * largeurImage + x];

            // Normaliser la valeur du pixel entre 0 et 1 (0 = noir, 255 = blanc)
            float valeurNormalisee = (float)valeurPixel / 255.0;

            // Afficher la valeur normalisée du pixel
            printf("%.2f ", valeurNormalisee);
        }
        printf("\n");
    }
}

void GetImage(uint8_t **image, uint8_t *label, int *imageRes, int n) {
    const char *nomFichierImages = "../data/train-images.idx3-ubyte";
    const char *nomFichierLabels = "../data/train-labels.idx1-ubyte";
    uint8_t *images, *labels;
    int nombreImages, largeurImage, hauteurImage, nombreLabels;

    lireMNISTImages(nomFichierImages, &images, &nombreImages, &largeurImage, &hauteurImage);
    lireMNISTLabels(nomFichierLabels, &labels, &nombreLabels);

    *label = labels[n];
    *image = images + n * largeurImage * hauteurImage;
    *imageRes = largeurImage;

    free(images);
    free(labels);
}

void GetImages(uint8_t ***images, uint8_t **labels, int *imageRes, int *nbImages) {
    const char *nomFichierImages = "../data/train-images.idx3-ubyte";
    const char *nomFichierLabels = "../data/train-labels.idx1-ubyte";
    uint8_t *_images, *_labels;
    int nombreImages, largeurImage, hauteurImage, nombreLabels;

    lireMNISTImages(nomFichierImages, &_images, &nombreImages, &largeurImage, &hauteurImage);
    lireMNISTLabels(nomFichierLabels, &_labels, &nombreLabels);

    *images = (uint8_t **)malloc(nombreImages * sizeof(uint8_t *));
    if (*images == NULL) {
        perror("Erreur d'allocation de mémoire pour les images");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nombreImages; i++) {
        (*images)[i] = _images + i * largeurImage * hauteurImage;
    }

    *labels = _labels;
    *imageRes = largeurImage;
    *nbImages = nombreImages;
}




/*
int main(int argc, char *argv[]) {

    if(argc != 2){
        errx(1, "Usage : %s : <nb>", argv[0]);
        return 1;
    }

    const char *nomFichierImages = "data/train-images.idx3-ubyte";
    const char *nomFichierLabels = "data/train-labels.idx1-ubyte";
    uint8_t *images, *labels;
    int nombreImages, largeurImage, hauteurImage, nombreLabels;

    lireMNISTImages(nomFichierImages, &images, &nombreImages, &largeurImage, &hauteurImage);
    lireMNISTLabels(nomFichierLabels, &labels, &nombreLabels);

    int indiceImageAVisualiser = atoi(argv[1]); // Indice de l'image à afficher

    //afficherValeursPixels(images, indiceImageAVisualiser, largeurImage, hauteurImage);
    // N'oubliez pas de libérer la mémoire une fois que vous avez terminé


    uint8_t label = labels[indiceImageAVisualiser];
    printf("Label de l'image à l'indice %d : %d\n", indiceImageAVisualiser, label);

    afficherImageGTK(images, indiceImageAVisualiser, largeurImage, hauteurImage);


    free(images);
    free(labels);

    return 0;
}

*/

