#include<gl/glew.h>
#include<gl/freeglut.h>
#include<glm/glm/glm.hpp>
#include<glm/glm/ext.hpp>
#include<glm/glm/gtc/matrix_transform.hpp>
//#include<glm/glm.hpp>
//#include<glm/ext.hpp>
//#include<glm/gtc/matrix_transform.hpp>
#include<iostream>
#include<vector>
#include<random>
#include<queue>
#include"readObj.h"
#include"filetobuf.h"
#include"BoxObj.h"
#include"SphereObj.h"
#include"BoundingRoom.h"

using namespace std;

//shader func
void makeVertexShader();
void makeFragmentShader();
void makeShaderID();
void InitBuffer();

//call_back
void timercall(int value);
void DrawSceneCall();
void ReshapeCall(int w, int h);
void keyboardCall(unsigned char key, int x, int y);
void specialkeycall(int key, int x, int y);
void mouseCall(int button, int status, int x, int y);
void motionCall(int x, int y);
//obj
void makeObj();
void makeSphere();
void makeBox();
//draw
void drawSphere(SphereObj o);
void drawBox(BoxObj o);

//mouse
bool isClick = false;
float originX;
float originY;

GLuint fragmentShader;
GLuint modelvertexShader;
GLuint ShaderID;

char* vertexSource;
char* fragmentSource;

GLuint topVAO;
GLuint bottomVAO;
GLuint leftVAO;
GLuint rightVAO;
GLuint backVAO;
GLuint frontVAO;

GLuint topVBO;
GLuint bottomVBO;
GLuint leftVBO;
GLuint rightVBO;
GLuint backVBO;
GLuint frontVBO;

GLuint BoxVAO;
GLuint BoxVBO;

int Wwidth = 800;
int Wheight = 600;

const string fileName[11]{
	"square1.obj",
	"square3.obj",
	"square2.obj",
	"cube.obj",
	"cube2.obj"

};
vector<glm::vec4> topSide;
vector<glm::vec4> bottomSide;
vector<glm::vec4> leftSide;
vector<glm::vec4> rightSide;
vector<glm::vec4> backSide;
bool bottomSideOpen = false;
bool boxOut = false;
float bottomAngle = 0.0f;
vector<glm::vec4> BoxSide;


//camera move
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.4f);
float cameraRevoluYAngle = 0.0f;
//projection

//class
BoundingRoom br;
queue<SphereObj> sphereQ;
BoxObj box[3];

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(Wwidth, Wheight);
	glutCreateWindow("실습 23");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		cerr << "fail Initialize" << endl;
	else cout << "Initialize" << endl;


	makeShaderID();
	glUseProgram(ShaderID);
	makeObj();
	makeBox();
	glutMouseFunc(mouseCall);
	glutMotionFunc(motionCall);
	glutDisplayFunc(DrawSceneCall);
	glutReshapeFunc(ReshapeCall);
	glutKeyboardFunc(keyboardCall);
	glutSpecialFunc(specialkeycall);
	glutTimerFunc(1, timercall, 1);
	glutMainLoop();
}



void timercall(int value)
{
	for (int i = 0; i < sphereQ.size(); i++) {
		SphereObj temp = sphereQ.front();
		sphereQ.pop();
		br.isCollideSphere(temp);
		temp.setCenter(temp.getCenter() + 0.007f * temp.getDir());
		sphereQ.push(temp);
	}
	glm::vec3 tempC[3];
	for (int i = 0; i < 3; i++)
		tempC[i] = box[i].getCenter();
	if (br.getRotate() >= -90.0f && br.getRotate() <= 90.0f && bottomAngle == 90.0f && bottomSideOpen) {
	for (int i = 0; i < 3; i++)
		box[i].setCenter(box[i].getCenter() + glm::vec3(-0.005, -0.007, 0));
	boxOut = true;
	}
	else if (br.getRotate() >= -90.0f && br.getRotate() <= 90.0f && bottomSideOpen) {
		for (int i = 0; i < 3; i++) {
			box[i].setCenter(tempC[i] + bottomAngle * 0.0005f * glm::vec3(cos(glm::radians(180.0f + bottomAngle)), sin(glm::radians(180.0f + bottomAngle)), 0));
			box[i].setCurrentAngle(bottomAngle);
		}
	}
	else
		for(int i=0;i<3;i++)
			if(!boxOut)
				br.isCollideBox(box[i]); 
	cout << "angle" << bottomAngle << endl;
	cout <<"angle" << cos(180 + bottomAngle) << ' ' << sin(180 + bottomAngle) << endl;
	//cout << "box Pos" << box[0].getCenter().x << ' ' << box[0].getCenter().y << ' ' << box[0].getCenter().z << endl;
	if (bottomSideOpen) {
		if (bottomAngle < 90)
			bottomAngle += 1;
		if (bottomAngle >= 90)
			bottomAngle = 90;
	}
	else {
		if (bottomAngle > 0)
			bottomAngle -= 1;
		if (bottomAngle <= 0)
			bottomAngle = 0;
	}
	cout << br.getRotate() << endl;
	glutPostRedisplay();
	glutTimerFunc(17, timercall, value);
}


