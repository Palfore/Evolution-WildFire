#ifndef TRAIL_H
#define TRAIL_H

#include "Vec.h"
#include <deque>
#include <iterator>

class Trail {
private:
    enum class DeletionType {CIRCULAR, HALVING, BLOCKING};
    static unsigned int constexpr DEFAULT_MAX_SIZE = 1000;
    static unsigned int constexpr DEFAULT_SAMPLING_FREQUENCY = 15;
    static constexpr DeletionType deleteType = DeletionType::CIRCULAR;
    static constexpr int SUB_DIVISIONS = 5;
    static constexpr double RADIUS = 0.6;

    std::deque<Vec> trail;
public:
    unsigned int const maxSize;
    unsigned int const samplingFrequency;


    Trail(unsigned int max_size=DEFAULT_MAX_SIZE, unsigned int sampling_frequency=DEFAULT_SAMPLING_FREQUENCY);
    virtual ~Trail();

    Vec operator[](int);
    Vec operator[](int) const;
    void clear();
    const std::deque<Vec>& getPoints() const;
    bool isFull() const;

    void addPoint(Vec point);
    void draw() const;
    long long int size() const;
};

#endif // TRAIL_H
