#include <iostream>
#include <GL/glew.h> // �ʿ��� ������� include
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <random>

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> urd{ -1,0.8 };

struct Rect {
    float xStart, yStart, xEnd, yEnd;
    float r, g, b;
    bool active{ false };

    Rect() {
        xStart = urd(dre);
        yStart = urd(dre);
        xEnd = xStart + 0.2;
        yEnd = yStart + 0.2;

        r = urd(dre);
        g = urd(dre);
        b = urd(dre);
    }

};

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);

bool left_button;
bool right_button;

Rect min_rect[10];

int m_cnt{};

int main(int argc, char** argv) { //--- ������ ����ϰ� �ݹ��Լ� ����
    //--- ������ �����ϱ�
    glutInit(&argc, argv); // glut �ʱ�ȭ
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
    glutInitWindowPosition(100, 100); // �������� ��ġ ����
    glutInitWindowSize(800, 800); // �������� ũ�� ����
    glutCreateWindow("Example1"); // ������ ����(������ �̸�)

    //--- GLEW �ʱ�ȭ�ϱ�
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { // glew �ʱ�ȭ
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        std::cout << "GLEW Initialized\n";
    }

    glutDisplayFunc(drawScene); // ��� �Լ��� ����
    glutKeyboardFunc(Keyboard);
    glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);


    glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() { //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // �������� ��blue�� �� ����
    glClear(GL_COLOR_BUFFER_BIT); //--- ������ ������ ��ü�� ĥ�ϱ�
    // �׸��� �κ� ����
    //--- �׸��� ���� �κ��� ���⿡ ���Եȴ�
    
    for (int i = 0; i < 10; ++i) {
        if (min_rect[i].active) {
            glColor3f(min_rect[i].r, min_rect[i].g, min_rect[i].b);
            glRectf(min_rect[i].xStart, min_rect[i].yStart, min_rect[i].xEnd, min_rect[i].yEnd);
        }
    }


    glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) { //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
    glViewport(0, 0, w, h);
}
 
void Keyboard(unsigned char key, int x, int y)
{

    switch (key) {
    case 'a':
        if (m_cnt > 9)
            break;
        min_rect[m_cnt].active = true;
        ++m_cnt;
        break;
    default:
        break;
    }
    glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

void Mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
        left_button = true;
}

void Motion(int x, int y)
{
    if (left_button == true)
    {
    }
}