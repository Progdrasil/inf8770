#ifndef QUANTIF_TYPE_H
#define QUANTIF_TYPE_H

#include <iostream>

enum quantifType  {
	BASIC,
	LEVEL,
	CONSTANT,
	TRI_DIAGONAL
};

inline std::istream& operator>>(std::istream& in, quantifType& type)
{
    std::string token;
    in >> token;
    if (token == "basic")
        type = quantifType::BASIC;
    else if (token == "level")
        type = quantifType::LEVEL;
	else if (token == "constant")
		type = quantifType::CONSTANT;
	else if (token == "tri-diagonal")
		type = quantifType::TRI_DIAGONAL;
	else
        in.setstate(std::ios_base::failbit);
    return in;
};

#endif
