#ifndef COMPONENT_H
#define COMPONENT_H

struct Component {
	Component();
    virtual ~Component() = 0;

    virtual void draw() const = 0;
    virtual void update(int t) = 0;
};

#endif // COMPONENT_H
