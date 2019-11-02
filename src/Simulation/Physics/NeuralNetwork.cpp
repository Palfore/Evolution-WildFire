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
RecurrentNeuralNetwork::RecurrentNeuralNetwork(int networkID, std::vector<AxonGene*> axons) : net(), sizes({}), potentialIn({}), potentialOut({}) {
    // Single RRN layer with 3 inputs and 7 hidden (using up to 2 sequences at a time) using the tanh activation function
    // Followed by a FFNN that takes those 7 hidden and connects it to 6 outputs.
    // net.bias_init(weight_init::constant(0.6));

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
        // std::cout<<'a'<<axonGene->weight<<'\n';
    }
    this->sizes = sizesCount;
    this->sizes[1] = 2;

    // std::cout << this->sizes[0] <<'\n';
    // std::cout << this->sizes[1] <<'\n';
    // std::cout << this->sizes[2] <<'\n';

    int sequence_size = 1;     // How many objects does the creature see? Default is 1, predictions changes this
    const int num_inputs = this->sizes[0];  // How much information is passed for one object?
    const int num_outputs = this->sizes[1]; // How much information are you getting out of the network?
    const int hidden_size = this->sizes[2]; // How big is the (single layer) network?
    // std::cout << this->sizes[0] <<','<< this->sizes[1]<<','<<this->sizes[2]<<'\n';
    this->net
         << recurrent_layer(rnn(num_inputs, hidden_size), sequence_size)
         << tanh_layer()
         << fully_connected_layer(hidden_size, num_outputs)
         << tanh_layer();

    this->net.weight_init(weight_init::constant(0));
    this->net.bias_init(weight_init::constant(0));


    for (const AxonGene* axonGene: axons) {
        if (axonGene->network != networkID) continue;

        // Input (numInputs x numHidden)
        if (axonGene->layer == 0) {
            (*net[2*axonGene->layer]->weights()[0])[
                axonGene->a + axonGene->b*this->sizes[axonGene->layer]
            ] = axonGene->weight;
        }

        // Recurrent (numHidden x numHidden). But (for an unknown reason - which should be investigated) there are
        // two matrices for this. (I'm assuming one might be forward and one might be backwards, but not sure).
        if (axonGene->layer == 1) {
            (*net[2*(axonGene->layer - 1)]->weights()[1])[
                axonGene->a + axonGene->b*this->sizes[axonGene->layer]
            ] = axonGene->weight;
            (*net[2*(axonGene->layer - 1)]->weights()[2])[
                axonGene->a + axonGene->b*this->sizes[axonGene->layer]
            ] = axonGene->weight;
        }

        // Output (numHidden x numOutput)
        if (axonGene->layer == 2) {
            (*net[2*(axonGene->layer - 1)]->weights()[0])[
                axonGene->a + axonGene->b*this->sizes[axonGene->layer]
            ] = axonGene->weight;
        }
    }

    // net.at<recurrent_layer>(0).bias_init(weight_init::xavier(4));

    // std::cout << "Starting: " << this->net.layer_size() << " layers\n";
    // for (unsigned int i = 0; i < this->net.layer_size(); i++) {
    //     std::vector<vec_t*> weights = this->net[i]->weights();
    //     std::cout << "There are " << weights.size() << " sets of weights in layer " << i << '\n';
    //     for (auto& a: weights) {
    //         std::cout << a->size() <<"Entries: " <<  '\n';
    //         for (auto& v: *a) {
    //             // v = pmRandf(1);
    //             std::cout<<'\t'<<v<<'\n';
    //         }
    //     }
    // }

    // // For multilayer recurrent network, does the 0 have to be incremented?
    net.at<recurrent_layer>(0).clear_state(); // Might not be needed
    net.at<recurrent_layer>(0).seq_len(1); // Fed in at a time
    // net.at<recurrent_layer>(0).bptt_max(3); // Max to remember
    // for (int i = 0; i < 20; i++) {
    //     auto x = net.predict({1, 2, 3});
    //     for (const auto& y: x) {
    //         std::cout << y <<'\n';
    //     }
    //     puts("");
    // }


    // exit(0);
}

