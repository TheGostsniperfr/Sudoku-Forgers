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
    // Random between -1 & 1
    return (double)rand() / RAND_MAX * 2.0 - 1.0;
}

double random_bias() {
    // Random between -0.5 & 0.5
    return (double)rand() / RAND_MAX - 0.5;
}




/***************************************************************
 *  Function initialize_network : 
 *
 *  Init the bias and wiehgt (random) of each neurons
 *
 *  @input :
 *      - *network (NeuralNetwork) : data outputs by neural networks
***************************************************************/

void initialize_network(NeuralNetwork *network) {

    // Seed of random set to the current time
    srand(time(NULL));

    for (int layer_i = 1; layer_i < network->nb_layers; layer_i++) {
        for (int neuron_i = 0; neuron_i < network->layers[layer_i].nb_neurons; neuron_i++) {
            printf("%d - nb neurons in layer 0 : %d\n", neuron_i, network->layers[0].nb_neurons);

            network->layers[layer_i].neurons[neuron_i].bias = random_bias();

            // Alloc the memory for the weight
            network->layers[layer_i].neurons[neuron_i].weights =
                (double *)malloc(network->layers[layer_i - 1].nb_neurons * sizeof(double));

            // Check of weight allocation
            if (network->layers[layer_i].neurons[neuron_i].weights == NULL) {
                perror("Error of weights memory allocation.");
                exit(EXIT_FAILURE);
            }

            // Init weight of each connexion between neurons
            for (int weight_i = 0; weight_i < network->layers[layer_i - 1].nb_neurons; weight_i++) {
                network->layers[layer_i].neurons[neuron_i].weights[weight_i] = random_weight();
            }
        }
    }

}


/***************************************************************
 *  Function destroy_network : 
 *
 *  Dump the memory of neural network
 * 
 *  @input :
 *      - *network (NeuralNetwork) : data outputs by neural networks
***************************************************************/

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


/***************************************************************
 *  Function inputPropagation : 
 *
 *  Assigns each pixel in the image to a input neuron
 * 
 *  @input :
 *      - *network (NeuralNetwork) : data outputs by neural networks
 *      - *image (uint8_t) : input image
***************************************************************/

void inputPropagation(NeuralNetwork *network, uint8_t *image){

    if(network->layers[0].nb_neurons != 784){
        errx(1, "First layer doesn't have the number of px of the image.");
    }


    for (int i = 0; i < network->layers[0].nb_neurons; i++)
    {
        network->layers[0].neurons[i].output = (double)image[i]/255.0;
    }    
}

/***************************************************************
 *  Function hiddenPropagation : 
 *
 *  Propagation of hidden layer(s) neurons
 * 
 *  @input :
 *      - *network (NeuralNetwork) : data outputs by neural networks
***************************************************************/

void hiddenPropagation(NeuralNetwork *network){
    for (int layer_i = 1; layer_i < network->nb_layers; layer_i++) {
        for (int neuron_i = 0; neuron_i < network->layers[layer_i].nb_neurons; neuron_i++) {
            double weighted_sum = network->layers[layer_i].neurons[neuron_i].bias;

            // Weighted sum
            for (int prev_neuron_i = 0; prev_neuron_i < network->layers[layer_i - 1].nb_neurons; prev_neuron_i++) {
                weighted_sum += network->layers[layer_i].neurons[neuron_i].weights[prev_neuron_i] * network->layers[layer_i - 1].neurons[prev_neuron_i].output;
            }

            // Activation function (ReLu function)
            network->layers[layer_i].neurons[neuron_i].output = relu(weighted_sum);
        }
    }
}


/***************************************************************
 *  Function outputPropagation : 
 *
 *  Propagation of output layer neurons
 * 
 *  @input :
 *      - *network (NeuralNetwork) : data outputs by neural networks
 *      - *output (double) : output data (the recognised number)
***************************************************************/

