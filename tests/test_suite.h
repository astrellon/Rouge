#pragma once

#include <string>
#include <vector>
#include <sstream>
using std::string;
using std::vector;
using std::stringstream;

#include <log/logger.h>

#define runSuite(s)	\
	{	\
		s suite;	\
		suite.run();	\
		suiteCount++;	\
	}
	
#define runCase(f)	\
	beforeCase(); \
	try { \
	if (!f()) { \
		testsFailed++; \
	}}	catch (...)	{ \
		testsFailed++; \
	}\
	totalTests++;	\
	afterCase();

namespace am {
namespace tests {

	class TestSuite {
	public:
		TestSuite() {
			testsFailed = 0;
			totalTests = 0;
		}
		~TestSuite() {}
		virtual void beforeSuite() {}
		virtual void afterSuite() {}
	
		virtual void beforeCase() {}
		virtual void afterCase() {}
	
		virtual void run() {
			stringstream ss;
			ss << "Starting tests for '" << getSuiteName() << "'";
			am_log("SUITE", ss);
			beforeSuite();
			runCases();
			ss.str("");
			try
			{

			}
			catch (...)
			{

			}
			ss << (totalTests - testsFailed) << " of " << totalTests << " passed ";

			if (testsFailed > 0) {
				ss << "FAIL!";
				addFailedTest(getSuiteName());
			}
			am_log("SUITE", ss);
			afterSuite();
		}
		virtual void runCases() = 0;
	
		virtual const char *getSuiteName() const {
			return "Unnamed";
		}

		static void addFailedTest(const char *testName) {
			sFailedTests.push_back(string(testName));
		}
		static const vector<string> &getFailedTests() {
			return sFailedTests;
		}
	protected:
		int testsFailed;
		int totalTests;

		static vector<string> sFailedTests;
	};

}
}
