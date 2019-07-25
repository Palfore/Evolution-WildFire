#include "NeuralNetwork.h"
#include "math.h"
#include "Shapes.h"

#include <iostream>

using namespace tiny_dnn;
NeuralNetwork::NeuralNetwork(int networkID, std::vector<AxonGene*> axons) : net(), sizes({}), potentials() {
    /* Get Network Size */
    int numLayers = 0;
    for (const AxonGene* axonGene: axons) {
        if (axonGene->network != networkID) continue;
        numLayers = std::max(numLayers, axonGene->layer + 1);
    }

    /* Get Layer Sizes */
    std::vector<unsigned int> sizesCount(numLayers+1);
    for (const AxonGene* axonGene: axons) {
        if (axonGene->network != networkID) continue;
        sizesCount[axonGene->layer] = std::max(sizesCount[axonGene->layer], static_cast<unsigned int>(axonGene->a)+1);
        sizesCount[axonGene->layer+1] = std::max(sizesCount[axonGene->layer+1], static_cast<unsigned int>(axonGene->b)+1);
    }
    this->sizes = sizesCount;

    /* Build Network */
    for (unsigned int i = 0; i < this->sizes.size() - 1; i++) {
        this->net << fully_connected_layer(this->sizes[i], this->sizes[i+1]) << activation::tanh();
    }

    /* Initialize Weights */
    for (const AxonGene* axonGene: axons) {
        if (axonGene->network != networkID) continue;
        (*net[2*axonGene->layer]->weights()[0])[ // Since every other layer corresponds to the activation function
            axonGene->a + axonGene->b*this->sizes[axonGene->layer]
        ] = axonGene->weight;
    }

    /* Initialize Potentials */
    this->potentials[0] = std::vector<double>(this->sizes.front());
    this->potentials[1] = std::vector<double>(this->sizes.back());

}


NeuralNetwork::NeuralNetwork(const NeuralNetwork& other) : net(other.net), sizes(other.sizes), potentials(other.potentials) {

}

NeuralNetwork::~NeuralNetwork() {

}


void NeuralNetwork::draw(bool drawAxons) const {
    double A = 0.1;
    double B = 0.9;

    const int N = net.layer_size() / 2; // NumLayers
    static auto getX = [A, B, N](int i, int bound){return (i+1) * (B - A) / static_cast<double>(bound + 2) + A;}; // index->position
    static auto getY = [A, B, N](int j, int bound){return (j+1) * (B - A) / static_cast<double>(bound + 1) + A;};

    if (drawAxons) {
        for (unsigned int i = 0; i < sizes.size() - 1; i++) {
            std::vector<const vec_t*> weights = net[2*i]->weights();
            auto& w = *weights[0];
            assert(w.size() == static_cast<unsigned int>(sizes[i]*sizes[i+1]));
            for (unsigned int j = 0; j < w.size(); j++) {
                double weight = w[j];
                if (fabs(weight) < 0.001) {
                    continue;
                }
                glLineWidth(fabs(weight) * 10);
                if (weight > 0) {
                    DrawLine<Appearance::SKY_BLUE>(getX(i  , N), getY(j%sizes[i], this->sizes[i]),
                                               getX(i+1, N), getY(j/sizes[i], this->sizes[i+1]));
                } else {
                    DrawLine<Appearance::RED>(getX(i  , N), getY(j%sizes[i], this->sizes[i]),
                                               getX(i+1, N), getY(j/sizes[i], this->sizes[i+1]));
                }
            }
        }
    }

    /* Draw Neurons */
    if (potentials[0].size() <= 0) return;
    for (unsigned int n = 0; n < this->sizes.size(); n++) {
        const double scale = std::min(((B - A) / static_cast<double>(N + 2) + A) / this->sizes[n], 0.025);
        for (unsigned int j = 0; j < this->sizes[n]; j++) {
            if (n == 0) {
                DrawCircle<Appearance::WHEAT>(getX(n, N), getY(j, this->sizes[n]), scale);
                if (potentials[0][j] > 0) {
                    DrawCircle<Appearance::BLUE>(getX(n, N), getY(j, this->sizes[n]), scale * potentials[0][j]);
                } else {
                    DrawCircle<Appearance::RED >(getX(n, N), getY(j, this->sizes[n]), -scale * potentials[0][j]);
                }
            } else if (n == this->sizes.size() - 1) {
                DrawCircle<Appearance::WHEAT>(getX(n, N), getY(j, this->sizes[n]), scale);
                if (potentials[1][j] > 0) {
                    DrawCircle<Appearance::BLUE>(getX(n, N), getY(j, this->sizes[n]), scale * potentials[1][j]);
                } else {
                    DrawCircle<Appearance::RED >(getX(n, N), getY(j, this->sizes[n]), -scale * potentials[1][j]);
                }
            } else if (drawAxons) {
                DrawCircle<Appearance::WHEAT>(getX(n, N), getY(j, this->sizes[n]), scale);
            }
        }
    }
}

