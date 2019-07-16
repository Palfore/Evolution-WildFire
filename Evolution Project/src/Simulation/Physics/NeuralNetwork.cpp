#include "NeuralNetwork.h"
#include "math.h"
#include "Shapes.h"

#include <iostream>

///< @todo Somewhere in the network cannot handle 0 outputs. Or layers of 0 size. Maybe 0 should default to sinusoidal muscles?
NeuralNetwork::NeuralNetwork(std::vector<AxonGene*> axons) : weights({}), layerSizes({}), potentials({}), w({}) {
    if (axons.size() == 0) {
        throw std::invalid_argument("A network with 0 axons was requested, but this is not possible.");
    }
    int numLayers = -1;
    for (const AxonGene* axonGene: axons) {
        while (this->weights.size() <= (unsigned) axonGene->layer) weights.push_back({});
        this->weights[axonGene->layer].push_back(Axon(axonGene->a, axonGene->b, axonGene->layer, axonGene->weight));
        numLayers = std::max(numLayers, axonGene->layer+1);
    }

    for (int i = 0; i < numLayers; i++) {
        int num1 = 0;
        int num2 = 0;
        for (const Axon& axon: this->weights[i]) {
            num1 = std::max(num1, axon.a);
            num2 = std::max(num2, axon.b);
        }
        num1++;
        num2++;
        if (i == 0) layerSizes.push_back(num1);
        layerSizes.push_back(num2);
    }
    for (unsigned int l = 0; l < layerSizes.size() - 1; l++) {
        w.push_back({});
        for (int i = 0; i < layerSizes[l]; i++) {
            w[l].push_back({});
            for (int j = 0; j < layerSizes[l+1]; j++) {
                w[l][i].push_back(0);
            }
        }
    }
    for (const AxonGene* axonGene: axons) {
        w[axonGene->layer][axonGene->a][axonGene->b] = axonGene->weight;
    }
    for (unsigned int l = 0; l < layerSizes.size(); l++) { // NumLayers
        potentials.push_back({});
        for (int i = 0; i < layerSizes[l]; i++) {
            potentials[l].push_back(randf(1));
        }
    }
}

NeuralNetwork::~NeuralNetwork() {}

void NeuralNetwork::draw(bool drawAxons) const { // Draw connections of one neuron on hover.
//    DrawRectangle<Appearance::BLACK>(0.1, 0.1, 0.9, 0.9);

    double A = 0.1;
    double B = 0.9;

    const int N = weights.size(); // NumLayers
    static auto getX = [A, B, N](int i, int bound){return (i+1) * (B - A) / static_cast<double>(bound + 2) + A;}; // index->position
    static auto getY = [A, B, N](int j, int bound){return (j+1) * (B - A) / static_cast<double>(bound + 1) + A;};

    if (drawAxons) {
        for (int i = 0; i < N; i++) { // Layers
            for (unsigned int j = 0; j < weights[i].size(); j++) { // Axons
                double weight = weights[i][j].weight;
                if (fabs(weight) < 0.001) {
                    weight = 0.001;
                    continue;
                }
                glLineWidth(fabs(weight) * 10);
                if (weights[i][j].weight > 0) {
                    DrawLine<Appearance::SKY_BLUE>(getX(weights[i][j].layer  , N), getY(weights[i][j].a, layerSizes[weights[i][j].layer  ]),
                                              getX(weights[i][j].layer+1, N), getY(weights[i][j].b, layerSizes[weights[i][j].layer+1]));
                } else {
                    DrawLine<Appearance::RED>(getX(weights[i][j].layer  , N), getY(weights[i][j].a, layerSizes[weights[i][j].layer  ]),
                                              getX(weights[i][j].layer+1, N), getY(weights[i][j].b, layerSizes[weights[i][j].layer+1]));
                }
            } // +1 here^ are because they are in the next layer over
        }
    }

    /* Draw Neurons */
    for (int i = 0; i < N + 1; i++) {
        const double scale = std::min(((B - A) / static_cast<double>(N + 2) + A) / layerSizes[i], 0.025);
        double x = getX(i, N);
        for (int j = 0; j < layerSizes[i]; j++) {
            double y = getY(j, layerSizes[i]);
            DrawCircle<Appearance::WHEAT>(x, y, scale);
            if (potentials[i][j] > 0) {
                DrawCircle<Appearance::BLUE>(x, y, scale * potentials[i][j]);
            } else {
                DrawCircle<Appearance::RED>(x, y, -scale * potentials[i][j]);
            }
        }
    }
}

double NeuralNetwork::activationFunction(double x) {
    return 3*x / (1 + 3*fabs(x));
}

std::vector<double> NeuralNetwork::propagate(const std::vector<double>& inputs) {
    if (inputs.size() != potentials[0].size()) {
        std::cout << inputs.size() << ',' << potentials[0].size() << '\n';
        LOG("Network input did not match its topology.");
    };
    potentials[0] = inputs;
    for (unsigned int l = 1; l < layerSizes.size(); l++) {
        for (int j = 0; j < layerSizes[l]; j++) {
            potentials[l][j] = 0.0;
            for (int i = 0; i < layerSizes[l-1]; i++) {
                potentials[l][j] += potentials[l-1][i] * w[l-1][i][j];
            }
            potentials[l][j] = activationFunction(potentials[l][j]);
        }
    }
    return potentials[potentials.size() - 1];
}























