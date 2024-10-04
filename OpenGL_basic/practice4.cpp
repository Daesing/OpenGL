#include <iostream> //--- 필요한 헤더파일 include
#include <random>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <random>


#define WIDTH 800
#define HEIGHT 600


float convertX(int x) {
    return ((float)x / (WIDTH / 2)) - 1.f;
}
float convertY(int y) {
    return 1.f - ((float)y / (HEIGHT / 2));
}

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> urd{ 0.0f,1.0f };
std::uniform_int_distribution uid{ 15,60 };

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);
GLvoid Mouse(int button, int state, int x, int y);

struct Rect {
    int x{}, y{};
    
    float r{}, g{}, b{};
    int size{ 15 };
    bool isActive = false;
    bool move_x{ true }, move_y{ true };


};
Rect r[5]{};
int origin_x[5];
int origin_y[5];
int currentIdx{};
bool dir{ true };
bool numFlag[4]{};

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
    glutTimerFunc(60, TimerFunction, 1);
    glutMouseFunc(Mouse);
    glutMainLoop(); //--- 이벤트 처리 시작
}

GLvoid drawScene() { //--- 콜백 함수: 그리기 콜백 함수
    //--- 변경된 배경색 설정
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //--- 바탕색을 변경 (예: 검은색)
    glClear(GL_COLOR_BUFFER_BIT); //--- 설정된 색으로 전체를 칠하기

    for (int i = 0; i < 5; ++i) {
        if (r[i].isActive) {
            glColor3f(r[i].r, r[i].g, r[i].b);
            glRectf(convertX(r[i].x- r[i].size), convertY(r[i].y - r[i].size), convertX(r[i].x+r[i].size), convertY(r[i].y+ r[i].size));
        }
    }

    glutSwapBuffers(); //--- 화면에 출력하기
}

GLvoid Reshape(int w, int h) { //--- 콜백 함수: 다시 그리기 콜백 함수
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) { //--- 키보드 입력 처리

    switch (key) {
    case '1':
    case '2':
    case '3':
    case '4':
        numFlag[key - '0' - 1] = (numFlag[key - '0' - 1] + 1) % 2;
        break;
    case 's':
        for (int i = 0; i < 4; ++i)
            numFlag[i] = false;
        break;
    case 'm':
        for (int i = 0; i < 5; ++i) {
            r[i].x = origin_x[i];
            r[i].y = origin_y[i];

        }
        break;
    case 'r':
        currentIdx = 0;
        for (int i = 0; i < 5; ++i) {
            r[i].isActive = false;
            r[i].size = 15;
        }
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
    switch (value) {
    case 1:
        if (numFlag[0]) {
            for (int i = 0; i < 5; ++i) {
                if (r[i].move_x) {
                    r[i].x += 10;
                    if (r[i].x + 10 > 750)
                        r[i].move_x = false;
                }
                else {
                    r[i].x -= 10;
                    if (r[i].x + 10 < 50)
                        r[i].move_x = true;
                }

                if (r[i].move_y) {
                    r[i].y += 10;

                    if (r[i].y + 10 > 550)
                        r[i].move_y = false;
                }
                else {
                    r[i].y -= 10;

                    if (r[i].y - 10 < 50)
                        r[i].move_y = true;
                }
            }
        }
        if (numFlag[1]) {
            for (int i = 0; i < 5; ++i) {
                if (dir) {
                    r[i].x += 20;
                    dir = false;
                }
                else {
                    r[i].x -= 20;
                    dir = true;
                }

                if (r[i].move_x) {
                    r[i].x += 10;
                    if (r[i].x + 10 > 750)
                        r[i].move_x = false;
                }
                else {
                    r[i].x -= 10;
                    if (r[i].x + 10 < 50)
                        r[i].move_x = true;
                }

                if (r[i].move_y) {
                    r[i].y += 10;

                    if (r[i].y + 10 > 550)
                        r[i].move_y = false;
                }
                else {
                    r[i].y -= 10;

                    if (r[i].y - 10 < 50)
                        r[i].move_y = true;
                }
            }



        }
        if (numFlag[2]) {
            for (int i = 0; i < 5; ++i) {
                r[i].size = uid(dre);
                r[i].x = r[i].x - r[i].size;
                r[i].y = r[i].y - r[i].size;
                r[i].x = r[i].x + r[i].size;
                r[i].y = r[i].y + r[i].size;
            }
        }
        if (numFlag[3]) {
            for (int i = 0; i < 5; ++i) {
                r[i].r = urd(dre);
                r[i].g = urd(dre);
                r[i].b = urd(dre);
            }
        }
        break;
    default:
        break;
    }
    glutTimerFunc(60, TimerFunction, 1); // 타이머함수 재 설정
    glutPostRedisplay(); // 화면 재 출력
}
void Mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (currentIdx < 5) {
            r[currentIdx].x = x;
            r[currentIdx].y = y;

            r[currentIdx].r = urd(dre);
            r[currentIdx].g = urd(dre);
            r[currentIdx].b = urd(dre);
            r[currentIdx].isActive = true;

            origin_x[currentIdx] = r[currentIdx].x;
            origin_y[currentIdx] = r[currentIdx].y;
            currentIdx++;
        }
    }

    glutPostRedisplay(); // 화면 재 출력
}

