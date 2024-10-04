#include <iostream> //--- �ʿ��� ������� include
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


    glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) { //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) { //--- Ű���� �Է� ó��



  
    glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
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
    glutPostRedisplay(); // ȭ�� �� ���
    glutTimerFunc(1000, TimerFunction, 1); // Ÿ�̸��Լ� �� ����
}


