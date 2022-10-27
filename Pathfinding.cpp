#include "Pathfinding.h"
Pathfinding::Pathfinding(void)
{
	m_intializedStartGoal = false;
	m_foundGoal = false;
}
Pathfinding::~Pathfinding(void)
{

}

void Pathfinding::FindPath(Vector3 currentPos, Vector3 targetPos)
{
	if (!m_intializedStartGoal)
	{
		for (int i = 0; i < m_openList(); i++)
		{
			delete m_openList[i];
		}
		m_openList.clear();

		for (int i = 0; i < m_visitedList.size() i++)
		{
			delete m_visitedList[i];
		}
		m_visitedList.clear();
		for (int i = 0; m_pathToGoal.size(); i++)
		{
			delete m_pathToGoal[i];
		}
		m_pathToGoal.clear();

		//Intial start
		SearchCell start;
		start.m_xcoord = currentPos.x;
		start.m_zcoord = currentPos.z;
//intialize goal
		SearchCell goal;
		goal.m_xcoord = targetPos.x;
		goal.m_zcoord = targetPos.z;

		SetStartAndGoal(start, goal);
		m_intializedStartGoal = true;

	}
	if (m_intializedStartGoal)
	{
		ContinuePath();
	}
}

void Pathfinding::SetStartAndGoal(SearchCell start, SearchCell goal)
{
	m_startCell = new SearchCell(start.m_xcoord, start.m_zcoord, NULL);
	m_goalCell = new SearchCell (goal.m_xcoord, goal.m_zcoord, & goal);

	m_startCell->G = 0;
	m_startCell->H = m_startCell->ManHattanDistance(m_goalCell);
	m_startCell->parent = 0;
	m_openList.push_back(m_startCell);
}


SearchCell* Pathfinding::GetNextCell()
{
	float bestF = 999999.0f;
	int cellIndex = -1;
	SearchCell* nextCell = NULL;

	for (int i = 0; i < m_openList.size(); i++)
	{
		if (m_openList[i]->GetF() < bestF)
		{
			bestF = m_openList[i]->GetF();
			cellIndex = i;
		}
	}
	if (cellIndex >= 0)
	{
		nextCell = m_openList[cellIndex];
		m_visitedList.push_back(nextCell);
		m_openList.erase(m_openList.begin() + cellIndex);
	}

	return nextCell;
}

void Pathfinding::PathOpened(int x, int z, float newCost, SearchCell* parent)
{
	/* if (CELL_BLOCKED)
	//{
		//return;
	//}*/
	int id = z * WORLD_SIZE + x;
	for (int i = 0; i < m_visitedList.size(); i++)
	{
		if (id == m_visitedList[i]->m_id)
		{
			return;
		}
	}
	SearchCell* newChild = new SearchCell(x, z, parent);
	newChild->G = newCost;
	newChild->H = parent->ManHattanDistance(m_goalCell);

	for (int i = 0; i < m_openList.size(); i++)
	{
		if (id == m_openList[i]->m_id)
		{
			float newF = newChild->G + newCost + m_openList[i]->H;
			if (m_openList[i]->GetF() > newF)
			{
				m_openList[i]->G = newChild->G + newCost;
				m_openList[i]->parent = newChild;
			}
			else //if the F is not better 
			{
				delete newChild;
				return;
			} 
		}
	}

	m_openList.push_back(newChild);
}

void Pathfinding::ContinuePath()
{
	if (m_openList.empty())
	{
		return;
	}

	SearchCell* currentCell = GetNextCell();
	if (currentCell->m_id == m_goalCell->m_id)
	{
		m_goalCell->parent = currentCell->parent;
		SearchCell* getPath;

		for (getPath = m_goalCell; getPath != NULL; getPath = getPath->parent)
		{
			m_pathToGoal.push_back(new Vector3(getPath->m_xcoord, 0, getPath->m_zcoord));
		}
		m_foundGoal = true;
		return;
	}

	else
	{
		//rightSideCell 
		PathOpened(currentCell->m_xcoord + 1, currentCell->m_zcoord, currentCell->G + 1, currentCell);
		//leftSideCell
		
		PathOpened(currentCell->m_xcoord - 1, currentCell->m_zcoord, currentCell->G + 1, currentCell);
	
		//up
		PathOpened(currentCell->m_xcoord, currentCell->m_zcoord + 1, currentCell->G + 1, currentCell);
	//Down
		PathOpened(currentCell->m_xcoord, currentCell->m_zcoord - 1, currentCell->G + 1, currentCell);
	

		//left_up_diagonal
		PathOpened(currentCell->m_xcoord - 1, currentCell->m_zcoord + 1, currentCell->G + 1.414f, currentCell);
	//right_up diagonal

		PathOpened(currentCell->m_xcoord + 1, currentCell->m_zcoord + 1, currentCell->G + 1.414f, currentCell);
	//left-down diaagonal
		PathOpened(currentCell->m_xcoord - 1, currentCell->m_zcoord - 1, currentCell->G + 1.414f, currentCell);
	
		//right-down diagonal
		PathOpened(currentCell->m_xcoord + 1, currentCell->m_zcoord - 1, currentCell->G + 1.414f, currentCell);
	
		for (int i = 0; i < m_openList.size(); i++)
		{
			if (currentCell->m_id == m_openList[i]->m_id)
			{
				m_openList.erase(m_openList.begin() + i);
			}
		}
	}



}

Vector3 Pathfinding::NextPathPos(AI _ai)
{
	int index = 1;

	Vector3 nextPos;
	nextPos.x = m_pathToGoal[m_pathToGoal.size() - index]->x;
	nextPos.z = m_pathToGoal[m_pathToGoal.size() - index]->z;

	Vector3 distance = nextPos - _ai.pos;

	if (index < m_pathToGoal.size())
	{
		if (distance.Length() < _ai.radius)
		{
			m_pathToGoal.erase(m_pathToGoal.end() - index);
		}
	}

	return nextPos;
}