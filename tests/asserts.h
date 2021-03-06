#pragma once

#define IN_TEST_CASE

#include <iostream>
#include <string>
#include <sstream>

#include <log/logger.h>

#include <base/return_codes.h>

#include <math/vector.h>
#include <math/matrix.h>

namespace am {
namespace tests {

	class Asserts {
	public:
		static void dispErrorLine(const char *file, int line);
		static bool _assert(bool a, const char *file, int line);
		static bool _assert(int a, const char *file, int line);
		static bool _assert(void *a, const char *file, int line);

		template <class T>
		static void dispError(const T &expected, const T &actual, const char *file, unsigned int line) 
		{
			dispErrorLine(file, line);
			std::stringstream ss;
			ss << "\n- Expected: >" << expected << "<\n- Actual: >" << actual << "<\n";
			am_log("ERR", ss);
		}

		template <class T>
		static void dispNotError(const T &expected, const char *file, unsigned int line) 
		{
			dispErrorLine(file, line);
			std::stringstream ss;
			ss << "- Did not expect: " << expected << "\n";
			am_log("ERR", ss);
		}

		template <class T>
		static void dispError(const T &expected, const T &actual, double delta, const char *file, unsigned int line) 
		{
			dispErrorLine(file, line);
			std::stringstream ss;
			ss << "- Expected: " << expected << "\n- Actual  : " << actual << "\n- Delta   : " << delta << "\n";
			am_log("ERR", ss);
		}

		template <class T>
		static void dispNotError(const T &expected, double delta, const char *file, unsigned int line) 
		{
			dispErrorLine(file, line);
			std::stringstream ss;
			ss << "- Did not expect: " << expected << "\n- Delta   : " << delta << "\n";
            am_log("ERR", ss);
		}

        template <class T>
        static void concatArray(const T *array, int n, std::stringstream &ss)
        {
            ss << '[';
            for (int i = 0; i < n; i++) {
                if (i > 0) {
                    ss << ", ";
                }
                ss << array[i];
            }
            ss << ']';
        }

        template <class T>
        static void dispArrayError(const T *expected, const T *actual, int n, int errn, double delta, const char *file, unsigned int line)
        {
            dispErrorLine(file, line);
            std::stringstream ss;
            ss <<   "- Expected: ";
            concatArray(expected, n, ss);
            ss << "\n- Actual  : ";
            concatArray(actual, n, ss);
            ss << "\n- Delta   : " << delta
               << "\n- Index   : " << errn;
            am_log("ERR", ss);
        }
        template <class T>
        static void dispArrayNotError(const T *expected, int n, int errn, double delta, const char *file, unsigned int line)
        {
            dispErrorLine(file, line);
            std::stringstream ss;
            ss << "- Did not expect: ";
            concatArray(expected, n, ss);
            ss << "\n- Delta: " << delta
               << "\n- Index: " << errn;
            am_log("ERR", ss);
        }

#define _simple_compare(e, a, r, f, l)	\
	if (r) {	\
	if (e == a) { Asserts::dispNotError(e, f, l); return false; }	return true;	\
	} else {	\
	if (e != a) { Asserts::dispError(e, a, f, l);	return false; }	return true; }

		static bool _equals(const char *file, unsigned int line, const int &expected, const int &actual, bool notCompare);
		static bool _equals(const char *file, unsigned int line, base::ReturnCode expected, base::ReturnCode actual, bool notCompare);
		static bool _equals(const char *file, unsigned int line, const unsigned int &expected, const unsigned int &actual, bool notCompare);
		static bool _equals(const char *file, unsigned int line, const double &expected, const double &actual, bool notCompare);
		static bool _equals(const char *file, unsigned int line, const char *expected, const char *actual, bool notCompare);
		static bool _equals(const char *file, unsigned int line, const double &expected, const double &actual, bool notCompare, double delta=0.00001);
		
		static bool _equalsStr(const char *file, unsigned int line, const char *expected, const char *actual, bool notCompare);
		static bool _equalsStr(const char *file, unsigned int line, const char *expected, const std::string &actual, bool notCompare);

