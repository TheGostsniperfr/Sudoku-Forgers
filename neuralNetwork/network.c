#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <err.h>
#include <math.h>
#include <string.h>
#include "../imageReader/imageReader.h"
#include "../include/neuralNetwork/network_Utils/struct.h"

#define NET_SAVE_PATH "netSave.txt"
#define NET_SAVE_XOR_PATH "netXorSave.txt"
#define true 1
#define false 0

//We put 0.01 instead of 0 to avoid dead neurons.
double relu(double x) {
    if(x > 0.0){
        return x;
    }
    return 0.01 * x;
}

double relu_derivative(double x) {
    if (x > 0.0) {
        return 1.0;
    }
    return 0.01;
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

double sigmoid(double x){
    return 1.0/(1.0+exp(-x));
}

double sigmoid_prime(double x){
    return sigmoid(x)*(1-sigmoid(x));
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

            // Init weight of each connexion between neurons
            for (int weight_i = 0; weight_i <
                    net->layers[layer_i - 1].nb_neurons; weight_i++) {
                net->layers[layer_i].neurons[neuron_i].weights[weight_i] =
                    random_weight();
            }
        }
    }

}


/***************************************************************
 *  Function destroy_network :
 *
 *  Dump the memory of neural net
 *
 *  @input :
 *      - *net (NeuralNetwork) : data outputs by neural networks
***************************************************************/

