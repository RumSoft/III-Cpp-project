#pragma once

class CActor;

class IActorState
{
public:
	virtual void Enter(CActor* actor) = 0;
	//virtual IActorState* Update(CActor* actor, float frameTime) = 0;
	virtual IActorState* HandleInput(CActor* actor, float frameTime) = 0;
	virtual void Exit(CActor* actor) = 0;

};

class ActorIdleState : public IActorState
{
public:
	void Enter(CActor* actor) override{}
	IActorState* HandleInput(CActor* actor, float frameTime) override { return nullptr; }
	void Exit(CActor* actor) override{}
};

class ActorWalkingState : public IActorState
{
public:
	void Enter(CActor* actor) override {}
	IActorState* HandleInput(CActor* actor, float frameTime) override { return nullptr; }
	void Exit(CActor* actor) override {}
};

class ActorSprintingState : IActorState
{
public:
	void Enter(CActor* actor) override {}
	IActorState* HandleInput(CActor* actor, float frameTime) override { return nullptr; }
	void Exit(CActor* actor) override {}
};