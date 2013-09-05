#pragma once

namespace am {
namespace base {

	class IManaged {
	public:
		typedef void (*ManagedCallback)(IManaged *obj);

		IManaged();
		IManaged(const IManaged &copy);
		virtual ~IManaged();

		virtual void release();
		virtual void retain();
		
		int getReferenceCounter() const;

		virtual void onRelease(ManagedCallback callback);
		virtual void onRetain(ManagedCallback callback);

	private:

		int mRefCounter;

#ifdef _DEBUG
		ManagedCallback mReleaseCallback;
		ManagedCallback mRetainCallback;
#endif

	};

}
}
