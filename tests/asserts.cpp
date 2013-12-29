#include "asserts.h"

#include <cstring>

namespace am {
namespace tests {

	void Asserts::dispErrorLine(const char *file, int line) {
		std::stringstream ss;
		ss << "Error in file " << file << '[' << line << "]";
		am_log("ERR", ss);
	}

	bool Asserts::_assert(bool a, const char *file, int line) {
		if (!a) {
			dispErrorLine(file, line);
			am_log("ERR", "- Assert failed!\n");
			return false;
		}
		return true;
	}
	bool Asserts::_assert(int a, const char *file, int line) {
		if (!a) {
			dispErrorLine(file, line);
			am_log("ERR", "- Assert failed!\n");
			return false;
		}
		return true;
	}
	bool Asserts::_assert(void *a, const char *file, int line) {
		if (!a) {
			dispErrorLine(file, line);
			am_log("ERR", "- Assert failed!\n");
			return false;
		}
		return true;
	}

	bool Asserts::_equals(const char *file, unsigned int line, const int &expected, const int &actual, bool notCompare) {
		_simple_compare(expected, actual, notCompare, file, line);
	}
	bool Asserts::_equals(const char *file, unsigned int line, base::ReturnCode expected, base::ReturnCode actual, bool notCompare) {
		//_simple_compare(expected, actual, notCompare, file, line);
		if (notCompare)
		{
			if (expected == actual)
			{
				dispNotError(base::getErrorName(expected), file, line);
				return false;
			}
		}
		else
		{
			if (expected != actual)
			{
				dispError(base::getErrorName(expected), base::getErrorName(actual), file, line);
				return false;
			}
		}
		return true;
	}

	bool Asserts::_equals(const char *file, unsigned int line, const unsigned int &expected, const unsigned int &actual, bool notCompare) {
		_simple_compare(expected, actual, notCompare, file, line);
	}
	bool Asserts::_equals(const char *file, unsigned int line, const double &expected, const double &actual, bool notCompare) {
		_simple_compare(expected, actual, notCompare, file, line);
	}

	bool Asserts::_equals(const char *file, unsigned int line, const char *expected, const char *actual, bool notCompare) {
		if (notCompare) {
			if (strcmp(expected, actual) == 0) {
				dispNotError(expected, file, line);
				return false;
			}
			return true;
		}
		if (strcmp(expected, actual) != 0) {
			dispError(expected, actual, file, line);
			return false;
		}
		return true;
	}

	bool Asserts::_equals(const char *file, unsigned int line, const double &expected, const double &actual, bool notCompare, double delta) {
		double diff = expected - actual;
		bool equal = diff >= -delta && diff <= delta;
		if (notCompare) {
			if (equal) {
				dispNotError(expected, actual, delta, file, line);
				return false;
			}
			return true;
		}
		if (!equal) {
			dispError(expected, actual, delta, file, line);
			return false;
		}
		return true;
	}

	bool Asserts::_equalsStr(const char *file, unsigned int line, const char *expected, const char *actual, bool notCompare) {
		int cmp = strcmp(expected, actual);
		if (notCompare && cmp == 0) {
			dispNotError(expected, file, line);
			return false;
		}
		else if (!notCompare && cmp != 0) {
			dispError(expected, actual, file, line);
			return false;
		}
		return true;
	}
	bool Asserts::_equalsStr(const char *file, unsigned int line, const char *expected, const std::string &actual, bool notCompare) {
		int cmp = strcmp(expected, actual.c_str());
		if (notCompare && cmp == 0) {
			dispNotError(expected, file, line);
			return false;
		}
		else if (!notCompare && cmp != 0) {
			dispError(expected, actual.c_str(), file, line);
			return false;
		}
		return true;
	}
/*
    template <>
    bool Asserts::_equalsVec(const char *file, unsigned int line, const math::Vector2<T> &actual, const math::Vector2<T> &expected, bool notCompare, double delta)
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
    template <>
    bool Asserts::_equalsVec(const char *file, unsigned int line, const math::Vector4<T> &actual, const math::Vector4<T> &expected, bool notCompare, double delta)
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
    */
}
}
