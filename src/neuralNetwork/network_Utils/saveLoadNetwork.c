#include "neuralNetwork/network_Utils/logicalBrain.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************
 *  Function saveNetwork :
 *
 *  Save all the net into a file to reload another time
 *
 *  @input :
 *      - filename (char*) : name of the file to save neural net
 *      - net (NeuralNetwork*) : (init) neural net
 *
***************************************************************/
void saveNetwork(char* filename, NeuralNetwork* net){
    //Create file
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        perror("Error to open or create file");
        exit(EXIT_FAILURE);
    }


    //save, nb of layers
    fprintf(file, "%d\n",  net->nb_layers);


    //save nb of neurons for each leayer
    for (int i = 0;  i != net->nb_layers; i++) {
        fprintf(file, "%d\n", net->layers[i].nb_neurons);
    }

    //save bias and weights(except l = 0) of each layer
    for (int i = 1;  i != net->nb_layers; i++) {
        //save bias
        for (int n = 0; n < net->layers[i].nb_neurons; n++)
        {
            fprintf(file, "%f|", net->layers[i].neurons[n].bias);
        }
        fprintf(file, "\n");

        //save weigths
        for (int n = 0; n < net->layers[i].nb_neurons; n++)
        {
            for (int preN = 0; preN < net->layers[i-1].nb_neurons; preN++)
            {
                fprintf(file, "%f|", net->layers[i].neurons[n].weights[preN]);
            }
            fprintf(file, "\n");
        }
    }

    fclose(file);
}

/***************************************************************
 *  Function loadNetwork :
 *
 *  Save all the net into a file to reload another time
 *
 *  @input :
 *      - filename (char*) : name of the file to load neural net
 *  @output :
 *      - net (NeuralNetwork*) : neural net
***************************************************************/
NeuralNetwork* loadNetwork(char* filename){
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error: file cannot be open!\n");
        exit(EXIT_FAILURE);
    }

    // Init of neural net
    NeuralNetwork *net = (NeuralNetwork *)malloc(sizeof(NeuralNetwork));
    if (net == NULL) {
        perror("Error can't alloc memories to init net");
        exit(EXIT_FAILURE);
    }


    //get number of layer
    fscanf(file, "%d", &net->nb_layers);

    //alloc for layers
    net->layers = (Layer *)malloc(net->nb_layers * sizeof(Layer));
    if (net->layers == NULL) {
        perror("Error can't alloc memories to init layers");
        exit(EXIT_FAILURE);
    }

    //set nb neurons per layers
    for (int i = 0; i < net->nb_layers; i++)
    {
        fscanf(file, "%d", &net->layers[i].nb_neurons);

        net->layers[i].neurons = (Neuron *)malloc(
            net->layers[i]
            .nb_neurons * sizeof(Neuron));

        if (net->layers[i].neurons == NULL) {
            perror("Error can't alloc memories to init neurons");
            exit(EXIT_FAILURE);
        }
    }

    char line[10000];
    fgets(line, sizeof(line), file);
    //load bias
    for (int i = 1; i < net->nb_layers; i++)
    {
        if (fgets(line, sizeof(line), file)) {
            int j = 0;
            char *data = strtok(line, "|");
            while (data != NULL && j < net->layers[i].nb_neurons) {
                sscanf(data, "%lf", &net->layers[i].neurons[j].bias);
                data = strtok(NULL, "|");
                j++;
            }
        }

        for (int n = 0; n < net->layers[i].nb_neurons; n++)
        {
            //allocate weights
            net->layers[i].neurons[n].weights = (double *)malloc(
                net->layers[i - 1].nb_neurons * sizeof(double));

            if (fgets(line, sizeof(line), file)) {
                int k = 0;
                char *data = strtok(line, "|");
                while (data != NULL && k < net->layers[i-1].nb_neurons) {
                    sscanf(data, "%lf",
                         &net->layers[i]
                            .neurons[n]
                            .weights[k]);

                    data = strtok(NULL, "|");
                    k++;
                }
            }
        }
    }

    return net;
}