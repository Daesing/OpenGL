#include <iostream>
#include <GL/glew.h> // �ʿ��� ������� include
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

void drawScene(); // �׸��� �Լ� ����
void Reshape(int w, int h); // ũ�� ���� �Լ� ����


int start_x1{ -1 }, start_y1{ 1 }, end_x1{ 0 }, end_y1{ 0 };
int start_x2{ 0 }, start_y2{ 1 }, end_x2{ 1 }, end_y2{ 0 };
int start_x3{ -1 }, start_y3{ 0 }, end_x3{ 0 }, end_y3{ -1 };
int start_x4{ 0 }, start_y4{ 0 }, end_x4{ 1 }, end_y4{ -1 };


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
    glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
    glutMainLoop(); // �̺�Ʈ ó�� ����

    return 0; // main �Լ� ����
}

void drawScene() { // �׸��� �ݹ� �Լ�
    glRectf(start_x1, start_y1, end_x1, end_y1);
    glRectf(start_x2, start_y2, end_x2, end_y2);
    glRectf(start_x3, start_y3, end_x3, end_y3);
    glRectf(start_x4, start_y4, end_x4, end_y4);
    glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

void Reshape(int w, int h) { // ũ�� ���� �ݹ� �Լ�
    glViewport(0, 0, w, h);
}
