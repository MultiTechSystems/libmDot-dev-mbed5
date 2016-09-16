#ifndef UTILS_H
#define UTILS_H

#include <string>

//Defines a max function that can be used.
inline int mts_max(int a, int b) { return a > b ? a : b; }

//Defines a min function that can be used.
inline int mts_min(int a, int b) { return a < b ? a : b; }

///An enumeration for relational operators
enum RelationalOperator {
    GREATER, LESS, EQUAL, GREATER_EQUAL, LESS_EQUAL
};

/** A static method for getting a string representation for the RelationalOperator
* enumeration.
*
* @param relationalOperator a RelationalOperator enumeration.
* @returns the enumeration name as a string.
*/
static inline std::string getRelationalOperatorNames(RelationalOperator relationalOperator)
{
    switch(relationalOperator) {
        case GREATER:
            return "GREATER";
        case LESS:
            return "LESS";
        case EQUAL:
            return "EQUAL";
        case GREATER_EQUAL:
            return "GREATER_EQUAL";
        case LESS_EQUAL:
            return "LESS_EQUAL";
        default:
            return "UNKNOWN ENUM";
    }
}

#endif
