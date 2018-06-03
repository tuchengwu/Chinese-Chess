#ifndef __GAMESTART_H__
#define __GAMESTART_H__

#include "cocos2d.h"

class GameStart :public cocos2d::Layer
{
public:
	CREATE_FUNC(GameStart);
	virtual bool init();
	static cocos2d::Scene* createScene();

	bool touchBegan(cocos2d::Touch*, cocos2d::Event*);
	void touchEnded(cocos2d::Touch*, cocos2d::Event*);
};

#endif
