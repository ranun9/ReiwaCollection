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

	auto bouquet = Sprite::create("bouquet_omedetou.png");
	bouquet->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bouquet);

    auto OtoLogic = Label::createWithTTF("Audio Effect by OtoLogic", "fonts/Marker Felt.ttf", 24);
    // position the label on the center of the screen
    OtoLogic->setPosition(Vec2(origin.x + visibleSize.width / 2,
                            visibleSize.height /4));
    // add the label as a child to this layer
    this->addChild(OtoLogic, 1);

	return true;
}