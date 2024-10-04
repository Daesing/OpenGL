#include <iostream> //--- �ʿ��� ������� include
#include <random>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#define WIDTH 800
#define HEIGHT 600

CONST INT RECTCNT = 10;

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> urd{ 0.0f,1.0f };
std::uniform_int_distribution uid{ 100,600 };

struct Rect {
    int x{ uid(dre) }, y{ uid(dre) };
    GLfloat r{ urd(dre) }, g{ urd(dre) }, b{ urd(dre) };
    int size{ 20 };
    bool isActive{ false };

};

Rect r[10];
int currentIdx{};
bool left_button{ false };
int currentRect{};


GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);
bool timerActive{ false };
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void checkCrash();

float convertX(int x) {
    return ((float)x / (WIDTH / 2)) - 1.f;
}
float convertY(int y) {
    return 1.f - ((float)y / (HEIGHT / 2));
}


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
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    glutMainLoop(); //--- �̺�Ʈ ó�� ����
}

GLvoid drawScene() { //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
    //--- ����� ���� ����
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //--- �������� ���� (��: ������)
    glClear(GL_COLOR_BUFFER_BIT); //--- ������ ������ ��ü�� ĥ�ϱ�

    for (int i = 0; i < RECTCNT; ++i) {
        if (r[i].isActive) {
            glColor3f(r[i].r, r[i].g, r[i].b);
            glRectf(convertX(r[i].x - r[i].size), convertY(r[i].y - r[i].size), convertX(r[i].x + r[i].size), convertY(r[i].y + r[i].size));
        }
    }


    glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) { //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) { //--- Ű���� �Է� ó��

    switch (key)
    {
    case 'a':
        if (currentIdx < 10) {
            r[currentIdx].isActive = true;
            ++currentIdx;
        }
        break;
    default:
        break;
    }
    
    glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}


void Mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        left_button = true;

        for (int i = 0; i < RECTCNT; ++i) {
            if (x < r[i].x + r[i].size && x > r[i].x - r[i].size && y < r[i].y + r[i].size && y > r[i].y - r[i].size) {
                currentRect = i;
                r[i].x = x;
                r[i].y = y;
            }
        }

    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        left_button = false;
        currentRect = -1;
    }
    glutPostRedisplay();
}

void Motion(int x, int y)
{
    

    if (currentRect != -1) {
        r[currentRect].x = x;
        r[currentRect].y = y;
    }

    checkCrash();

    glutPostRedisplay();
}

void checkCrash() {
    int temp_x{}, temp_y{};

    for (int i = 0; i < RECTCNT; ++i) {
        for (int j = currentRect; j < currentRect+1; ++j) {
            if (i == j)
                continue;
            if (r[i].x - r[i].size < r[j].x + r[j].size && r[i].y - r[i].size < r[j].y + r[j].size
                && r[i].x + r[i].size>r[j].x - r[j].size && r[i].y + r[i].size>r[j].y - r[j].size && r[i].isActive && r[j].isActive) {

                temp_x = ((r[i].x * r[i].size) + (r[j].x * r[j].size)) / (r[i].size + r[j].size);
                temp_y = ((r[i].y * r[i].size) + (r[j].y * r[j].size)) / (r[i].size + r[j].size);
                r[j].size += r[i].size;
                r[i].isActive = false;

                r[j].x = temp_x;
                r[j].y = temp_y;
                r[j].r = urd(dre);
                r[j].g = urd(dre);
                r[j].b = urd(dre);


            }
        }
    }
    glutPostRedisplay();
}


