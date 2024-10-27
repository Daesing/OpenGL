#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <GL/glew.h> // 필요한 헤더파일 include
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
CONST FLOAT CHANGE = 10.0f;
std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> urd{ 0.f,1.f };
std::uniform_int_distribution uid{ 10, 25 };
std::uniform_int_distribution uid_pos{ 100,500 };

bool anime_flag[4]{ false };
char active{};

float y0_rotation{ 10 };

float x1_translation{ 0.5 };
float y1_translation{};
float x1_rotation{ 10 };
float y1_rotation{ 10 };

float x2_translation{ -0.5 };
float y2_translation{};
float x2_rotation{ 10.0f };
float y2_rotation{ 10.0f };

GLUquadricObj* qobj1;


//--- 메인 함수
// 
// 
// 
//--- 함수 선언 추가하기


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


float convertX(int x) {
	return ((float)x / (WIDTH / 2)) - 1.f;
}
float convertY(int y) {
	return 1.f - ((float)y / (HEIGHT / 2));
}

void modify_self(float& obj1, float& obj2, char active, char cal) {
	if (active == '1' && cal == '+') {
		obj2 -= CHANGE;
	}
	else if (active == '2' && cal == '+') {
		obj1 -= CHANGE;
	}
	else if (active == '1' && cal == '-') {
		obj2 += CHANGE;
	}
	else if (active == '2' && cal == '-') {
		obj1 += CHANGE;
	}
	else if (active == '3') {
		return;
	}
}

GLuint vao, vbo[2];
GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수 
GLuint vertexShader, fragmentShader; //--- 세이더 객체
GLuint shaderProgramID; //--- 셰이더 프로그램
GLuint shaderProgramID1;


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
	InitBuffer();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(60, TimerFunction, 1);
	glutMainLoop();
}




GLvoid drawScene()
{
	//--- 변경된 배경색 설정
	glClearColor(1, 1, 1, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);


	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderProgramID);

	glm::mat4 Tx0 = glm::mat4(1.0f);
	glm::mat4 Tx1 = glm::mat4(1.0f);
	glm::mat4 Tx2 = glm::mat4(1.0f);
	glm::mat4 Tx3 = glm::mat4(1.0f);
	glm::mat4 Tx4 = glm::mat4(1.0f);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f); //--- 투영 공간 설정: [-100.0, 100.0]
	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform"); //--- 투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	//--- 적용할 모델링 변환 행렬 만들기
	Tx0 =
		glm::rotate(Tx0, glm::radians(10.0f), glm::vec3(0.0, 1.0, 0.0)) *
		glm::rotate(Tx0, glm::radians(10.0f), glm::vec3(1.0, 0.0, 0.0));
	Tx1 =
		glm::translate(Tx1, glm::vec3(x1_translation, 0.0, 0.0)) *
		glm::translate(Tx1, glm::vec3(0.0, y1_translation, 0.0)) *
		glm::rotate(Tx1, glm::radians(y1_rotation), glm::vec3(0.0, 1.0, 0.0)) *
		glm::rotate(Tx1, glm::radians(x1_rotation), glm::vec3(1.0, 0.0, 0.0));
	Tx2 =
		glm::translate(Tx2, glm::vec3(x2_translation, 0.0, 0.0)) *
		glm::translate(Tx2, glm::vec3(0.0, y2_translation, 0.0)) *
		glm::rotate(Tx2, glm::radians(y2_rotation), glm::vec3(0.0, 1.0, 0.0)) *
		glm::rotate(Tx2, glm::radians(x2_rotation), glm::vec3(1.0, 0.0, 0.0));


	glBindVertexArray(vao);

	//--- 세이더 프로그램에서 modelTransform 변수 위치 가져오기
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
	//--- modelTransform 변수에 변환 값 적용하기
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Tx0));
	//--- 도형 그리기
	for (int i = 0; i < 3; ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(line[i]), &line[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(line_colors[i]), &line_colors[i]);
		glDrawArrays(GL_LINES, 0, 2);
	}


	//--- modelTransform 변수에 변환 값 적용하기
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Tx1));

	for (int i = 0; i < 6; ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube[i]), &cube[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_colors[0]), &cube_colors[i / 2]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}



	//--- modelTransform 변수에 변환 값 적용하기
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Tx2));
	qobj1 = gluNewQuadric();
	gluQuadricDrawStyle(qobj1, GLU_LINE);
	gluSphere(qobj1, 0.2, 20, 20);


	glutSwapBuffers();
}



void Keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'y':
		anime_flag[0] = true;
		anime_flag[1] = false;
		break;
	case 'Y':
		anime_flag[1] = true;
		anime_flag[0] = false;
		break;
	case 'x':
		anime_flag[2] = true;
		anime_flag[3] = false;
		break;

	case 'X':
		anime_flag[3] = true;
		anime_flag[2] = false;
		break;
	case '1':
	case '2':
	case '3':
		active = key;
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
		if (anime_flag[0] == true) {
			std::cout << "flag 0" << '\n';
			y1_rotation += CHANGE;
			y2_rotation += CHANGE;
			modify_self(y1_rotation, y2_rotation, active, '+');
		}
		if (anime_flag[1]) {
			std::cout << "flag 1" << '\n';
			y1_rotation -= CHANGE;
			y2_rotation -= CHANGE;
			modify_self(y1_rotation, y2_rotation, active, '-');
		}
		if (anime_flag[2]) {
			x1_rotation += CHANGE;
			x2_rotation += CHANGE;
			modify_self(x1_rotation, x2_rotation, active, '+');
		}
		if (anime_flag[3]) {
			x1_rotation -= CHANGE;
			x2_rotation -= CHANGE;
			modify_self(x1_rotation, x2_rotation, active, '-');
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

//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기

	glBindVertexArray(vao); //--- VAO를 바인드하기

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
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
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