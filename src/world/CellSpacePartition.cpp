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

//Partitions the world into cells. Called once the world boundaries are set.
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
	QVector<Pointer<Entity> >::iterator result = std::find(oldCell->members.begin(), oldCell->members.end(), e);
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
		QVector<Pointer<Entity> >::iterator result = std::find(oldCell->members.begin(), oldCell->members.end(), e);
		if (result != oldCell->members.end()) {
			std::swap(*result, oldCell->members.back());
			oldCell->members.pop_back();
		}
	}

	//add to new one
	if (newCell) newCell->members.push_back(e);
}

void CellSpacePartition::findNeighbors(Math::Point p, double radius, QString type)
{
	mResult.clear();

	for (QVector<Cell>::iterator i = mCells.begin(); i != mCells.end(); ++i) {
		//check if cell falls in "square radius" (the square whose width is the same as the circle radius)
		//this condition is TOTALLY NOT BOGUS
		if (
			//x part
			(
				//either one of the cell edges lies within the circle
				((p.x-radius <= i->left && i->left <= p.x+radius) ||
				(p.x-radius <= i->right && i->right <= p.x+radius))
				|| //OR the circle is completely inside the cell
				(i->left <= p.x-radius && p.x+radius <= i->right)
			) && (
			//AND y part. edges:
				((p.y-radius <= i->top && i->top <= p.y+radius) ||
				(p.y-radius <= i->bottom && i->bottom <= p.y+radius))
				|| //OR circle is inside:
				(i->top <= p.y-radius && p.y+radius <= i->bottom)
			)
		) {
			//cell (may be) in neighborhood, check all members
			for (unsigned int j = 0; j < i->members.size(); ++j)
				if (World::inherits(i->members[j], type) && Math::distanceSq(i->members[j]->pos(), p) < radius*radius)
					mResult.push_back(i->members[j]);
		}
	}
}

inline void CellSpacePartition::checkCell(Math::Point p, QString type, int cell, Pointer<Entity>& nearest, double& nearestDistSq)
{
	if (cell < 0 || cell > mCells.size()) return;

	CellSpacePartition::Cell* c = &mCells[cell];

	for (unsigned int i = 0; i < c->members.size(); ++i)
		if (World::inherits(c->members[i], type) && Math::distanceSq(c->members[i]->pos(), p) < nearestDistSq) {
			nearest = c->members[i];
			nearestDistSq = Math::distanceSq(c->members[i]->pos(), p);
		}
}

Pointer<Entity> CellSpacePartition::findNearest(Math::Point p, QString type, double maxDistance)
{
	int startCell = posToIdx(p);

	int reach = 1; //how many cells out we will be searching, each iteration
	int timesFound = 0; //will go n iterations past a find because a closer one could be found (ie if the first one is found in a corner)
	int maxTimesFound = 2;
	Pointer<Entity> nearest;
	double nearestDistSq = Math::maxDouble;

	checkCell(p, type, startCell, nearest, maxDistance); //check home cell first
	if (nearest) timesFound = 1;

	do {
		//start at top-left and go clockwise
		int cell = startCell - reach - reach*mNumHCells - 1; //tl
		for (int i = 0; i < reach*2 + 1; ++i) { //top row
			++cell;
			checkCell(p, type, cell, nearest, nearestDistSq);
		}
		for (int i = 0; i < reach*2; ++i) { //right col
			cell += mNumHCells;
			checkCell(p, type, cell, nearest, nearestDistSq);
		}
		for (int i = 0; i < reach*2; ++i) { //bottom row
			--cell;
			checkCell(p, type, cell, nearest, nearestDistSq);
		}
		for (int i = 0; i < reach*2 - 1; ++i) { //left col
			cell -= mNumHCells;
			checkCell(p, type, cell, nearest, nearestDistSq);
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
