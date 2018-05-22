#ifndef MUSCLE_H
#define MUSCLE_H

#include "Genetics/Gene.h"


class Muscle : public Gene {
    public:
        /** Default constructor */
        Muscle();

        /** Default destructor */
        virtual ~Muscle();

        /** Copy constructor
         *  \param other Object to copy from
         */
        Muscle(const Muscle& other);

        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        Muscle& operator=(const Muscle& other);

    protected:

    private:
};

#endif // MUSCLE_H
