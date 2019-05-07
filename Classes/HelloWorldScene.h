#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "firebase/admob/banner_view.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
	virtual bool init();

	// a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

	void initEvents();

	void update(float frame);

	cocos2d::Sprite *player;
	cocos2d::PhysicsBody *playerPhysics;
	cocos2d::Vector<cocos2d::PhysicsBody*> ItemsPhysics;//std::vectorだとHelloWorld::updateでぬるぽ？

	std::vector<std::string> nextNeedItems;
	std::vector<std::string>::iterator it;

	firebase::admob::BannerView* banner_view;

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
