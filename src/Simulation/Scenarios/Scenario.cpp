#include "Scenario.h"
#include "Creature.h"


#include <iostream>
#include <fstream>
Scenario::Scenario(Creature* creature_t, const Terrain& terrain_t, unsigned int maxTime) :
    creature(creature_t), terrain(terrain_t), maxEvaluationTime(maxTime), food() {
        int numFood = 400; // 400
        int exclusion = 100; // 100
        int distance = 2000; // 2000
        for (int i = 0; i < numFood; i++) {
            Vec f = Vec(pmRandf(exclusion, distance), pmRandf(exclusion, distance), 0);
            f.z = terrain.getHeight(f);
            food.insertAt(f.x, f.y, f);
        }

        /* Spiral */
        // int numFood = 10; // 400
        // std::ofstream myfile;
        // myfile.open ("positions.txt", std::ios::app);
        // for (int i = 0; i < 2*numFood; i++) {
        //     double angle = static_cast<double>(i)/static_cast<double>(numFood) * 2.0*3.14159;
        //     double d = 50*angle;
        //     Vec f = Vec(d*sin(angle), d*cos(angle), 0);
        //     f.z = terrain.getHeight(f);
        //     food.insertAt(f.x, f.y, f);
        //     myfile << f <<'\n';
        // }

        /* Double Circle */
      //   int numFood = 10; // 400
      //   std::ofstream myfile;
      //   myfile.open ("positions.txt", std::ios::app);
      //   for (int i = 0; i < numFood; i++) {
      //       double angle = static_cast<double>(i)/static_cast<double>(numFood) * 2.0*3.14159;
      //       double d = 200;
      //       Vec f = Vec(d*sin(angle), d*cos(angle), 0);
      //       f.z = terrain.getHeight(f);
      //       food.insertAt(f.x, f.y, f);
      //   myfile << f <<'\n';
      //   }
      //   for (int i = 0; i < numFood; i++) {
      //       double angle = static_cast<double>(i)/static_cast<double>(numFood) * 2.0*3.14159;
      //       double d = 400;
      //       Vec f = Vec(d*sin(angle), d*cos(angle), 0);
      //       f.z = terrain.getHeight(f);
      //       food.insertAt(f.x, f.y, f);
      //       myfile << f <<'\n';
      //   }
      // myfile.close();


        /* Grid */
        // int numFood = 20;
        // std::ofstream myfile;
        // myfile.open ("positions.txt", std::ios::app);
        // for (int i = -numFood/2; i <= numFood/2; i++) {
        //     for (int j = -numFood/2; j <= numFood/2; j++) {
        //         double d = 100;
        //         Vec f = Vec(d*i, d*j, 0);
        //         f.z = terrain.getHeight(f);
        //         food.insertAt(f.x, f.y, f);
        //         myfile << f <<'\n';
        //     }
        // }
}

Scenario::~Scenario() {
    delete creature;
}
