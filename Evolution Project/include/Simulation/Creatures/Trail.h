#ifndef TRAIL_H
#define TRAIL_H

#include <deque>
#include "Vec.h"

class Trail {
private:
    enum class DeletionType {CIRCULAR, HALVING, BLOCKING};
    static unsigned int constexpr MAX_TRAIL_SIZE = 500;
    static constexpr DeletionType deleteType = DeletionType::CIRCULAR;
    static constexpr int SUB_DIVISIONS = 10;
    static constexpr double RADIUS = 0.3;

    std::deque<Vec> trail;
public:
    static int constexpr SAMPLING_FREQUENCY = 20; // Record every n frames

    Trail();
    virtual ~Trail();

    void addPoint(Vec point);
    void draw() const;
};

#endif // TRAIL_H