void DrawSceneCall()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	InitBuffer();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//카메라
	glm::vec3 objCenter = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::mat4 cameraRevolMatrix = glm::mat4(1.0f);
	cameraRevolMatrix = glm::rotate(cameraRevolMatrix, glm::radians(cameraRevoluYAngle), glm::vec3(0, 1, 0));
	glm::vec4 cameraPos_4 = glm::vec4(cameraPos, 1);
	cameraPos_4 = cameraRevolMatrix * cameraPos_4;
	glm::vec3 cameraR = glm::vec3(cameraPos_4.x, cameraPos_4.y, cameraPos_4.z);
	glm::vec3 cameraDir = glm::normalize(cameraR - objCenter);//n
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::cross(up, cameraDir);//v
	glm::vec3 cameraUp = glm::cross(cameraDir, cameraRight);
	glm::mat4 cameraView = glm::mat4(1.0f);
	cameraView = glm::lookAt(cameraR, objCenter, cameraUp);
	unsigned int cameraViewLocation = glGetUniformLocation(ShaderID, "viewTransform");
	glUniformMatrix4fv(cameraViewLocation, 1, GL_FALSE, glm::value_ptr(cameraView));
	//카메라#
	{
		//원근 투영
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::translate(projection, glm::vec3(0, 0, -5.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)Wwidth / (float)Wheight, 0.1f, 50.0f);
		unsigned int projectionLocation = glGetUniformLocation(ShaderID, "projectionTransform");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));


		//상자 내부
		glm::mat4 cubeMatrix = glm::mat4(1.0f);
		cubeMatrix = glm::rotate(cubeMatrix, glm::radians(br.getRotate()), glm::vec3(0, 0, 1));
		unsigned int cubeMatLocation = glGetUniformLocation(ShaderID, "Matrix");
		glUniformMatrix4fv(cubeMatLocation, 1, GL_FALSE, glm::value_ptr(cubeMatrix));
		glm::vec3 cubeColor = glm::vec3(0.2f, 0.2f, 0.2f);
		unsigned int cubeColorLocation = glGetUniformLocation(ShaderID, "fragment");
		glUniform3fv(cubeColorLocation, 1, glm::value_ptr(cubeColor));
		glBindVertexArray(rightVAO);
		glDrawArrays(GL_QUADS, 0, 24);

		cubeColor = glm::vec3(0.2f, 0.2f, 0.2f);
		glUniform3fv(cubeColorLocation, 1, glm::value_ptr(cubeColor));
		glBindVertexArray(leftVAO);
		glDrawArrays(GL_QUADS, 0, 24);

		cubeColor = glm::vec3(0.4f, 0.4f, 0.4f);
		glUniform3fv(cubeColorLocation, 1, glm::value_ptr(cubeColor));
		glBindVertexArray(topVAO);
		glDrawArrays(GL_QUADS, 0, 24);

		cubeColor = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(cubeColorLocation, 1, glm::value_ptr(cubeColor));
		glBindVertexArray(backVAO);
		glDrawArrays(GL_QUADS, 0, 24);

		cubeMatrix = glm::translate(cubeMatrix, glm::vec3(0.5, -0.5, 0));
		cubeMatrix = glm::rotate(cubeMatrix, glm::radians(bottomAngle), glm::vec3(0, 0, 1));
		cubeMatrix = glm::translate(cubeMatrix, glm::vec3(-0.5, 0.5, 0));
		glUniformMatrix4fv(cubeMatLocation, 1, GL_FALSE, glm::value_ptr(cubeMatrix));
		cubeColor = glm::vec3(1, 1, 1);
		glUniform3fv(cubeColorLocation, 1, glm::value_ptr(cubeColor));
		glBindVertexArray(bottomVAO);
		glDrawArrays(GL_QUADS, 0, 24);
	}

	for (int i = 0; i < sphereQ.size(); i++) {
		SphereObj temp = sphereQ.front();
		sphereQ.pop();
		drawSphere(temp);
		sphereQ.push(temp);
	}
	for (int i = 0; i < 3; i++) {
		drawBox(box[i]);
	}

	glutSwapBuffers();
}

