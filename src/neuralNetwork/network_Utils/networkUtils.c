#include "neuralNetwork/network_Utils/logicalBrain.h"
#include "GUI/handleUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <err.h>


void printTrainingPara(TrainingPara tP){
    printf
    (
        "################################################\n\n"

        "       📋 Training data spec :\n\n"

        "################################################\n\n"

        "🧮 Number of epoch : %d\n"
        "📦 Batch size : %d\n"
        "🎓 Learning rate : %.2f\n\n"
        "💾 Save training : %s\n\n",

        tP.nbEpoch,
        tP.batchSize,
        tP.learningRate,
        tP.saveTraining == 1 ? "On" : "Off"
    );
}


void printNetworkPara(NetworkPara netPara){
        printf
    (
        "################################################\n\n"

        "       ⚙️  Neural network parameters :\n\n"

        "################################################\n\n"

        "⬅️ Input layer :\n"
        "       🧠 Number of neurons : %d\n\n"

        "↔️ Hidden layer :\n"
        "       📊 Number of layers : %d\n"
        "       🧠 Number of neurons per layer : %d\n\n"

        "➡️ Output layer :\n"
        "       🧠 Number of neurons : %d\n\n",

        netPara.nbNeuronsFirstLayer,
        netPara.nbHiddenLayers,
        netPara.nbNeuronsHiddenLayer,
        netPara.nbNeuronsOutputLayer
    );
}

void printNetworkSpec(NeuralNetwork* net){
        printf
    (
        "################################################\n\n"

        "       ⚙️ Neural network parameters :\n\n"

        "################################################\n\n"

        "⬅️ Input layer :\n"
        "       🧠 Number of neurons : %d\n\n"

        "↔️ Hidden layer :\n"
        "       📊 Number of layers : %d\n"
        "       🧠 Number of neurons per layer : %d\n\n"

        "➡️ Output layer :\n"
        "       🧠 Number of neurons : %d\n\n",

        net->layers[0].nb_neurons,
        net->nb_layers - 1,
        net->nb_layers > 2 ? net->layers[1].nb_neurons : 0,
        net->layers[net->nb_layers-1].nb_neurons
    );
}