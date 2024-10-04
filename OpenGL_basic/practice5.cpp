#include <iostream> //--- 필요한 헤더파일 include
#include <random>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#define WIDTH 800
#define HEIGHT 600

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> urd{ 0.0f,1.0f };
std::uniform_int_distribution uid{ 100,600 };

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);
bool timerActive{ false };
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);

float convertX(int x) {
    return ((float)x / (WIDTH / 2)) - 1.f;
}
float convertY(int y) {
    return 1.f - ((float)y / (HEIGHT / 2));
}

struct Rect
{
    int x{ uid(dre) }, y{ uid(dre) };
    GLfloat r{ urd(dre) }, g{ urd(dre) }, b{ urd(dre) };
    bool isActive{ true };

};

int r_size{ 15 };
int eraser_size{ 30 };

Rect r[20];
Rect eraser;
bool left_button;


int main(int argc, char** argv) { //--- 윈도우 출력하고 콜백함수 설정
    //--- 윈도우 생성하기
    glutInit(&argc, argv); //--- glut 초기화
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //--- 디스플레이 모드 설정
    glutInitWindowPosition(0, 0); //--- 윈도우의 위치 지정
    glutInitWindowSize(800, 600); //--- 윈도우의 크기 지정
    glutCreateWindow("Example1"); //--- 윈도우 생성 (윈도우 이름)

    //--- GLEW 초기화하기
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { //--- glew 초기화
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        std::cout << "GLEW Initialized\n";
    }

    glutDisplayFunc(drawScene); //--- 출력 콜백함수의 지정
    glutReshapeFunc(Reshape); //--- 다시 그리기 콜백함수 지정
    glutKeyboardFunc(Keyboard); //--- 키보드 입력 콜백함수 지정
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    glutMainLoop(); //--- 이벤트 처리 시작
}

GLvoid drawScene() { //--- 콜백 함수: 그리기 콜백 함수
    //--- 변경된 배경색 설정
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //--- 바탕색을 변경 (예: 검은색)
    glClear(GL_COLOR_BUFFER_BIT); //--- 설정된 색으로 전체를 칠하기

    for (int i = 0; i < 20; ++i) {
        if (r[i].isActive) {
            glColor3f(r[i].r, r[i].g, r[i].b);
            glRectf(convertX(r[i].x - r_size), convertY(r[i].y - r_size), convertX(r[i].x + r_size), convertY(r[i].y + r_size));
        }
    }
    
    if (eraser.isActive) {
        glColor3f(eraser.r, eraser.g, eraser.b);
        glRectf(convertX(eraser.x - eraser_size), convertY(eraser.y - eraser_size), convertX(eraser.x + eraser_size), convertY(eraser.y + eraser_size));
    }


    glutSwapBuffers(); //--- 화면에 출력하기
}

GLvoid Reshape(int w, int h) { //--- 콜백 함수: 다시 그리기 콜백 함수
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) { //--- 키보드 입력 처리



  
    glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

void Mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        left_button == true;
        eraser.isActive = true;
        eraser.x = x;
        eraser.y = y;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        left_button == false;
        eraser.isActive = false;
    }
     
    glutPostRedisplay();
}

void Motion(int x, int y)
{
    eraser.x = x;
    eraser.y = y;


    glutPostRedisplay();
}


void TimerFunction(int value)
{
    GLclampf red{ urd(dre) }, green{ urd(dre) }, blue{ urd(dre) };

    if (!timerActive)
        return;

    glClearColor(red, green, blue, 1.0f);
    glutPostRedisplay(); // 화면 재 출력
    glutTimerFunc(1000, TimerFunction, 1); // 타이머함수 재 설정
}


