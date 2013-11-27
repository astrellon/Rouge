#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#ifndef IS_WHITE
#	define IS_WHITE(c)	c == ' ' || c == '\t' || c == '\r' || c == '\n'
#endif

#ifndef STR
#	define STR(s) #s
#endif

#include <base/handle.h>

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
		static bool fromString(T &t, const std::string &input, std::ios_base &(*f)(std::ios_base &) = std::dec)
		{
			std::istringstream iss(input);
			return !(iss >> f >> t).fail();
		}

		template <class T>
		static T tryFromString(const std::string &input, T defaultValue, std::ios_base &(*f)(std::ios_base &) = std::dec)
		{
			T t;
			std::istringstream iss(input);
			bool result = !(iss >> f >> t).fail();
			if (result)
			{
				return t;
			}
			return defaultValue;
		}

		template <class T>
		static std::string toString(const T &input)
		{
			std::stringstream ss;
			ss << input;
			return ss.str();
		}

		/**
		 * Parses a string input into a number type assuming base 10 values (which can be changed).
		 * Returns true if the parse was successful.
		 * If the parse was unsuccessful the input value is not affected.
		 */
		template <class T>
		static bool fromString(T &t, const char *input, std::ios_base &(*f)(std::ios_base &) = std::dec)
		{
			if (input == nullptr)
			{
				return false;
			}
			std::istringstream iss(input);
			return !(iss >> f >> t).fail();
		}

		static int countWordLetters(const std::string &str, int position, bool toRight);

		static std::string toLowerCase(const char *input);
		static std::string toUpperCase(const char *input);

		static float randf();
		static float randf(float lower, float upper);
		static int rand(int lower, int upper);
		static void setRandSeed(unsigned int seed);

		template <class T>
		static bool listContains(const std::vector<T> &list, const T &item)
		{
			return find(list, item) != -1;
		}

		template <class T>
		static int find(const std::vector<T> &list, const T &item)
		{
			size_t size = list.size();
			int result = -1;
			for (size_t i = 0; i < size; ++i)
			{
				if (list[i] == item)
				{
					result = static_cast<int>(i);
					break;
				}
			}
			return result;
		}

		static bool fileExists(const char *filename);

	};

}
}
