#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <err.h>
#include <math.h>
#include "../imageReader/imageReader.h"

typedef struct Neuron {
    double *weights;
    double bias;
    double output;
} Neuron;

typedef struct Layer {
    int nb_neurons;
    Neuron *neurons;
} Layer;

typedef struct NeuralNetwork {
    int nb_layers;
    Layer *layers;
} NeuralNetwork;


double relu(double x) {
    return fmax(0, x); 
}

double relu_derivative(double x) {
    if (x > 0.0) {
        return 1.0;
    }
    return 0.0;    
}

void softmax(double *x, int len) {
    double max_val = x[0];
    for (int i = 1; i < len; i++) {
        if (x[i] > max_val) {
            max_val = x[i];
        }
    }

    double sum_exp = 0.0;
    for (int i = 0; i < len; i++) {
        x[i] = exp(x[i] - max_val); 
        sum_exp += x[i];
    }

    for (int i = 0; i < len; i++) {
        x[i] /= sum_exp;
    }
}


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

    for (int layer_i = 1; layer_i < network->nb_layers; layer_i++) {
        for (int neuron_i = 0; neuron_i < network->layers[layer_i].nb_neurons; neuron_i++) {
            printf("%d - nb neurons in layer 0 : %d\n", neuron_i, network->layers[0].nb_neurons);

            network->layers[layer_i].neurons[neuron_i].bias = random_bias();

            // Allouer de la mémoire pour les poids
            network->layers[layer_i].neurons[neuron_i].weights =
                (double *)malloc(network->layers[layer_i - 1].nb_neurons * sizeof(double));

            // Vérifier l'allocation de mémoire
            if (network->layers[layer_i].neurons[neuron_i].weights == NULL) {
                perror("Erreur d'allocation de mémoire pour les poids");
                exit(EXIT_FAILURE);
            }

            // Initialiser les poids pour chaque connexion
            for (int weight_i = 0; weight_i < network->layers[layer_i - 1].nb_neurons; weight_i++) {
                network->layers[layer_i].neurons[neuron_i].weights[weight_i] = random_weight();
            }
        }
    }

}





// Fonction pour libérer la mémoire du réseau
void destroy_network(NeuralNetwork *network) {
    for (int layer_i = 0; layer_i < network->nb_layers; layer_i++) {
        for (int neuron_i = 0; neuron_i < network->layers[layer_i].nb_neurons; neuron_i++) {
            free(network->layers[layer_i].neurons[neuron_i].weights);
        }
        free(network->layers[layer_i].neurons);
    }
    free(network->layers);
    free(network);
}

//Fonction qui init l'input layer avec l'image 
void inputPropagation(NeuralNetwork *network, uint8_t *image){

    if(network->layers[0].nb_neurons != 784){
        errx(1, "First layer doesn't have the number of px of the image.");
    }


    for (int i = 0; i < network->layers[0].nb_neurons; i++)
    {
        network->layers[0].neurons[i].output = (double)image[i]/255.0;
    }    
}


void hiddenPropagation(NeuralNetwork *network){
    for (int layer_i = 1; layer_i < network->nb_layers; layer_i++) {
        for (int neuron_i = 0; neuron_i < network->layers[layer_i].nb_neurons; neuron_i++) {
            double weighted_sum = network->layers[layer_i].neurons[neuron_i].bias;

            // Somme pondérée
            for (int prev_neuron_i = 0; prev_neuron_i < network->layers[layer_i - 1].nb_neurons; prev_neuron_i++) {
                weighted_sum += network->layers[layer_i].neurons[neuron_i].weights[prev_neuron_i] * network->layers[layer_i - 1].neurons[prev_neuron_i].output;
            }

            // Activation function (ReLu function)
            network->layers[layer_i].neurons[neuron_i].output = relu(weighted_sum);
        }
    }
}

void outputPropagation(NeuralNetwork *network,  double *output){
    int output_layer_index = network->nb_layers - 1;
    int nb_output_neurons = network->layers[output_layer_index].nb_neurons;
    double output_values[nb_output_neurons];

    for (int neuron_i = 0; neuron_i < nb_output_neurons; neuron_i++) {
        double weighted_sum = network->layers[output_layer_index].neurons[neuron_i].bias;

        // Somme pondérée 
        for (int prev_neuron_i = 0; prev_neuron_i < network->layers[output_layer_index - 1].nb_neurons; prev_neuron_i++) {
            weighted_sum += network->layers[output_layer_index].neurons[neuron_i].weights[prev_neuron_i] * network->layers[output_layer_index - 1].neurons[prev_neuron_i].output;
        }

        output_values[neuron_i] = weighted_sum;
    }

    // Activation function (SoftMax function)
    softmax(output_values, nb_output_neurons);
    
    for (int i = 0; i < nb_output_neurons; i++) {
        output[i] = output_values[i];
    }
}


void forward_propagation(NeuralNetwork *network, uint8_t *image, double *output){
    inputPropagation(network, image);
    hiddenPropagation(network);
    outputPropagation(network, output);
}

double categorical_cross_entropy_loss(NeuralNetwork *network, double *predicted_probs, double *true_probs) {
    double loss = 0.0;

    int output_layer_index = network->nb_layers - 1;
    int nb_output_neurons = network->layers[output_layer_index].nb_neurons;

    for (int i = 0; i < nb_output_neurons; i++) {
        loss -= true_probs[i] * log(predicted_probs[i] + 1e-15); 
    }

    return loss;
}

