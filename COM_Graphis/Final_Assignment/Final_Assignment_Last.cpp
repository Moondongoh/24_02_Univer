#include <vector>
#include <cstdlib> // 랜덤 함수 사용
#include <ctime>   // 시간 기반 시드 설정
#include <cmath>
#include <GL/glut.h>
#include <string>
#define M_PI 3.14159265358979323846

// 건물 속성을 저장할 구조체
struct Building {
    float x, z;     // 위치
    float height;   // 높이
    float red, green, blue; // 색상
};

// 건물 리스트
std::vector<Building> buildings;

// 차량의 초기 위치 및 각도
float carX = 0.0f, carY = 0.0f, carZ = 0.0f;
float carAngle = 0.0f; // 차량의 회전 각도

// 텍스트 그리기 함수
void drawText(const char* text, float x, float y) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0); // 2D 평면 설정

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// 키보드 입력 콜백
void keyboardCallback(unsigned char key, int x, int y) {
    float speed = 0.1f;           // 전진 및 후진 속도
    float rotationSpeed = 5.0f;   // 각도 회전 속도

    switch (key) {
    case 'w': // 전진
        carX += speed * cos(carAngle * M_PI / 180.0f);
        carZ += speed * sin(carAngle * M_PI / 180.0f);
        break;
    case 's': // 후진
        carX -= speed * cos(carAngle * M_PI / 180.0f);
        carZ -= speed * sin(carAngle * M_PI / 180.0f);
        break;
    case 'a': // 우회전 (기존 좌회전 기능을 우회전으로 변경)
        carAngle -= rotationSpeed;
        break;
    case 'd': // 좌회전 (기존 우회전 기능을 좌회전으로 변경)
        carAngle += rotationSpeed;
        break;
    case 27: // ESC 키로 종료
        exit(0);
    }

    glutPostRedisplay(); // 화면 갱신
}

// 환경 초기화 함수 (건물 생성)
void initializeEnvironment() {
    srand(static_cast<unsigned>(time(0))); // 랜덤 시드 초기화

    for (float x = -5.0f; x <= 5.0f; x += 2.0f) {
        for (float z = -5.0f; z <= 5.0f; z += 2.0f) {
            Building building;
            building.x = x;
            building.z = z;
            building.height = 1.0f + static_cast<float>(rand() % 5); // 1.0 ~ 5.0 사이
            building.red = static_cast<float>(rand() % 100) / 100.0f;
            building.green = static_cast<float>(rand() % 100) / 100.0f;
            building.blue = static_cast<float>(rand() % 100) / 100.0f;
            buildings.push_back(building);
        }
    }
}

// 환경 그리기 함수
void drawEnvironment() {
    // 전체 바닥 (검은색 도로)
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_QUADS);
    glVertex3f(-10.0f, 0.0f, -10.0f);
    glVertex3f(-10.0f, 0.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, 10.0f);
    glVertex3f(10.0f, 0.0f, -10.0f);
    glEnd();

    // 흰색 도로 (검은 바닥 위)
    glColor3f(1.0f, 1.0f, 1.0f); // 흰색
    glBegin(GL_QUADS);
    glVertex3f(-6.5f, 0.01f, -6.5f); // 바닥보다 약간 위로 설정 (y=0.01f)
    glVertex3f(-6.5f, 0.01f, 6.5f);
    glVertex3f(6.5f, 0.01f, 6.5f);
    glVertex3f(6.5f, 0.01f, -6.5f);
    glEnd();

    // 건물 그리기
    for (const auto& building : buildings) {
        glColor3f(building.red, building.green, building.blue); // 고정된 색상 사용
        glPushMatrix();
        glTranslatef(building.x, building.height / 2.0f, building.z); // 중심점 설정
        glScalef(1.0f, building.height, 1.0f);                       // 높이 반영
        glutSolidCube(1.0f);
        glPopMatrix();
    }
}

void drawScreenText(const char* text, int x, int y) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // 창 크기 기반으로 오쏘그래픽 설정
    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    glOrtho(0, windowWidth, 0, windowHeight, -1, 1); // 왼쪽 하단 원점, 픽셀 단위 좌표계

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // 텍스트 출력 좌표 지정 (왼쪽 하단 기준, 텍스트 출력 영역 보장)
    glRasterPos2i(x, y);

    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // 글자 출력
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void renderViewport(int x, int y, int width, int height, float cameraX, float cameraY, float cameraZ, float lookX, float lookY, float lookZ, const char* label) {
    glViewport(x, y, width, height); // 뷰포트 설정
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, lookX, lookY, lookZ, 0.0, 1.0, 0.0);
    drawEnvironment(); // 환경 그리기

    // 뷰포트 중앙 상단에 텍스트 출력
    glDisable(GL_DEPTH_TEST); // 깊이 테스트 비활성화
    glColor3f(1.0f, 1.0f, 1.0f); // 흰색 텍스트
    drawScreenText(label, width / 2, height - 20); // 중앙 상단
    glEnable(GL_DEPTH_TEST); // 깊이 테스트 복원
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

    // 상단 (후면): 창 높이의 1/3만 사용
    renderViewport(0, 2 * windowHeight / 3, windowWidth, windowHeight / 3, carX, 0.5f, carZ,
        carX - cos(carAngle * M_PI / 180.0f), 0.5f, carZ - sin(carAngle * M_PI / 180.0f),
        "후면");

    // 하단 왼쪽 (우측): 좌측과 우측 카메라를 교환
    renderViewport(0, 0, windowWidth / 3, 2 * windowHeight / 3, carX, 0.5f, carZ,
        carX + cos((carAngle - 90.0f) * M_PI / 180.0f), 0.5f, carZ + sin((carAngle - 90.0f) * M_PI / 180.0f),
        "우측");

    // 하단 중간 (정면): 동일
    renderViewport(windowWidth / 3, 0, windowWidth / 3, 2 * windowHeight / 3, carX, 0.5f, carZ,
        carX + cos(carAngle * M_PI / 180.0f), 0.5f, carZ + sin(carAngle * M_PI / 180.0f),
        "정면");

    // 하단 오른쪽 (좌측): 좌측과 우측 카메라를 교환
    renderViewport(2 * windowWidth / 3, 0, windowWidth / 3, 2 * windowHeight / 3, carX, 0.5f, carZ,
        carX + cos((carAngle + 90.0f) * M_PI / 180.0f), 0.5f, carZ + sin((carAngle + 90.0f) * M_PI / 180.0f),
        "좌측");

    glutSwapBuffers();
}

// 윈도우 크기 조정
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)width / height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// 메인 함수
int main(int argc, char** argv) {

    glutInit(&argc, argv);

    // 환경 초기화
    initializeEnvironment();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("2020E7014_문동오_Computer_Graphiis_Final_Assigment");

    // 초기화
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.8f, 0.8f, 1.0f, 1.0f);

    // 콜백 등록
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardCallback);

    glutMainLoop();
    return 0;
}