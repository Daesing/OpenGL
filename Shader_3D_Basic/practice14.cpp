#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <GL/glew.h> // �ʿ��� ������� include
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtc/type_ptr.hpp>

CONST INT WIDTH = 800;
CONST INT HEIGHT = 600;
CONST INT LISTSIZE = 4;
std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> urd{ 0.f,1.f };
std::uniform_int_distribution uid{ 10, 25 };
std::uniform_int_distribution uid_pos{ 100,500 };
bool shape_flag{ false };
bool hide_flag{ false };
bool polygon_flag{ false };
bool anime_flag[4]{ false };
float x_rotation{};
float y_rotation{};
float x_translation{};
float y_translation{};


//--- ���� �Լ�
// 
// 
// 
//--- �Լ� ���� �߰��ϱ�


void make_vertexShaders();
void make_fragmentShaders();
void make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
void InitBuffer();
char* filetobuf(const char* file);
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);


float cube[6][4][3]{
	{ {-0.2,-0.2,0},{0.2,-0.2,0},{-0.2,0.2,0},{0.2,0.2,0} } ,
	{ {-0.2,-0.2,-0.2},{0.2,-0.2,-0.2},{-0.2,0.2,-0.2 },{0.2,0.2,-0.2} },
	{ {0.2,-0.2,0},{0.2,-0.2,-0.2},{0.2,0.2,0},{0.2,0.2,-0.2} },
	{ {-0.2,-0.2,0},{-0.2,-0.2,-0.2},{-0.2,0.2,0},{-0.2,0.2,-0.2} },
	{ {-0.2,-0.2,0},{-0.2,-0.2,-0.2},{0.2,-0.2,0},{0.2,-0.2,-0.2} },
	{ {-0.2,0.2,0},{-0.2,0.2,-0.2},{0.2,0.2,0},{0.2,0.2,-0.2} }
};

float cube_colors[3][4][3]{ 
	{ {0,0,1},{0,0,1},{0,0,1},{0,0,1} },
	{ {0,1,0},{0,1,0},{0,1,0},{0,1,0} },
	{ {1,0,0},{1,0,0},{1,0,0},{1,0,0} }
};

float pyramid_bottom[4][3]{
	{-0.2,0.2,0},{0.2,0.2,0},{-0.2,-0.2,0},{0.2,-0.2,0} };

float pyramid_bottom_color[4][3]{
	{0,0,1},{0,0,1},{0,0,1},{0,0,1}
};

float pyramid[4][3][3]{
	{ {-0.2,0.2,0},{-0.2,-0.2,0},{0,0,0.2} } ,
	{ {-0.2,-0.2,0},{0.2,-0.2,0},{0,0,0.2} },
	{ {0.2,-0.2,0},{0.2,0.2,0},{0,0,0.2} },
	{ {0.2,0.2,0},{-0.2,0.2,0},{0,0,0.2} }

};

float pyramid_colors[4][3][3]{
	{ {0,1,0},{0,1,0},{0,1,0} },
	{ {1,0,0},{1,0,0},{1,0,0} },
	{ {1,1,0},{1,1,0},{1,1,0} },
	{ {1,0,1},{1,0,1},{1,0,1} }
};

float convertX(int x) {
	return ((float)x / (WIDTH / 2)) - 1.f;
}
float convertY(int y) {
	return 1.f - ((float)y / (HEIGHT / 2));
}


GLuint vao, vbo[2];
GLchar* vertexSource, * fragmentSource; //--- �ҽ��ڵ� ���� ���� 
GLuint vertexShader, fragmentShader; //--- ���̴� ��ü
GLuint shaderProgramID; //--- ���̴� ���α׷�


float line[3][2][3]{
	{ {-1,0,0},{1,0,0} },
	{ {0,-1,0},{0,1,0} },
	{ {0,0,-1},{0,0,1} }
};
float line_colors[3][2][3]{
	{ {0,0,1},{0,0,1}},
	{ {0,1,0},{0,1,0}},
	{ {1,0,0},{1,0,0},}
};



void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example1");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	InitBuffer();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(60, TimerFunction, 1);
	glutMainLoop();
}


