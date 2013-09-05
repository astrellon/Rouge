#include "imanaged.h"

namespace am {
namespace base {

	IManaged::IManaged() :
		mRefCounter(0)
#ifdef _DEBUG
		,mReleaseCallback(nullptr),
		mRetainCallback(nullptr)
#endif
	{
	}
	IManaged::IManaged(const IManaged &copy) :
		mRefCounter(0)
#ifdef _DEBUG
		,mReleaseCallback(nullptr),
		mRetainCallback(nullptr)
#endif
	{
	}
	IManaged::~IManaged()
	{
	}

	void IManaged::release()
	{
		mRefCounter--;

#ifdef _DEBUG
		if (mReleaseCallback)
		{
			mReleaseCallback(this);
		}
#endif
		if (mRefCounter <= 0)
		{
			delete this;
		}
	}

	void IManaged::retain()
	{
		mRefCounter++;
#ifdef _DEBUG
		if (mRetainCallback)
		{
			mRetainCallback(this);
		}
#endif
	}

	int IManaged::getReferenceCounter() const
	{
		return mRefCounter;
	}

	void IManaged::onRelease(IManaged::ManagedCallback callback)
	{
#ifdef _DEBUG
		mReleaseCallback = callback;
#endif
	}
	void IManaged::onRetain(IManaged::ManagedCallback callback)
	{
#ifdef _DEBUG
		mRetainCallback = callback;
#endif
	}

}
}
