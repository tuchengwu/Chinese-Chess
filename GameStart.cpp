#include "Define.h"
#include "GameStart.h"
#include "GameScene.h"

USING_NS_CC;

bool GameStart::init()
{
	if (!Layer::init())
		return false;

	auto size = Director::getInstance()->getVisibleSize();

	auto red = Sprite::create("chessRed.png");
	red->setPosition(size.width / 2 - 100, size.height / 2);
	addChild(red, 0, TAG::RED);

	auto black = Sprite::create("chessBlack.png");
	black->setPosition(size.width / 2 + 100, size.height / 2);
	addChild(black, 0, TAG::BLACK);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameStart::touchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameStart::touchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

Scene* GameStart::createScene()
{
	auto scene = Scene::create();
	auto layer = GameStart::create();
	scene->addChild(layer);
	return scene;
}

bool GameStart::touchBegan(Touch*, Event*)
{
	return true;//touchEnded
}

void GameStart::touchEnded(Touch* touch, Event* event)
{
	//getLocation返回的是OpenGL的坐标系，getBoundingBox返回的是父节点坐标系
	auto pos = touch->getLocation();//OpenGL
	auto red = static_cast<Sprite*>(getChildByTag(TAG::RED));
	auto black = static_cast<Sprite*>(getChildByTag(TAG::BLACK));
	auto size = Director::getInstance()->getVisibleSize();
	auto select = TAG::NONE;

	if (red->getBoundingBox().containsPoint(this->convertToNodeSpace(pos)))
	{
		select = TAG::RED;
		red->setZOrder(red->getZOrder() + 1);
	}
	else if (black->getBoundingBox().containsPoint(this->convertToNodeSpace(pos)))
	{
		select = TAG::BLACK;
		black->setZOrder(black->getZOrder() + 1);
	}

	if (select != TAG::NONE)
	{
		auto moveto1 = MoveTo::create(1, size / 2);
		auto rotate1 = RotateBy::create(1, 360);
		auto spawn1 = Spawn::create(moveto1, rotate1, NULL);

		auto moveto2 = moveto1->clone();
		auto rotate2 = rotate1->reverse();
		auto spawn2 = Spawn::create(moveto2, rotate2, NULL);

		auto action1 = TargetedAction::create(red, spawn1);
		auto action2 = TargetedAction::create(black, spawn2);

		auto spawn = Spawn::create(action1, action2, NULL);
		auto sequence = Sequence::create(spawn,
			CallFunc::create([=]() {
			Director::getInstance()->replaceScene(TransitionJumpZoom::create(2, GameScene::createScene(select)));
		}),
			NULL);

		runAction(sequence);
	}

}