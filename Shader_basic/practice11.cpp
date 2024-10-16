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
CONST INT LISTSIZE = 4;
std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> urd{ 0.f,1.f };
std::uniform_int_distribution uid{ 10, 25 };
std::uniform_int_distribution uid_pos{ 100,500 };
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


float convertX(int x) {
	return ((float)x / (WIDTH / 2)) - 1.f;
}
float convertY(int y) {
	return 1.f - ((float)y / (HEIGHT / 2));
}

GLuint vao, vbo[2];
GLchar* vertexSource, * fragmentSource; //--- 소스코드 저장 변수
GLuint vertexShader, fragmentShader; //--- 세이더 객체
GLuint shaderProgramID; //--- 셰이더 프로그램
bool num_flag[5]{ false };


struct Shape {
	int x{}, y{};
	int type{};
	GLfloat colors[5][3] = { //--- 삼각형 꼭지점 색상
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
	glutTimerFunc(200, TimerFunction, 1);
	glutMainLoop();
}


GLvoid drawScene()
{
	//--- 변경된 배경색 설정
	glClearColor(1, 1, 1, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(shaderProgramID);
	//--- 사용할 VAO 불러오기
	glBindVertexArray(vao);
	for (int i = 0; i < LISTSIZE; ++i) {
		if(shape[i].is_active)
			shape[i].draw_shape();
	}
	//--- 삼각형 그리기
	glutSwapBuffers(); //--- 화면에 출력하기
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
		//선 - 삼각형
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
		//삼각형 - 사각형
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
		//사각형 - 오각형
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
		//오각형 - 선
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
		//전체 동시 변경
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

//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기
	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
	//--- triShape 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);
	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//--- 변수 colors에서 버텍스 색상을 복사한다.
	//--- colors 배열의 사이즈: 9 *float
	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
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