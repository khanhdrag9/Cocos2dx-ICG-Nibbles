#include "GamePlay.h"
#include "Defines.h"
#include <ctime>

USING_NS_CC;

GamePlay::GamePlay()
{

}

GamePlay::~GamePlay()
{
	for (auto x : _listComponents)
		CC_SAFE_DELETE(x);
	_listComponents.clear();

}

cocos2d::Scene* GamePlay::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0,0));
	auto layer = GamePlay::create();
	layer->_physicsWorld = scene->getPhysicsWorld();
	scene->addChild(layer);
	return scene;
}

bool GamePlay::init()
{
	if (!Layer::init())
		return false;

	srand(time(NULL));
	_screenSize = Director::getInstance()->getVisibleSize();

	createHead();
	dropComponentDownBoard();	//first component
	
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GamePlay::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GamePlay::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GamePlay::onTouchRelease, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GamePlay::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	scheduleUpdate();

	return true;
}

void GamePlay::update(float dt)
{
	if (_listComponents.size() > 0)
	{
		//check eat component here
	}
}

void GamePlay::createHead()
{
	//create head of snake
	auto head = Sprite::create(COMPONENT1);
	head->setPosition(_screenSize * 0.5f);
	float ratioW = _screenSize.width / ORIGIN_SCALE_W;
	float ratioH = _screenSize.height / ORIGIN_SCALE_H;
	float ratio = (ratioW < ratioH ? ratioW : ratioH);
	Size sizeHead = head->getContentSize();
	_ratioScale = (64 / (head->getContentSize().width)) * ratio;
	head->setScale(_ratioScale);

	//set physics for head
	PhysicsBody* body = PhysicsBody::createCircle(head->getContentSize().width * 0.5f);
	body->setContactTestBitmask(true);
	body->setCollisionBitmask(0x000001);
	head->setPhysicsBody(body);

	_listComponents.push_back(head);

	this->addChild(head);
}

void GamePlay::dropComponentDownBoard()
{
	if (_listComponents.size() > 0)
	{
		auto com = Sprite::create(COMPONENT1);
		com->setScale(_ratioScale);

		PhysicsBody* body = PhysicsBody::createCircle(com->getContentSize().width * 0.5f);
		body->setCollisionBitmask(0x000002);
		body->setContactTestBitmask(true);
		com->setPhysicsBody(body);

		float posX;
		float posY;
		while (true)
		{
			int minX = com->getBoundingBox().size.width;
			int minY = com->getBoundingBox().size.height;
			int maxX = _screenSize.width - minX;
			int maxY = _screenSize.height - minY;

			posX = rand() % (maxX - minX) + minX;
			posY = rand() % (maxY - minY) + minY;

			//check hit player
			int isRepeat = false;
			for (auto x : _listComponents)
			{
				if (x->getBoundingBox().containsPoint(Vec2(posX, posY)))
				{
					isRepeat = true;
					break;
				}
			}
			if (!isRepeat)break;
		}

		_componentOuting = com;
		com->setPosition(posX, posY);
		this->addChild(com);
	}
}

bool GamePlay::onContactBegin(cocos2d::PhysicsContact& contact)
{
	PhysicsBody* A = contact.getShapeA()->getBody();
	PhysicsBody* B = contact.getShapeB()->getBody();
	if (A->getCollisionBitmask() != B->getCollisionBitmask())
	{
		auto t = contact.getCurrentTarget()->getPosition();
		CCLOG("%d-%d", t.x, t.y);
	}

	return true;
}

bool GamePlay::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (_listComponents.size() > 0)
	{
		_spaceBetweenTouchObj = touch->getLocation() - _listComponents[0]->getPosition();
	}

	return true;
}

void GamePlay::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if (_listComponents.size() > 0)
	{
		_listComponents[0]->setPosition(touch->getLocation() - _spaceBetweenTouchObj);
	}
}

void GamePlay::onTouchRelease(cocos2d::Touch* touch, cocos2d::Event* event)
{

	_spaceBetweenTouchObj = Size(0, 0);
}