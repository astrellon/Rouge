#pragma once

#include <string>
#include <map>

using namespace std;

namespace am {
namespace util {

	template <class Type, class Key = string>
	class ResourceManager {
	public:
		ResourceManager()
		{
			//mLoaded = map<Type, Key>();
			//mRefCounters = map<Type, int>();
		}
		~ResourceManager()
		{
		}

		template <class T>
		T getLoaded(const Key &key)
		{
			Type value = getLoaded(key);
			if (value != NULL)
			{
				return static_cast<T *>(value);
			}
			return NULL;
		}
		Type getLoaded(const Key &key)
		{
			map<Key, Type>::iterator iter = mLoaded.find(key);
			if (iter == mLoaded.end())
			{
				return NULL;
			}
			return iter->second;
		}
		void assignLoaded(const Key &key, Type value)
		{
			mLoaded[key] = value;
		}
		void removeLoaded(const Key &key)
		{
			mLodaed.erase(key);
		}
		void altRefs(Type *value, int ref)
		{
			if (ref == 0) 
			{
				return false;
			}
	
			map<Type, int>::iterator iter = mRefCounters.find(textureId);
			if (iter == mRefCounters.end())
			{
				if (ref > 0)
				{
					mRefCounters[value] = ref;
				}
				return false;
			}

			iter->second += ref;
			if (iter->second <= 0)
			{
				//glDeleteTextures(1, &iter->first);
				value->destroy();
				return true;
			}
			return false;
		}

	protected:

		map<Key, Type> mLoaded;
		map<Type, int> mRefCounters;
	};
}
}
