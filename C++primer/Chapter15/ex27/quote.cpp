//
// Chapter15 Exercise27
//

#include <iostream>
#include "quote.h"

void Quote::debug() const
{
	std::cout << "data member of the class is:\n"
		      << "bookNo = " << this->bookNo << " "
			  << "price = "  << this->price << ".\n";
}