#ifndef MOVINGENTITY_H_
#define MOVINGENTITY_H_

#include "Entity.h"
#include "math/Point.h"
#include "math/Vector.h"

class MovingEntity : public Entity
{
protected:
	MovingEntity(Pointer<Entity> parent, std::string name="Entity");

public:
	virtual ~MovingEntity() {}

	unsigned int mass() const { return mMass; }
	Math::Vector velocity() const { return mVelocity; }
	Math::Vector heading() const { return mHeading; }
	Math::Vector force() const { return mForce; }

	double maxSpeed() const { return mMaxSpeed; }
	double maxForce() const { return mMaxForce; }

	/// Converts a point from local space to world space
	Math::Point toWorldSpace(Math::Point local) const;

protected:
	void setMass(unsigned int m) { mMass = m; }
	void setVelocity(Math::Vector v) { mVelocity = v; }
	void setHeading(Math::Vector h) { mHeading = h; }
	void setForce(Math::Vector f) { mForce = f; }

	void setMaxSpeed(double s) { mMaxSpeed = s; }
	void setMaxForce(double f) { mMaxForce = f; }

	virtual void updateEvent(double secsElapsed);

private:
	unsigned int mMass;
	Math::Vector mVelocity;
	Math::Vector mHeading;	///< normalized vector with heading
	Math::Vector mForce;

	double mMaxSpeed;
	double mMaxForce;
};

#endif /*MOVINGENTITY_H_*/
