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
std::uniform_int_distribution uid_cube{ 0,5 };
std::uniform_int_distribution uid_polyhydron{ 0,3 };

struct Shape {
	float shape_face[4][3]{};
	float colors[4][3]{ {0,0,1},{0,0,1},{0,0,1},{0,0,1} };
	GLuint vao, vbo[2];
};

Shape cube[6]{};

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
GLuint shaderProgramID1; //--- ���̴� ���α׷�

bool cube_flag[6]{ false };
bool polyhydron_flag[4]{ false };

float cube1[4][3]{ {-0.2,-0.2,0},{0.2,-0.2,0},{-0.2,0.2,0},{0.2,0.2,0} };
float cube2[4][3]{ {-0.2,-0.2,-0.2},{0.2,-0.2,-0.2},{-0.2,0.2,-0.2 },{0.2,0.2,-0.2} };

float cube3[4][3]{ {0.2,-0.2,0},{0.2,-0.2,-0.2},{0.2,0.2,0},{0.2,0.2,-0.2} };
float cube4[4][3]{ {-0.2,-0.2,0},{-0.2,-0.2,-0.2},{-0.2,0.2,0},{-0.2,0.2,-0.2} };

float cube5[4][3]{ {-0.2,-0.2,0},{-0.2,-0.2,-0.2},{0.2,-0.2,0},{0.2,-0.2,-0.2} };
float cube6[4][3]{ {-0.2,0.2,0},{-0.2,0.2,-0.2},{0.2,0.2,0},{0.2,0.2,-0.2} };

float polyhydron1[3][3]{ {0,0.2,0},{-0.2,-0.2,0},{0.2,-0.2,0} };
float polyhydron2[3][3]{ {0,0,0.2},{0.2,-0.2,0},{0,0.2,0} };
float polyhydron3[3][3]{ {0,0,0.2},{0.2,-0.2,0},{-0.2,-0.2,0} };
float polyhydron4[3][3]{ {0,0,0.2},{-0.2,-0.2,0},{0,0.2,0} };

float colors1[4][3]{ {0,0,1},{0,0,1},{0,0,1},{0,0,1} };
float colors2[4][3]{ {0,1,0},{0,1,0},{0,1,0},{0,1,0} };
float colors3[4][3]{ {1,0,0},{1,0,0},{1,0,0},{1,0,0} };

float colors4[3][3]{ {0,0,1},{0,0,1},{0,0,1} };
float colors5[3][3]{ {0,1,0},{0,1,0},{0,1,0} };
float colors6[3][3]{ {1,0,0},{1,0,0},{1,0,0} };
float colors7[3][3]{ {0.5,0.5,0},{0.5,0.5,0},{0.5,0.5,0} };

float trishape[3][3]{ {0.2,0,0.2},{-0.2,0,0.4},{0,0.5,-0.5} };

float line[6][3]{ {-1,0,0},{1,0,0}, {0,-1,0},{0,1,0}, {0,0,-1},{0,0,1} };
float line1[2][3]{ {-1,0,0},{1,0,0} };
float line2[2][3]{ {0,-1,0},{0,1,0} };
float line3[2][3]{ {0,0,-1},{0,0,1} };

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ����
{
	//--- ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA| GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example1");
	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	InitBuffer();
	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}


GLvoid drawScene()
{
	//--- ����� ���� ����
	glClearColor(1, 1, 1, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderProgramID);
	glm::mat4 Rz = glm::mat4(1.0f); //--- ȸ�� ��� ����
	glm::mat4 Tx = glm::mat4(1.0f); //--- �̵� ��� ����
	glm::mat4 TR = glm::mat4(1.0f);


	


	//--- ������ �𵨸� ��ȯ ��� �����
	Tx = glm::translate(Tx, glm::vec3(0.5, 0.0, 0.0));
	Rz = glm::rotate(Rz, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
	Rz = glm::rotate(Rz, glm::radians(30.0f), glm::vec3(0.0, 1.0, 0.0));
	TR = Rz;

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

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(line1), &line1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors1), &colors1);
	glDrawArrays(GL_LINES, 0, 2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(line2), &line2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors2), &colors2);
	glDrawArrays(GL_LINES, 0, 2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(line3), &line3);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors3), &colors3);
	glDrawArrays(GL_LINES, 0, 2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube1), &cube1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors1), &colors1);
	if (cube_flag[0] == true)
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube2), &cube2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors1), &colors1);
	if (cube_flag[1] == true)
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube3), &cube3);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors2), &colors2);
	if (cube_flag[2] == true)
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube4), &cube4);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors2), &colors2);
	if (cube_flag[3] == true)
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube5), &cube5);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors3), &colors3);
	if (cube_flag[4] == true)
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube6), &cube6);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors3), &colors3);
	if (cube_flag[5] == true)
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(polyhydron1), &polyhydron1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors4), &colors4);
	if (polyhydron_flag[0] == true)
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(polyhydron2), &polyhydron2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors5), &colors5);
	if (polyhydron_flag[1] == true)
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(polyhydron3), &polyhydron3);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors6), &colors6);
	if (polyhydron_flag[2] == true)
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(polyhydron4), &polyhydron4);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors7), &colors7);
	if (polyhydron_flag[3] == true)
	glDrawArrays(GL_TRIANGLES, 0, 3);



	glutSwapBuffers();
}

void Keyboard(unsigned char key, int x, int y) {
	for (int i = 0; i < 6; ++i) {
		cube_flag[i] = false;
	}

	for (int i = 0; i < 4; ++i) {
		polyhydron_flag[i] = false;
	}

	switch (key)
	{
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '0':
		cube_flag[key - '0'] = true;
		break;
	case '6':
	case '7':
	case '8':
	case '9':
		polyhydron_flag[key - '6'] = true;
		break;
	case 'c':
		cube_flag[uid_cube(dre)] = true;
		cube_flag[uid_cube(dre)] = true;
		break;

	case 't':
		polyhydron_flag[uid_polyhydron(dre)] = true;
		polyhydron_flag[uid_polyhydron(dre)] = true;
		break;
	default:
		break;
	}
	glutPostRedisplay();
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
	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�
	glGenBuffers(2, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- ���� diamond ���� ���ؽ� ������ ���� ���ۿ� �����Ѵ�.
	//--- triShape �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), trishape, GL_STATIC_DRAW);
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