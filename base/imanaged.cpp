#include "imanaged.h"

namespace am {
namespace base {

	IManaged::IManaged() :
		mRefCounter(0)
	{
	}
	IManaged::IManaged(const IManaged &copy) :
		mRefCounter(0)
	{
	}
	IManaged::~IManaged()
	{
	}

	void IManaged::release()
	{
		mRefCounter--;
		
		if (mRefCounter <= 0)
		{
			delete this;
		}
	}

	void IManaged::retain()
	{
		mRefCounter++;
	}

	int IManaged::getReferenceCounter() const
	{
		return mRefCounter;
	}

}
}
