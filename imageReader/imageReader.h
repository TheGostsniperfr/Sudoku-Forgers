#ifndef IMAGEREADER_H
#define IMAGEREADER_H

void GetImage(uint8_t **image, uint8_t *label,  int imageRes,int n);
void GetImages(uint8_t **images, uint8_t **labels, int *imageRes, int *nbImages);

#endif