#pragma once

#include <string>
#include <sstream>
#include <iostream>
using namespace std;

#ifndef IS_WHITE
#	define IS_WHITE(c)	c == ' ' || c == '\t' || c == '\r' || c == '\n'
#endif

#ifndef STR
#	define STR(s) #s
#endif

namespace am {
namespace util {

	class Utils {
	public:
		/**
		 * Parses a string input into a number type assuming base 10 values (which can be changed).
		 * Returns true if the parse was successful.
		 * If the parse was unsuccessful the input value is not affected.
		 */
		template <class T>
		static bool fromString(T &t, const string &input, ios_base &(*f)(ios_base &) = dec)
		{
			istringstream iss(input);
			return !(iss >> f >> t).fail();
		}

		/**
		 * Parses a string input into a number type assuming base 10 values (which can be changed).
		 * Returns true if the parse was successful.
		 * If the parse was unsuccessful the input value is not affected.
		 */
		template <class T>
		static bool fromString(T &t, const char *input, ios_base &(*f)(ios_base &) = dec)
		{
			if (input == NULL)
			{
				return false;
			}
			istringstream iss(input);
			return !(iss >> f >> t).fail();
		}

		static int countWordLetters(const string &str, int position, bool toRight);

		static string toLowerCase(const char *input);
		static string toUpperCase(const char *input);
	};

}
}
