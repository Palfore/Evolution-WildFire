#ifndef MULTITHREAD_H
#define MULTITHREAD_H

#include <functional>
#include <thread>
#include <vector>

class Creature;
class MultiThread {
    public:
        static constexpr int SIMULATION_TIME = 4000;

        std::vector<double> fitnesses;
        MultiThread() : fitnesses({}), t(), finished(false) {}

        void spawn(std::function<void(std::vector<Creature>, std::vector<double> &, bool &)> f, std::vector<Creature> creatures);
        bool isFinished() const;
        static void processCreatures(std::vector<Creature> creatures, std::vector<double> & fitnesses, bool & done);
        static void spawnChildren(std::vector<MultiThread*> mt, std::vector<Creature> creatures);

    private:
        std::thread t;
        bool finished;
};


#endif // MULTITHREAD_H
