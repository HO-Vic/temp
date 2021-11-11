#pragma once
#include<gl/glew.h>
#include<gl/freeglut.h>
#include<glm/glm/glm.hpp>
#include<glm/glm/ext.hpp>
#include<glm/glm/gtc/matrix_transform.hpp>
#include<iostream>
#include<vector>
#include<random>
#include<queue>
#include<utility>
#include<math.h>
#include"SphereObj.h"
#include"BoxObj.h"
class BoundingRoom
{
	float rotateZ = 0.0f;
	const std::pair<float, float> controlPoints[4] = {
		{-0.5f, -0.5f},
		{0.5f, -0.5f},
		{0.5f, 0.5f},
		{-0.5f,0.5f}
	};
	std::pair<float, float> points[4] = {
		{-0.5f, -0.5f},
		{0.5f, -0.5f},
		{0.5f, 0.5f},
		{-0.5f,0.5f}
	};

public:
	float getRotate();
public:
	void setRotate(float r);
public:
	bool isCollideSphere(SphereObj& dif);
	bool isCollideBox(BoxObj& dif);
};