std::vector<double> NeuralNetwork::propagate(const std::vector<double>& inputs) {
    if (inputs.size() != this->sizes[0]) {
        std::cout << inputs.size() << ',' << this->sizes[0] << '\n';
        LOG("Network input did not match its topology.", LogDegree::FATAL);
    };
    auto output = net.predict(inputs);
    this->potentials[0] = inputs;
    this->potentials[1] = std::vector<double>(
        std::make_move_iterator(output.begin()),
        std::make_move_iterator(output.end())
    );
    if (potentials[1].size() != this->sizes.back()) {
        std::cout << potentials[1].size() << ',' << this->sizes.back() << '\n';
        LOG("Network input did not match its topology.", LogDegree::FATAL);
    };
    return potentials[1];
}





#include "NeuralNetwork.h"
#include "math.h"
#include "Shapes.h"

#include <iostream>

using namespace tiny_dnn;
RecurrentNeuralNetwork::RecurrentNeuralNetwork(int networkID, std::vector<AxonGene*> axons) : net(), sizes({}), potentials() {}
RecurrentNeuralNetwork::RecurrentNeuralNetwork() : net(), sizes({}), potentials() {
    // Single RRN layer with 3 inputs and 7 hidden (using up to 2 sequences at a time) using the tanh activation function
    // Followed by a FFNN that takes those 7 hidden and connects it to 6 outputs.
    // net.bias_init(weight_init::constant(0.6));
    this->net << recurrent_layer(rnn(3, 8), 2) << tanh_layer() << fully_connected_layer(8, 3) << tanh_layer();
net.bias_init(weight_init::xavier(4));

    std::cout << "Starting: " << this->net.layer_size() << "layers\n";
    for (unsigned int i = 0; i < this->net.layer_size(); i++) {
        std::vector<vec_t*> weights = net[i]->weights();
        std::cout << "There are " << weights.size() << "entries in layer " << i << '\n';
        for (auto& a: weights) {
            std::cout << a->size() <<"Entries: " <<  '\n';
            for (auto& v: *a) {
                // v = pmRandf(1);
                std::cout<<'\t'<<v<<'\n';
            }
        }
    }

    net.at<recurrent_layer>(0).clear_state(); // Might not be needed
    net.at<recurrent_layer>(0).seq_len(1); // Fed in at a time
    net.at<recurrent_layer>(0).bptt_max(3); // Max to remember
    auto x = net.predict({1, 2, 3});
    for (const auto& y: x) {
        // for (const auto& z: y) {
            // std::cout << z <<'\n';
            std::cout << y <<'\n';
        // }
    }
    x = net.predict({1, 2, 3});
    for (const auto& y: x) {
        // for (const auto& z: y) {
            // std::cout << z <<'\n';
            std::cout << y <<'\n';
        // }
    }
    x = net.predict({1, 2, 3});
    for (const auto& y: x) {
        // for (const auto& z: y) {
            // std::cout << z <<'\n';
            std::cout << y <<'\n';
        // }
    }











    exit(0);
}


RecurrentNeuralNetwork::RecurrentNeuralNetwork(const RecurrentNeuralNetwork& other) : net(other.net), sizes(other.sizes), potentials(other.potentials) {

}

RecurrentNeuralNetwork::~RecurrentNeuralNetwork() {

}


