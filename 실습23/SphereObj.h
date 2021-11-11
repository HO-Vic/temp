#pragma once
#include<gl/glew.h>
#include<gl/freeglut.h>
#include<glm/glm/glm.hpp>
#include<glm/glm/ext.hpp>
#include<glm/glm/gtc/matrix_transform.hpp>
#include<iostream>
#include<vector>
#include<random>
class SphereObj
{
	glm::vec3 center = glm::vec3(0, 0, 0);
	float radious = 1.0f;
	glm::vec3 objDir = glm::vec3(0, 0, 0);
	glm::vec3 color;
public:
	glm::vec3 getCenter();
	float getRadious();
	glm::vec3 getDir();
	glm::vec3 getColor();
public:
	void setCenter(glm::vec3 c);
	void setRadious(float r);//scale * r
	void setDir(glm::vec3 d);
	void setColor(glm::vec3 c);
};

