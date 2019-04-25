#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"

class TitleScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	bool init();
	void initEvents();

	// implement the "static create()" method manually
	CREATE_FUNC(TitleScene);
};
#endif // __TITLE_SCENE_H__