void ReshapeCall(int w, int h)
{
	glViewport(0, 0, w, h);
	Wwidth = w;
	Wheight = h;
}

void keyboardCall(unsigned char key, int x, int y)
{
	switch (key)
	{	
	case'y':
		cameraRevoluYAngle += 1;
		break;
	case'Y':
		cameraRevoluYAngle -= 1;
		break;
	case'b':
		makeSphere();
		break;
	case'B':
		if(sphereQ.size() > 0)
			sphereQ.pop();
		break;
	case'o':
		bottomSideOpen = true;
		break;
	case'O':
		bottomSideOpen = false;
		break;
	default:
	case'k':
		makeBox();
		break;
		break;
	}
	glutPostRedisplay();
}

void specialkeycall(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_HOME:
		break;
	default:
		break;
	}
}

void mouseCall(int button, int status, int x, int y)
{
	if (status == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		isClick = true;
		/*originX = (float)(x - Wwidth / 2) / Wwidth * 2;
		originY = (float)(Wheight / 2 - y) / Wheight * 2;*/
		originX = x;
		originY = y;
	}
	else isClick = false;
	glutPostRedisplay();
}

void motionCall(int x, int y)
{
	/*float Mx = (float)(x - Wwidth / 2) / Wwidth * 2;
	float My = (float)(Wheight / 2 - y) / Wheight * 2;*/
	if (isClick) {
		if(br.getRotate() > 360)
		br.setRotate((originX - x) * 0.003f);
		else br.setRotate(br.getRotate() + (originX - x) * 0.003f);
		if (br.getRotate() < -360)
			br.setRotate((originX - x) * 0.003f);
		else br.setRotate(br.getRotate() + (originX - x) * 0.003f);
	}
	glutPostRedisplay();
}

void makeObj()
{
	ReadObj(fileName[0], topSide);
	bottomSide = topSide;
	ReadObj(fileName[1], leftSide);
	rightSide = leftSide;
	ReadObj(fileName[2], backSide);
	for (auto iter = topSide.begin(); iter < topSide.end(); iter++) {
		iter->y += 0.5;
	}
	for (auto iter = bottomSide.begin(); iter < bottomSide.end(); iter++) {
		iter->y -= 0.5;
	}
	for (auto iter = leftSide.begin(); iter < leftSide.end(); iter++) {
		iter->x -= 0.5f;
	}
	for (auto iter = rightSide.begin(); iter < rightSide.end(); iter++) {
		iter->x += 0.5f;
	}
	for (auto iter = backSide.begin(); iter < backSide.end(); iter++) {
		iter->z -= 0.5;
	}
	ReadObj(fileName[3], BoxSide);
	for (auto iter = BoxSide.begin(); iter < BoxSide.end(); iter++) {
		iter->y -= 0.5f;
	}
}

void makeSphere()
{
	if (sphereQ.size() != 5) {
		random_device rd;
		default_random_engine dre(rd());
		uniform_real_distribution<float> ColorUrd(0, 1);
		uniform_real_distribution<float> posUrd(-0.5f, 0.5f);
		uniform_real_distribution<float> dirUrd(-1.0f, 1.0f);
		uniform_real_distribution<float> radUrd(0.4f, 1.2f);
		uniform_real_distribution<float> scaleUrd(0.03f, 0.05f);
		SphereObj temp;
		temp.setColor(glm::vec3(ColorUrd(dre), ColorUrd(dre), ColorUrd(dre)));
		glm::vec4 colVec = glm::vec4(posUrd(dre), posUrd(dre), posUrd(dre), 1);
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::rotate(matrix, glm::radians(br.getRotate()), glm::vec3(0, 0, 1));
		colVec = matrix * colVec;
		temp.setCenter(glm::vec3(colVec.x, colVec.y, colVec.z));
		temp.setRadious(radUrd(dre) * scaleUrd(dre));
		temp.setDir(glm::vec3(dirUrd(dre), dirUrd(dre), 0));
		sphereQ.push(temp);
	}
}

void makeBox()
{
	boxOut = false;
	random_device rd;
	default_random_engine dre(rd());
	uniform_real_distribution<float> scaleUrd(0.07f, 0.2f);
	for (int i = 0; i < 3; i++) {
		float tempScale = scaleUrd(dre);
		box[i].setScale(glm::vec3(tempScale, tempScale, tempScale));
		box[i].setColor(glm::vec3(0.8, 0, 0));
	}
	box[0].setCenter(glm::vec3(0, -0.5f + 0.5f * box[0].getScale().y, -0.5f + 0.1));
	box[1].setCenter(glm::vec3(0, -0.5f + 0.5f * box[1].getScale().y, -0.5f + 0.3));
	box[2].setCenter(glm::vec3(0, -0.5f + 0.5f * box[2].getScale().y, 0.1f));
}

