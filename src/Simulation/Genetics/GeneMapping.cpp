// #include "GeneMapping.h"

// #include "NodeGene.h"
// #include "MuscleGene.h"
// #include "BoneGene.h"
// #include "CubeGene.h"
// #include "AxonGene.h"

// //const std::unordered_ap<char, const std::function<Gene*(const std::string)>> Something::m = Something::create_map();
// const std::unordered_map<char, const std::function<Gene*(const std::string)>> GENE_MAP = {{
//     NodeGene::symbol, [](const std::string representation) {
//         return new NodeGene(representation);
//     }},{
//     MuscleGene::symbol, [](const std::string representation) {
//         return new MuscleGene(representation);
//     }},{
//     BoneGene::symbol, [](const std::string representation) {
//         return new BoneGene(representation);
//     }},{
//     CubeGene::symbol, [](const std::string representation) {
//         return new CubeGene(representation);
//     }},{
//     AxonGene::symbol, [](const std::string representation) {
//         return new AxonGene(representation);
//     }},
// };
