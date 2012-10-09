#pragma once

#define IN_TEST_CASE

#include <iostream>
#include <string>
#include <sstream>

#include <log/logger.h>

using namespace std;

namespace am {
namespace tests {

	class Asserts {
	public:
		static void dispErrorLine(const char *file, int line);
		static bool _assert(bool a, const char *file, int line);

		template <class T>
		static void dispError(const T &expected, const T &actual, const char *file, unsigned int line) 
		{
			dispErrorLine(file, line);
			stringstream ss;
			ss << "- Expected: " << expected << "\n- Actual  : " << actual << "\n";
			am_log("ERR", ss);
		}

		template <class T>
		static void dispNotError(const T &expected, const char *file, unsigned int line) 
		{
			dispErrorLine(file, line);
			stringstream ss;
			ss << "- Did not expect: " << expected << "\n";
			am_log("ERR", ss);
		}

		template <class T>
		static void dispError(const T &expected, const T &actual, double delta, const char *file, unsigned int line) 
		{
			dispErrorLine(file, line);
			stringstream ss;
			ss << "- Expected: " << expected << "\n- Actual  : " << actual << "\n- Delta   : " << delta << "\n";
			am_log("ERR", ss);
		}

		template <class T>
		static void dispNotError(const T &expected, const T &actual, double delta, const char *file, unsigned int line) 
		{
			dispErrorLine(file, line);
			stringstream ss;
			ss << "- Did not expect: " << expected << "\n- Actual: " << actual << "\n- Delta   : " << delta << "\n";
		}

#define _simple_compare(e, a, r, f, l)	\
	if (r) {	\
	if (e == a) { Asserts::dispNotError(e, f, l); return false; }	return true;	\
	} else {	\
	if (e != a) { Asserts::dispError(e, a, f, l);	return false; }	return true; }

		static bool _equals(const char *file, unsigned int line, const int &expected, const int &actual, bool notCompare);
		static bool _equals(const char *file, unsigned int line, const unsigned int &expected, const unsigned int &actual, bool notCompare);
		static bool _equals(const char *file, unsigned int line, const double &expected, const double &actual, bool notCompare);
		static bool _equals(const char *file, unsigned int line, const char *expected, const char *actual, bool notCompare);
		static bool _equals(const char *file, unsigned int line, const double &expected, const double &actual, bool notCompare, double delta=0.00001);
		
		static bool _equalsStr(const char *file, unsigned int line, const char *expected, const char *actual, bool notCompare);

	};
}
}

#ifdef IN_TEST_CASE
#	define assert(a) \
	if (!Asserts::_assert(a, __FILE__, __LINE__)) { return false; }

#	define equals(expected, actual) \
	if (!Asserts::_equals(__FILE__, __LINE__, expected, actual, false)) { return false; }
#	define equalsDelta(expected, actual, delta) \
	if (!Asserts::_equals(__FILE__, __LINE__, expected, actual, false, delta)) { return false; }

#	define notEquals(expected, actual) \
	if (!Asserts::_equals(__FILE__, __LINE__, expected, actual, true)) { return false; }
#	define notEqualsDelta(expected, actual, delta) \
	if (!Asserts::_equals(__FILE__, __LINE__, expected, actual, true, delta)) { return false; }

#	define equalsStr(expected, actual)	\
	if (!Asserts::_equalsStr(__FILE__, __LINE__, expected, actual, false)) { return false; }
#	define notEqualsStr(expected, actual)	\
	if (!Asserts::_equalsStr(__FILE__, __LINE__, expected, actual, true)) { return false; }
#else
#	define assert(a) _assert(a, __FILE__, __LINE__)

#	define equals(expected, actual) _equals(__FILE__, __LINE__, expected, actual, false)
#	define equalsDelta(expected, actual, delta) _equals(__FILE__, __LINE__, expected, actual, false, delta)

#	define notEquals(expected, actual) _equals(__FILE__, __LINE__, expected, actual, true)
#	define notEqualsDelta(expected, actual, delta) _equals(__FILE__, __LINE__, expected, actual, true, delta)
#endif
