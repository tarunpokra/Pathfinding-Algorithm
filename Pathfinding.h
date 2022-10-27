#pragma once
#include <vector>
//#include "Vector3.h"
#include "SearchCell.h"

class Pathfinding
{

public:
	Pathfinding(void);
	~Pathfinding(void);

	void FindPath(Vector3 currentPos, Vector3 targetPos);
	Vector3 NextPathPos();
	void ClearOpenList() { m_openList.clear(); }
	void ClearVisitedList() { m_visitedList.clear(); }
	void ClearPathToGoal() { m_pathToGoal.clear(); }

	bool m_intializedStartGoal;
	bool m_foundGoal;

private:
	void SetStartAndGoal(SearchCell start, SearchCell goal);
	void PathOpened(int x, int z, float newCost, SearchCell* parent);
	SearchCell* GetNextCell();
	void ContinuePath();

	SearchCell* m_startCell;
	SearchCell* m_goalCell;
	std::vector<SearchCell*> m_openList;
	std::vector<SearchCell*> m_visitedList;
	std::vector<Vector3*>m_pathToGoal;
};



