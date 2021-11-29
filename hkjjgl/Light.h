#pragma once
#include "Vector4.h"
#include "Vector3.h"
#include "SceneNode.h"

enum LightType{
	POINT_LIGHT,
	DIRECTIONAL_LIGHT,
	CONE_SPOT_LIGHT,
	PARALLEL_SPOT_LIGHT,
	MAX_TYPE_LIGHT
} ;

class Light : public SceneNode{
public:
	Light() {} // Default  constructor , we’ll be  needing  this  later!
	Light(std::string name, int type ,const  Vector3& position, const  Vector3& direction, const  Vector4& colour, float  radius) : SceneNode(name){
		this->position = position;
		this->colour = colour;
		this->radius = radius;
		this->direction = direction;
		this->type = type;
	}
	~Light(void) {};

	virtual void Update(float dt) override {
		SceneNode::Update(dt);
		position = worldTransform.GetPositionVector();
	}

	Vector3 GetPosition()  const { return  position; }
	void	SetPosition(const  Vector3& val) { position = val; }

	Vector3 GetDirectionn()  const { return  direction; }
	void	SetDirection(const  Vector3& val) { direction = val; }

	float	GetRadius() const { return  radius; }
	void	SetRadius(float  val) { radius = val; }

	Vector4 GetColour() const { return  colour; }
	void	SetColour(const  Vector4& val) { colour = val; }

	int     GetType() const { return  type; }
	void	SetType(const int type) { this->type = type; }

protected:
	Vector3   position;
	Vector3	  direction;
	float     radius;
	Vector4   colour;
	int		  type;
};