void destroy_network(NeuralNetwork *net) {
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
 *  Function inputPropagation :
 *
 *  Assigns each pixel in the image to a input neuron
 *
 *  @input :
 *      - *net (NeuralNetwork) : data outputs by neural networks
 *      - *image (uint8_t) : input image
***************************************************************/

void inputPropagation(NeuralNetwork *net, uint8_t *image){

    if(net->layers[0].nb_neurons != 784){
        errx(1, "First layer doesn't have the number of px of the image.");
    }


    for (int i = 0; i < net->layers[0].nb_neurons; i++)
    {
        net->layers[0].neurons[i].output = (double)image[i]/255.0;
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

void forwardPropagation(NeuralNetwork *net, uint8_t *image, double *output){
    inputPropagation(net, image);
    hiddenPropagation(net);
    outputPropagation(net, output);
}


/***************************************************************
 *  Function categorical_cross_entropy_loss:
 *
 *  Calculation of neural net reconisation performance
 *
 *  @input :
 *      - *predicted_probs (double) : data outputs by neural networks
 *      - *trueProbs (double) : real data that the neural net needs to find
 *  @output :
 *      - (double) : ration
***************************************************************/

double categorical_cross_entropy_loss(NeuralNetwork *net,
    double *predicted_probs, double *trueProbs) {

    double loss = 0.0;

    int output_layer_index = net->nb_layers - 1;
    int nb_output_neurons = net->layers[output_layer_index].nb_neurons;

    for (int i = 0; i < nb_output_neurons; i++) {
        loss -= trueProbs[i] * log(predicted_probs[i] + 1e-15);
    }

    return loss;
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


/***************************************************************
 *  Function digitTraining :
 *
 *  Train the neural network to digit recognition
 *
 *  @input :
 *      - net (NeuralNetwork*) : neural network to train
 *      - tP (TrainingPara) : paramters of training

***************************************************************/
void digitTraining(NeuralNetwork* net, TrainingPara tP){
    //Check if the network is valid
    if(net->layers[0].nb_neurons != 784
            && net->layers[net->nb_layers-1].nb_neurons != 10){

        errx(1, "Error : Network para not valid for digit recognition.");
    }

    // Get images from data set

    uint8_t *images = NULL;
    uint8_t *labels = NULL;
    int imageRes = 0;
    int batch = 0;



    GetImages(&images, &labels, &imageRes, &batch);

    if(images == NULL || labels == NULL){
        errx(1, "Error to load data training.");
    }

    if(batch > tP.batchSize){
        batch = tP.batchSize;
    }


    for (int epoch = 0; epoch < tP.nbEpoch; epoch++) {
        int correctPredictions = 0;

        for (int imageI = 0; imageI < batch; imageI++) {
            uint8_t *currentImage = images + imageI * imageRes * imageRes;
            uint8_t correctDigit = labels[imageI];

            // Forward Propagation
            double output[10];
            forwardPropagation(net, currentImage, output);

            // prediction calculation
            int digitRecognised = 0;
            for (int i = 1; i < 10; i++) {
                if (output[i] > output[digitRecognised]) {
                    digitRecognised = i;
                }
            }

            // check if the prediction is correct
            if (digitRecognised == correctDigit) {
                correctPredictions++;
            }

            double trueProbs[10] = {0.0};
            trueProbs[correctDigit] = 1.0;

            //back propagation
            backPropagation(net, output, trueProbs, tP.learningRate);
        }

        // Calculating the percentage of correct answers
        double accuracy = (double)correctPredictions / tP.batchSize * 100.0;
        printf("Epoch %d, Correct answers : %.2f%%\n",
            epoch + 1, accuracy);
    }

    if(tP.saveTraining == 1){
        saveNetwork(NET_SAVE_PATH, net);
    }

    free(images);
    free(labels);
}

/***************************************************************
 *  Function xorTraining :
 *
 *  Train the neural network to digit recognition
 *
 *  @input :
 *      - net (NeuralNetwork*) : neural network to train
 *      - tP (TrainingPara) : paramters of training

***************************************************************/
void xorTraining(NeuralNetwork* net, TrainingPara tP){
    //Check if the network is valid
    if(net->layers[0].nb_neurons != 2
            && net->layers[net->nb_layers-1].nb_neurons != 2){

        errx(1, "Error : Network para not valid for xor recognition.");
    }


    for (int epoch = 0; epoch < tP.nbEpoch; epoch++) {
        int correctPredictions = 0;

        for (int batch = 0; batch < tP.batchSize; batch++)
        {
            //generate a xor data
            double inputA = rand() % 2;
            double inputB = rand() % 2;
            int result = inputA != inputB;

            net->layers[0].neurons[0].output = inputA;
            net->layers[0].neurons[1].output = inputB;

            hiddenPropagation(net);

            double output[2];
            outputPropagation(net, output);

            int digitRecognised = 0;
            for (int i = 1; i < net->layers[0].nb_neurons; i++) {
                if (output[i] > output[digitRecognised]) {
                    digitRecognised = i;
                }
            }

            // check if the prediction is correct
            if (digitRecognised == result) {
                correctPredictions++;
            }

            double trueProbs[2] = {0.0};
            trueProbs[result] = 1.0;

            //back propagation
            backPropagation(net, output, trueProbs, tP.learningRate);
        }

        // Calculating the percentage of correct answers
        double accuracy = (double)correctPredictions / tP.batchSize * 100.0;
        printf("Epoch %d, Correct answers : %.2f%%\n",
            epoch + 1, accuracy);

    }

    if(tP.saveTraining == 1){
        saveNetwork(NET_SAVE_XOR_PATH, net);
    }
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


int main() {

    /*

    //digit recognition

    NetworkPara netPara = {
        .nbNeuronsFirstLayer = 784,
        .nbNeuronsHiddenLayer = 256,
        .nbNeuronsOutputLayer = 10,

        .nbHiddenLayers = 1,
    };

    NeuralNetwork* net = createNetwork(netPara);

    TrainingPara trainPara = {
        .batchSize = 1000,
        .nbEpoch = 1000,
        .learningRate = 0.1,
        .printDebug = true,
        .saveTraining = true,
    };

    digitTraining(net, trainPara);*/


    // xor recognition

    NetworkPara netPara = {
        .nbHiddenLayers = 1,
        .nbNeuronsFirstLayer = 2,
        .nbNeuronsHiddenLayer = 5,
        .nbNeuronsOutputLayer = 2,
    };

    NeuralNetwork* net = createNetwork(netPara);

    TrainingPara trainPara = {
        .batchSize = 1000,
        .learningRate = 10,
        .nbEpoch = 100,
        .saveTraining = true,
    };

    xorTraining(net, trainPara);


    destroy_network(net);

    return 0;
}


