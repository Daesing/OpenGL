#include <iostream> //--- �ʿ��� ������� include
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

int main(int argc, char** argv) { //--- ������ ����ϰ� �ݹ��Լ� ����
    //--- ������ �����ϱ�
    glutInit(&argc, argv); //--- glut �ʱ�ȭ
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //--- ���÷��� ��� ����
    glutInitWindowPosition(0, 0); //--- �������� ��ġ ����
    glutInitWindowSize(800, 600); //--- �������� ũ�� ����
    glutCreateWindow("Example1"); //--- ������ ���� (������ �̸�)

    //--- GLEW �ʱ�ȭ�ϱ�
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { //--- glew �ʱ�ȭ
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        std::cout << "GLEW Initialized\n";
    }

    glutDisplayFunc(drawScene); //--- ��� �ݹ��Լ��� ����
    glutReshapeFunc(Reshape); //--- �ٽ� �׸��� �ݹ��Լ� ����
    glutKeyboardFunc(Keyboard); //--- Ű���� �Է� �ݹ��Լ� ����
    glutTimerFunc(60, TimerFunction, 1);
    glutMouseFunc(Mouse);
    glutMainLoop(); //--- �̺�Ʈ ó�� ����
}

GLvoid drawScene() { //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
    //--- ����� ���� ����
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //--- �������� ���� (��: ������)
    glClear(GL_COLOR_BUFFER_BIT); //--- ������ ������ ��ü�� ĥ�ϱ�

    for (int i = 0; i < 5; ++i) {
        if (r[i].isActive) {
            glColor3f(r[i].r, r[i].g, r[i].b);
            glRectf(convertX(r[i].x- r[i].size), convertY(r[i].y - r[i].size), convertX(r[i].x+r[i].size), convertY(r[i].y+ r[i].size));
        }
    }

    glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) { //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) { //--- Ű���� �Է� ó��

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
    glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
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
    glutTimerFunc(60, TimerFunction, 1); // Ÿ�̸��Լ� �� ����
    glutPostRedisplay(); // ȭ�� �� ���
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

    glutPostRedisplay(); // ȭ�� �� ���
}

