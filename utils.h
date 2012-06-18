#pragma once

#include <string>
#include <sstream>
#include <iostream>
using namespace std;

namespace am {
namespace util {

	template <class T>
	bool from_string(T &t, const string &input, ios_base &(*f)(ios_base &) = dec)
	{
		istringstream iss(input);
		return !(iss >> f >> t).fail();
	}

}
}
