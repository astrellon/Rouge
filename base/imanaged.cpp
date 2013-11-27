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

	void base::IManaged::release()
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

	void base::IManaged::release() const
	{
		mRefCounter--;
	}

	void base::IManaged::retain()
	{
		mRefCounter++;
#ifdef _DEBUG
		if (mRetainCallback)
		{
			mRetainCallback(this);
		}
#endif
	}

	void base::IManaged::retain() const
	{
		mRefCounter++;
	}

	int base::IManaged::getReferenceCounter() const
	{
		return mRefCounter;
	}

	void base::IManaged::onRelease(base::IManaged::ManagedCallback callback)
	{
#ifdef _DEBUG
		mReleaseCallback = callback;
#endif
	}
	void base::IManaged::onRetain(base::IManaged::ManagedCallback callback)
	{
#ifdef _DEBUG
		mRetainCallback = callback;
#endif
	}

}
}
