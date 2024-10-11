#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <GL/glew.h> // �ʿ��� ������� include
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#define _USE_MATH_DEFINES
#include <math.h>

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
bool num_flag[4]{ false };


struct Shape {

	int x{}, y{};
	int x1{}, y1{};
	int xs{}, ys{};
	int xe{}, ye{};
	int duration{ uid(dre) };

	GLfloat colors[3][3] = { //--- �ﰢ�� ������ ����
	{ urd(dre), urd(dre), urd(dre)}, {urd(dre), urd(dre), urd(dre)}, {urd(dre), urd(dre), urd(dre)} };
	


	void draw_shape() {
		GLfloat cord[3][3]{};

		cord[0][0] = convertX(x);
		cord[0][1] = convertY(y - 30);

		cord[1][0] = convertX(x - 40);
		cord[1][1] = convertY(y + 20);

		cord[2][0] = convertX(x + 40);
		cord[2][1] = convertY(y + 20);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cord), &cord);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors), &colors);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void modify_cord(int x_, int y_) {
		x = x_;
		y = y_;
	}

	void random_pos() {
		x = uid_pos(dre);
		y = uid_pos(dre);
	}

	void bounce() {
		if (x < 50)
			x1 = 15;

		if (x > 750)
			x1 = -15;

		if (y < 50)
			y1 = 15;

		if (y > 550)
			y1 = -15;

		x += x1;
		y += y1;
	}

	void zigzag() {
		if (x < 50) {
			x = 50;
			x1 = 15;
			y += 30;
		}

		if (x > 750) {
			x = 750;
			x1 = -15;
			y += 30;
		}

		x += x1;
		y += y1;
	}

	void rect_spiral() {
		if (x < xs) {
			x = xs;
			x1 = 0;
			y1 = 15;
			xe -= 60;
		}

		//������ �� �ε���
		if (x > xe) {
			x = xe;
			x1 = 0;
			y1 = -15;
			xs += 60;
		}
		//���� �� �ε���
		if (y < ys) {
			y = ys;
			y1 = 0;
			x1 = -15;
			ye -= 30;

		}
		//�Ʒ��� �� �ε���
		if (y > ye) {
			y = ye;
			y1 = 0;
			x1 = 15;
			ys += 30;
		}

		x += x1;
		y += y1;
	}

	void spiral() {
		float t = duration;
		duration += 15;
		x = 400 + t / 2 * cos(t * M_PI / 180);
		y = 300 + t / 2 * sin(t * M_PI / 180);
	}
};

Shape shape[4];
int cnt{};

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
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(60, TimerFunction, 1);
	glutMainLoop();
}


GLvoid drawScene()
{
	//--- ����� ���� ����
	glClearColor(1, 1, 1, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(shaderProgramID);
	//--- ����� VAO �ҷ�����
	glBindVertexArray(vao);
	//--- �ﰢ�� �׸���
	for (int i = 0; i < LISTSIZE; ++i) {
		shape[i].draw_shape();
	}
	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}
//--- �ٽñ׸��� �ݹ� �Լ�
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}


void Mouse(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (cnt > 3)
			cnt = 0;
		shape[cnt].modify_cord(x, y);
		++cnt;
	}
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y) {

	for (int i = 0; i < 4; ++i) {
		num_flag[i] = false;
	}

	switch (key) {
	case '1':
		num_flag[0] = true;
		for (int i = 0; i < LISTSIZE; ++i) {
			shape[i].random_pos();
			shape[i].x1 = 15;
			shape[i].y1 = 15;
		}
		break;
	case '2':
		num_flag[1] = true;
		for (int i = 0; i < LISTSIZE; ++i) {
			shape[i].random_pos();
			shape[i].x1 = 15;
			shape[i].y1 = 0;
		}
		break;
	case '3':
		num_flag[2] = true;
		for (int i = 0; i < LISTSIZE; ++i) {
			if (i == 0)
				shape[i].x = 750;
			else
				shape[i].x = shape[i-1].x - 50;
			shape[i].y = 50;
			shape[i].x1 = -15;
			shape[i].y1 = 0;
			shape[i].xs = 50;
			shape[i].xe = 750;
			shape[i].ys = 50;
			shape[i].ye = 550;
		}
		break;
	case '4':
		num_flag[3] = true;
		for (int i = LISTSIZE - 1; i >= 0; --i) {
			if (i == LISTSIZE - 1)
				shape[i].duration = 0;
			else 
				shape[i].duration = shape[i + 1].duration + 10;
			
		}
		break;
	default:
		break;
	}
	glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

void TimerFunction(int value) {
	std::cout << "value"<<value << '\n';
	std::cout << num_flag[0] << '\n';
	switch (value)
	{
	case 1:
		if (num_flag[0]) {
			for (int i = 0; i < LISTSIZE; ++i) {
				shape[i].bounce();

			}
		}
		else if (num_flag[1]) {
			for (int i = 0; i < LISTSIZE; ++i) {
				shape[i].zigzag();
			}


		}
		else if (num_flag[2]) {
			//���� �� �ε���
			for (int i = 0; i < LISTSIZE; ++i) {
				shape[i].rect_spiral();
			}
		}
		else if (num_flag[3]) {
			for (int i = 0; i < LISTSIZE; ++i) {
				shape[i].spiral();
			}

		}
		break;

	default:
		break;
	}
	glutPostRedisplay();

	glutTimerFunc(60, TimerFunction, 1);
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
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glEnableVertexAttribArray(0);
	//--- 2��° VBO�� Ȱ��ȭ �Ͽ� ���ε� �ϰ�, ���ؽ� �Ӽ� (����)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- ���� colors���� ���ؽ� ������ �����Ѵ�.
	//--- colors �迭�� ������: 9 *float
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
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