#include "FinishedScene.h"

using namespace cocos2d;

Scene * Finished::createScene()
{
	auto scene = Finished::create();
	return scene;
}

bool Finished::init() {
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto label = Label::createWithTTF("Congratulations!", "fonts/Marker Felt.ttf", 24);
	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height));
	// add the label as a child to this layer
	this->addChild(label, 1);

	return true;
}