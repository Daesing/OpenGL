#include <iostream>
#include <GL/glew.h> // 필요한 헤더파일 include
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <random>

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> urd{ -1,0.8 };

struct Rect {
    float xStart{}, yStart{}, xEnd{}, yEnd{};
    float r, g, b;
    bool active{ false };

    Rect() {
        r = urd(dre);
        g = urd(dre);
        b = urd(dre);
    }
};

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);

Rect rect[5];

int rect_cnt{};

int main(int argc, char** argv) { //--- 윈도우 출력하고 콜백함수 설정
    //--- 윈도우 생성하기
    glutInit(&argc, argv); // glut 초기화
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
    glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
    glutInitWindowSize(800, 600); // 윈도우의 크기 지정
    glutCreateWindow("Example1"); // 윈도우 생성(윈도우 이름)

    //--- GLEW 초기화하기
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { // glew 초기화
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        std::cout << "GLEW Initialized\n";
    }

    glutDisplayFunc(drawScene); // 출력 함수의 지정
    glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
    glutMouseFunc(Mouse);
    glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() { //--- 콜백 함수: 그리기 콜백 함수
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // 바탕색을 ‘blue’ 로 지정
    glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
    // 그리기 부분 구현
    //--- 그리기 관련 부분이 여기에 포함된다
    for (int i = 0; i < 5; ++i) {
        if (rect[i].active) {
            glColor3f(rect[i].r, rect[i].g, rect[i].b);
            glRectf(rect[i].xStart, rect[i].yStart, rect[i].xEnd, rect[i].yEnd);
        }
    }
    glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) { //--- 콜백 함수: 다시 그리기 콜백 함수
    glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (rect_cnt < 5) {
            rect[rect_cnt].xStart = x - 0.1;
            rect[rect_cnt].yEnd = y + 0.1;
            rect[rect_cnt].xEnd = x + 0.1;
            rect[rect_cnt].yEnd = y - 0.1;
            rect[rect_cnt].active = true;
            ++rect_cnt;
        }
    }
        
}