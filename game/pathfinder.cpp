#include "pathfinder.h"

#include <math/math.h>

#include <algorithm>

#include <util/utils.h>

#include "game_object.h"
#include "engine.h"

namespace am {
namespace game {

	Pathfinder *Pathfinder::sPathfinder = nullptr;

	Pathfinder *Pathfinder::getPathfinder()
	{
		if (!sPathfinder)
		{
			sPathfinder = new Pathfinder();
		}
		return sPathfinder;
	}
	void Pathfinder::releasePathfinder()
	{
		if (sPathfinder)
		{
			delete sPathfinder;
			sPathfinder = nullptr;
		}
	}

	Pathfinder::Pathfinder() :
		mNodeUseCounter(0)
	{	
	}
	Pathfinder::~Pathfinder()
	{
	}

	bool Pathfinder::search(const math::Vector2i &start, math::Vector2i end, NodePath &path, Map *map, const GameObject *forObj)
	{
		if (!map || 
			start.x < 0 || start.x >= map->getMapWidth() ||
			start.y < 0 || start.y >= map->getMapHeight() ||
			end.x < 0 || end.x >= map->getMapWidth() ||
			end.y < 0 || end.y >= map->getMapHeight() ||
			(start.x == end.x && start.y == end.y))
		{
			return false;
		}

		mMap = map;
		int startGroup = map->mMapData[start.x][start.y].group;
		int endGroup = map->mMapData[end.x][end.y].group;
		if(startGroup != endGroup)
		{
			mMap = nullptr;
			return false;
		}

		if (!map->isValidGridLocation(end.x, end.y, forObj))
		{
			// Move back towards the start position until we do find a passable location.
			Engine *engine = Engine::getEngine();
			math::Vector2f fstart(start);
			math::Vector2f fend(end);
			math::Vector2f toStart(fend.sub(fstart));
			int numSteps = static_cast<int>(toStart.length() * 2.0f);
			toStart.normalise();
			toStart.scale(0.5f);

			for (int step = 0; step < numSteps; step++)
			{
				fend = fend.sub(toStart);
				math::Vector2i grid(fend);
				if (map->isValidGridLocation(grid.x, grid.y, forObj))
				{
					break;
				}
			}

			end.x = static_cast<int>(fend.x);
			end.y = static_cast<int>(fend.y);

			endGroup = map->mMapData[end.x][end.y].group;
			if(startGroup != endGroup)
			{
				mMap = nullptr;
				return false;
			}
		}

		mOpenList.clear();
		mClosedList.clear();

		mNodeUseCounter++;

		mOpenList.push_back(&map->mMapData[start.x][start.y]);

		AStarNode *endNode = &map->mMapData[end.x][end.y];
		endNode->parent = nullptr;

		while(!mOpenList.empty())
		{
			AStarNode *node = mOpenList.front();
			if(node->useCounter < mNodeUseCounter)
			{
				node->g = 0;
				node->useCounter = mNodeUseCounter;
				node->parent = nullptr;
			}
		
			if (node == endNode)
			{
				// Complete
				getPath(node, path);
				mMap = nullptr;
				return true;
			}
			else
			{
				mOpenList.erase(mOpenList.begin());
				mClosedList.push_back(node);

				mNeighbors.clear();
				getNeighbors(node->gridPosition, forObj);
				for(auto iter = mNeighbors.begin(); iter != mNeighbors.end(); ++iter)
				{
					AStarNode *n = *iter;
					if (!util::Utils::listContains(mOpenList, n) &&
						!util::Utils::listContains(mClosedList, n))
					{
						if (n->useCounter < mNodeUseCounter)
						{
							n->g = 0;
							n->useCounter = mNodeUseCounter;
						}
						n->g += node->g;

						n->f = n->g + manhattanDistance(n->position, endNode->position);

						n->parent = node;
						mOpenList.push_back(n);
					}
				}

				mNeighbors.clear();
				sortAStarList(mOpenList);
			}
		}
		// NO PATH! D:
		mMap = nullptr;
		return false;
	}

	void Pathfinder::getPath(AStarNode *node, NodePath &path)
	{
		while(node != nullptr)
		{
			path.push_back(node->position);
			node = node->parent;
		}

		reverse(path.begin(), path.end());
	}

	inline bool Pathfinder::checkNeighbor(const int &x, const int &y, const GameObject *forObj)
	{
		if(x >= 0 && x < mMap->mWidth && y >= 0 && y < mMap->mHeight)
		{
			AStarNode *node = &mMap->mMapData[x][y];

			if (mMap->isValidGridLocation(x, y, forObj))
			{
				mNeighbors.push_back(node);
				return true;
			}
		}
		return false;
	}

	void Pathfinder::getNeighbors(const Vector2i &position, const GameObject *forObj)
	{
		int posX = position.x;
		int posY = position.y;
		bool left =		checkNeighbor(posX - 1,	posY, forObj);
		bool top =		checkNeighbor(posX,		posY - 1, forObj);
		bool right =	checkNeighbor(posX + 1,	posY, forObj);
		bool bottom =	checkNeighbor(posX,		posY + 1, forObj);

		if (left && top)
		{
			checkNeighbor(posX - 1, posY - 1, forObj);
		}
		if (top && right)
		{
			checkNeighbor(posX + 1, posY - 1, forObj);
		}
		if (bottom && left)
		{
			checkNeighbor(posX - 1, posY + 1, forObj);
		}
		if (bottom && right)
		{
			checkNeighbor(posX + 1, posY + 1, forObj);
		}
	}

	inline double Pathfinder::manhattanDistance(const math::Vector2f &p1, const math::Vector2f &p2)
	{
		return math::abs(p1.x - p2.x) + math::abs(p1.y - p2.y);
	}
	inline void Pathfinder::sortAStarList(AStarList &list)
	{
		sort(list.begin(), list.end(), compare);
	}
	inline bool Pathfinder::compare(AStarNode *n1, AStarNode *n2)
	{
		return n1->f < n2->f;
	}

}
}
