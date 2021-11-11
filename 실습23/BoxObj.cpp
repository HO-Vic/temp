#include "BoxObj.h"

glm::vec3 BoxObj::getCenter()
{
	return center;
}

glm::vec3 BoxObj::getScale()
{
	return glm::vec3(rangeX, rangeY, rangeZ);
}

glm::vec3 BoxObj::getColor()
{
	return color;
}

glm::vec3 BoxObj::getGravity()
{
	return objGravity;
}

glm::vec3 BoxObj::getDir()
{
	return objDir;
}

float BoxObj::getAngle()
{
	return boundingRoomAngle;
}

float BoxObj::getCurrentAngle()
{
	return objAngle;
}

void BoxObj::setCenter(glm::vec3 c)
{
	center = c;
}

void BoxObj::setScale(glm::vec3 s)
{
	rangeX = s.x;
	rangeY = s.y;
	rangeZ = s.z;
}

void BoxObj::setColor(glm::vec3 c)
{
	color = c;
}

void BoxObj::setDIr(glm::vec3 d)
{
	objDir = 0.003f * glm::normalize(d);
}

void BoxObj::setAngle(float a)
{
	boundingRoomAngle = a;
}

void BoxObj::setCurrentAngle(float ca)
{
	objAngle = ca;
}
