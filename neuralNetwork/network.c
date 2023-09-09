#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "../imageReader/imageReader.h"

typedef struct Neuron {
    double *weights;
    double bias;
} Neuron;

typedef struct Layer {
    int num_neurons;
    Neuron *neurons;
} Layer;

typedef struct NeuralNetwork {
    int num_layers;
    Layer *layers;
} NeuralNetwork;

double random_weight() {
    // Random entre -1 et 1
    return (double)rand() / RAND_MAX * 2.0 - 1.0;
}

double random_bias() {
    // Random entre -0.5 et 0.5
    return (double)rand() / RAND_MAX - 0.5;
}

void initialize_network(NeuralNetwork *network) {
    // Seed du générateur de nombres aléatoires avec le temps actuel
    srand(time(NULL));

    for (int layer_i = 0; layer_i < network->num_layers; layer_i++) {
        for (int neuron_i = 0; neuron_i < network->layers[layer_i].num_neurons; neuron_i++) {
            network->layers[layer_i].neurons[neuron_i].bias = random_bias();

            // Allouer de la mémoire pour les poids
            network->layers[layer_i].neurons[neuron_i].weights =
                (double *)malloc(network->layers[layer_i].num_neurons * sizeof(double));

            // Vérifier l'allocation de mémoire
            if (network->layers[layer_i].neurons[neuron_i].weights == NULL) {
                perror("Erreur d'allocation de mémoire pour les poids");
                exit(EXIT_FAILURE);
            }

            // Initialiser les poids pour chaque connexion
            for (int weight_i = 0; weight_i < network->layers[layer_i].num_neurons; weight_i++) {
                network->layers[layer_i].neurons[neuron_i].weights[weight_i] = random_weight();
            }
        }
    }
}

// Fonction pour libérer la mémoire du réseau
void destroy_network(NeuralNetwork *network) {
    for (int layer_i = 0; layer_i < network->num_layers; layer_i++) {
        for (int neuron_i = 0; neuron_i < network->layers[layer_i].num_neurons; neuron_i++) {
            free(network->layers[layer_i].neurons[neuron_i].weights);
        }
        free(network->layers[layer_i].neurons);
    }
    free(network->layers);
    free(network);
}

int main(){
    uint8_t *image;
    uint8_t label;
    GetImage(&image, &label, 100);
    printf("%d\n", label);

    return 0;
}
