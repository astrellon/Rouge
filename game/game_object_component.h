#pragma once

#include <string>
#include <map>
#include <vector>
using std::string;
using std::map;
using std::vector;

#include <base/imanaged.h>

#include <game/component.h>

namespace am {
namespace game {

	class GameObjectComponent : public am::base::IManaged {
	public:

		typedef vector<Component *> ComponentArray;

		GameObjectComponent(const char *id = NULL);
		~GameObjectComponent();

		virtual const char *getId() const;
		virtual void setId(const char *id);

		const ComponentArray &getComponents() const;
		Component *getComponent(Component::ComponentKey key) const;
		template <class T>
		T *getComponent() const
		{
			for (size_t i = 0; i < mComponents.size(); ++i)
			{
				if (mComponents[i]->getCompKey() == T::getKey())
				{
					return dynamic_cast<T *>(mComponents[i]);
				}
			}
			return NULL;
		}

		virtual void setLocation(float x, float y);
		virtual float getLocationX() const;
		virtual float getLocationY() const;

		virtual bool setGridLocation(int x, int y);
		virtual int getGridLocationX() const;
		virtual int getGridLocationY() const;


	protected:

		string mId;
		ComponentArray mComponents;

		float mLocationX;
		float mLocationY;

	};

}
}
