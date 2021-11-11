#include "SphereObj.h"

glm::vec3 SphereObj::getCenter()
{
    return center;
}

float SphereObj::getRadious()
{
    return radious;
}

glm::vec3 SphereObj::getDir()
{
    return objDir;
}

glm::vec3 SphereObj::getColor()
{
    return color;
}

void SphereObj::setCenter(glm::vec3 c)
{
    center = c;
}

void SphereObj::setRadious(float r)
{
    radious = r;
}

void SphereObj::setDir(glm::vec3 d)
{
    objDir = glm::normalize(d);
}

void SphereObj::setColor(glm::vec3 c)
{
    color = c;
}

