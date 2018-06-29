#include "TestSuite.h"

#include <chrono>
#include <ctime>
#include <vector>
#include <iostream>
#include "Connection.h"
#include "Phylogeny.h"

TestSuite::TestSuite() {
    //ctor
}

TestSuite::~TestSuite() {
    //dtor
}


void TestSuite::connectionTest() {
    struct TestConnection {
        const int actualLoops;
        const int actualTrails;
        std::vector<Connection> connections;
        TestConnection(int l, int t, std::vector<Connection> c) : actualLoops(l), actualTrails(t), connections(c) {}

    };
    std::vector<TestConnection> testConnections = {
        TestConnection(0, 0, {   }   ),

        TestConnection(0, 1, {   {0,1}  }),

        TestConnection(0, 1, {   {0,1}, {1,2}   }),
        TestConnection(1, 0, {   {0,1}, {1,2}, {2,0} }),

        TestConnection(0, 1, {   {0,1}, {1,2}, {2,3} }),
        TestConnection(1, 1, {   {0,1}, {1,2}, {2,3}, {3,1} }),
        TestConnection(1, 0, {   {0,1}, {1,2}, {2,3}, {3,0} }),
        TestConnection(3, 0, {   {0,1}, {1,2}, {2,3}, {3,0}, {3,1} }),
        TestConnection(5, 0, {   {0,1}, {1,2}, {2,3}, {3,0}, {3,1}, {0,2} }),

        TestConnection(0, 1, {   {0,1}, {1,2}, {2,3}, {3,4}  }),
        TestConnection(1, 1, {   {0,1}, {1,2}, {2,3}, {3,4}, {4,2} }),
        TestConnection(8, 0, {   {0,1}, {1,2}, {2,3}, {3,4}, {4,0},
                                    {0,2}, {0,3},
                                    {1,3}, {1,4},
                                    {2,4}   }),

        TestConnection(1, 2, {    {0,1},{1,2},{2,3},{2,4},{3,4}, {4,5}}),
        TestConnection(1, 0, {    {0,1},{1,2},{2,0}}),
        TestConnection(0, 1, {    {0,1},{1,2},{2,3}}),
    };

    for (const auto& test: testConnections) {
        int numLoops = Phylogeny::getNumLoops(test.connections);
        int numTrails = Phylogeny::getNumTrails(test.connections);

        if ((test.actualLoops == numLoops) && (test.actualTrails == numTrails)) {
            constexpr int numSamples = 1000;
            auto time1 = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < numSamples; i++) Phylogeny::getNumLoops(test.connections);
            auto time2 = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < numSamples; i++) Phylogeny::getNumTrails(test.connections);
            auto time3 = std::chrono::high_resolution_clock::now();

            std::cout << "Passed: with " << numLoops << " and " << numTrails <<
                " for " << test.connections.size() <<" connections, in " << std::chrono::duration<double, std::milli>(time2-time1).count()/numSamples*1000 << " ms, and " <<
                    std::chrono::duration<double, std::milli>(time3 - time2).count()/numSamples*1000 << " ms\n";
        } else {
            std::cout << "Failed: " << numTrails << '/' << test.actualTrails << '\n';
        }
    }
}
