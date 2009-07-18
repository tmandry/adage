#ifndef ENTITY_H_
#define ENTITY_H_

#include <vector>
#include <set>
#include <QPainter>
#include <QObject>
#include <QString>
#include <QSet>
#include <cassert>
#include "world/View.h"
#include "math/Point.h"
#include "Pointer.h"
#include "world/GameBase.h"

#define ENTITY(type) \
	private: \
	Pointer<type> _mThis; \
	virtual void subclass() \
	{ \
		_mThis = Pointer<type>(this); \
		Entity::subclass(); \
	} \
	virtual QString _className() const \
	{ \
		return #type; \
	} \
	public: \
	struct MetaInfo { \
		static const QString className() { return #type; } \
		typedef type Type; \
	}; \
	Pointer<type> pointer() const { assert(_mThis); return _mThis; }


template <class E> class SimpleEntityFactory;
#define FACTORY(factory) \
	public: \
	typedef factory Factory;

template<class E> class AutoEntityFactory;
#define AUTO_FACTORY FACTORY(AutoEntityFactory<MetaInfo::Type>)

template<class E> class MinimalEntityFactory;
#define MINIMAL_FACTORY FACTORY(MinimalEntityFactory<MetaInfo::Type>)

//forward declaration
class World;

//Any Entity subclass MUST have an ENTITY(Type) macro in its definition, define its own constructor, and call subclass() in every constructor
class Entity
{
public:
	typedef std::set<Pointer<Entity> > ChildList;

protected:
	//subclasses MUST redefine constructor and call subclass()
	Entity(Pointer<Entity> parent, QString name="Entity");
	virtual void subclass();

public:
	template<class E>
	bool inherits() const { return _inherits(_className<E>()); }

	void setName(const QString name) { mName = name; }
	QString name() const { return mName; }

	//replace with region? or abstract intersects function?
	inline void setPos(const Math::Point loc); //definition in World.h
	Math::Point pos() const { return mLoc; }
	bool movable() const { return mMovable; }

	void remove();
	bool removed() { return mRemove; }

	Pointer<Entity> parent() const { return mParent; }
	Pointer<World> world() const { return mWorld; }
	View* view() const { return mView; }
	/*template<class T>
	Pointer<T> pointer() { return Pointer<T>::staticPointerCast(mThis); }*/

	bool visible() const { return mVisible; }

	template<class E>
	static const QString _className()
	{
		return E::MetaInfo::className();
		typedef Entity Type;
	}

	struct MetaInfo {
		static const QString className() { return "Entity"; }
	};

protected:
	virtual ~Entity();
	Pointer<Entity> pointer() { return mThis; }

	friend void Pointer<Entity>::free() const;

	void update(double secsElapsed);
	void paint(QPainter* p);

	void setVisible(bool v) { mVisible = v; }
	void setMovable(bool m) { mMovable = m; }
	void setView(View* view) { if (mView) delete mView; mView = view; }

	inline void printComm(QString msg) const; //definition below

private:
	virtual Pointer<World> theWorld() { return mWorld; }
	virtual QString _className() const=0;

	friend class World;
	bool _inherits(const QString type) const;

	void addChild(Pointer<Entity> child) { assert(!removed()); mChildren.insert(child); }
	void delChild(Pointer<Entity> child) { mChildren.erase(child); }

	virtual void updateEvent(double /*secsElapsed*/) {}

	virtual QString formatComm(QString msg) const;

	QSet<QString> mSubclasses;

	Pointer<Entity> mParent;
	Pointer<World> mWorld;
	ChildList mChildren;
	Pointer<Entity> mThis;

	QString mName;
	Math::Point mLoc;

	View* mView;
	bool mVisible;
	bool mMovable;

	bool mRemove;
};

#include "World.h"

inline Entity::Entity(Pointer<Entity> parent, QString name)
	:	mParent(parent),
		mThis(this),
		mName(name),
		mLoc(0,0),
		mView(0),
		mVisible(false),
		mMovable(true),
		mRemove(false)
{
	if (parent) {
		parent->addChild(mThis);
		mWorld = parent->theWorld();
		mSubclasses.insert("Entity");
		world()->addEntity("Entity", mThis);
	} else {
		mWorld = Pointer<World>::staticPointerCast(Pointer<Entity>(this));
	}
}

inline void Entity::printComm(QString msg) const
{
	world()->game()->log(formatComm(msg));
}

#endif /*ENTITY_H_*/
