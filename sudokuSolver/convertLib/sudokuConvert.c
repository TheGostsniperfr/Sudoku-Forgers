#include <stdio.h>
#include <err.h>

/***************************************************************
 *  Function charToInt: 
 *
 *  A simple function to convert char hexa character 
 *  to int representation
 *
 *  @input :
 *      - str (char) : character to convert
 *
 *  @output :
 *      - (int) : int hexa representation
***************************************************************/
int charToInt(char str){
    if(str >= '0' && str <= '9'){
        return str - '0';
    }

    if(str >= 'A' && str <= 'F'){
        return str - 'A' + 10;
    }

    if(str >= 'a' && str <= 'f'){
        return str - 'a' + 10;
    }

    return -1;
}


/***************************************************************
 *  Function intToChar: 
 *
 *  A simple function to convert int to hexa character 
 *  to int representation
 *
 *  @input :
 *      - nb (int) : number to convert
 *
 *  @output :
 *      - (char) : Character hexa representation
***************************************************************/
char intToChar(int nb){
    if(nb >= 0 && nb <= 9){
        return ('0' + nb);
    }

    if(nb >= 10 && nb <= 15){
        return ('A' + nb - 10);
    }

    return -1;
}


/***************************************************************
 *  Function gridSize: 
 *
 *  Return the size of the grid, either 9 or 16 (Hexa grid)
 *
 *  @input :
 *      - str (char) : character to convert
 *
 *  @output :
 *      - (int) : Character hexa representation
***************************************************************/
int gridSize(const char* filename){
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        errx(1, "Error : gridSize can't open %s.\n", filename);
        return -1;
    }

    int count = 0;
    int character;

    while ((character = fgetc(file)) != EOF && character != '\n') {
        count++;
    }

    fclose(file);

    if(count != 9 && count != 16){
        printf("Value count %d\n", count);
        errx(1, "Error : Invalid grid format.");
    }else{
        return count;
    }   
}

