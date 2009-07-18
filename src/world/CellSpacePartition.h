#ifndef CELLSPACEPARTITION_H_
#define CELLSPACEPARTITION_H_

#include <QVector>
#include <QString>
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

		QVector<Pointer<Entity> > members;
	};

	CellSpacePartition(World* parent);
	virtual ~CellSpacePartition();

	void partition();

	Cell* posToCell(Math::Point p);

	void remove(Pointer<Entity> e, Math::Point oldPos);
	void updatePos(Pointer<Entity> e, Math::Point oldPos, Math::Point newPos);

	void findNeighbors(Math::Point p, double radius, QString type);
	//invalidated at next find operation
	const QVector<Pointer<Entity> >& result() const { return mResult; }

	Pointer<Entity> findNearest(Math::Point p, QString type, double maxDistance=Math::maxDouble);

private:
	int posToIdx(Math::Point p) const;
	inline void checkCell(Math::Point p, QString type, int cell, Pointer<Entity>& nearest, double& nearestDistSq);

	World* mWorld;

	double mCellSize;
	int mNumHCells;
	int mNumVCells;

	QVector<Cell> mCells;

	QVector<Pointer<Entity> > mResult;
};

#endif /*CELLSPACEPARTITION_H_*/
