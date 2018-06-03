#include "cocos2d.h"

enum TAG;
enum TYPE;

class Chess : public cocos2d::Sprite
{
public:
	CREATE_FUNC(Chess);
	virtual bool init();
	static Chess* createChess(TAG, TYPE, cocos2d::Vec2);
	void initChess(TAG, TYPE, cocos2d::Vec2);

	void InitPos();//¸´Î»Æå×Ó
	void setInitPos(cocos2d::Point pos) { initPos = pos; }
	cocos2d::Point getInitPos() { return initPos; }

	cocos2d::Point getPos() { return currentPos; }
	void setPos(cocos2d::Point pos) { currentPos = pos; }

	TAG getTag() { return tag; }
	TYPE getType() { return type; }

private:
	TAG tag;
	TYPE type;
	cocos2d::Point initPos;
	cocos2d::Point currentPos;
};
