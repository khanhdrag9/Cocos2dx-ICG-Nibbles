#pragma once
#include "cocos2d.h"
#include <vector>

class GamePlay : public cocos2d::Layer
{
	cocos2d::Size _screenSize;


	std::vector<cocos2d::Sprite*> _listComponents;
	cocos2d::Sprite* _componentOuting;

	cocos2d::Size _spaceBetweenTouchObj;
	float _ratioScale;

	void createHead();
	void dropComponentDownBoard();
	bool onContactBegin(cocos2d::PhysicsContact& contact);
public:
	cocos2d::PhysicsWorld* _physicsWorld;

	GamePlay();
	virtual ~GamePlay();

	static cocos2d::Scene* createScene();
	bool init() override;

	CREATE_FUNC(GamePlay);

	void update(float dt) override;

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchRelease(cocos2d::Touch* touch, cocos2d::Event* event);
};