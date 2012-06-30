#pragma once

namespace am {
namespace base {

	template <class T>
	class Handle {
	public:
		Handle() :
		  mObject(0)
		{
		}
		Handle(T *obj) :
			mObject(obj)
		{
			if (mObject)
			{
				mObject->retain();
			}
		}
		Handle(const Handle<T> &handle)
		{
			mObject = handle.mObject;
			if (mObject)
			{
				mObject->retain();
			}
		}
		~Handle()
		{
			if (mObject)
			{
				mObject->release();
			}
		}

		T &operator*() const
		{
			return *mObject;
		}
		T *get() const
		{
			return mObject;
		}
		T *operator->() const
		{
			return mObject;
		}

		Handle<T> &operator=(T *obj)
		{
			if (mObject)
			{
				mObject->release();
			}
			mObject = obj;
			if (mObject)
			{
				mObject->retain();
			}

			return *this;
		}

		Handle<T> &operator=(const Handle<T> &handle)
		{
			if (mObject)
			{
				mObject->release();
			}
			mObject = handle.mObject;
			if (mObject)
			{
				mObject->retain();
			}

			return *this;
		}

	private:
		T *mObject;
	};

}
}
