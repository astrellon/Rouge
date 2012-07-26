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

		operator T *() const
		{
			return mObject;
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

		bool operator==(const T *rhs) const
		{
			return mObject == rhs;
		}
		bool operator==(const T &rhs) const
		{
			return *mObject == rhs;
		}
		bool operator!=(const T *rhs) const
		{
			return mObject != rhs;
		}
		bool operator!=(const T &rhs) const
		{
			return *mObject != rhs;
		}
		/*bool operator!=(int rhs) const
		{
			return mObject != rhs;
		}
		bool operator!=(unsigned int rhs) const
		{
			return mObject != rhs;
		}*/

	private:
		T *mObject;
	};

}
}