        template <class T>
        static bool _equalsVec(const char *file, unsigned int line, const math::Vector2<T> &expected, const math::Vector2<T> &actual, bool notCompare, double delta=0.00001)
        {
            bool equal = expected.equals(actual, delta);
            if (notCompare && equal) {
                dispNotError(expected, file, line);
                return false;
            }
            else if (!notCompare && !equal) {
                dispError(expected, actual, file, line);
                return false;
            }
            return true;
        }
        template <class T>
        static bool _equalsVec(const char *file, unsigned int line, const math::Vector4<T> &expected, const math::Vector4<T> &actual, bool notCompare, double delta=0.00001)
        {
            bool equal = expected.equals(actual, delta);
            if (notCompare && equal) {
                dispNotError(expected, file, line);
                return false;
            }
            else if (!notCompare && !equal) {
                dispError(expected, actual, file, line);
                return false;
            }
            return true;
        }
        template <class T>
        static bool _equalsArray(const char *file, unsigned int line, const T *expected, const T *actual, bool notCompare, double delta=0.00001, int n=16)
        {
            bool equal = true;
            int i = 0;
            for (; i < n; i++) {
                double diff = expected[i] - actual[i];
                if (diff < -delta || diff > delta) {
                    equal = false;
                    break;
                }
            }
            if (notCompare && equal) {
                dispArrayNotError(expected, n, i, delta, file, line);
                return false;
            }
            else if (!notCompare && !equal) {
                dispArrayError(expected, actual, n, i, delta, file, line);
                return false;
            }
            return true;
        }
	};
}
}

#ifdef IN_TEST_CASE
#	define assert(a) \
	if (!Asserts::_assert(a, __FILE__, __LINE__)) { return false; }

//  General equals
#	define am_equals(expected, actual) \
	if (!Asserts::_equals(__FILE__, __LINE__, expected, actual, false)) { return false; }
#	define am_equalsDelta(expected, actual, delta) \
	if (!Asserts::_equals(__FILE__, __LINE__, expected, actual, false, delta)) { return false; }

#	define am_notEquals(expected, actual) \
	if (!Asserts::_equals(__FILE__, __LINE__, expected, actual, true)) { return false; }
#	define am_notEqualsDelta(expected, actual, delta) \
	if (!Asserts::_equals(__FILE__, __LINE__, expected, actual, true, delta)) { return false; }

//  String equals
#	define am_equalsStr(expected, actual)	\
	if (!Asserts::_equalsStr(__FILE__, __LINE__, expected, actual, false)) { return false; }
#	define notEqualsStr(expected, actual)	\
	if (!Asserts::_equalsStr(__FILE__, __LINE__, expected, actual, true)) { return false; }

//  Vector equals
#   define am_equalsVec(expected, actual)   \
    if (!Asserts::_equalsVec(__FILE__, __LINE__, expected, actual, false)) { return false; }
#   define notEqualsVec(expected, actual)   \
    if (!Asserts::_equalsVec(__FILE__, __LINE__, expected, actual, true)) { return false; }

//  Array equals
#   define am_equalsArray(expected, actual, n)  \
    if (!Asserts::_equalsArray(__FILE__, __LINE__, expected, actual, false, 0.00001, n)) { return false; }
#   define am_notEqualsArray(expected, actual, n)  \
    if (!Asserts::_equalsArray(__FILE__, __LINE__, expected, actual, true, 0.00001, n)) { return false; }

#else
#	define assert(a) _assert(a, __FILE__, __LINE__)

#	define am_equals(expected, actual) _equals(__FILE__, __LINE__, expected, actual, false)
#	define am_equalsDelta(expected, actual, delta) _equals(__FILE__, __LINE__, expected, actual, false, delta)

#	define am_notEquals(expected, actual) _equals(__FILE__, __LINE__, expected, actual, true)
#	define am_notEqualsDelta(expected, actual, delta) _equals(__FILE__, __LINE__, expected, actual, true, delta)
#endif