void RecurrentNeuralNetwork::draw(bool drawAxons) const {
    double A = 0.1;
    double B = 0.9;

    const int N = net.layer_size() / 2; // NumLayers
    static auto getX = [A, B, N](int i, int bound){return (i+1) * (B - A) / static_cast<double>(bound + 2) + A;}; // index->position
    static auto getY = [A, B, N](int j, int bound){return (j+1) * (B - A) / static_cast<double>(bound + 1) + A;};

    if (drawAxons) {
        for (unsigned int i = 0; i < sizes.size() - 1; i++) {
            std::vector<const vec_t*> weights = net[2*i]->weights();
            auto& w = *weights[0];
            assert(w.size() == static_cast<unsigned int>(sizes[i]*sizes[i+1]));
            for (unsigned int j = 0; j < w.size(); j++) {
                double weight = w[j];
                if (fabs(weight) < 0.001) {
                    continue;
                }
                glLineWidth(fabs(weight) * 10);
                if (weight > 0) {
                    DrawLine<Appearance::SKY_BLUE>(getX(i  , N), getY(j%sizes[i], this->sizes[i]),
                                               getX(i+1, N), getY(j/sizes[i], this->sizes[i+1]));
                } else {
                    DrawLine<Appearance::RED>(getX(i  , N), getY(j%sizes[i], this->sizes[i]),
                                               getX(i+1, N), getY(j/sizes[i], this->sizes[i+1]));
                }
            }
        }
    }

    /* Draw Neurons */
    // std::cout << potentials[0].size()<<'\n';
    if (potentials[0].size() <= 0) return;
    for (unsigned int n = 0; n < this->sizes.size(); n++) {
        const double scale = std::min(((B - A) / static_cast<double>(N + 2) + A) / this->sizes[n], 0.025);
        for (unsigned int j = 0; j < this->sizes[n]; j++) {
            // assert(potentials[0][0] < 0);
            if (n == 0) {
                DrawCircle<Appearance::WHEAT>(getX(n, N), getY(j, this->sizes[n]), scale);
                // std::cout << potentials[0].size() <<','<< j<<'\n';
                // if (potentials[0].size() >= j) continue;
                if (potentials[0][j] > 0) {
                    // puts("daokc");
                    DrawCircle<Appearance::BLUE>(getX(n, N), getY(j, this->sizes[n]), scale * potentials[0][j]);
                } else {
                    // puts("ref");
                    // std::cout<<potentials[0][j]<<'n';
                    DrawCircle<Appearance::RED >(getX(n, N), getY(j, this->sizes[n]), -scale * potentials[0][j]);
                }
            } else if (n == this->sizes.size() - 1) {
                DrawCircle<Appearance::WHEAT>(getX(n, N), getY(j, this->sizes[n]), scale);
                // if (potentials[1].size() >= j) continue;
                if (potentials[1][j] > 0) {
                    DrawCircle<Appearance::BLUE>(getX(n, N), getY(j, this->sizes[n]), scale * potentials[1][j]);
                } else {
                    DrawCircle<Appearance::RED >(getX(n, N), getY(j, this->sizes[n]), -scale * potentials[1][j]);
                }
            } else if (drawAxons) {
                DrawCircle<Appearance::WHEAT>(getX(n, N), getY(j, this->sizes[n]), scale);
            }
        }
    }
}

std::vector<double> RecurrentNeuralNetwork::propagate(const std::vector<double>& inputs) {
    if (inputs.size() != this->sizes[0]) {
        std::cout << inputs.size() << ',' << this->sizes[0] << '\n';
        LOG("Network input did not match its topology.", LogDegree::FATAL);
    };
    auto output = net.predict(inputs);
    this->potentials[0] = inputs;
    this->potentials[1] = std::vector<double>(
        std::make_move_iterator(output.begin()),
        std::make_move_iterator(output.end())
    );
    if (potentials[1].size() != this->sizes.back()) {
        std::cout << potentials[1].size() << ',' << this->sizes.back() << '\n';
        LOG("Network input did not match its topology.", LogDegree::FATAL);
    };
    return potentials[1];
}


