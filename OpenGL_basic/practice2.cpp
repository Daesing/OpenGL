#include <iostream> //--- �ʿ��� ������� include
#include <random>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> urd{ 0.0f,1.0f };

struct Rect {
    int x1, x2, y1, y2;
    float r{ urd(dre) }, g{ urd(dre) }, b{ urd(dre)};
    int size_cnt{ 10 };

    Rect(int x1_, int y1_, int x2_, int y2_)
        : x1(x1_), y1(y1_), x2(x2_), y2(y2_) {
    }
};

Rect r[4] = {
        Rect(0, 0, 400, 300),
        Rect(400, 0, 800, 300),
        Rect(0, 300, 400, 600),
        Rect(400, 300, 800, 600)
};



CONST INT WIDTH = 800;
CONST INT HEIGHT = 600;

float convertX(int x) {
    return ((float)x / (WIDTH / 2)) - 1.f;
}
float convertY(int y) {
    return 1.f - ((float)y / (HEIGHT / 2));
}



GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);
GLvoid Mouse(int button, int state, int x, int y);
bool timerActive{ false };


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
    glutMainLoop(); //--- �̺�Ʈ ó�� ����
}

GLvoid drawScene() { //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ�
    //--- ����� ���� ����
    
    glClear(GL_COLOR_BUFFER_BIT); //--- ������ ������ ��ü�� ĥ�ϱ�

    for (int i = 0; i < 4; ++i) {
        glColor3f(r[i].r, r[i].g, r[i].b);
        glRectf(convertX(r[i].x1), convertY(r[i].y1), convertX(r[i].x2), convertY(r[i].y2));
    }




    glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) { //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) { //--- Ű���� �Է� ó��

    glutPostRedisplay(); //--- ������ �ٲ� ������ ��� �ݹ� �Լ��� ȣ���Ͽ� ȭ���� refresh �Ѵ�
}

GLvoid Mouse(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		if (x > r[0].x1 && x < r[0].x2 && y > r[0].y1 && y < r[0].y2) {
			r[0].r = urd(dre);
			r[0].g = urd(dre);
			r[0].b = urd(dre);
		}
        else if (x > r[1].x1 && x < r[1].x2 && y > r[1].y1 && y < r[1].y2) {
            r[1].r = urd(dre);
            r[1].g = urd(dre);
            r[1].b = urd(dre);
        }
        else if (x > r[2].x1 && x < r[2].x2 && y > r[2].y1 && y < r[2].y2) {
            r[2].r = urd(dre);
            r[2].g = urd(dre);
            r[2].b = urd(dre);
        }
        else if (x > r[3].x1 && x < r[3].x2 && y > r[3].y1 && y < r[3].y2) {
            r[3].r = urd(dre);
            r[3].g = urd(dre);
            r[3].b = urd(dre);
        }
        else {
            glClearColor(urd(dre), urd(dre), urd(dre), 1.0f);
        }

    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {

        if (x > r[0].x1 && x < r[0].x2 && y > r[0].y1 && y < r[0].y2) {
            if (r[0].size_cnt != 0) {
                r[0].x1 += 10;
                r[0].y1 += 10;
                r[0].x2 -= 10;
                r[0].y2 -= 10;
                r[0].size_cnt -= 1;
            }
            
        }
        else if (x > r[1].x1 && x < r[1].x2 && y > r[1].y1 && y < r[1].y2) {
            if (r[1].size_cnt != 0) {
                r[1].x1 += 10;
                r[1].y1 += 10;
                r[1].x2 -= 10;
                r[1].y2 -= 10;
                r[1].size_cnt -= 1;
            }
        }
        else if (x > r[2].x1 && x < r[2].x2 && y > r[2].y1 && y < r[2].y2) {
            if (r[2].size_cnt != 0) {
                r[2].x1 += 10;
                r[2].y1 += 10;
                r[2].x2 -= 10;
                r[2].y2 -= 10;
                r[2].size_cnt -= 1;
            }
        }
        else if (x > r[3].x1 && x < r[3].x2 && y > r[3].y1 && y < r[3].y2) {
            if (r[3].size_cnt != 0) {
                r[3].x1 += 10;
                r[3].y1 += 10;
                r[3].x2 -= 10;
                r[3].y2 -= 10;
                r[3].size_cnt -= 1;
            }
        }
        //������ Ŭ�� �簢�� �ܺ�
        else if (x > 0 && x < 400 && y>0 && y < 300) {
            r[0].x1 -= 10;
            r[0].y1 -= 10;
            r[0].x2 += 10;
            r[0].y2 += 10;
            r[0].size_cnt += 1;
        }
        else if (x > 400 && x < 800 && y>0 && y < 300) {
            r[1].x1 -= 10;
            r[1].y1 -= 10;
            r[1].x2 += 10;
            r[1].y2 += 10;
            r[1].size_cnt += 1;
        }
        else if (x > 0 && x < 400 && y>300 && y < 600) {
            r[2].x1 -= 10;
            r[2].y1 -= 10;
            r[2].x2 += 10;
            r[2].y2 += 10;
            r[2].size_cnt += 1;
        }
        else if (x > 0 && x < 800 && y>300 && y < 600) {
            r[3].x1 -= 10;
            r[3].y1 -= 10;
            r[3].x2 += 10;
            r[3].y2 += 10;
            r[3].size_cnt += 1;
        }
    }


}

