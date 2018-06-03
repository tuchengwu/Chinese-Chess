#include "Define.h"
#include "Chess.h"

USING_NS_CC;

bool Chess::init()
{
	if (!Sprite::init())
		return false;
	return true;
}

Chess* Chess::createChess(TAG tag, TYPE type, cocos2d::Vec2 pos)
{
	auto chess = new Chess();
	if (chess && chess->init())
	{
		chess->autorelease();
		chess->initChess(tag, type, pos);
	}
	else
	{
		delete chess;
		chess = NULL;
	}
	return chess;
}
void Chess::initChess(TAG tag, TYPE type, cocos2d::Vec2 pos)
{
	std::string picName;

	if (tag == TAG::RED)
	{
		picName += "r";
	}
	else if (tag == TAG::BLACK)
	{
		picName += "b";
	}
	else
	{
		CCLOG("initChess error : tag = %d", tag);
		return;
	}
	this->tag = tag;

	if (type < TYPE::JIANG || type > TYPE::BING)
	{
		CCLOG("initChess error £º type = %d", type);
		return;
	}
	else
	{
		picName += TypeStr[type];
		picName += ".png";
	}

	this->type = type;
	this->setDisplayFrame(SpriteFrame::create(picName, Rect(0, 0, 56, 56)));
	this->setPosition(pos);
	this->setScale(0.75);
}

void Chess::InitPos()
{
	this->runAction(MoveTo::create(1.5, Vec2(35 + initPos.x * 36, 20 + initPos.y * 36)));
	this->currentPos = initPos;
	this->setVisible(true);
}