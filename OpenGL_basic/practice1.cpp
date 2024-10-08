#include <iostream> //--- 필요한 헤더파일 include
#include <random>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> urd{ 0.0f,1.0f };

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);

bool timerActive{false};

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
    glutTimerFunc(1000, TimerFunction, 1);
    glutMainLoop(); //--- 이벤트 처리 시작
}

GLvoid drawScene() { //--- 콜백 함수: 그리기 콜백 함수
    //--- 변경된 배경색 설정
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //--- 바탕색을 변경 (예: 검은색)
    glClear(GL_COLOR_BUFFER_BIT); //--- 설정된 색으로 전체를 칠하기
    glutSwapBuffers(); //--- 화면에 출력하기
}

GLvoid Reshape(int w, int h) { //--- 콜백 함수: 다시 그리기 콜백 함수
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) { //--- 키보드 입력 처리

    GLclampf red{ urd(dre) }, green{ urd(dre) }, blue{ urd(dre) };


    switch (key) {
    case 'c':
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f); //--- 배경색을 청록색으로 설정
        break;
    case 'm':
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f); //--- 배경색을 자홍색으로 설정
        break;
    case 'y':
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f); //--- 배경색을 노랑색으로 설정
        break;
    case 'w':
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        break;
    case 'k':
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        break;
    case 'a':
        glClearColor(red, green, blue, 1.0f);
        break;
    case 't':
        timerActive = true;
        glutTimerFunc(1000, TimerFunction, 1); // 타이머함수 재 설정
        break;
    case 's':
        timerActive = false;
        break;
    case 'q':
        glutLeaveMainLoop();
        break;
    default:
        break;
    }
    glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
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


