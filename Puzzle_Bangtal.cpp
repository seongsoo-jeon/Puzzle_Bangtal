#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<Bangtal.h>
#pragma comment (lib, "Bangtal.lib")

SceneID scene, scene1;

ObjectID P[4][4];
ObjectID p[15], blank;
ObjectID startButton, endButton, restart;

TimerID timer1;

int num[15];

int blanki = 15;
int blankx = 3;
int blanky = 0;

int X[4] = { 270, 450, 630, 810 };
int Y[4] = { 0, 180, 360, 540 };

void swap(ObjectID& object1, ObjectID& object2) {
    ObjectID temp;
    temp = object1;
    object1 = object2;
    object2 = temp;
}
void left() {
    locateObject(p[blanki - 1], scene1, X[blankx], Y[blanky]);
    blanki = blanki - 1;
    blankx--;
}
void right() {
    locateObject(p[blanki + 1], scene1, X[blankx], Y[blanky]);
    blanki = blanki + 1;
    blankx++;
}
void up() {
    locateObject(p[blanki - 4], scene1, X[blankx], Y[blanky]);
    blanki = blanki - 4;
    blanky++;
}
void down() {
    locateObject(p[blanki + 4], scene1, X[blankx], Y[blanky]);
    blanki = blanki + 4;
    blanky--;
}
void change() {
    int ex = 0;
    srand((unsigned int)time(NULL));
    for (int i = 0; i < 15; i++)
    {
        num[i] = rand() % 15;
        for (int j = 0; j < i; j++) {
            if (num[i] == num[j]) {
                i--;
                break;
            }
        }
    }
}
void New() {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            hideObject(p[((3 - y) * 4) + x]);
        }
    }
}

void create() {
    char rnum[50];
    int x, y;
    for (x = 0; x < 4; x++) {
        for (y = 0; y < 4; y++) {
            if ((x == 3) && (y == 0)) {
                blank = createObject("빈칸", "15.png");
                locateObject(blank, scene1, X[3], Y[0]);
            }
            else {
                sprintf_s(rnum, "%d.jpg", num[((3 - y) * 4) + x]);
                p[((3 - y) * 4) + x] = createObject("p", rnum);
                locateObject(p[((3 - y) * 4) + x], scene1, X[x], Y[y]);
                showObject(p[((3 - y) * 4) + x]);
            }
        }
    }
}

void Restart() {
    enterScene(scene);
    showObject(startButton);
    showObject(endButton);
    stopTimer(timer1);
    New();
    blanki = 15;
    blankx = 3;
    blanky = 0;
}
void enter() {
    enterScene(scene1);
    hideObject(startButton);
    hideObject(endButton);
    setTimer(timer1, 3600.0f);
    startTimer(timer1);
}
ObjectID CreateObject(const char* name, const char* image, SceneID scene, int x, int y) {

    ObjectID object = createObject(name, image);
    locateObject(object, scene, x, y);
    showObject(object);
    return object;
}
void timerCallback(TimerID timer) {
    if (timer == timer1) {
        enterScene(scene);
        stopTimer(timer1);
    }
}
void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
    if (object == startButton) {
        enter();
        change();
        create();
    }
    else if (object == endButton) {
        endGame();
    }
    else if (object == restart) {
        Restart();
    }
    if (blankx > 0) {
        if ((object == p[blanki - 1])) {
            left();
        }
    }
    if (blankx < 3) {
        if (object == p[blanki + 1]) {
            right();
        }
    }
    if (blanky < 3) {
        if (object == p[blanki - 4]) {
            up();
        }
    }
    if (blanky > 0) {
        if (object == p[blanki + 4]) {
            down();
        }
    }
}

int main() {
    setMouseCallback(mouseCallback);
    scene = createScene("롤토체스 퍼즐", "배경1.jpg");
    scene1 = createScene("퍼즐", "배경.jpg");

    bool checked[100];
    bool sucess = true;

    for (int j = 0; j < 4; j++) {
        for (int k = 0; k < 4; k++) {
            P[j][k] = createObject("조각((3 - k) * 4 + j)", "((3 - k) * 4 + j).jpg");
        }
    }
    for (int j = 0; j < 4; j++) {
        for (int k = 0; k < 4; k++) {
            if (P[j][k] == p[((3 - k) * 4 + j)]) {
                checked[((3 - k) * 4 + j)] = true;
            }
            else {
                checked[((3 - k) * 4 + j)] = false;
            }
        }
    }
    int start, end;
    char timestore[100];
    float time;
    start = clock();
    for (int i = 0; i < 100000000; i++);
    end = clock();

    time = (float)(end - start) / CLOCKS_PER_SEC;
    sprintf_s(timestore, "걸린시간은 %d초", time);

    for (int g = 0; g < 15; g++) {
        if (checked[g] == true) {
            sucess = true;
        }
        else {
            sucess = false;
        }
    }
    if (sucess == true) {
        stopTimer(timer1);
        showMessage(timestore);
    }

    restart = CreateObject("재시작", "restart.png", scene1, 100, 100);
    startButton = CreateObject("시작", "start.png", scene, 590, 70);
    endButton = CreateObject("종료", "end.png", scene, 590, 20);

    timer1 = createTimer(3600.0f);
    showTimer(timer1);

    startGame(scene);
}
