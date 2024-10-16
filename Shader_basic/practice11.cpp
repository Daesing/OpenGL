#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <GL/glew.h> // �ʿ��� ������� include
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>



CONST INT WIDTH = 800;
CONST INT HEIGHT = 600;
CONST INT LISTSIZE = 4;
std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> urd{ 0.f,1.f };
std::uniform_int_distribution uid{ 10, 25 };
std::uniform_int_distribution uid_pos{ 100,500 };
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
bool num_flag[5]{ false };


struct Shape {
	int x{}, y{};
	int type{};
	GLfloat colors[5][3] = { //--- �ﰢ�� ������ ����
	{ urd(dre), urd(dre), urd(dre)}, {urd(dre), urd(dre), urd(dre)}, {urd(dre), urd(dre), urd(dre)}, {urd(dre), urd(dre), urd(dre)} , {urd(dre), urd(dre), urd(dre)} };
	bool is_active{ false };

	void draw_shape() {
		GLfloat cord[5][3]{};

		cord[0][0] = convertX(x - 80);
		cord[0][1] = convertY(y);

		cord[1][0] = convertX(x - 40);
		cord[1][1] = convertY(y + 80);

		cord[2][0] = convertX(x);
		cord[2][1] = convertY(y - 80);

		cord[3][0] = convertX(x + 40);
		cord[3][1] = convertY(y + 80);

		cord[4][0] = convertX(x + 80);
		cord[4][1] = convertY(y);

		std::cout << cord[3][0] << '\n';
		std::cout << cord[3][1] << '\n';

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cord), &cord);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors), &colors);
		switch (type)
		{
		case 2:
			glDrawArrays(GL_LINES, 0, 2);
			break;
		case 3:
			glDrawArrays(GL_TRIANGLES, 0, 3);
			break;
		case 4:
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			break;
		case 5:
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
			break;
		default:
			break;
		}
		
	}


};

Shape shape[4];

void reset_shape(Shape* shape) {

	shape[0].x = 200;
	shape[0].y = 150;
	shape[0].type = 2;

	shape[1].x = 600;
	shape[1].y = 150;
	shape[1].type = 3;

	shape[2].x = 200;
	shape[2].y = 450;
	shape[2].type = 4;

	shape[3].x = 600;
	shape[3].y = 450;
	shape[3].type = 5;


}



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
	glutTimerFunc(200, TimerFunction, 1);
	glutMainLoop();
}


GLvoid drawScene()
{
	//--- ����� ���� ����
	glClearColor(1, 1, 1, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderProgramID);
	//--- ����� VAO �ҷ�����
	glBindVertexArray(vao);
	for (int i = 0; i < LISTSIZE; ++i) {
		if(shape[i].is_active)
			shape[i].draw_shape();
	}
	//--- �ﰢ�� �׸���
	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}


void Keyboard(unsigned char key, int x, int y) {

	reset_shape(shape);
	for (int i = 0; i < LISTSIZE; ++i) {
		shape[i].is_active = false;
		shape[i].x = 400;
		shape[i].y = 300;
	}

	for (int i = 0; i < 5; ++i) {
		num_flag[i] = false;
	}
	

	switch (key)
	{
	case '1':
		num_flag[0] = true;
		break;
	case '2':
		num_flag[1] = true;
		break;
	case '3':
		num_flag[2] = true;
		break;
	case '4':
		num_flag[3] = true;
		break;
	case '5':
		num_flag[4] = true;
		for (int i = 0; i < LISTSIZE; ++i) {
			shape[i].is_active = true;
		}
		reset_shape(shape);
		std::cout << "majong" << '\n';
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
		//�� - �ﰢ��
		if (num_flag[0]) {
			if (shape[0].is_active == false) {
				shape[0].is_active = true;
				shape[1].is_active = false;
			}
			else {
				shape[1].is_active = true;
				shape[0].is_active = false;
			}
		}
		//�ﰢ�� - �簢��
		else if (num_flag[1]) {
			if (shape[1].is_active == false) {
				shape[1].is_active = true;
				shape[2].is_active = false;
			}
			else {
				shape[2].is_active = true;
				shape[1].is_active = false;
			}
		}
		//�簢�� - ������
		else if (num_flag[2]) {
			if (shape[2].is_active == false) {
				shape[2].is_active = true;
				shape[3].is_active = false;
			}
			else {
				shape[3].is_active = true;
				shape[2].is_active = false;
			}
		}
		//������ - ��
		else if (num_flag[3]) {
			if (shape[3].is_active == false) {
				shape[3].is_active = true;
				shape[0].is_active = false;
			}
			else {
				shape[0].is_active = true;
				shape[3].is_active = false;
			}
		}
		//��ü ���� ����
		else if (num_flag[4]) {
			for (int i = 0; i < LISTSIZE; ++i) {
				if (shape[i].type < 5)
					shape[i].type += 1;
				else
					shape[i].type = 2;
			}

		}

		
		break;
	default:
		break;
	}

	glutPostRedisplay();

	glutTimerFunc(200, TimerFunction, 1);

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
	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);
	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	//--- colors �迭�� ������: 9 *float
	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
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