#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"
#include "firebase/admob/banner_view.h"

class TitleScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	bool init();
	void initEvents();

    void update(float delta);

    firebase::admob::BannerView* banner_view;

	// implement the "static create()" method manually
	CREATE_FUNC(TitleScene);
};
#endif // __TITLE_SCENE_H__
