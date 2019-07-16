#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
struct Connection {
    int a;
    int b;
    Connection(int a_t, int b_t): a(a_t), b(b_t) {};

    bool operator== (const Connection&) const;
    bool operator!= (const Connection&) const;
    int& operator[] (int index);
    const int& operator[] (int index) const;

    friend std::ostream &operator<<(std::ostream&, const Connection&);
};



#endif // CONNECTION_H
