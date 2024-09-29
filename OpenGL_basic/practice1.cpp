#include <iostream> //--- �ʿ��� ������� include
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

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
    glutMainLoop(); //--- �̺�Ʈ ó�� ����
}

GLvoid drawScene() { //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
    //--- ����� ���� ����
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //--- �������� ���� (��: ������)
    glClear(GL_COLOR_BUFFER_BIT); //--- ������ ������ ��ü�� ĥ�ϱ�
    glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) { //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) { //--- Ű���� �Է� ó��
    switch (key) {
    case 'c':
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f); //--- ������ û�ϻ����� ����
        break;
    case 'm':
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f); //--- ������ ��ȫ������ ����
        break;
    case 'y':
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f); //--- ������ ��������� ����
        break;
    case 'w':
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        break;
    case 'k':
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        break;
        
    default:
        break;
    }
    glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}