/***************************************************************
 *  Function mean_categorical_cross_entropy_loss: 
 *
 *  Total calculation of neural network reconisation performance
 *
 *  @input :
 *      - **predicted_probs (double) : data outputs by neural networks
 *      - **true_probs (double) : real data that the neural network needs to find
 *      - nb_images (int) : number of images input
 *
 *  @output :
 *      - (double) : ration
***************************************************************/
double mean_categorical_cross_entropy_loss(NeuralNetwork *network, double **predicted_probs, double **true_probs, int nb_images) {
    double total_loss = 0.0;

    for (int i = 0; i < nb_images; i++) {
        total_loss += categorical_cross_entropy_loss(network, predicted_probs[i], true_probs[i]);
    }

    return total_loss / nb_images;
}


void backward_propagation(NeuralNetwork *network, double **predicted_probs, double **true_probs, int nb_images, double learning_rate) {
    int output_layer_index = network->nb_layers - 1;
    int nb_output_neurons = network->layers[output_layer_index].nb_neurons;

    for (int image_i = 0; image_i < nb_images; image_i++) {
        // Gradients pour les poids et les biais de la couche de sortie
        double output_gradients[nb_output_neurons];

        // Calcul des gradients pour la couche de sortie
        for (int neuron_i = 0; neuron_i < nb_output_neurons; neuron_i++) {
            output_gradients[neuron_i] = predicted_probs[image_i][neuron_i] - true_probs[image_i][neuron_i];
        }

        // Gradients pour les couches cachées (en remontant depuis la couche de sortie)
        for (int layer_i = output_layer_index; layer_i > 0; layer_i--) {
            int nb_neurons = network->layers[layer_i].nb_neurons;

            for (int neuron_i = 0; neuron_i < nb_neurons; neuron_i++) {
                // Calcul de la dérivée de la fonction d'activation (par exemple, dérivée de ReLU)
                double activation_derivative = relu_derivative(network->layers[layer_i].neurons[neuron_i].output);

                // Calcul de la somme pondérée des gradients des neurones de la couche suivante
                double output_gradient = 0.0;

                if (layer_i == output_layer_index) {
                    output_gradient = output_gradients[neuron_i];
                } else {
                    for (int next_neuron_i = 0; next_neuron_i < network->layers[layer_i + 1].nb_neurons; next_neuron_i++) {
                        output_gradient += output_gradients[next_neuron_i] * network->layers[layer_i + 1].neurons[next_neuron_i].weights[neuron_i];
                    }
                }

                // Calcul du gradient pour le neurone
                double neuron_gradient = output_gradient * activation_derivative;

                // Mise à jour du biais
                network->layers[layer_i].neurons[neuron_i].bias -= learning_rate * neuron_gradient;

                // Mise à jour des poids avec la règle de la chaîne
                for (int prev_neuron_i = 0; prev_neuron_i < network->layers[layer_i - 1].nb_neurons; prev_neuron_i++) {
                    network->layers[layer_i].neurons[neuron_i].weights[prev_neuron_i] -= learning_rate * neuron_gradient * network->layers[layer_i - 1].neurons[prev_neuron_i].output;
                }
            }
        }
    }
}




int main() {
    // Initialisation du réseau neuronal
    NeuralNetwork *network = (NeuralNetwork *)malloc(sizeof(NeuralNetwork));
    if (network == NULL) {
        perror("Erreur d'allocation de mémoire pour le réseau neuronal");
        exit(EXIT_FAILURE);
    }

    network->nb_layers = 3; 
    network->layers = (Layer *)malloc(network->nb_layers * sizeof(Layer));
    if (network->layers == NULL) {
        perror("Erreur d'allocation de mémoire pour les couches du réseau neuronal");
        exit(EXIT_FAILURE);
    }

    // Définissez le nombre de neurones dans chaque couche et initialisez le réseau
    // Vous devrez remplacer les valeurs par celles correspondant à votre architecture
    network->layers[0].nb_neurons = 784; // Nombre d'entrées
    network->layers[1].nb_neurons = 128; // Nombre de neurones dans la couche cachée
    network->layers[2].nb_neurons = 10;  // Nombre de neurones dans la couche de sortie

    initialize_network(network);

    // Chargez les images, les labels et l'imageRes
    uint8_t *images = NULL;
    uint8_t *labels = NULL;
    int imageRes = 0;
    int nbImages = 0;

    GetImages(&images, &labels, &imageRes, &nbImages);

    // Effectuez l'apprentissage en boucle sur toutes les images
    int correct_predictions = 0;
    for (int image_i = 0; image_i < nbImages; image_i++) {
        // Préparez l'image actuelle et le label correspondant
        uint8_t *current_image = images + image_i * imageRes * imageRes;
        uint8_t current_label = labels[image_i];

        // Propagation avant
        double output[10]; // Vous devrez définir correctement la taille en fonction de votre architecture
        forward_propagation(network, current_image, output);

        // Calcul de la prédiction (classe prédite)
        int predicted_class = 0;
        for (int i = 1; i < 10; i++) {
            if (output[i] > output[predicted_class]) {
                predicted_class = i;
            }
        }

        // Vérifiez si la prédiction est correcte
        if (predicted_class == current_label) {
            correct_predictions++;
        }

        // Propagation arrière
        // Vous devrez ajouter cette étape pour mettre à jour les poids et les biais du réseau

        // Affichez la progression
        printf("Image %d/%d - Prédiction : %d, Label : %d\n", image_i + 1, nbImages, predicted_class, current_label);
    }

    // Calcul du pourcentage de bonnes réponses
    double accuracy = (double)correct_predictions / nbImages * 100.0;
    printf("Pourcentage de bonnes réponses : %.2f%%\n", accuracy);

    // Libérez la mémoire du réseau neuronal, des images et des labels
    destroy_network(network);
    free(images);
    free(labels);

    return 0;
}


