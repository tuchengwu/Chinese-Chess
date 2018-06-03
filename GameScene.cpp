#include "Define.h"
#include "Chess.h"
#include "GameScene.h"

USING_NS_CC;

bool GameScene::init()
{
	if (!Layer::init())
		return false;
	return true;
}

Scene* GameScene::createScene(TAG tag)
{
	auto scene = Scene::create();
	auto layer = GameScene::createGame(tag);
	scene->addChild(layer);
	return scene;
}

GameScene* GameScene::createGame(TAG tag)
{
	auto layer = new GameScene();
	if (layer && layer->init())
	{
		layer->autorelease();
		layer->initGame(tag);
	}
	else
	{
		delete layer;
		layer = NULL;
	}
	return layer;
}
void GameScene::initGame(TAG Tag)
{
	this->tag = Tag;

	auto size = Director::getInstance()->getVisibleSize();
	auto background = Sprite::create("background.png");
	background->setScale(size.height / background->getContentSize().height);
	background->setPosition(background->getBoundingBox().size.width / 2, size.height / 2);
	addChild(background);

	auto floor = Sprite::create("floor.jpg");
	floor->setScaleX((size.width - background->getBoundingBox().size.width) / floor->getContentSize().width);
	floor->setScaleY(size.height / floor->getContentSize().height);
	floor->setPosition(size.width - floor->getBoundingBox().size.width / 2, size.height / 2);
	addChild(floor);

	auto mNew = MenuItemImage::create("new.jpg", "new.jpg", CC_CALLBACK_1(GameScene::menuNew, this));
	auto mReg = MenuItemImage::create("regret.jpg", "regret.jpg", CC_CALLBACK_1(GameScene::menuRegret, this));
	auto menu = Menu::create(mNew, mReg, NULL);
	menu->alignItemsVertically();

	menu->setPosition(floor->getPosition());
	addChild(menu);

	srand(time(NULL));
	int i = 0;
	for (int tag = TAG::RED; tag <= TAG::BLACK; ++tag)//先创建红色棋子
	{
		chesses[i] = Chess::createChess((TAG)tag, TYPE::JIANG, Vec2(rand() % HEIGHT, rand() % HEIGHT));
		chesses[i]->setInitPos(Point(4, tag == Tag ? 0 : 9));
		addChild(chesses[i]);
		++i;

		int flag = 0;
		for (int times = 0; times < 2; ++times, ++flag)
		{
			chesses[i] = Chess::createChess((TAG)tag, TYPE::SHI, Vec2(rand() % HEIGHT, rand() % HEIGHT));
			chesses[i]->setInitPos(Point(3 + flag * 2, tag == Tag ? 0 : 9));
			addChild(chesses[i]);
			++i;
			chesses[i] = Chess::createChess((TAG)tag, TYPE::XIANG, Vec2(rand() % HEIGHT, rand() % HEIGHT));
			chesses[i]->setInitPos(Point(2 + flag * 4, tag == Tag ? 0 : 9));
			addChild(chesses[i]);
			++i;
			chesses[i] = Chess::createChess((TAG)tag, TYPE::MA, Vec2(rand() % HEIGHT, rand() % HEIGHT));
			chesses[i]->setInitPos(Point(1 + flag * 6, tag == Tag ? 0 : 9));
			addChild(chesses[i]);
			++i;
			chesses[i] = Chess::createChess((TAG)tag, TYPE::CHE, Vec2(rand() % HEIGHT, rand() % HEIGHT));
			chesses[i]->setInitPos(Point(0 + flag * 8, tag == Tag ? 0 : 9));
			addChild(chesses[i]);
			++i;
			chesses[i] = Chess::createChess((TAG)tag, TYPE::PAO, Vec2(rand() % HEIGHT, rand() % HEIGHT));
			chesses[i]->setInitPos(Point(1 + flag * 6, tag == Tag ? 2 : 7));
			addChild(chesses[i]);
			++i;
		}
		flag = 0;
		for (int times = 0; times < 5; ++times, ++flag)
		{
			chesses[i] = Chess::createChess((TAG)tag, TYPE::BING, Vec2(rand() % HEIGHT, rand() % HEIGHT));
			chesses[i]->setInitPos(Point(0 + flag * 2, tag == Tag ? 3 : 6));
			addChild(chesses[i]);
			++i;
		}
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::touchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	selected = Sprite::create("selected.png");
	selected->setScale(0.75);
	selected->setVisible(false);
	addChild(selected);
	selectIndex = -1;

	for (auto& i : pane)
	{
		for (auto &j : i)
		{
			j = -1;
		}
	}

	win = Sprite::create("yingjiemian.png");
	win->setPosition(size / 2);
	addChild(win);
	win->setVisible(false);
}

void GameScene::menuNew(cocos2d::Ref*)
{
	Point pos;
	int index = 0;
	for (auto i : chesses)
	{
		i->InitPos();
		pos = i->getInitPos();
		pane[(int)pos.x][(int)pos.y] = index;
		++index;
	}
	selected->setVisible(false);
	selectIndex = -1;
	turn = tag;

	steps.clear();
}

void GameScene::menuRegret(cocos2d::Ref*)
{
	if (steps.size() > 0)
	{
		Step step = steps.back();
		Point from = step.from;
		Point to = step.to;
		chesses[pane[(int)to.x][(int)to.y]]->runAction(MoveTo::create(0.1, Vec2(35 + from.x * 36, 20 + from.y * 36)));
		chesses[pane[(int)to.x][(int)to.y]]->setPos(from);
		pane[(int)from.x][(int)from.y] = pane[(int)to.x][(int)to.y];
		pane[(int)to.x][(int)to.y] = step.killnum;
		if (step.killnum != -1)
			chesses[step.killnum]->setVisible(true);
		steps.pop_back();
		selected->setVisible(false);
		selectIndex = -1;
		turn = turn == TAG::RED ? TAG::BLACK : TAG::RED;
	}

}

bool GameScene::touchBegan(cocos2d::Touch* touch, cocos2d::Event*)
{
	auto touchPos = touch->getLocation();//OpenGL
	int x = (touchPos.x - 35 + 18) / 36;
	int y = (touchPos.y - 20 + 18) / 36;
	CCLOG("x=%d,y=%d", x, y);
	if (x<0 || x>9 || y<0 || y>10)
		return false;
	moveChess(selectIndex, Point(x, y));

	return false;
}

bool GameScene::moveChess(int index, Point pos)
{
	if (pane[(int)pos.x][(int)pos.y] != -1 && chesses[pane[(int)pos.x][(int)pos.y]]->getTag() == turn)//点中棋子是我方
	{
		selected->setVisible(true);
		selected->setPosition(35 + 36 * pos.x, 20 + 36 * pos.y);
		selectIndex = pane[(int)pos.x][(int)pos.y];
		return false;//没有移动
	}
	else
	{
		if (index == -1)
			return false;
		if (moveChessFromTo(chesses[index]->getType(), chesses[index]->getPos(), pos))
		{
			selected->setVisible(false);
			selectIndex = -1;
			turn = turn == TAG::RED ? TAG::BLACK : TAG::RED;
		}
	}
}

bool GameScene::moveChessFromTo(TYPE type, cocos2d::Point from, cocos2d::Point to)
{
	int x1 = from.x;
	int x2 = to.x;
	int y1 = from.y;
	int y2 = to.y;

	int d_x = abs(x1 - x2);
	int d_y = abs(y1 - y2);

	bool res = false;
	int num = 0;

	Step step;
	switch (type)
	{
	case TYPE::JIANG:
		if (x2 < 3 || x2 >5 || (y2 > 2 && y2 <7) || d_x>1 || d_y > 1 || (x1 != x2 && y1 != y2))
			res = false;
		else
			res = true;
		break;
	case TYPE::SHI:
		if (d_x != 1 || d_y != 1 || x2 < 3 || x2 >5 || (y2 > 2 && y2 <7))
			res = false;
		else
			res = true;
		break;
	case TYPE::XIANG:
		if (d_x != 2 || d_y != 2 || (y1 <= 4 && y2>4) || (y1 > 4 && y2 <= 4) || pane[(x1 + x2) / 2][(y1 + y2) / 2] != -1)
			res = false;
		else
			res = true;
		break;
	case TYPE::MA:
		if (d_x == 2 && d_y == 1)
		{
			if (pane[(x1 + x2) / 2][y1] != -1)
				res = false;
			else
				res = true;
		}
		else if (d_y == 2 && d_x == 1)
		{
			if (pane[x1][(y1 + y2) / 2] != -1)
				res = false;
			else
				res = true;
		}
		else
			res = false;
		break;
	case TYPE::CHE:
		res = true;
		if (d_x == 0 && d_y > 0)
		{
			auto min = y1 < y2 ? y1 : y2;
			auto max = y1 > y2 ? y1 : y2;
			for (int temp = min + 1; temp < max; ++temp)
			{
				if (pane[x1][temp] != -1)
					res = false;
			}
		}
		else if (d_y == 0 && d_x > 0)
		{
			auto min = x1 < x2 ? x1 : x2;
			auto max = x1 > x2 ? x1 : x2;
			for (int temp = min + 1; temp < max; ++temp)
			{
				if (pane[temp][y1] != -1)
					res = false;
			}
		}
		else
			res = false;
		break;
	case TYPE::PAO:
		num = 0;
		if (d_x == 0)
		{
			auto min = y1 < y2 ? y1 : y2;
			auto max = y1 > y2 ? y1 : y2;
			for (int temp = min + 1; temp < max; ++temp)
			{
				if (pane[x1][temp] != -1)
					++num;
			}
			if ((num == 0 && pane[x2][y2] == -1) || (num == 1 && pane[x2][y2] != -1 && chesses[pane[x2][y2]]->getTag() != turn))
				res = true;
		}
		else if (d_y == 0)
		{
			auto min = x1 < x2 ? x1 : x2;
			auto max = x1 > x2 ? x1 : x2;
			for (int temp = min + 1; temp < max; ++temp)
			{
				if (pane[temp][y1] != -1)
					++num;
			}
			if ((num == 0 && pane[x2][y2] == -1) || (num == 1 && pane[x2][y2] != -1 && chesses[pane[x2][y2]]->getTag() != turn))
				res = true;
		}
		else
			res = false;
		break;
	case TYPE::BING:
		if (d_y == 0 && d_x == 1 && ((chesses[pane[x1][y1]]->getTag() == tag && y1>4) || (chesses[pane[x1][y1]]->getTag() != tag && y1<5)))
			res = true;
		else if (d_x == 0 && d_y == 1)
		{
			if ((chesses[pane[x1][y1]]->getTag() == tag && y2 > y1) || (chesses[pane[x1][y1]]->getTag() != tag && y2 < y1))
				res = true;
			else res = false;
		}
		else
			res = false;
		break;
	default:
		res = false;
		break;
	}

	if (res)
	{
		step.from = from;
		step.to = to;
		step.killnum = -1;
		chesses[pane[(int)from.x][(int)from.y]]->runAction(MoveTo::create(0.5, Vec2(35 + x2 * 36, 20 + y2 * 36)));
		chesses[pane[(int)from.x][(int)from.y]]->setPos(to);
		if (pane[(int)to.x][(int)to.y] != -1)
		{
			step.killnum = pane[(int)to.x][(int)to.y];
			chesses[pane[(int)to.x][(int)to.y]]->setVisible(false);
			if (chesses[pane[(int)to.x][(int)to.y]]->getType() == TYPE::JIANG)
			{
				win->setVisible(true);
				scheduleOnce(schedule_selector(GameScene::winCall), 2.0f);
			}
		}
		steps.push_back(step);
		pane[(int)to.x][(int)to.y] = pane[(int)from.x][(int)from.y];
		pane[(int)from.x][(int)from.y] = -1;
	}
	return res;
}

void GameScene::winCall(float dt)
{
	win->setVisible(false);
}