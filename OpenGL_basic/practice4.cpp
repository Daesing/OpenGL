#include <iostream>
#include <GL/glew.h> // �ʿ��� ������� include
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

int main(int argc, char** argv) { //--- ������ ����ϰ� �ݹ��Լ� ����
    //--- ������ �����ϱ�
    glutInit(&argc, argv); // glut �ʱ�ȭ
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
    glutInitWindowPosition(100, 100); // �������� ��ġ ����
    glutInitWindowSize(800, 600); // �������� ũ�� ����
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
    glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
    glutMouseFunc(Mouse);
    glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() { //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // �������� ��blue�� �� ����
    glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�
    // �׸��� �κ� ����
    //--- �׸��� ���� �κ��� ���⿡ ���Եȴ�
    for (int i = 0; i < 5; ++i) {
        if (rect[i].active) {
            glColor3f(rect[i].r, rect[i].g, rect[i].b);
            glRectf(rect[i].xStart, rect[i].yStart, rect[i].xEnd, rect[i].yEnd);
        }
    }
    glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) { //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
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