void drawSphere(SphereObj o)
{
	GLUquadric* sphere = gluNewQuadric();
	glm::mat4 sphereMatrix = glm::mat4(1.0f);
	sphereMatrix = glm::translate(sphereMatrix, o.getCenter());	
	unsigned int SphereMatrixLocation = glGetUniformLocation(ShaderID, "Matrix");
	glUniformMatrix4fv(SphereMatrixLocation, 1, GL_FALSE, glm::value_ptr(sphereMatrix));
	unsigned int colorLocation = glGetUniformLocation(ShaderID, "fragment");
	glUniform3fv(colorLocation, 1, glm::value_ptr(o.getColor()));
	gluSphere(sphere, o.getRadious(), 20, 20);

}

void drawBox(BoxObj o)
{
	glm::mat4 BoxMatrix = glm::mat4(1.0f);
	BoxMatrix = glm::rotate(BoxMatrix, glm::radians(o.getAngle()), glm::vec3(0, 0, 1));
	BoxMatrix = glm::translate(BoxMatrix, o.getCenter());
	BoxMatrix = glm::rotate(BoxMatrix, glm::radians(o.getCurrentAngle()), glm::vec3(0, 0, 1));
	BoxMatrix = glm::scale(BoxMatrix, o.getScale());
	unsigned int cubeMatLocation = glGetUniformLocation(ShaderID, "Matrix");
	glUniformMatrix4fv(cubeMatLocation, 1, GL_FALSE, glm::value_ptr(BoxMatrix));
	glm::vec3 BoxColor = glm::vec3(0.8f, 0.2f, 0.2f);
	unsigned int BoxColorLocation = glGetUniformLocation(ShaderID, "fragment");
	glUniform3fv(BoxColorLocation, 1, glm::value_ptr(BoxColor));
	glBindVertexArray(BoxVAO);
	glDrawArrays(GL_QUADS, 0, 24);
}


void makeVertexShader()
{

	vertexSource = filetobuf("modelVertexShader.glsl");
	modelvertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(modelvertexShader, 1, &vertexSource, NULL);
	glCompileShader(modelvertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(modelvertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(modelvertexShader, 512, NULL, errorLog);
		cerr << "VERTEXSHADER ERROR: " << errorLog << endl;
	}
}

void makeFragmentShader()
{
	fragmentSource = filetobuf("fragmentShader.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "FRAGMENT SHADER ERROR: " << errorLog << endl;
	}
}

void makeShaderID()
{
	makeVertexShader();
	makeFragmentShader();


	ShaderID = glCreateProgram();

	glAttachShader(ShaderID, modelvertexShader);
	glAttachShader(ShaderID, fragmentShader);

	glLinkProgram(ShaderID);

	glDeleteShader(modelvertexShader);
	glDeleteShader(fragmentShader);

	GLint result;
	glGetProgramiv(ShaderID, GL_LINK_STATUS, &result);

	GLchar errorLog[512];
	if (!result) {
		glGetProgramInfoLog(ShaderID, 512, NULL, errorLog);
		cerr << "ShaderID Program ERROR: " << errorLog << endl;
	}
}

void InitBuffer()
{
	glUseProgram(ShaderID);

	glGenVertexArrays(1, &topVAO);
	glBindVertexArray(topVAO);
	glGenBuffers(1, &topVBO);
	glBindBuffer(GL_ARRAY_BUFFER, topVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * topSide.size(), &topSide[0], GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &bottomVAO);
	glBindVertexArray(bottomVAO);
	glGenBuffers(1, &bottomVBO);
	glBindBuffer(GL_ARRAY_BUFFER, bottomVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * bottomSide.size(), &bottomSide[0], GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &leftVAO);
	glBindVertexArray(leftVAO);
	glGenBuffers(1, &leftVBO);
	glBindBuffer(GL_ARRAY_BUFFER, leftVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * leftSide.size(), &leftSide[0], GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &rightVAO);
	glBindVertexArray(rightVAO);
	glGenBuffers(1, &rightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * rightSide.size(), &rightSide[0], GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &backVAO);
	glBindVertexArray(backVAO);
	glGenBuffers(1, &backVBO);
	glBindBuffer(GL_ARRAY_BUFFER, backVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * backSide.size(), &backSide[0], GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(0);

	glGenVertexArrays(1, &BoxVAO);
	glBindVertexArray(BoxVAO);
	glGenBuffers(1, &BoxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, BoxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * BoxSide.size(), &BoxSide[0], GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(0);
}
