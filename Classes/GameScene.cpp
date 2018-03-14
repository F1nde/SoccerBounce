#include <sstream>

#include "GameScene.h"
#include "Definitions.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	// Game starts without gravity
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));

	auto layer = GameScene::create();
	layer->SetPhysicsWorld(scene->getPhysicsWorld());
	scene->addChild(layer);

    return scene;
}

GameScene::~GameScene()
{
	ball_ = NULL;
	world_ = NULL;
	scoreLabel_ = NULL;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize_ = Director::getInstance()->getVisibleSize();
	origin_ = Director::getInstance()->getVisibleOrigin();

	// Create background
	Sprite* backgroundSprite = Sprite::create("Background.png");
	backgroundSprite->setPosition(Point((visibleSize_.width / 2) + origin_.x, (visibleSize_.height / 2) + origin_.y));
	this->addChild(backgroundSprite);


	// Create ball
	ball_ = new Ball(Point((visibleSize_.width / 2) + origin_.x, (visibleSize_.height / 2) + origin_.y), this);


	// Create walls
	auto edgeBody = PhysicsBody::createEdgeBox(Size(visibleSize_.width, visibleSize_.height * 2), PHYSICSBODY_MATERIAL_DEFAULT, 3);
	edgeBody->setCollisionBitmask(WALL_COLLISION_BITMASK);
	edgeBody->setContactTestBitmask(true);

	auto edgeNode = Node::create();
	edgeNode->setPosition(Point((visibleSize_.width / 2) + origin_.x, (visibleSize_.height / 2) + origin_.y));
	edgeNode->setPhysicsBody(edgeBody);

	this->addChild(edgeNode);


	// Create floor
	auto floor = PhysicsBody::createBox(Size((visibleSize_.width + origin_.x),1), PHYSICSBODY_MATERIAL_DEFAULT);
	floor->setCollisionBitmask(FLOOR_COLLISION_BITMASK);
	floor->setContactTestBitmask(true);
	floor->setDynamic(false);

	auto nodeFloor = Node::create();
	nodeFloor->setPosition(Point((visibleSize_.width / 2) + origin_.x, origin_.y));
	nodeFloor->setPhysicsBody(floor);

	this->addChild(nodeFloor);


	// Create score label
	__String* score = __String::createWithFormat( "%i", score_);
	scoreLabel_ = Label::createWithTTF(score->getCString(), "fonts/Marker Felt.ttf",
		visibleSize_.height * SCORE_FONT_SIZE);
	scoreLabel_->setColor(Color3B::BLACK);
	scoreLabel_->setPosition(Point((visibleSize_.width / 2) + origin_.x, (origin_.y + visibleSize_.height)*0.90));
	this->addChild(scoreLabel_, 100);


	// Creating collision listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactSeparate = CC_CALLBACK_1(GameScene::onContactSeparate, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	// Create touch lístener
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	return true;
}

void GameScene::update(float) 
{
	endGame();
	resetGame();

	this->unscheduleUpdate();
}

void GameScene::startGame()
{
	// Add gravity
	world_->setGravity(Vec2(0, GRAVITY_FORCE));
}

void GameScene::endGame()
{
	// Create message box for score
	std::ostringstream message;
	message << "Your score is: " << score_;
	MessageBox(message.str().c_str(), "Well done!");
}

void GameScene::resetGame()
{
	// Reset ball
	ball_->reset();

	// Gravity to 0
	world_->setGravity(Vec2(0, 0));

	gameGoing_ = false;
	score_ = 0;

	// Update score label
	__String* score = __String::createWithFormat("%i", score_);
	scoreLabel_->setString(score->getCString());
}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::Vec2 location = touch->getLocation();

	// Ask ball if the click hit the ball
	// If there is multiple balls -> check them all
	if (ball_->clickCheck(location))
	{
		// Start game if this is the first click
		if (gameGoing_ == false)
		{
			gameGoing_ = true;
			startGame();
		}

		++score_;

		// Update scoreLabel
		__String* score = __String::createWithFormat("%i", score_);
		scoreLabel_->setString(score->getCString());

		// Send information to ball about click location
		ball_->push(location);

		return true;
	}

	return false;
} 

bool GameScene::onContactSeparate(cocos2d::PhysicsContact& contact)
{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	// Ball collision with floor and walls
	if ((a->getCollisionBitmask() == BALL_COLLISION_BITMASK && b->getCollisionBitmask() == FLOOR_COLLISION_BITMASK)
		|| (a->getCollisionBitmask() == FLOOR_COLLISION_BITMASK && b->getCollisionBitmask() == BALL_COLLISION_BITMASK))
	{
		// Ball hit floor
		this->scheduleUpdate();
		return true;
	}
	else if ((a->getCollisionBitmask() == BALL_COLLISION_BITMASK && b->getCollisionBitmask() == WALL_COLLISION_BITMASK)
		|| (a->getCollisionBitmask() == WALL_COLLISION_BITMASK && b->getCollisionBitmask() == BALL_COLLISION_BITMASK)
		|| (a->getCollisionBitmask() == BALL_COLLISION_BITMASK && b->getCollisionBitmask() == BALL_COLLISION_BITMASK))
	{
		// Ball hit wall
		return true;
	}
	else
	{
		// Ball didn't hit floor/wall -> not interesting
		return false;
	}
}
