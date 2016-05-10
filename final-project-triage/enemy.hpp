#ifndef __ENEMY_HPP__
#define __ENEMY_HPP__

#include <destructo-base/stdafx.h>
#include <destructo-base/TextureCache.h>
#include <destructo-base/ModelGroup.h>
#include <destructo-base/ModelInstance.h>

#include <destructo-base/GL3DProgram.h>
#include <destructo-base/MaterialProgram.h>

struct Enemy
{
	double health;

	ModelInstance *model;
	Node          *targetNode;

	vec3           a;
	vec3           b;
	bool           target;
	bool           alive;

	Enemy(ModelGroup& group, Node *sceneRoot, vec3 a, vec3 b);
	~Enemy();

	void Update(double delta, double now);
	void Draw(MaterialProgram* program);
	void Die();
};

#endif
