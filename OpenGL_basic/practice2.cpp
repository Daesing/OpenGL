#include <iostream> //--- 필요한 헤더파일 include
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
    glutMouseFunc(Mouse);
    glutMainLoop(); //--- 이벤트 처리 시작
}

GLvoid drawScene() { //--- 콜백 함수: 그리기 콜백 함수
    //--- 변경된 배경색 설정
    
    glClear(GL_COLOR_BUFFER_BIT); //--- 설정된 색으로 전체를 칠하기

    for (int i = 0; i < 4; ++i) {
        glColor3f(r[i].r, r[i].g, r[i].b);
        glRectf(convertX(r[i].x1), convertY(r[i].y1), convertX(r[i].x2), convertY(r[i].y2));
    }




    glutSwapBuffers(); //--- 화면에 출력하기
}

GLvoid Reshape(int w, int h) { //--- 콜백 함수: 다시 그리기 콜백 함수
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y) { //--- 키보드 입력 처리

    glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
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
        //오른쪽 클릭 사각형 외부
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

