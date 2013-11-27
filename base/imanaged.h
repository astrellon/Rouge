#pragma once

namespace am {
namespace base {

	class IManaged 
	{
	public:
		typedef void (*ManagedCallback)(IManaged *obj);

		IManaged();
		IManaged(const am::base::IManaged &copy);
		virtual ~IManaged();

		virtual void release();
		virtual void retain();
		virtual void release() const;
		virtual void retain() const;
		
		int getReferenceCounter() const;

		virtual void onRelease(ManagedCallback callback);
		virtual void onRetain(ManagedCallback callback);

	private:

		mutable int mRefCounter;

#ifdef _DEBUG
		ManagedCallback mReleaseCallback;
		ManagedCallback mRetainCallback;
#endif

	};

}
}
