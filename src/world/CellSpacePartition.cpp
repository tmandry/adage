#include <cmath>
#include <cassert>
#include <algorithm>
#include "CellSpacePartition.h"
#include "world/World.h"
#include "world/Entity.h"

CellSpacePartition::CellSpacePartition(World* parent)
	:	mWorld(parent)
{
	mCellSize = 10;
}

CellSpacePartition::~CellSpacePartition()
{
}

void CellSpacePartition::partition()
{
	mNumHCells = (int)ceil((mWorld->rightBound() - mWorld->leftBound()) / mCellSize);
	mNumVCells = (int)ceil((mWorld->bottomBound() - mWorld->topBound()) / mCellSize);

	mCells.resize(mNumHCells*mNumVCells);
	double left = mWorld->leftBound(), top = mWorld->topBound();
	int rowCount = 0;
	for (int i = 0; i < mNumHCells*mNumVCells; ++i) {
		mCells[i].left = left; mCells[i].top = top;
		mCells[i].right = left + mCellSize; mCells[i].bottom = top + mCellSize;
		mCells[i].members.clear();

		left += mCellSize;
		++rowCount;
		if (rowCount == mNumHCells) {
			top += mCellSize;
			left = mWorld->leftBound();
			rowCount = 0;
		}
	}
}

CellSpacePartition::Cell* CellSpacePartition::posToCell(Math::Point p)
{
	assert(p.x > mWorld->leftBound() && p.x < mWorld->rightBound());
	assert(p.y > mWorld->topBound() && p.y < mWorld->bottomBound());

	int cellX = (int)floor((p.x - mWorld->leftBound()) / mCellSize);
	int cellY = (int)floor((p.y - mWorld->topBound()) / mCellSize);
	return &mCells[cellY*mNumHCells + cellX];
}

void CellSpacePartition::remove(Pointer<Entity> e, Math::Point oldPos)
{
	Cell* oldCell = posToCell(oldPos);

	//remove from old cell
	std::vector<Pointer<Entity> >::iterator result = std::find(oldCell->members.begin(), oldCell->members.end(), e);
	if (result != oldCell->members.end()) {
		std::swap(*result, oldCell->members.back());
		oldCell->members.pop_back();
	}
}

void CellSpacePartition::updatePos(Pointer<Entity> e, Math::Point oldPos, Math::Point newPos)
{
	Cell* oldCell = posToCell(oldPos);
	Cell* newCell = posToCell(newPos);
	if (oldCell == newCell) return;

	//remove from old cell
	std::vector<Pointer<Entity> >::iterator result = std::find(oldCell->members.begin(), oldCell->members.end(), e);
	if (result != oldCell->members.end()) {
		std::swap(*result, oldCell->members.back());
		oldCell->members.pop_back();
	}

	//add to new one
	newCell->members.push_back(e);
}

void CellSpacePartition::findNeighbors(Math::Point p, double radius, std::string type)
{
	mResult.clear();

	for (std::vector<Cell>::iterator i = mCells.begin(); i != mCells.end(); ++i) {
		//check if cell falls in "square radius"
		if ((i->right > p.x-radius || i->left < p.x+radius) &&
			(i->bottom < p.y+radius || i->top > p.y-radius)
		) {
			//cell (may be) in neighborhood, check all members
			for (unsigned int j = 0; j < i->members.size(); ++j)
				if (i->members[j]->inherits(type) && Math::distanceSq(i->members[j]->pos(), p) < radius*radius)
					mResult.push_back(i->members[j]);
		}
	}
}
