#include "astar_node.h"

#include "tile.h"

namespace am {
namespace game {

	AStarNode::AStarNode()
	{
		reset();
		useCounter = 0;
		parent = nullptr;
		group = -1;
	}
	AStarNode::~AStarNode(void)
	{
	}

}
}
