#include "cocos2d.h"

enum TAG;
class Chess;

typedef struct
{
	cocos2d::Point from;
	cocos2d::Point to;
	int killnum;
}Step;

class GameScene : public cocos2d::Layer
{
public:
	CREATE_FUNC(GameScene);
	virtual bool init();
	static cocos2d::Scene* createScene(TAG);
	static GameScene* createGame(TAG);
	void initGame(TAG);

	void menuNew(cocos2d::Ref*);
	void menuRegret(cocos2d::Ref*);
	bool touchBegan(cocos2d::Touch*, cocos2d::Event*);

	bool moveChess(int index, cocos2d::Point pos);
	bool moveChessFromTo(TYPE type, cocos2d::Point from, cocos2d::Point to);

	void winCall(float dt);

private:
	TAG turn;
	TAG tag;
	Chess* chesses[32];//32Ö»Æå×Ó
	cocos2d::Sprite* selected;
	int selectIndex;
	int pane[9][10];//ÆåÅÌ

	cocos2d::Sprite* win;
	std::vector<Step> steps;
};
