#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <GL/glew.h> // 필요한 헤더파일 include
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>

CONST INT WIDTH = 800;
CONST INT HEIGHT = 600;
CONST INT LISTSIZE = 3;

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_int_distribution uid{ 0,9 };
std::uniform_real_distribution<float> urd{ 0.f,1.f };
std::uniform_int_distribution uid_y{ 10,50 };

void make_vertexShaders();
void make_fragmentShaders();
void make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
void InitBuffer();
char* filetobuf(const char* file);
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void print_tri_list();


float convertX(int x) {
	return ((float)x / (WIDTH / 2)) - 1.f;
}
float convertY(int y) {
	return 1.f - ((float)y / (HEIGHT / 2));
}


GLuint vao, vbo[2];
GLint width, height;
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체

int cnt_list1{};
int cnt_list2{};
int cnt_list3{};
int cnt_list4{};

int type{};

//--- 메인 함수
//--- 함수 선언 추가하기

struct Shape {

	GLfloat cord[3][3]{};
	GLfloat colors[3][3] = { //--- 삼각형 꼭지점 색상
	{ urd(dre), urd(dre), urd(dre)}, {urd(dre), urd(dre), urd(dre)}, {urd(dre), urd(dre), urd(dre)} };

	bool is_active{ false };


	void tri_draw() {

		if (!is_active)
			return;
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cord), &cord);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(colors), &colors);
		if (type == 1)
			glDrawArrays(GL_LINES, 0, 2);
		else if (type == 2)
			glDrawArrays(GL_TRIANGLES, 0, 3);
	}


	void modify_cord(int x, int y) {

		cord[0][0] = convertX(x);
		cord[0][1] = convertY(y - uid_y(dre));

		cord[1][0] = convertX(x - 40);
		cord[1][1] = convertY(y + 20);

		cord[2][0] = convertX(x + 40);
		cord[2][1] = convertY(y + 20);

		is_active = true;
	}

};

Shape list1[LISTSIZE];
Shape list2[LISTSIZE];
Shape list3[LISTSIZE];
Shape list4[LISTSIZE];

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Example1");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();

	glutMouseFunc(Mouse);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	InitBuffer();
	glutMainLoop();
}

GLvoid drawScene()
{
	//--- 변경된 배경색 설정
	glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderProgramID);
	//--- 사용할 VAO 불러오기
	glBindVertexArray(vao);
	//--- 삼각형 그리기
	print_tri_list();




	glutSwapBuffers(); //--- 화면에 출력하기
}
//--- 다시그리기 콜백 함수

void print_tri_list() {

	for (int i = 0; i < LISTSIZE; ++i) {
		list1[i].tri_draw();
		list2[i].tri_draw();
		list3[i].tri_draw();
		list4[i].tri_draw();
	}
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (x < 400 && y < 300) {
			for (int i = 0; i < LISTSIZE; ++i) {
				list1[i].is_active = false;
			}
			list1[0].modify_cord(x, y);

		}
		else if (x > 400 && y < 300) {
			for (int i = 0; i < LISTSIZE; ++i) {
				list2[i].is_active = false;
			}
			list2[0].modify_cord(x, y);

		}
		else if (x < 400 && y > 300) {
			for (int i = 0; i < LISTSIZE; ++i) {
				list3[i].is_active = false;
			}
			list3[0].modify_cord(x, y);

		}
		else if (x > 400 && y > 300) {
			for (int i = 0; i < LISTSIZE; ++i) {
				list4[i].is_active = false;
			}
			list4[0].modify_cord(x, y);

		}


	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {

		if (x < 400 && y < 300) {

			++cnt_list1;

			if (cnt_list1 == 3)
				cnt_list1 = 0;

			list1[cnt_list1].modify_cord(x, y);
		}

		if (x > 400 && y < 300) {

			++cnt_list2;

			if (cnt_list2 == 3)
				cnt_list2 = 0;

			list2[cnt_list2].modify_cord(x, y);
		}

		if (x < 400 && y > 300) {

			++cnt_list3;

			if (cnt_list3 == 3)
				cnt_list3 = 0;

			list3[cnt_list3].modify_cord(x, y);
		}

		if (x > 400 && y > 300) {

			++cnt_list4;

			if (cnt_list4 == 3)
				cnt_list4 = 0;

			list4[cnt_list4].modify_cord(x, y);
		}


	}
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y) {

	switch (key)
	{
	case '1':
		type = 1;
		break;

	case '2':
		type = 2;
		break;

	default:
		break;
	}

	glutPostRedisplay();
}



void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지 정하고 할당하기
	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
	//--- triShape 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);
	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- 변수 colors에서 버텍스 색상을 복사한다.
	//--- colors 배열의 사이즈: 9 *float
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);
}

void make_shaderProgram()
{
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(shaderProgramID);
}

GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
void make_vertexShaders()
{
	vertexSource = filetobuf("vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexShader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentShader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
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
