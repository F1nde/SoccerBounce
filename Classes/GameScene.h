#ifndef __GAMESCENE_SCENE_H__
#define __GAMESCENE_SCENE_H__

#include "cocos2d.h"
#include "Ball.h"

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
	~GameScene();
    virtual bool init();  
    CREATE_FUNC(GameScene);

private:

	// Update function is scheduled when game needs ending/resetion
	// - calls endGame() and resetGame()
	virtual void update(float) override;

	// startGame: starts gravity
	void startGame();

	// endGame: creates message box for ending score and ending message
	void endGame();

	// resetGame: resets game state and ball
	void resetGame();


	// Reaction to touches/clicks
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

	// Reaction to collisions
	bool onContactSeparate(cocos2d::PhysicsContact& contact);

	// Setting physicsWorld
	cocos2d::PhysicsWorld* world_;
	void SetPhysicsWorld(cocos2d::PhysicsWorld* world) { world_ = world; };

	Ball* ball_;
	cocos2d::Label* scoreLabel_;

	cocos2d::Size visibleSize_;
	cocos2d::Vec2 origin_;

	bool gameGoing_ = false;
	int score_ = 0;
};

#endif // __GAMESCENE_SCENE_H__
