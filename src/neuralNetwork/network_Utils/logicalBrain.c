#include "neuralNetwork/network_Utils/networkUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


double sigmoid(double x){
    return 1.0/(1.0+exp(-x));
}

double sigmoid_prime(double x){
    return sigmoid(x)*(1-sigmoid(x));
}


double random_weight(double interval) {
    double normalizedRd = (double)rand() / RAND_MAX * 2.0 - 1.0;
    return normalizedRd * interval;
}

double random_bias() {
    // Random between -0.5 & 0.5
    return (double)rand() / RAND_MAX - 0.5;
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

/***************************************************************
 *  Function initializeNetwork :
 *
 *  Init the bias and wiehgt (random) of each neurons
 *
 *  @input :
 *      - *net (NeuralNetwork) : data outputs by neural networks
***************************************************************/

void initializeNetwork(NeuralNetwork *net) {

    // Seed of random set to the current time
    srand(time(NULL));

    for (int layer_i = 1; layer_i < net->nb_layers; layer_i++) {
        for (int neuron_i = 0; neuron_i <
                net->layers[layer_i].nb_neurons; neuron_i++) {

            net->layers[layer_i].neurons[neuron_i].bias = random_bias();

            // Alloc the memory for the weight
            net->layers[layer_i].neurons[neuron_i].weights =
                (double *)malloc(
                    net->layers[layer_i - 1].nb_neurons * sizeof(double));

            // Check of weight allocation
            if (net->layers[layer_i].neurons[neuron_i].weights == NULL) {
                perror("Error of weights memory allocation.");
                exit(EXIT_FAILURE);
            }


            /*
                Xavier Glorot init weight method

                get a random number between -interval and interval

                interval = sqrt[6/(nb_L(i) + nb_L(i+1)]

                nb : number of neurons
                L(a) : the ath layers

            */

            double interval = sqrt
                (
                    6.0 /
                    (double) (
                        net->layers[layer_i].nb_neurons +
                        net->layers[layer_i - 1].nb_neurons
                    )
                );

            // Init weight of each connexion between neurons
            for (int weight_i = 0; weight_i <
                    net->layers[layer_i - 1].nb_neurons; weight_i++) {
                net->layers[layer_i].neurons[neuron_i].weights[weight_i] =
                    random_weight(interval);
            }
        }
    }
}

/***************************************************************
 *  Function hiddenPropagation :
 *
 *  Propagation of hidden layer(s) neurons
 *
 *  @input :
 *      - *net (NeuralNetwork) : data outputs by neural networks
***************************************************************/

void hiddenPropagation(NeuralNetwork *net){
    for (int layer_i = 1; layer_i < net->nb_layers; layer_i++) {
        for (int neuron_i = 0; neuron_i <
                net->layers[layer_i].nb_neurons; neuron_i++) {
            double weighted_sum = net->layers[layer_i].neurons[neuron_i].bias;

            // Weighted sum
            for (int prev_neuron_i = 0; prev_neuron_i <
                    net->layers[layer_i - 1].nb_neurons; prev_neuron_i++) {
                weighted_sum +=
                    net->layers[layer_i]
                        .neurons[neuron_i]
                        .weights[prev_neuron_i]

                    * net->layers[layer_i - 1]
                        .neurons[prev_neuron_i]
                        .output;
        }

            // Activation function (ReLu function)
            net->layers[layer_i].neurons[neuron_i].output =
                sigmoid(weighted_sum);
        }
    }
}


/***************************************************************
 *  Function outputPropagation :
 *
 *  Propagation of output layer neurons
 *
 *  @input :
 *      - *net (NeuralNetwork) : data outputs by neural networks
 *      - *output (double) : output data (the recognised number)
***************************************************************/

void outputPropagation(NeuralNetwork *net,  double *output){
    int output_layer_index = net->nb_layers - 1;
    int nb_output_neurons = net->layers[output_layer_index].nb_neurons;
    double output_values[nb_output_neurons];

    for (int neuron_i = 0; neuron_i < nb_output_neurons; neuron_i++) {
        double weighted_sum = net->layers[output_layer_index]
                                .neurons[neuron_i]
                                .bias;

        // Weighted sum
        for (int prev_neuron_i = 0; prev_neuron_i <
                net->layers[output_layer_index - 1].nb_neurons;
                prev_neuron_i++) {
            weighted_sum +=
                net->layers[output_layer_index]
                    .neurons[neuron_i]
                    .weights[prev_neuron_i]

                * net->layers[output_layer_index - 1]
                    .neurons[prev_neuron_i]
                    .output;
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
 *  Function backPropagation :
 *
 *  Takes the data from the input image,
 *  propagates it through all the net layers
 *  and recovers the processed data (the recognised number)
 *
 *  @input :
 *      - *net (NeuralNetwork) : data outputs by neural networks
 *      - **predicted_probs (double) : input images digit
 *      - **trueProbs (double) : output data (the recognised number)
 *      - num_examples (int) : number of pixel for a image
 *      - learningRate (double) : rate of the neural net learn
***************************************************************/

void backPropagation(NeuralNetwork *net, double *predicted_probs,
    double *trueProbs, double learningRate) {

    int output_layer_index = net->nb_layers - 1;
    int nb_output_neurons = net->layers[output_layer_index].nb_neurons;

    for (int neuron_i = 0; neuron_i < nb_output_neurons; neuron_i++) {
        double output_gradient = predicted_probs[neuron_i]
                                - trueProbs[neuron_i];

        double activation_derivative = sigmoid_prime(
            net->layers[output_layer_index]
                .neurons[neuron_i]
                .output);

        // Updating the output layer bias
        net->layers[output_layer_index].neurons[neuron_i].bias -=
            learningRate * output_gradient;

        // Updating output layer weights
        for (int prev_neuron_i = 0; prev_neuron_i <
                net->layers[output_layer_index - 1]
                    .nb_neurons; prev_neuron_i++) {

            double weight_gradient = output_gradient * activation_derivative *
                net->layers[output_layer_index - 1]
                    .neurons[prev_neuron_i]
                    .output;

            net->layers[output_layer_index]
                .neurons[neuron_i]
                .weights[prev_neuron_i]
                -= learningRate * weight_gradient;
        }
    }

    // Backpropagation into the hidden layers
    for (int layer_i = output_layer_index - 1; layer_i > 0; layer_i--) {
        int num_neurons = net->layers[layer_i].nb_neurons;

        for (int neuron_i = 0; neuron_i < num_neurons; neuron_i++) {
            double activation_derivative = sigmoid_prime(
                net->layers[layer_i]
                    .neurons[neuron_i]
                    .output);

            double output_gradient = 0.0;

            // weighted sum of the next layer
            for (int next_neuron_i = 0; next_neuron_i <
                    net->layers[layer_i + 1]
                        .nb_neurons; next_neuron_i++) {

                output_gradient +=
                    predicted_probs[next_neuron_i]
                    - trueProbs[next_neuron_i];
            }

            // Calculating the gradient for the neuron
            output_gradient *= activation_derivative;

            // Updating the bias of the hidden layer
            net->layers[layer_i].neurons[neuron_i].bias
                -= learningRate * output_gradient;

            // Mise à jour des poids de la couche cachée
            for (int prev_neuron_i = 0; prev_neuron_i <
                    net->layers[layer_i - 1]
                        .nb_neurons; prev_neuron_i++) {

                double weight_gradient =
                    output_gradient *
                    net->layers[layer_i - 1]
                        .neurons[prev_neuron_i]
                        .output;

                net->layers[layer_i]
                    .neurons[neuron_i]
                    .weights[prev_neuron_i]
                    -= learningRate * weight_gradient;
            }
        }
    }
}