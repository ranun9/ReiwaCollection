#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

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
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

	//create Wall
	auto node = DrawNode::create();
	node->setName("Wall");
	node->drawRect(Vec2(0, 0), Vec2(visibleSize.width, visibleSize.height), Color4F::WHITE);
	this->addChild(node);
	auto physicsBody=PhysicsBody::createEdgeBox(Size(visibleSize.width, visibleSize.height));
	physicsBody->setDynamic(false);
	physicsBody->setPositionOffset(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	physicsBody->getPosition();
	node->addComponent(physicsBody);

	//create Player
	player = Sprite::create("pose_sagasu_kyorokyoro_man.png");
	player->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	player->setScale(0.3);
	this->addChild(player);
	playerPhysics = PhysicsBody::createBox(Size(100,100));
	player->addComponent(playerPhysics);	

	HelloWorld::initEvents();

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
		player->setPosition(player->getPosition() + touch->getDelta());
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}