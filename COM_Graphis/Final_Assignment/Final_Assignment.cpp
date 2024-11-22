#include <cmath>
#include <GL/glut.h>
#define M_PI 3.14

// 자동차 위치와 회전각도
float carX = 0.0f;
float carY = 0.0f;
float carAngle = 0.0f;
float carSpeed = 0.0f;

void drawCar() {
    glPushMatrix();
    glTranslatef(carX, carY, 0.0f);
    glRotatef(carAngle, 0.0f, 0.0f, 1.0f);

    // 자동차 본체
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, -0.1f);
    glVertex2f(0.2f, -0.1f);
    glVertex2f(0.2f, 0.1f);
    glVertex2f(-0.2f, 0.1f);
    glEnd();

    // 바퀴
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-0.15f, -0.1f, 0.0f);
    glutSolidSphere(0.05, 8, 8);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.15f, -0.1f, 0.0f);
    glutSolidSphere(0.05, 8, 8);
    glPopMatrix();

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawCar();
    glutSwapBuffers();
}

void update(int value) {
    // 자동차 움직임 업데이트
    carX += carSpeed * cos(carAngle * M_PI / 180.0f);
    carY += carSpeed * sin(carAngle * M_PI / 180.0f);

    // 화면 경계 처리
    if (carX > 1.0f) carX = -1.0f;
    if (carX < -1.0f) carX = 1.0f;
    if (carY > 1.0f) carY = -1.0f;
    if (carY < -1.0f) carY = 1.0f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // ~60 FPS
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': // 전진
        carSpeed = 0.01f;
        break;
    case 's': // 후진
        carSpeed = -0.01f;
        break;
    case 'a': // 좌회전
        carAngle += 5.0f;
        break;
    case 'd': // 우회전
        carAngle -= 5.0f;
        break;
    case ' ': // 정지
        carSpeed = 0.0f;
        break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Car Simulation");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}