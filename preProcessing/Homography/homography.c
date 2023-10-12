#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "../SDL_Function/sdlFunction.h"

#include <err.h>

#include "../Pixel/pixel.h"
#include "../Corner_Finder/FindCorners.h"
#include "../Matrix/mat.h"

#include <stdio.h> // a enlever

#define FILL(mat, x, y) mat[x*8+y]

/***************************************************************
 *  Function Fill_Matrix: 
 *  
 *  Fonction to get the homographic transform matrix 
 *
 *  @input :
 *      - image (SDL_Surface*) : a surface from many filter
 *      - size (int) : size of the new image (grid -> square)
 *  @output :
 *      - matrix (double*) : homography transform matrix
***************************************************************/
double* Fill_Matrix(SDL_Surface* image, int size)
{
    //Get corner of the grid
    int* points = FindCoins(image);

    /*
    Fill and initialize the matrix :
    We want to map a point in rectangle A to rectangle B.
       for each point:
        {xA, yA, 1, 0, 0, 0, -xA*xB, -yA*xB}
        {0, 0, 0, xA, yA, 1, -xA*yB, -yA*yB}
    */
    double* mat = calloc(8*8, sizeof(double));

    //Row 1 / Point 1 -- upper left
    FILL( mat, 0, 2) = 1;
    FILL( mat, 1, 5) = 1;
    
    //Row 2 / Point 2 -- upper right
    FILL( mat, 2, 0) = size;
    FILL( mat, 2, 2) = 1;
    FILL( mat, 2, 6) = -points[6]*size;    
    FILL( mat, 3, 3) = size;    
    FILL( mat, 3, 5) = 1; 
    FILL( mat, 3, 6) = -points[7]*size; 
    
    //Row 3 / Point 3 -- lower left
    FILL( mat, 4,1) = size;
    FILL( mat, 4,2) = 1;
    FILL( mat, 4,7) = -points[2]*size;
    FILL( mat, 5,4) = size;
    FILL( mat, 5,5) = 1;
    FILL( mat, 5,7) = -points[3]*size;   

    //Row 4 / Point 4 -- lower right
    FILL( mat, 6,0) = size;
    FILL( mat, 6,1) = size;
    FILL( mat, 6,2) = 1;
    FILL( mat, 6,6) = -points[4]*size;
    FILL( mat, 6,7) = -points[5]*size;
    FILL( mat, 7,3) = size;
    FILL( mat, 7,4) = size;
    FILL( mat, 7,5) = 1;
    FILL( mat, 7,6) = -points[4]*size;
    FILL( mat, 7,7) = -points[5]*size;

   //Make the matrix with each corner of the grid 
    double corner_matrix[] = {
        (double)points[0],
        (double)points[1],
        (double)points[6],
        (double)points[7],
        (double)points[2],
        (double)points[3],
        (double)points[4],
        (double)points[5],
    };
    
    /*
    We are finding the so called H matrix. I am using this formula:
        H = ( M1 transpose * M1 )inv * ( M1 transpose * M2)
    */ 

    //Create the transpose matrix
    double* transpose_mat = calloc(8*8, sizeof(double));
    //Get the transpose matrix from mat
    Transpose_Mat(mat, transpose_mat, 8, 8);

    //Get the multiplication with transpose_mat and mat
    double* mult_mat = calloc(8*8, sizeof(double));
    Mult_Mat(transpose_mat, mat, 8, 8, 8, mult_mat);

    //Free mat because we don't need it anymore
    free(mat);

    //Check whether the matrix is reversible and get the result or show a error
    double* inverse_mat = calloc(8*8, sizeof(double));
    if (!Inverse_Mat(mult_mat, inverse_mat, 8))
    {
        errx(1, "Matrix is singular");
    }

    //Free mult_mat because we don't need it anymore
    free(mult_mat);

    //Get the multiplication with invers_mat and transpose_mat
    double* mat_mult = calloc(8*8, sizeof(double));
    Mult_Mat(inverse_mat, transpose_mat, 8, 8, 8, mat_mult);

    //Free inverse_mat and transpose_mat because we don't need it anymore
    free(inverse_mat);
    free(transpose_mat);

    //Create the matrix that'll be returned
    double* result = calloc(8, sizeof(double));

    Mult_Mat(mat_mult, corner_matrix, 8, 8, 1, result);
    //Free mat_mult because we don't need it anymore
    free(mat_mult);

    //return the homography transform matrix    
    return result;
}


/***************************************************************
 *  Function Homography_Transform: 
 *  
 *  Fonction to get a perfect image of the grid from the old image 
 *  after the blob filter
 *
 *  @input :
 *      - image (SDL_Surface*) : a surface from many filter
 *      - size (int) : size of the new image (grid -> square)
 *  @output :
 *      - image (SDL_Surface*) : transformed image homographicely
***************************************************************/
SDL_Surface* Homography_Transform(SDL_Surface* image, int size)
{
    //Get the homography transform matrix
    double* mat = Fill_Matrix(image, size); 

    //Create a new surface to draw the homographic transformation from image
    SDL_Surface* new_img = SDL_CreateRGBSurface(0, size, size, 32, 0, 0, 0, 0);

    //looping on each pixel from the image
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            //Get the new position of the pixel
            double x = (mat[0] * i + mat[1] * j + mat[2])/(mat[6] * i + mat[7] * j + 1);
            double y = (mat[3] * i + mat[4] * j + mat[5])/(mat[6] * i + mat[7] * j + 1);

            //Put the pixel from the image to the new if it is into it
            if (x >= 0 && x < image->w && y >= 0 && y < image->h)
            {
                Uint32 pixel = getPixel32_t(image, x, y);
                putPixel(new_img, i, j, pixel);
            }
        }
    }

    return new_img;
}