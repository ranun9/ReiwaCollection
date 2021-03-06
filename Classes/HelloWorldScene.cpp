#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Finished/FinishedScene.h"

#include "FirebaseHelper.h"

#include "firebase/admob.h"
#include "firebase/admob/types.h"
#include "firebase/app.h"
#include "firebase/future.h"
#include "firebase/admob/banner_view.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	/*
	auto scene = (HelloWorld*)HelloWorld::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->HelloWorld::init();
	↑で書くと終了時にメモリリーク？でエラーが出たので、↓のlayerを使う古い？記法を教えて貰った
	*/
	auto scene = Scene::createWithPhysics();
	auto world = scene->getPhysicsWorld();
	world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	world->setGravity(Vec2::ZERO);

	auto layer = HelloWorld::create();
	scene->addChild(layer);

	return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 3. add your codes below...
	HelloWorld::initEvents();

	//create Wall
	auto node = DrawNode::create();
	auto wallBorder = 1919;//壁の厚さ。すり抜けないよう厚めに
	node->setName("Wall");
	node->drawRect(Vec2(0, 0), Vec2(visibleSize.width, visibleSize.height), Color4F::WHITE);
	this->addChild(node);
	auto physicsBody=PhysicsBody::createEdgeBox(Size(visibleSize.width+wallBorder*2, visibleSize.height+wallBorder*2), PhysicsMaterial(0, 1, 0), wallBorder,Vec2::ZERO);
	physicsBody->setDynamic(false);
	physicsBody->setPositionOffset(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	physicsBody->getPosition();
	node->addComponent(physicsBody);

	//setCollisionBitmask()らを意図した挙動で扱えなかったので断念
	/*
	int categoryPlayer = 1;
	int categoryItem = 1;
	*/

	//create Player
	player = Sprite::create("pose_sagasu_kyorokyoro_man.png");
	player->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	player->setScale(0.3);
	this->addChild(player);
	playerPhysics = PhysicsBody::createBox(Size(100,100));
	playerPhysics->setRotationEnable(false);//回転すると移動の向きが変わるので
	playerPhysics->setContactTestBitmask(1);
	playerPhysics->setName("Player");
	player->addComponent(playerPhysics);


	//crate Items
	Vector<Sprite*> Items;
	Items.pushBack(Sprite::create("hiragana_72_re.png"));
	Items.pushBack(Sprite::create("hiragana_02_i.png"));
	Items.pushBack(Sprite::create("hiragana_74_wa.png"));
	nextNeedItems.push_back("れ");
	nextNeedItems.push_back("い");
	nextNeedItems.push_back("わ");
	iteratorNextNeed = nextNeedItems.begin();
	for (int i = 0; i < Items.size();i++) {
		ItemsPhysics.pushBack(PhysicsBody::createBox(Size(50, 50), PhysicsMaterial(1e-4,1,0)));
		ItemsPhysics.at(i)->setName(nextNeedItems.at(i));
		ItemsPhysics.at(i)->setContactTestBitmask(1);
		Items.at(i)->setPosition(Vec2(cocos2d::random<int>(0,visibleSize.width), cocos2d::random<int>(0,visibleSize.height)));
		this->addChild(Items.at(i));
		Items.at(i)->addComponent(ItemsPhysics.at(i));
	}

	/*
	 * カタカナも加えたかったがsetNameの都合上面倒臭そう
	Vector<Sprite*> katakana;
	nextNeedItems.push_back("レ");
	otherItems.push_back("イ");
	otherItems.push_back("ワ");
	for (int i = 0; i < hiragana.size();i++) {
		ItemsPhysics.pushBack(PhysicsBody::createBox(Size(50, 50), PhysicsMaterial(1e-4,1,0)));
		ItemsPhysics.at(i)->setName(nextNeedItems.at(i));
		ItemsPhysics.at(i)->setContactTestBitmask(1);
		hiragana.at(i)->setPosition(Vec2(cocos2d::random<int>(0,visibleSize.width), cocos2d::random<int>(0,visibleSize.height)));
		this->addChild(hiragana.at(i));
		hiragana.at(i)->addComponent(ItemsPhysics.at(i));
	}*/


	this->scheduleUpdate();

    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

void HelloWorld::initEvents()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
		return true;
		//http://discuss.cocos2d-x.org/t/invalid-eventlistenertouchonebyone-error/15949
		//onTouchBegan は必須らしい
	};
	touchListener->onTouchMoved = [this](Touch* touch, Event* event) {
		playerPhysics->applyImpulse(touch->getDelta()*100);
		//log(('x' + std::to_string(touch->getDelta().x) + (" y" + std::to_string(touch->getDelta().y))).c_str());
		//touch-> getStartLocationとか？を利用で押しっぱなしでも動くようにしたい
		//playerPhysics->applyImpulse(touch->getLocation() - touch->getStartLocation() * 10, Vec2::ANCHOR_MIDDLE);
		
		//playerPhysics->setVelocity(touch->getDelta());
		//playerPhysics->applyForce(playerPhysics->getVelocity());
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = [&](PhysicsContact &contact) -> bool {
		auto bodyA = contact.getShapeA()->getBody();
		auto bodyB = contact.getShapeB()->getBody();

		if ("Player" == bodyA->getName() && *iteratorNextNeed == bodyB->getName())
		{
			bodyB->getNode()->removeFromParent();
			iteratorNextNeed++;

			auto audio = SimpleAudioEngine::getInstance();
			audio->playEffect("Onmtp-Impact06-1.mp3", false, 1.0f, 1.0f, 1.0f);
		} else if ("Player" == bodyB->getName() && *iteratorNextNeed == bodyA->getName())
		{
			bodyA->getNode()->removeFromParent();
			iteratorNextNeed++;
			auto audio = SimpleAudioEngine::getInstance();
			audio->playEffect("Onmtp-Impact06-1.mp3", false, 1.0f, 1.0f, 1.0f);
		}

		if (iteratorNextNeed == nextNeedItems.end()) {
			Director::getInstance()->replaceScene(Finished::createScene());
		}

		return true;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void HelloWorld::update(float frame) {
	for (auto e : ItemsPhysics) {
		e->applyImpulse(Vec2(random(0,1), random(0,1)));
	}
}