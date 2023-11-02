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
        "🎓 Learning rate : %f\n\n"
        "💾 Save training : %s\n",

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

        "       ⚙️ Neural network parameters :\n\n"

        "################################################\n\n"

        "⬅️ Input layer :\n"
        "       🧠 Number of neurons : %d\n\n"

        "↔️ Hidden layer :\n"
        "       📊 Number of layers : %d\n"
        "       🧠 Number of neurons per layer : %d\n\n"

        "➡️ Output layer :\n"
        "       🧠 Number of neurons : %d\n",

        netPara.nbNeuronsFirstLayer,
        netPara.nbHiddenLayers,
        netPara.nbNeuronsHiddenLayer,
        netPara.nbNeuronsOutputLayer
    );
}