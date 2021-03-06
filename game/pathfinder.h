#pragma once

#include <math/vector.h>

#include <vector>

#include <base/handle.h>

#include "astar_node.h"
#include "map.h"

namespace am {
namespace game {

	class GameObject;

	class Pathfinder 
	{
	public:
		Pathfinder();
		~Pathfinder();

		bool search(const math::Vector2i &start, math::Vector2i end, NodePath &path, Map *map, const GameObject *forObj);

		static Pathfinder *getPathfinder();
		static void releasePathfinder();

	protected:

		AStarList mOpenList;
		AStarList mClosedList;
		AStarList mNeighbors;
		long mNodeUseCounter;
		// Should not keep a long running reference on the map.
		// Should only be kept for the time it takes to complete a path.
		base::Handle<Map> mMap;

		void getPath(AStarNode *node, NodePath &path);
		void getNeighbors(const math::Vector2i &position, const GameObject *forObj);
		bool checkNeighbor(const int &x, const int &y, const GameObject *forObj);

		static void sortAStarList(AStarList &list);
		static double manhattanDistance(const math::Vector2f &p1, const math::Vector2f &p2);
		static bool compare(AStarNode *n1, AStarNode *n2);

		static Pathfinder *sPathfinder;
	};

}
}