void outputPropagation(NeuralNetwork *network,  double *output){
    int output_layer_index = network->nb_layers - 1;
    int nb_output_neurons = network->layers[output_layer_index].nb_neurons;
    double output_values[nb_output_neurons];

    for (int neuron_i = 0; neuron_i < nb_output_neurons; neuron_i++) {
        double weighted_sum = network->layers[output_layer_index].neurons[neuron_i].bias;

        // Weighted sum
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


/***************************************************************
 *  Function forward_propagation : 
 *
 *  Takes the data from the input image, propagates it through all the network layers 
 *  and recovers the processed data (the recognised number)
 * 
 *  @input :
 *      - *network (NeuralNetwork) : data outputs by neural networks
 *      - *image (uint8_t) : input image
 *      - *output (double) : output data (the recognised number)
***************************************************************/

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


/***************************************************************
 *  Function backward_propagation : 
 *
 *  Takes the data from the input image, propagates it through all the network layers 
 *  and recovers the processed data (the recognised number)
 * 
 *  @input :
 *      - *network (NeuralNetwork) : data outputs by neural networks
 *      - **predicted_probs (double) : input image
 *      - **true_probs (double) : output data (the recognised number)
 *      - nb_images (int) : number of pixel for a image 
 *      - learning_rate (double) : rate of the neural network learn 
***************************************************************/

void backward_propagation(NeuralNetwork *network, double **predicted_probs, double **true_probs, int nb_images, double learning_rate) {
    int output_layer_index = network->nb_layers - 1;
    int nb_output_neurons = network->layers[output_layer_index].nb_neurons;

    for (int image_i = 0; image_i < nb_images; image_i++) {
        // Gradients for output layer weights and biases
        double output_gradients[nb_output_neurons];

        // Calcul des gradients pour la couche de sortie
        for (int neuron_i = 0; neuron_i < nb_output_neurons; neuron_i++) {
            output_gradients[neuron_i] = predicted_probs[image_i][neuron_i] - true_probs[image_i][neuron_i];
        }

        // Calcul of gradients for the output layer
        for (int layer_i = output_layer_index; layer_i > 0; layer_i--) {
            int nb_neurons = network->layers[layer_i].nb_neurons;

            for (int neuron_i = 0; neuron_i < nb_neurons; neuron_i++) {
                double activation_derivative = relu_derivative(network->layers[layer_i].neurons[neuron_i].output);

                // Calculation of the weighted sum of the gradients of the neurons in the next layer
                double output_gradient = 0.0;

                if (layer_i == output_layer_index) {
                    output_gradient = output_gradients[neuron_i];
                } else {
                    for (int next_neuron_i = 0; next_neuron_i < network->layers[layer_i + 1].nb_neurons; next_neuron_i++) {
                        output_gradient += output_gradients[next_neuron_i] * network->layers[layer_i + 1].neurons[next_neuron_i].weights[neuron_i];
                    }
                }

                // Gradient calcul
                double neuron_gradient = output_gradient * activation_derivative;

                // Update of hte bias
                network->layers[layer_i].neurons[neuron_i].bias -= learning_rate * neuron_gradient;

                // Update of the weight
                for (int prev_neuron_i = 0; prev_neuron_i < network->layers[layer_i - 1].nb_neurons; prev_neuron_i++) {
                    network->layers[layer_i].neurons[neuron_i].weights[prev_neuron_i] -= learning_rate * neuron_gradient * network->layers[layer_i - 1].neurons[prev_neuron_i].output;
                }
            }
        }
    }
}




int main() {
    // Init of neural network
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

    //Numebr of neurone by layers
    //layers.first -> input layer | layers.last -> output layer
    network->layers[0].nb_neurons = 784; 
    network->layers[1].nb_neurons = 128; 
    network->layers[2].nb_neurons = 10;  

    initialize_network(network);


    //Get images from data set
    uint8_t *images = NULL;
    uint8_t *labels = NULL;
    int imageRes = 0;
    int nbImages = 0;

    GetImages(&images, &labels, &imageRes, &nbImages);

    // Perform training on all images
    int correct_predictions = 0;
    for (int image_i = 0; image_i < nbImages; image_i++) {

        uint8_t *current_image = images + image_i * imageRes * imageRes;
        uint8_t current_label = labels[image_i];

        // Forward propagation
        double output[10]; 
        forward_propagation(network, current_image, output);

        // Prediction calcul
        int predicted_class = 0;
        for (int i = 1; i < 10; i++) {
            if (output[i] > output[predicted_class]) {
                predicted_class = i;
            }
        }

        // Check that the prediction is correct
        if (predicted_class == current_label) {
            correct_predictions++;
        }

        // Backward Propagation

        // Show progression
        printf("Image %d/%d - Prédiction : %d, Label : %d\n", image_i + 1, nbImages, predicted_class, current_label);
    }

    // Show ration of performence
    double accuracy = (double)correct_predictions / nbImages * 100.0;
    printf("Pourcentage de bonnes réponses : %.2f%%\n", accuracy);

    // Dump memory
    destroy_network(network);
    free(images);
    free(labels);

    return 0;
}


