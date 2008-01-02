#ifndef MOVINGENTITY_H_
#define MOVINGENTITY_H_

#include "Entity.h"
#include "math/Vector.h"

class MovingEntity : public Entity
{
protected:
	MovingEntity(Entity* parent, std::string name="Entity"): Entity(parent,name) { subclass("MovingEntity"); }
	
public:
	virtual ~MovingEntity() {}
	
	Math::Vector velocity() const { return mVelocity; }
	
protected:
	void setVelocity(Math::Vector v) { mVelocity=v; }
	
private:
	virtual void updateEvent(double secsElapsed);
	
	Math::Vector mVelocity;
};

#endif /*MOVINGENTITY_H_*/
