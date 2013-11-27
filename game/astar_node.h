#pragma once

#include <math/vector.h>
using namespace am::math;

#include <vector>

namespace am {
namespace game {

	class Tile;

	typedef std::vector<Vector2f> NodePath;

	class AStarNode	{
	public:
		AStarNode();
		~AStarNode(void);

		inline void reset()
		{
			h = 0.0;
			f = 0.0;
			g = 1.0;
			parent = nullptr;
		}

		double f;
		double g;
		double h;

		int group;

		long useCounter;

		// Should be float instead of int because as it's more
		// flexible.
		Vector2f position;
		Vector2i gridPosition;

		AStarNode *parent;
	};

	typedef std::vector<AStarNode *> AStarList;

}
}
