#include "enemy.hpp"

Enemy::Enemy(ModelGroup& group, Node *sceneRoot, vec3 a, vec3 b)
	: a(a), b(b)
{
	health = 2.0f;
	target = true;
	alive = true;

	model = group.NewInstance("robot02");
	model->GetNode()->Id() = "ENEMY DERP SHIT";
	model->GetNode()->LocalTransform().Translation() = a;
	model->PlayAnimation("walking");

	sceneRoot->AddChild(model->GetNode());
}

Enemy::~Enemy()
{
	delete model;
}

void Enemy::Update(double delta, double now)
{
	model->Update(delta, now);

	if(!alive)
		return;

	vec3 t = (target) ? b : a;
	vec3 pos = model->GetNode()->LocalTransform().Translation();

	if(distance(t, pos) < 0.5f)
		target = !target;

	vec3 diff = t - pos;
	vec3 dir = normalize(diff);
	pos += dir * 0.65f * (float) delta;
	model->GetNode()->LocalTransform().Translation() = pos;
	model->GetNode()->LocalTransform().Rotation() = glm::angleAxis(atan2(-diff.x, diff.z), vec3(0,1,0));
}

void Enemy::Draw(MaterialProgram* program)
{
    program->SetShadowMode(false);
    model->Draw(program);
    program->SetShadowMode(true);
    model->Draw(program);
}

void Enemy::Die()
{
	alive = false;
	model->PlayAnimation("destroyed");
}
