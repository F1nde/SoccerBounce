#ifndef __BALL_H__
#define __BALL_H__

#include "cocos2d.h"

class Ball
{
public:
	Ball(cocos2d::Point position, cocos2d::Layer* layer);
	~Ball();
	
	// clickCheck: checks if click is inside ball sprite 
	// - cocos2d::Point position = click cordinates
	bool clickCheck(cocos2d::Point position);

	// reset: resets ball's Velocity, AngularVelocity and position
	void reset();

	// push: adds velocity to ball based on the given cordinate
	// - cocos2d::Point position = click cordinate
	void push(cocos2d::Point position);

private:
	cocos2d::Point initialPosition_;

	cocos2d::Sprite* ball_;
};

#endif // __BALL_H__
