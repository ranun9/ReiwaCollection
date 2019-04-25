#ifndef __FINISHED_SCENE_H__
#define __FINISHED_SCENE_H__

#include "cocos2d.h"

class Finished : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(Finished);
};
#endif // __TITLE_SCENE_H__
