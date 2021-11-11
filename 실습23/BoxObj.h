#pragma once
#include<gl/glew.h>
#include<gl/freeglut.h>
#include<glm/glm/glm.hpp>
#include<glm/glm/ext.hpp>
#include<glm/glm/gtc/matrix_transform.hpp>
#include<iostream>
#include<vector>
#include<random>
class BoxObj
{
	glm::vec3 center = glm::vec3(0, 0, 0);
	glm::vec3 objGravity = glm::vec3(0, -1, 0);
	glm::vec3 objDir = glm::vec3(0, 0, 0);
	float rangeX=0.0f;
	float rangeY=0.0f;
	float rangeZ=0.0f;
	glm::vec3 color;
	float boundingRoomAngle = 0.0f;
	float objAngle = 0.0f;
public://get
	glm::vec3 getCenter();
	glm::vec3 getScale();
	glm::vec3 getColor();
	glm::vec3 getGravity();
	glm::vec3 getDir();
	float getAngle();
	float getCurrentAngle();
public://set
	void setCenter(glm::vec3 c);
	void setScale(glm::vec3 s);
	void setColor(glm::vec3 c);
	void setDIr(glm::vec3 d);
	void setAngle(float a);
	void setCurrentAngle(float ca);
};

