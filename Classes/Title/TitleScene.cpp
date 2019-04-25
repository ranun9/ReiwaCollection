#include "TitleScene.h"
#include "HelloWorldScene.h"

using namespace cocos2d;

Scene * TitleScene::createScene()
{
	auto scene = TitleScene::create();
	//auto layer = TitleScene::create();
	//scene->addChild(layer);
	return scene;
}

bool TitleScene::init() {
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	TitleScene::initEvents();

	auto label = Label::createWithTTF("ReiwaCollection", "fonts/Marker Felt.ttf", 24);
	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height));
	// add the label as a child to this layer
	this->addChild(label, 1);

	return true;
}

void TitleScene::initEvents() {
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
		Director::getInstance()->replaceScene(HelloWorld::createScene());

		return true;
		//http://discuss.cocos2d-x.org/t/invalid-eventlistenertouchonebyone-error/15949
		//onTouchBegan ‚Í•K{‚ç‚µ‚¢
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}