#ifndef EDITORGAME_H_
#define EDITORGAME_H_

#include "world/GameBase.h"

class EditorGame: public GameBase
{
public:
	EditorGame() {}
	virtual ~EditorGame() {}

	void log(QString msg);
};

#endif /* EDITORGAME_H_ */
