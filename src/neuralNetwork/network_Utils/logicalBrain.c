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
 *  Function forwardPropagation :
 *
 *  Takes the data from the input image,
 *  propagates it through all the net layers
 *  and recovers the processed data (the recognised number)
 *
 *  @input :
 *      - *net (NeuralNetwork) : data outputs by neural networks
 *      - *image (uint8_t) : input image
 *      - *output (double) : output data (the recognised number)
***************************************************************/

void forwardPropagation(NeuralNetwork* net, double* input){
    Layer* currentLayer = &net->layers[0];


    //First layer
    for (int i = 0; i < currentLayer->nb_neurons; i++)
    {
        currentLayer->neurons[i].output = input[i];
    }


    //Hidden layers
    for (int layer_i = 1; layer_i < net->nb_layers; layer_i++)
    {
        currentLayer = &net->layers[layer_i];
        Layer* previousLayer = &net->layers[layer_i - 1];

        for (int n_i = 0; n_i < currentLayer->nb_neurons; n_i++)
        {
            double sum = 0.0;

            for (int nP_i = 0; nP_i < previousLayer->nb_neurons; nP_i++)
            {
                sum += currentLayer->neurons[n_i].weights[nP_i] *
                    previousLayer->neurons[nP_i].output;
            }

            currentLayer->neurons[n_i].output = sigmoid(sum);
        }
    }


    //Output layer
    double sum = 0.0;
    currentLayer = &net->layers[net->nb_layers - 1];

    for (int n_i = 0; n_i < currentLayer->nb_neurons; n_i++)
    {
        currentLayer->neurons[n_i].output =
            exp(currentLayer->neurons[n_i].output);

        sum += currentLayer->neurons[n_i].output;
    }

    for (int n_i = 0; n_i < currentLayer->nb_neurons; n_i++)
    {
        currentLayer->neurons[n_i].output =
            currentLayer->neurons[n_i].output / sum;
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

double backPropagation(NeuralNetwork* net, double* true_probs){
    double errorRate = 0.0;
    double errorTmp = 0.0;

    Layer* outLayer = &net->layers[net->nb_layers-1];

    for (int i = 0; i < outLayer->nb_neurons; i++)
    {
        errorTmp = true_probs[i] -
                    outLayer->neurons[i].output;

        outLayer->neurons[i].delta =
            errorTmp * sigmoid_prime(outLayer->neurons[i].output);

        errorRate += (errorTmp * errorTmp);
    }

    for(int i = net->nb_layers - 1; i >= 2; i--){
        Layer* currentLayer = &net->layers[i];
        Layer* previousLayer = &net->layers[i-1];

        for (int j = 0; j < previousLayer->nb_neurons; j++)
        {
            errorTmp = 0.0;

            for (int k = 0; k < currentLayer->nb_neurons; k++)
            {
                errorTmp +=
                    currentLayer->neurons[k].delta *
                    currentLayer->neurons[k].weights[j];
            }

            previousLayer->neurons[j].delta = errorTmp *
                sigmoid_prime(previousLayer->neurons[j].output);
        }
    }

    return errorRate;
}


void gradientDescent(NeuralNetwork* net, double learningRate){
    for (int i = net->nb_layers - 1; i >= 1; i--)
    {
        Layer* currentLayer = &net->layers[i];
        Layer* previousLayer = &net->layers[i-1];

        for (int j = 0; j < currentLayer->nb_neurons; j++)
        {
            for (int k = 0; k < previousLayer->nb_neurons; k++)
            {
                currentLayer->neurons[j].weights[k] +=
                    currentLayer->neurons[j].delta *
                    previousLayer->neurons[k].output *
                    learningRate;

                currentLayer->neurons[j].bias +=
                    currentLayer->neurons[j].delta *
                    previousLayer->neurons[k].output *
                    learningRate;
            }
        }
    }
}