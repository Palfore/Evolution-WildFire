#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "AxonGene.h"
//#include "tiny_dnn.h"
//#include "tiny_dnn/tiny_dnn.h"
#include "tiny_dnn.h"
//#include "libraries/tiny-dnn-master/tiny_dnn/tiny_dnn.h"

using namespace tiny_dnn;
class NeuralNetwork {
    public:
        NeuralNetwork(int networkID, std::vector<AxonGene*> axons);
        NeuralNetwork(const NeuralNetwork& other);
        virtual ~NeuralNetwork();

        void draw(bool drawAxons) const;
        std::vector<double> propagate(const std::vector<double>& inputs);

        network<sequential> net;
        std::vector<unsigned int> sizes;
    private:
        std::array<std::vector<double>, 2> potentials; // In and Out only since hidden are hard to access from api
};

class RecurrentNeuralNetwork {
    public:
        RecurrentNeuralNetwork(); // @TODO: remove me after testing
        RecurrentNeuralNetwork(int networkID, std::vector<AxonGene*> axons);
        RecurrentNeuralNetwork(const RecurrentNeuralNetwork& other);
        virtual ~RecurrentNeuralNetwork();

        void draw(bool drawAxons) const;
        std::vector<double> propagate(const std::vector<double>& inputs);

        network<sequential> net;
        std::vector<unsigned int> sizes;
    private:
        std::array<std::vector<double>, 2> potentials; // In and Out only since hidden are hard to access from api
};



#endif // NEURALNETWORK_H
