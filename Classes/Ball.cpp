#include "Ball.h"
#include "Definitions.h"

USING_NS_CC;

Ball::Ball(cocos2d::Point position, cocos2d::Layer* layer)
{
	// Pic from: https://pixabay.com/en/football-ball-sport-soccer-round-157931/
	ball_ = Sprite::create("Ball.png");

	initialPosition_ = position;

	ball_->setPosition(initialPosition_);
	ball_->setScale(0.15f);

	auto ballBody = PhysicsBody::createCircle(ball_->getContentSize().width / 2, PhysicsMaterial(1, 1, 1));

	ballBody->setMass(100);
	ballBody->setGravityEnable(true);
	ballBody->setAngularDamping(1);
	ballBody->setCollisionBitmask(BALL_COLLISION_BITMASK);
	ballBody->setContactTestBitmask(true);

	ball_->setPhysicsBody(ballBody);

	layer->addChild(ball_);
}

Ball::~Ball() { ball_ = NULL; }

bool Ball::clickCheck(cocos2d::Point position)
{
	// Check if click position is inside ball's Bounding Box
	if (ball_->getBoundingBox().containsPoint(position)) 
	{
		return true;
	}
	else 
	{
		return false;
	}
}

void Ball::reset()
{
	std::ostringstream buffer;
	buffer << "Ball position: x: " << ball_->getPosition().x << " y: " << ball_->getPosition().y << std::endl;
	buffer << "Ball initial: x: " << initialPosition_.x << " y: " << initialPosition_.y << std::endl;
	cocos2d::log(buffer.str().c_str());

	ball_->getPhysicsBody()->setVelocity(Vect(0, 0));
	ball_->getPhysicsBody()->setAngularVelocity(0);
	ball_->setPosition(initialPosition_);
}

void Ball::push(cocos2d::Point position) 
{
	std::ostringstream buffer;
	buffer << "Click: x: " << position.x << " y: " << position.y << std::endl;
	buffer << "Ball: x: " << ball_->getPosition().x << " y: " << ball_->getPosition().y << std::endl;
	buffer << "x: " << (ball_->getPosition().x - position.x) << " y: " << (ball_->getPosition().y - position.y) << std::endl;
	cocos2d::log(buffer.str().c_str());

	// Create push cordinates
	int posx = ball_->getPosition().x - position.x;
	int posy = ball_->getPosition().y - position.y;

	// Apply Velocity
	ball_->getPhysicsBody()->setVelocity(Vect(posx, posy) * PUSH_FORCE);

	// Add AngularVelocity
	ball_->getPhysicsBody()->setAngularVelocity(10);
}
