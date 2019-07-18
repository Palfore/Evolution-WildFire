/* This file contains the map between gene getSymbol()s and their string-constructor.
    New genes need to be added here.
*/

#include "Gene.h"
#include "NodeGene.h"
#include "MuscleGene.h"
#include "BoneGene.h"
#include "CubeGene.h"
#include "AxonGene.h"

#include <functional>
#include <unordered_map>

const std::unordered_map<char, const std::function<Gene*(const std::string)>> GENE_MAP = {{
    NodeGene::symbol, [](const std::string representation) {
        return new NodeGene(representation);
    }},{
    MuscleGene::symbol, [](const std::string representation) {
        return new MuscleGene(representation);
    }},{
    BoneGene::symbol, [](const std::string representation) {
        return new BoneGene(representation);
    }},{
    CubeGene::symbol, [](const std::string representation) {
        return new CubeGene(representation);
    }},{
    AxonGene::symbol, [](const std::string representation) {
        return new AxonGene(representation);
    }},
};
