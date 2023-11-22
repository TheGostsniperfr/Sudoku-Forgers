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


void forwardPropagation(NeuralNetwork* net, double* input){
    //input value on the input layer
    Layer* l1 = &net->layers[0];
    for (int n_i = 0; n_i < l1->nb_neurons; n_i++)
    {
        l1->neurons[n_i].output = input[n_i];
    }

    //Forward propagation calcul (from l_{1} to l_{n-1})
    for (int l_i = 1; l_i < net->nb_layers; l_i++)
    {
        Layer* currentL = &net->layers[l_i];
        Layer* prevL = &net->layers[l_i-1];

        for (int n_i = 0; n_i < currentL->nb_neurons; n_i++)
        {
            Neuron* currentN = &currentL->neurons[n_i];
            double sum = currentN->bias;

            for (int nP_i = 0; nP_i < prevL->nb_neurons; nP_i++)
            {
                Neuron* prevN = &prevL->neurons[nP_i];
                sum += prevN->output * currentN->weights[nP_i];
            }

            currentN->output = sigmoid(sum);
            currentN->z = sum;
        }
    }

    //Output layer calcul
    Layer* currentL = &net->layers[net->nb_layers-1];
    Layer* prevL = &net->layers[net->nb_layers-2];
    double expSum = 0;
    double maxSum = -DBL_MAX;

    for (int n_i = 0; n_i < currentL->nb_neurons; n_i++) {
        Neuron* currentN = &currentL->neurons[n_i];
        double sum = currentN->bias;

        for (int nP_i = 0; nP_i < prevL->nb_neurons; nP_i++) {
            Neuron* prevN = &prevL->neurons[nP_i];
            sum += prevN->output * currentN->weights[nP_i];
        }

        if (sum > maxSum) {
            maxSum = sum;
        }
    }

    //Apply softmax with numerical stability
    for (int n_i = 0; n_i < currentL->nb_neurons; n_i++) {
        Neuron* currentN = &currentL->neurons[n_i];
        double sum = currentN->bias;

        for (int nP_i = 0; nP_i < prevL->nb_neurons; nP_i++) {
            Neuron* prevN = &prevL->neurons[nP_i];
            sum += prevN->output * currentN->weights[nP_i];
        }

        currentN->output = exp(sum - maxSum);
        expSum += currentN->output;
    }

    //Apply the softmax function (with numerical stability)
    for (int n_i = 0; n_i < currentL->nb_neurons; n_i++) {
        Neuron* currentN = &currentL->neurons[n_i];
        currentN->output /= expSum;
    }

}

void gradientDescent(NeuralNetwork* net, double* expectedVal){
    // Calculate the error of the last layer
    Layer* lL = &net->layers[net->nb_layers - 1];
    for (int n_i = 0; n_i < lL->nb_neurons; n_i++)
    {
        Neuron* cN = &lL->neurons[n_i];

        // delta = (a_{j}^L - y_{j}) * sigmoidPrime(Z_{j}^L)
        cN->delta = (cN->output - expectedVal[n_i]) * sigmoid_prime(cN->z);
    }

    // Compute all the delta of the previous layer
    for (int l_i = net->nb_layers - 2; l_i >= 0; l_i--)
    {
        Layer* currentL = &net->layers[l_i];
        Layer* nextL = &net->layers[l_i + 1];

        // delta_{k} = (Sum(W_{jk}^{l+1} * delta_{j}_ {l+1}) *
        //               sigmoidPrime(Z_{k}^L)
        for (int nC_i = 0; nC_i < currentL->nb_neurons; nC_i++)
        {
            Neuron* currentN = &currentL->neurons[nC_i];
            double sum = 0;

            for (int nN_i = 0; nN_i < nextL->nb_neurons; nN_i++)
            {
                Neuron* nextN = &nextL->neurons[nN_i];
                sum += nextN->weights[nC_i] * nextN->delta;
            }

            currentN->delta = sum * sigmoid_prime(currentN->z);
        }
    }
}

// Function to compute new weights and bias
void updateWB(NeuralNetwork* net, double learningRate){
    for (int l_i = 1; l_i < net->nb_layers; l_i++)
    {
        Layer* currentL = &net->layers[l_i];
        Layer* prevL = &net->layers[l_i - 1];

        for (int n_i = 0; n_i < currentL->nb_neurons; n_i++)
        {
            Neuron* cN = &currentL->neurons[n_i];

            cN->bias -= learningRate * cN->delta;

            for (int pN_i = 0; pN_i < prevL->nb_neurons; pN_i++)
            {
                Neuron* prevN = &prevL->neurons[pN_i];
                cN->weights[pN_i] -= learningRate * cN->delta * prevN->output;
            }
        }
    }
}


void backPropagation(
    NeuralNetwork* net,
    double* expectedVal,
    double learningRate)
{

    gradientDescent(net, expectedVal);
    updateWB(net, learningRate);
}