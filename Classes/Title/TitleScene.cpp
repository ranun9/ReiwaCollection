#include "TitleScene.h"
#include "HelloWorldScene.h"

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

	// General scene setup ...
	#if defined(__ANDROID__)
		// Android ad unit IDs.
		const char* kBannerAdUnit = "ca-app-pub-3940256099942544/6300978111";
	#else
		// iOS ad unit IDs.
	  const char* kBannerAdUnit = "ca-app-pub-3940256099942544/2934735716";
	#endif

	// Create and initialize banner view.
	//firebase::admob::BannerView* banner_view;
	banner_view = new firebase::admob::BannerView();
	firebase::admob::AdSize ad_size;
	ad_size.ad_size_type = firebase::admob::kAdSizeStandard;
	ad_size.width = 320;
	ad_size.height = 50;
	banner_view->Initialize(getAdParent(), kBannerAdUnit, ad_size);

    //banner_view->MoveTo(firebase::admob::BannerView::Position::kPositionBottom);
    //banner_view->InitializeLastResult().OnCompletion(onCompletionInitialize,this);

    // Schedule updates so that the Cocos2d-x update() method gets called.
	this->scheduleUpdate();

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

void TitleScene::update(float delta) {
	// Check that the banner has been initialized.
	if (banner_view->InitializeLastResult().status() ==
		firebase::kFutureStatusComplete) {
		// Check that the banner hasn't started loading.

		if (banner_view->LoadAdLastResult().status() ==
			firebase::kFutureStatusInvalid) {
			// Make the banner visible and load an ad.
			CCLOG("Loading a banner.");
			banner_view->Show();
			firebase::admob::AdRequest my_ad_request = {};
			banner_view->LoadAd(my_ad_request);
		}
	}
}