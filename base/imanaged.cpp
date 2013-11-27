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

	void am::base::IManaged::release()
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

	void am::base::IManaged::release() const
	{
		mRefCounter--;
	}

	void am::base::IManaged::retain()
	{
		mRefCounter++;
#ifdef _DEBUG
		if (mRetainCallback)
		{
			mRetainCallback(this);
		}
#endif
	}

	void am::base::IManaged::retain() const
	{
		mRefCounter++;
	}

	int am::base::IManaged::getReferenceCounter() const
	{
		return mRefCounter;
	}

	void am::base::IManaged::onRelease(am::base::IManaged::ManagedCallback callback)
	{
#ifdef _DEBUG
		mReleaseCallback = callback;
#endif
	}
	void am::base::IManaged::onRetain(am::base::IManaged::ManagedCallback callback)
	{
#ifdef _DEBUG
		mRetainCallback = callback;
#endif
	}

}
}
