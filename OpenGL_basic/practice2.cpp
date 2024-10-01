#include <iostream>
#include <GL/glew.h> // �ʿ��� ������� include
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <random>


std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> urd{ 0.0f,1.0f };

void drawScene(); // �׸��� �Լ� ����
void Reshape(int w, int h); // ũ�� ���� �Լ� ����
void Mouse(int button, int state, int x, int y);

struct Rect {
    float xStart, yStart, xEnd, yEnd;
    float r{ urd(dre) }, g{ urd(dre) }, b{ urd(dre) };
    
    Rect(int xStart_, int yStart_, int xEnd_, int yEnd_) {
        xStart = xStart_;
        yStart = yStart_;
        xEnd = xEnd_;
        yEnd = yEnd_;
    }

};

Rect rect1(-1, 1, 0, 0);
Rect rect2(0, 1, 1, 0);
Rect rect3(-1, 0, 0, -1);
Rect rect4(0, 0, 1, -1);

int main(int argc, char** argv) { // ������ ����ϰ� �ݹ��Լ� ����
    // ������ �����ϱ�
    glutInit(&argc, argv); // GLUT �ʱ�ȭ
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
    glutInitWindowPosition(100, 100); // �������� ��ġ ����
    glutInitWindowSize(800, 600); // �������� ũ�� ����
    glutCreateWindow("Example1"); // ������ ���� (������ �̸�)

    // GLEW �ʱ�ȭ�ϱ�
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { // GLEW �ʱ�ȭ
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        std::cout << "GLEW Initialized\n";
    }

   
    glutDisplayFunc(drawScene); // ��� �Լ� ����
    glutMouseFunc(Mouse);
    //glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
    glutMainLoop(); // �̺�Ʈ ó�� ����

    return 0; // main �Լ� ����
}

void drawScene() { // �׸��� �ݹ� �Լ�
    glColor3f(rect1.r, rect1.g, rect1.b);
    glRectf(rect1.xStart, rect1.yStart, rect1.xEnd, rect1.yEnd);

    glColor3f(rect2.r, rect2.g, rect2.b);
    glRectf(rect2.xStart, rect2.yStart, rect2.xEnd, rect2.yEnd);

    glColor3f(rect3.r, rect3.g, rect3.b);
    glRectf(rect3.xStart, rect3.yStart, rect3.xEnd, rect3.yEnd);

    glColor3f(rect4.r, rect4.g, rect4.b);
    glRectf(rect4.xStart, rect4.yStart, rect4.xEnd, rect4.yEnd);

    glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

void Reshape(int w, int h) { // ũ�� ���� �ݹ� �Լ�
    glViewport(0, 0, w, h);
}


void Mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && x < 400 && y < 300) {
        rect1.r = urd(dre); rect1.g = urd(dre); rect1.b = urd(dre);
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && x > 400 && y < 300) {
        rect2.r = urd(dre); rect2.g = urd(dre); rect2.b = urd(dre);
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && x < 400 && y > 300) {
        rect3.r = urd(dre); rect3.g = urd(dre); rect3.b = urd(dre);
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && x > 400 && y > 300) {
        rect4.r = urd(dre); rect4.g = urd(dre); rect4.b = urd(dre);
    }

       
}