RecurrentNeuralNetwork::RecurrentNeuralNetwork(const RecurrentNeuralNetwork& other) : net(other.net), sizes(other.sizes), potentialIn(other.potentialIn), potentialOut(other.potentialOut) {

}

RecurrentNeuralNetwork::~RecurrentNeuralNetwork() {

}


void RecurrentNeuralNetwork::draw(bool drawAxons) const {
    for (int i = 0; i < this->potentialOut.size(); i++) {
        const double pot = this->potentialOut[i];
        DrawCircle<Appearance::WHEAT>(0.8, 0.45 + i*0.1, 0.025);
        if (pot > 0) {
            DrawCircle<Appearance::BLUE>(0.8, 0.45 + i*0.1, 0.025*pot);
        } else {
            DrawCircle<Appearance::RED >(0.8, 0.45 + i*0.1, 0.025*pot);
        }
    }

    for (int i = 0; i < this->potentialIn.size(); i++) {
        for (int j = 0; j < this->potentialIn[i].size(); j++) {
            const double pot = this->potentialIn[i][j];
            const double x = 0.2 + j*0.1;
            const double y = 0.2 + i*0.1;
            DrawCircle<Appearance::WHEAT>(x, y, 0.025);
            if (pot > 0) {
                DrawCircle<Appearance::BLUE>(x, y, 0.025*pot);
            } else {
                DrawCircle<Appearance::RED >(x, y, 0.025*pot);
            }
        }
    }
}

std::vector<double> RecurrentNeuralNetwork::propagate(const std::deque<std::vector<double>>& inputs) {
    if (inputs.size() == 0) {
        return {0, 0}; // HARDCODED, should be # in output vector.
    }
    for (const auto& input: inputs) {
        if (input.size() != this->sizes[0]) {
            std::cout << input.size() << ',' << this->sizes[0] << '\n';
            LOG("Network input did not match its topology.", LogDegree::FATAL);
        }
    }
    net.at<recurrent_layer>(0).clear_state(); // Might not be needed
    net.at<recurrent_layer>(0).seq_len(1); // Fed in at a time
    net.at<recurrent_layer>(0).bptt_max(inputs.size()); // Max to remember
    // assert(inputs[0].size() == 3);

    for (unsigned int i = 0; i < inputs.size() - 1; i++) {
        this->net.predict(inputs[i]);
    }
    // std::vector<double> output = this->net.predict(inputs[inputs.size() - 1]);
    auto output = net.predict(inputs[inputs.size() - 1]);

    // // for (unsigned int i = 0; i < inputs.size(); i++) {
    // //    outputs = net.predict(inputs[i]);
    // // }
    // return std::vector<double>(
    //     std::make_move_iterator(output.begin()),
    //     std::make_move_iterator(output.end())
    // );

    // auto x = net.predict({pmRandf(1), pmRandf(1), pmRandf(1)});
    // for (int i = 0; i < 10; i++) {
    //     x = net.predict({pmRandf(1), pmRandf(1), pmRandf(1)});
    //     for (const auto& y: x) {
    //         // std::cout << y <<'\n';
    //     }
    //     // puts("");
    // }

    this->potentialIn = inputs;
    this->potentialOut = std::vector<double>(
        std::make_move_iterator(output.begin()),
        std::make_move_iterator(output.end())
    );
    assert(output.size() == 2);
    return this->potentialOut;
    // auto output = net.predict(inputs[inputs.size() - 1]);
    // std::cout << output[0] << ','<<output[1]<<'\n';

    // this->potentials[0] = inputs;
    // this->potentials[1] = std::vector<double>(
    //     std::make_move_iterator(output.begin()),
    //     std::make_move_iterator(output.end())
    // );
    // if (potentials[1].size() != this->sizes.back()) {
    //     std::cout << potentials[1].size() << ',' << this->sizes.back() << '\n';
    //     LOG("Network input did not match its topology.", LogDegree::FATAL);
    // };
    // return potentials[1];
}


