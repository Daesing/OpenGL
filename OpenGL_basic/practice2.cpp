#include <iostream>
#include <GL/glew.h> // 필요한 헤더파일 include
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

void drawScene(); // 그리기 함수 선언
void Reshape(int w, int h); // 크기 조정 함수 선언


int start_x1{ -1 }, start_y1{ 1 }, end_x1{ 0 }, end_y1{ 0 };
int start_x2{ 0 }, start_y2{ 1 }, end_x2{ 1 }, end_y2{ 0 };
int start_x3{ -1 }, start_y3{ 0 }, end_x3{ 0 }, end_y3{ -1 };
int start_x4{ 0 }, start_y4{ 0 }, end_x4{ 1 }, end_y4{ -1 };


int main(int argc, char** argv) { // 윈도우 출력하고 콜백함수 설정
    // 윈도우 생성하기
    glutInit(&argc, argv); // GLUT 초기화
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
    glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
    glutInitWindowSize(800, 600); // 윈도우의 크기 지정
    glutCreateWindow("Example1"); // 윈도우 생성 (윈도우 이름)

    // GLEW 초기화하기
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { // GLEW 초기화
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        std::cout << "GLEW Initialized\n";
    }

    glutDisplayFunc(drawScene); // 출력 함수 지정
    glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
    glutMainLoop(); // 이벤트 처리 시작

    return 0; // main 함수 종료
}

void drawScene() { // 그리기 콜백 함수
    glRectf(start_x1, start_y1, end_x1, end_y1);
    glRectf(start_x2, start_y2, end_x2, end_y2);
    glRectf(start_x3, start_y3, end_x3, end_y3);
    glRectf(start_x4, start_y4, end_x4, end_y4);
    glutSwapBuffers(); // 화면에 출력하기
}

void Reshape(int w, int h) { // 크기 조정 콜백 함수
    glViewport(0, 0, w, h);
}
