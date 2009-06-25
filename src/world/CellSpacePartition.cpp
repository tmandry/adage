#include <cmath>
#include <cassert>
#include <algorithm>
#include "CellSpacePartition.h"
#include "world/World.h"
#include "world/Entity.h"
#include "math/const.h"

CellSpacePartition::CellSpacePartition(World* parent)
	:	mWorld(parent)
{
	mCellSize = 20;
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
	int idx = posToIdx(p);
	if (idx < 0) return 0; else return &mCells[idx];
}

void CellSpacePartition::remove(Pointer<Entity> e, Math::Point oldPos)
{
	Cell* oldCell = posToCell(oldPos);
	if (!oldCell) return;

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
	if (oldCell) {
		std::vector<Pointer<Entity> >::iterator result = std::find(oldCell->members.begin(), oldCell->members.end(), e);
		if (result != oldCell->members.end()) {
			std::swap(*result, oldCell->members.back());
			oldCell->members.pop_back();
		}
	}

	//add to new one
	if (newCell) newCell->members.push_back(e);
}

void CellSpacePartition::findNeighbors(Math::Point p, double radius, std::string type)
{
	mResult.clear();

	for (std::vector<Cell>::iterator i = mCells.begin(); i != mCells.end(); ++i) {
		//check if cell falls in "square radius"
		//TODO: BOGUS
		/*if ((i->right > p.x-radius || i->left < p.x+radius) &&
			(i->bottom < p.y+radius || i->top > p.y-radius)
		) {*/
			//cell (may be) in neighborhood, check all members
			for (unsigned int j = 0; j < i->members.size(); ++j)
				if (i->members[j]->inherits(type) && Math::distanceSq(i->members[j]->pos(), p) < radius*radius)
					mResult.push_back(i->members[j]);
		//}
	}
}

inline void checkCell(Math::Point p, std::string type, int cell, Pointer<Entity>& nearest, double& nearestDistSq, std::vector<CellSpacePartition::Cell>& mCells)
{
	if (cell < 0 || cell > mCells.size()) return;

	CellSpacePartition::Cell* c = &mCells[cell];

	for (unsigned int i = 0; i < c->members.size(); ++i)
		if (c->members[i]->inherits(type) && Math::distanceSq(c->members[i]->pos(), p) < nearestDistSq) {
			nearest = c->members[i];
			nearestDistSq = Math::distanceSq(c->members[i]->pos(), p);
		}
}

Pointer<Entity> CellSpacePartition::findNearest(Math::Point p, std::string type, double maxDistance)
{
	int startCell = posToIdx(p);

	int reach = 1; //how many cells out we will be searching, each iteration
	int timesFound = 0; //will go n iterations past a find because a closer one could be found (ie if the first one is found in a corner)
	int maxTimesFound = 2;
	Pointer<Entity> nearest;
	double nearestDistSq = Math::maxDouble;

	checkCell(p, type, startCell, nearest, maxDistance, mCells); //check home cell first
	if (nearest) timesFound = 1;

	do {
		//start at top-left and go clockwise
		int cell = startCell - reach - reach*mNumHCells - 1; //tl
		for (int i = 0; i < reach*2 + 1; ++i) { //top row
			++cell;
			checkCell(p, type, cell, nearest, nearestDistSq, mCells);
		}
		for (int i = 0; i < reach*2; ++i) { //right col
			cell += mNumHCells;
			checkCell(p, type, cell, nearest, nearestDistSq, mCells);
		}
		for (int i = 0; i < reach*2; ++i) { //bottom row
			--cell;
			checkCell(p, type, cell, nearest, nearestDistSq, mCells);
		}
		for (int i = 0; i < reach*2 - 1; ++i) { //left col
			cell -= mNumHCells;
			checkCell(p, type, cell, nearest, nearestDistSq, mCells);
		}

		if (nearest) {
			++timesFound;
			if (timesFound > 2) maxTimesFound = timesFound*2 - 2; //good estimate of how far to go (not perfect)
		}
		++reach;
	} while (timesFound < maxTimesFound && reach*mCellSize < maxDistance);

	return nearest;
}

int CellSpacePartition::posToIdx(Math::Point p) const
{
	if (p.x < mWorld->leftBound() || p.x > mWorld->rightBound() ||
		p.y < mWorld->topBound() || p.y > mWorld->bottomBound()
	)
		return -1;

	int cellX = (int)floor((p.x - mWorld->leftBound()) / mCellSize);
	int cellY = (int)floor((p.y - mWorld->topBound()) / mCellSize);

	return cellY*mNumHCells + cellX;
}
