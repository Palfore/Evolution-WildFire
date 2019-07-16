#include "Connection.h"

std::ostream &operator<<(std::ostream& stream, const Connection& obj){
    stream << '(' << obj.a << ", " << obj.b << ')';
    return stream;
}

inline bool Connection::operator== (const Connection& other) const {
    return ((this->a == other.a) && (this->b == other.b)) || ((this->a == other.b) && (this->b == other.a));
}

inline bool Connection::operator!= (const Connection& other) const {
    return !(*this == other);
}

inline int& Connection::operator[] (int index) {
    return (&a)[index];
}

// Overloaded operator for value access
inline const int& Connection::operator[] (int index) const {
    return (&a)[index];
}
