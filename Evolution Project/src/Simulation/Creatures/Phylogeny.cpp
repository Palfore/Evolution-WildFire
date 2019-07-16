#include "Phylogeny.h"
#include "utility.h"
#include "Genome.h"
#include "BoneGene.h"
#include "MuscleGene.h"
#include "NodeGene.h"
#include "AxonGene.h"

#include <numeric>

bool Phylogeny::scientificNames = false;

Phylogeny::Phylogeny(Genome g) : numNodes(0), numMuscles(0), numBones(0), numAxons(0), numLoops(0), numTrails(0) {
    std::vector<MuscleGene*> muscleGenes = g.getGenes<MuscleGene>();
    std::vector<BoneGene*> boneGenes = g.getGenes<BoneGene>();

    numNodes = g.getGenes<NodeGene>().size();
    numMuscles = muscleGenes.size();
    numBones = boneGenes.size();
    numAxons = g.getGenes<AxonGene>().size();


    std::vector<Connection> connections = {};

    for (const MuscleGene* m: muscleGenes) {
        connections.push_back(m->connection);
    }
    for (const BoneGene* b: boneGenes) {
        connections.push_back(b->connection);
    }

    numLoops = getNumLoops(connections);
    numTrails = getNumTrails(connections, numNodes);
}

Phylogeny::~Phylogeny() {
    //dtor
}

std::vector<std::string> Phylogeny::getPhylogenyNames() const {
    return {getDomain(), getKingdom(), getPhylum(), getOrder(), getGenus(), getSpecies()};
}

std::vector<std::array<std::string, 2>> Phylogeny::getPhylogenyDict() const {
    std::vector<std::array<std::string, 2>> dictionary = {};
    const auto phylogenyNames = getPhylogenyNames();
    for (unsigned int i = 1; i < phylogenyNames.size(); i++) { // skip domain
        dictionary.push_back({scientificNames?RANKS_scientific[i]:RANKS[i], phylogenyNames[i]});
    }
    return dictionary;
}

std::string Phylogeny::toString() const {
    auto names = getPhylogenyNames();
    return std::accumulate(names.cbegin(), names.cend(), std::string(""), [](std::string sum, std::string name){
                                return sum + name + " ";
                           });
}

std::string Phylogeny::getKingdom() const {
    return scientificNames ? "Moves" : "Animalia";
}

std::string Phylogeny::getDomain() const {
    return "Eukaria";
}

std::string Phylogeny::getPhylum() const {
    std::string name = "";
    if (scientificNames) {
        name += this->numNodes > 0 ? "Nodes " : "";
        name += this->numMuscles > 0 ? "Muscles " : "";
        name += this->numBones > 0 ? "Bones " : "";
        name += this->numAxons > 0 ? "Brain " : "";
    } else {
        name += this->numNodes > 0 ? "" : "";
        name += this->numMuscles > 0 ? "fl" : "";  // FLuid
        name += this->numBones > 0 ? "id" : "";    // rigID
        name += this->numAxons > 0 ? "ought" : ""; // thOUGHT
    }
    return utility::toTitle(name);
}

std::string Phylogeny::getOrder() const {
    static const std::vector<std::string> names = {"nu", "wahd", "twi", "tra", "uda", "cic", "xi", "tept", "ouit", "nix", "dom"};
    std::string name = "";
    if (scientificNames) {
        name += std::to_string(numNodes) + "n ";
        name += std::to_string(numMuscles) + "m ";
        name += std::to_string(numBones) + "b ";
        name += std::to_string(numAxons) + "a ";
    } else {
        name += names[numNodes % names.size()];
        name += names[numMuscles % names.size()];
        name += names[numBones % names.size()];
    }
    return utility::toTitle(name);
}

std::string Phylogeny::getGenus() const {
    static const std::vector<std::string> names = {"op", "wawn", "twil", "chok", "maud", "pent", "hiss", "pol", "uit", "nil", "kekt"};
    std::string name = "";
    if (scientificNames) {
        name += std::to_string(numLoops) + "Loops ";
        name += std::to_string(numTrails) + "Trails ";
    } else {
        name += names[numLoops % names.size()];
        name += names[numTrails % names.size()];
    }
    return utility::toTitle(name);
}

std::string Phylogeny::getSpecies() const {
    return "Walker";
}


/**** Getting Number of Loops and Trails ****/
static bool sameChain(std::vector<int> chain1, std::vector<int> chain2) {
    return std::all_of(chain1.cbegin(), chain1.cend(), [chain2](int link){return std::find(chain2.begin(), chain2.end(), link) != chain2.end();}) &&
            std::all_of(chain2.cbegin(), chain2.cend(), [chain1](int link){return std::find(chain1.begin(), chain1.end(), link) != chain1.end();});
}

