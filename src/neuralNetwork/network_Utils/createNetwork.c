#include "neuralNetwork/network_Utils/logicalBrain.h"

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

/***************************************************************
 *  Function destroyNetwork :
 *
 *  Dump the memory of neural net
 *
 *  @input :
 *      - *net (NeuralNetwork) : data outputs by neural networks
***************************************************************/

void destroyNetwork(NeuralNetwork* net) {
    for (int layer_i = 0; layer_i < net->nb_layers; layer_i++) {
        for (int neuron_i = 0; neuron_i <
                net->layers[layer_i].nb_neurons; neuron_i++) {
            free(net->layers[layer_i].neurons[neuron_i].weights);
        }
        free(net->layers[layer_i].neurons);
    }
    free(net->layers);
    free(net);
}


/***************************************************************
 *  Function createNetwork :
 *
 *  Train the neural network to digit recognition
 *
 *  @input :
 *      - netPara (NetworkPara) : neural network parameter
 *  @output :
 *      - (NeuralNetwork*) : neural network ready to be used
***************************************************************/

NeuralNetwork* createNetwork(NetworkPara netPara){
    // Alloc memory for neural network
    NeuralNetwork *net = (NeuralNetwork *)malloc(sizeof(NeuralNetwork));
    if (net == NULL) {
        errx(1, "Error to alloc memory to neural network");
    }

    // Set nb layers
    net->nb_layers = netPara.nbHiddenLayers + 2;

    // Alloc memory for all layers
    net->layers = (Layer *)malloc(net->nb_layers * sizeof(Layer));
    if (net->layers == NULL) {
        errx(1, "Error to alloc memory to layers");
    }

    // Set nb of neurons for each layer
    net->layers[0].nb_neurons = netPara.nbNeuronsFirstLayer;

    int i = 0;

    for(; i < netPara.nbHiddenLayers; i++){
        net->layers[i + 1].nb_neurons = netPara.nbNeuronsHiddenLayer;
    }

    net->layers[i + 1].nb_neurons = netPara.nbNeuronsOutputLayer;

    //Alloc memory for neurons in each layer
    for (int layer_i = 0; layer_i < net->nb_layers; layer_i++) {
        net->layers[layer_i].neurons = (Neuron *)malloc(
            net->layers[layer_i]
                .nb_neurons * sizeof(Neuron));

        if (net->layers[layer_i].neurons == NULL) {
            errx(1, "Error to alloc memory to neurons");
        }
    }

    // Alloc and init bias & weight for each layer
    initializeNetwork(net);

    return net;
}



