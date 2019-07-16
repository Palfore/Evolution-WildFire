#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "AxonGene.h"

class NeuralNetwork { ///< @todo There are two representations of weights, this should be reduced to one.
    public:
        NeuralNetwork(std::vector<AxonGene*> axons);
        NeuralNetwork(const NeuralNetwork& other) : weights(other.weights), layerSizes(other.layerSizes), potentials(other.potentials), w(other.w) {}
        virtual ~NeuralNetwork();

        void draw(bool drawAxons) const;
        std::vector<double> propagate(const std::vector<double>& inputs);


    private:
        struct Axon {
            int a;
            int b;
            int layer;
            double weight;
            Axon(int A, int B, int L, double W) : a(A), b(B), layer(L), weight(W) {};
            Axon(const Axon& other) : a(other.a), b(other.b), layer(other.layer), weight(other.weight) {}
            ~Axon() {}
        };

        std::vector<std::vector<Axon>> weights;
        std::vector<int> layerSizes;
        std::vector<std::vector<double>> potentials;
        std::vector<std::vector<std::vector<double>>> w;
        double activationFunction(double x);
};

#endif // NEURALNETWORK_H