static bool uniqueChain(std::vector<int> chain, std::vector<std::vector<int>> chains) {
    return !std::any_of(chains.begin(), chains.end(), [chain](std::vector<int> c){return sameChain(chain, c);});
}

static int followChain(int starting, int lookingFor, unsigned int i, std::vector<Connection> connections, std::vector<std::vector<int>>& chainsSeen) {
    int numLoops = 0;
    std::vector<int> chain = {starting};
    for (unsigned int j = 0; j < connections.size(); j++) {
        if (i == j) continue;
        int a = connections[j].a;
        int b = connections[j].b;
        if (lookingFor == a) {
            chain.push_back(a);
            lookingFor = b;
            if (b == starting) {
                chain.push_back(starting);
                if (uniqueChain(chain, chainsSeen)) {
                    chainsSeen.push_back(chain);
                    numLoops++;
                }
            }
        } else if (lookingFor == b) {
            chain.push_back(b);
            lookingFor = a;
            if (a == starting) {
                chain.push_back(starting);
                if (uniqueChain(chain, chainsSeen)) {
                    chainsSeen.push_back(chain);
                    numLoops++;
                }
            }
        }
    }
    return numLoops;
}

int Phylogeny::getNumLoops(std::vector<Connection> connections) {
    int num = 0;
    std::vector<std::vector<int>> chainsSeen = {};
    for (unsigned int i = 0; i < connections.size(); i++) {
        int starting = connections[i].a;
        int lookingFor = connections[i].b;
        num += followChain(starting, lookingFor, i, connections, chainsSeen);
    }
    for (unsigned int i = 0; i < connections.size(); i++) {
        int starting = connections[i].b;
        int lookingFor = connections[i].a;
        num += followChain(starting, lookingFor, i, connections, chainsSeen);
    }
    return num;
}



static int getNumCk(std::vector<std::vector<int>> A, int k) {
    return std::accumulate(A[k].begin()+k, A[k].end(), 0) +
                std::accumulate(A.begin(), A.begin()+k, 0, [k](int sum, std::vector<int> a_j) {
                    return sum + a_j[k];
                });
}

static std::vector<int> getNext(std::vector<std::vector<int>> A, int index) {
    std::vector<int> next = {};
    for (unsigned int i = 0; i < A.size(); i++) {
        if (A[index][i] >= 1 || A[i][index] >= 1) { // Have a connection to index
            next.push_back(i);
        }
    }
    return next;
}

static std::vector<int> getTrail(std::vector<std::vector<int>> A, int start) {
    std::vector<int> trail = {start};
    while (true) { // Since this isnt an important feature, this loop should have a limit so it doesnt hang the game if it breaks.
        std::vector<int> next = getNext(A, start);
        switch(next.size()) {
            case 1:
                if (std::find(trail.begin(), trail.end(), next[0]) == trail.end()) { // At begining
                    start = next[0];
                } else { // At other end, think: 0-1-2-3
                    goto finishedLoop;
                }
                break;
            case 2: // Start next iteration at connected node
                start = std::find(trail.begin(), trail.end(), next[0]) != trail.end() ? next[1] : next[0];
                break;
            default: // Three or more means trail is done
                goto finishedLoop;
        }
        trail.push_back(start);
    }
    finishedLoop:

    return trail;
}


int Phylogeny::getNumTrails(std::vector<Connection> connections, int n) {
    if (connections.empty()) return 0;
    if (n < 0) { // If not given
        for (auto const& c: connections) {
            n = std::max(std::max(c.a, c.b), n) + 1;
        }
    }

    /* Construct the Adjacency Matrix */
    std::vector<std::vector<int>> A(n, std::vector<int>(n));
    for (auto const& c: connections) {
        int a = std::min(c.a, c.b);
        int b = std::max(c.a, c.b);
        (A[a][b])++;
    }

    /* Find the Nodes at the End of Trails */
    std::vector<int> ends = {};
    for (int k = 0; k < n; k++) {
        if (getNumCk(A, k) == 1) { // Single connection => trail ending
            ends.push_back(k);
        }
    }

    /* Add Unique Trails */
    std::vector<std::vector<int>> trails = {};
    for (const auto& e: ends) {
        std::vector<int> trail = getTrail(A, e);
        std::sort(trail.begin(), trail.end());
        if (!any_of(trails.begin(), trails.end(), [trail](std::vector<int> t){
                        std::sort(t.begin(), t.end());
                        return t == trail;
                   })) {
            trails.push_back(trail);
        }
    }

    return trails.size();
}
