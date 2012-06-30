#include "imanaged.h"

#include "logger.h"
#include <sstream>

namespace am {
namespace util {

	IManaged::IManaged() :
		mRefCounter(0)
	{
		//am_log("TEST", "IManaged constructor");
	}
	IManaged::~IManaged()
	{
		//am_log("TEST", "IManaged destructor");
	}

	void IManaged::release()
	{
		mRefCounter--;
		/*std::stringstream ss;
		ss << "IManaged release: " << mRefCounter;
		am_log("TEST", ss.str().c_str());*/
		
		if (mRefCounter <= 0)
		{
			delete this;
		}
	}

	void IManaged::retain()
	{
		mRefCounter++;
		/*std::stringstream ss;
		ss << "IManaged retain: " << mRefCounter;
		am_log("TEST", ss.str().c_str());*/
	}
}
}
