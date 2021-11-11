#include "BoundingRoom.h"

float BoundingRoom::getRotate()
{
	return rotateZ;
}

void BoundingRoom::setRotate(float r)
{
	rotateZ = r;
	for (int i = 0; i < 4; i++) {
		glm::vec4 temp = glm::vec4(controlPoints[i].first, controlPoints[i].second, 0, 1);
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::rotate(matrix, glm::radians(rotateZ), glm::vec3(0, 0, 1));
		temp = matrix * temp;
		points[i].first = temp.x;
		points[i].second = temp.y;
	}
}

bool BoundingRoom::isCollideSphere(SphereObj& dif)
{
	for (int i = 0; i < 4; i++) {
		float result = std::abs((points[i].first - points[(i + 1) % 4].first) * (points[(i + 1) % 4].second - dif.getCenter().y) - (points[(i + 1) % 4].first - dif.getCenter().x) * (points[i].second - points[(i + 1) % 4].second))
			/ (std::sqrtf(std::powf(points[i].first - points[(i + 1) % 4].first, 2) + std::powf(points[i].second - points[(i + 1) % 4].second, 2)));
		if (abs(result) <= abs(dif.getRadious())) {
			dif.setDir(-1.0f * dif.getDir());
			dif.setCenter(dif.getCenter() + 0.05f * dif.getDir());
			return true;
		}
	}
	return false;
}

bool BoundingRoom::isCollideBox(BoxObj& dif)
{
		if (rotateZ > 0.0f && rotateZ < 90.0f) {
			if (dif.getCenter().y - 0.5 * dif.getScale().y >= -0.5f)
				dif.setCenter(dif.getCenter() + glm::vec3(0, -0.005f, 0));
			if (dif.getCenter().x - 0.5 * dif.getScale().x >= -0.5f)
				dif.setCenter(dif.getCenter() + glm::vec3(-0.005f, 0, 0));
		}
		else if (rotateZ < 0.0f && rotateZ > -90.0f) {
			if (dif.getCenter().y - 0.5 * dif.getScale().y >= -0.5f)
				dif.setCenter(dif.getCenter() + glm::vec3(0, -0.005f, 0));
			if (dif.getCenter().x + 0.5 * dif.getScale().x <= 0.5f)
				dif.setCenter(dif.getCenter() + glm::vec3(0.005f, 0, 0));
		}
		else if (rotateZ == 90.0f || rotateZ == -270.0f) {
			if (dif.getCenter().x - 0.5 * dif.getScale().x >= -0.5f)
				dif.setCenter(dif.getCenter() + glm::vec3(-0.005f, 0, 0));
		}
		else if (rotateZ == -90.0f || rotateZ == 270.0f) {
			if (dif.getCenter().x + 0.5 * dif.getScale().x <= 0.5f)
				dif.setCenter(dif.getCenter() + glm::vec3(0.005f, 0, 0));
		}

		else if (rotateZ > 90.0f && rotateZ < 180.0f) {
			if (dif.getCenter().y + 0.5 * dif.getScale().y <= 0.5f)
				dif.setCenter(dif.getCenter() + glm::vec3(0, 0.005f, 0));
			if (dif.getCenter().x - 0.5 * dif.getScale().x >= -0.5f)
				dif.setCenter(dif.getCenter() + glm::vec3(-0.005f, 0, 0));
		}
		else if (rotateZ < -90.0f && rotateZ > -180.0f) {
			if (dif.getCenter().y + 0.5 * dif.getScale().y <= 0.5f)
				dif.setCenter(dif.getCenter() + glm::vec3(0, 0.005f, 0));
			if (dif.getCenter().x + 0.5 * dif.getScale().x <= 0.5f)
				dif.setCenter(dif.getCenter() + glm::vec3(0.005f, 0, 0));
		}
		else if (rotateZ == 180.0f || rotateZ == -180.0f) {
			if (dif.getCenter().y + 0.5 * dif.getScale().y <= 0.5f)
				dif.setCenter(dif.getCenter() + glm::vec3(0, 0.005f, 0));
		}

		else if (rotateZ > 180.0f && rotateZ < 270.0f) {
			if (dif.getCenter().y + 0.5 * dif.getScale().y <= 0.5f)
				dif.setCenter(dif.getCenter() + glm::vec3(0, 0.005f, 0));
			if (dif.getCenter().x + 0.5 * dif.getScale().x <= 0.5f)
				dif.setCenter(dif.getCenter() + glm::vec3(0.005f, 0, 0));
		}
		else if (rotateZ < -180.0f && rotateZ > -270.0f) {
			if (dif.getCenter().y + 0.5 * dif.getScale().y <= 0.5f)
				dif.setCenter(dif.getCenter() + glm::vec3(0, 0.005f, 0));
			if (dif.getCenter().x - 0.5 * dif.getScale().x >= -0.5f)
				dif.setCenter(dif.getCenter() + glm::vec3(-0.005f, 0, 0));
		}
		else if (rotateZ > 270.0f && rotateZ < 360.0f) {
			if (dif.getCenter().y - 0.5 * dif.getScale().y >= -0.5f)
				dif.setCenter(dif.getCenter() + glm::vec3(0, -0.005f, 0));
			if (dif.getCenter().x + 0.5 * dif.getScale().x <= 0.5f)
				dif.setCenter(dif.getCenter() + glm::vec3(0.005f, 0, 0));
		}
		else if (rotateZ < -270.0f && rotateZ > -360.0f) {
			if (dif.getCenter().y - 0.5 * dif.getScale().y >= -0.5f)
				dif.setCenter(dif.getCenter() + glm::vec3(0, -0.005f, 0));
			if (dif.getCenter().x - 0.5 * dif.getScale().x >= -0.5f)
				dif.setCenter(dif.getCenter() + glm::vec3(-0.005f, 0, 0));
		}
		dif.setAngle(rotateZ);
	return false;
}
