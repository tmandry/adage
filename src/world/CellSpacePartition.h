#ifndef CELLSPACEPARTITION_H_
#define CELLSPACEPARTITION_H_

#include <vector>
#include <string>
#include "math/Point.h"
//#include "world/Entity.h"
#include "Pointer.h"

class World;
class Entity;

class CellSpacePartition
{
public:
	struct Cell {
		double left, top, right, bottom;

		std::vector<Pointer<Entity> > members;
	};

	CellSpacePartition(World* parent);
	virtual ~CellSpacePartition();

	void partition();

	Cell* posToCell(Math::Point p);

	void remove(Pointer<Entity> e, Math::Point oldPos);
	void updatePos(Pointer<Entity> e, Math::Point oldPos, Math::Point newPos);

	void findNeighbors(Math::Point p, double radius, std::string type);
	//invalidated at next find operation
	const std::vector<Pointer<Entity> >& result() const { return mResult; }

	Pointer<Entity> findNearest(Math::Point p, std::string type, double maxDistance=Math::maxDouble);

private:
	int posToIdx(Math::Point p) const;

	World* mWorld;

	double mCellSize;
	int mNumHCells;
	int mNumVCells;

	std::vector<Cell> mCells;

	std::vector<Pointer<Entity> > mResult;
};

#endif /*CELLSPACEPARTITION_H_*/
