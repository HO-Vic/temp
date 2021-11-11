#version 330 core

in vec3 outFrag;
out vec4 FragColor;
void main()
{
	FragColor = vec4(outFrag, FragColor);
}