GLvoid drawScene()
{
	//--- ����� ���� ����
	glClearColor(1, 1, 1, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (hide_flag)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	if(polygon_flag)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderProgramID);
	glm::mat4 Rz = glm::mat4(1.0f); //--- ȸ�� ��� ����
	glm::mat4 Tx = glm::mat4(1.0f); //--- �̵� ��� ����
	glm::mat4 TR = glm::mat4(1.0f);


	//--- ������ �𵨸� ��ȯ ��� �����
	Tx = glm::translate(Tx, glm::vec3(x_translation, 0.0, 0.0));
	Tx = glm::translate(Tx, glm::vec3(0.0, y_translation, 0.0));
	Rz = glm::rotate(Rz, glm::radians(y_rotation), glm::vec3(0.0, 1.0, 0.0));
	Rz = glm::rotate(Rz, glm::radians(x_rotation), glm::vec3(1.0, 0.0, 0.0));
	TR = Tx * Rz;



	//--- ���̴� ���α׷����� modelTransform ���� ��ġ ��������
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
	//--- modelTransform ������ ��ȯ �� �����ϱ�
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(TR));
	//--- ���� �׸���

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f); //--- ���� ���� ����: [-100.0, 100.0]
	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform"); //--- ���� ��ȯ �� ����
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	glBindVertexArray(vao);


	for (int i = 0; i < 3; ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(line[i]), &line[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(line_colors[i]), &line_colors[i]);
		glDrawArrays(GL_LINES, 0, 2);
	}
	if (shape_flag) {
		for (int i = 0; i < 6; ++i) {
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube[i]), &cube[i]);
			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_colors[0]), &cube_colors[i / 2]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	}
	else {

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pyramid_bottom), &pyramid_bottom);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pyramid_bottom_color), &pyramid_bottom_color);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		for (int i = 0; i < 4; ++i) {
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pyramid[i]), &pyramid[i]);
			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pyramid_colors[i]), &pyramid_colors[i]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
	}

	glutSwapBuffers();
}



void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'c':
		if (!shape_flag)
			shape_flag = true;
		else
			shape_flag = false;
		break;
	case 'h':
		if (!hide_flag)
			hide_flag = true;
		else
			hide_flag = false;
		break;
	case 'w':
		if (!polygon_flag)
			polygon_flag = true;
		else
			polygon_flag = false;
		break;
	case 'x':
		anime_flag[0] = true;
		anime_flag[1] = false;
		break;
	case 'X':
		anime_flag[0] = false;
		anime_flag[1] = true;
		break;
	case 'y':
		anime_flag[3] = false;
		anime_flag[2] = true;
		break;
	case 'Y':
		anime_flag[3] = true;
		anime_flag[2] = false;
		break;
	case 'i':
		y_translation += 0.1f;
		break;
	case 'k':
		y_translation -= 0.1f;
		break;
	case 'j':
		x_translation -= 0.1f;
		break;
	case 'l':
		x_translation += 0.1f;
		break;
	case 's':
		x_rotation = 0;
		y_rotation = 0;
		x_translation = 0;
		y_translation = 0;
		for (int i = 0; i < 4; ++i) {
			anime_flag[i] = false;
		}
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void TimerFunction(int value) {

	switch (value)
	{
	case 1:
		if (anime_flag[0]) {
			x_rotation += 5.0f;
		}
		if (anime_flag[1]) {
			x_rotation -= 5.0f;
		}
		if (anime_flag[2]) {
			y_rotation += 5.0f;
		}
		if (anime_flag[3]) {
			y_rotation -= 5.0f;
		}
		break;

	default:
		break;
	}

	glutPostRedisplay();

	glutTimerFunc(60, TimerFunction, 1);
}

void Mouse(int button, int state, int x, int y) {


}

//--- �ٽñ׸��� �ݹ� �Լ�
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�
	
	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	//--- triShape �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);
	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	//--- colors �迭�� ������: 9 *float
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	//--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(1);

	
}


void make_shaderProgram()
{
	make_vertexShaders(); //--- ���ؽ� ���̴� �����
	make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program ����ϱ�
	glUseProgram(shaderProgramID);
}



void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");
	//--- ���ؽ� ���̴� ��ü �����
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexShader);
	//--- �������� ����� ���� ���� ���: ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}


void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl");
	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentShader);
	//--- �������� ����� ���� ���� ���: ������ ���� üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading
	if (!fptr) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator
	return buf; // Return the buffer
}