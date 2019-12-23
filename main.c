/*
    By ericericericjin
    A BS plane-hitting game
*/

#include <stdio.h>
#include <malloc.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>

struct point {
    int x;
    int y;
};

int getKey() {
    if (_kbhit()) {
        // printf("%d", _getch());
        system("pause");
        return _getch();
    } else {
        return 0;
    }
}


int randomInt(int a, int b) {
    time_t t;
    // printf("%d\n", (unsigned) time(&t));
    srand((unsigned int)clock());
    // printf("%d\n", rand());
    return (rand() % (b - a) + a);
}

void show(struct point* cannon, struct point* planes, int plane_no, struct point* bullets, int bullet_no, int scr_x, int scr_y) {

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

    int** screen;
    screen = (int**)malloc(sizeof(int*) * scr_x);
    for (int x = 0; x < scr_x; x++) {
        *(screen + x) = (int*)malloc(sizeof(int) * scr_y);
        for (int y = 0; y < scr_y; y++) {
            *(*(screen + x) + y) = 0;
        }
    }

    /*
    Big gun / Cannon

       [ ]
    [ ][c][ ]
    */
    *(*(screen + (cannon -> x)) + (cannon -> y)) = 1;
    *(*(screen + (cannon -> x) + 1) + (cannon -> y)) = 1;
    *(*(screen + (cannon -> x) - 1) + (cannon -> y)) = 1;
    *(*(screen + (cannon -> x)) + (cannon -> y) - 1) = 1;
    
    // printf("%d", (planes + plane_no - 1) -> x);
    for (int i = 0; i < plane_no; i++) {
        /*
        Plane
        
           [ ]
        [ ][c][ ]
           [ ]
        */
        if (((planes + i) -> x) != -1) {
            *(*(screen + ((planes + i) -> x)) + ((planes + i) -> y)) = 1;
            *(*(screen + ((planes + i) -> x) + 1) + ((planes + i) -> y)) = 1;
            *(*(screen + ((planes + i) -> x) - 1) + ((planes + i) -> y)) = 1;
            *(*(screen + ((planes + i) -> x)) + ((planes + i) -> y) + 1) = 1;
            *(*(screen + ((planes + i) -> x)) + ((planes + i) -> y) - 1) = 1;
        }
    }
    for (int i = 0; i < bullet_no; i++) {
        /*
        Bullet

            [c]
        */
        if (((bullets + i) -> x) != -1) {
            *(*(screen + ((bullets + i) -> x)) + ((bullets + i) -> y)) = 1;
        }
    }

    for (int y = 0; y < scr_y; y++) {
        for (int x = 0; x < scr_x; x++) {
            if (*(*(screen + x) + y) == 1) {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int crush(struct point* plane, int plane_no, struct point* bullets, int bullet_no) {
    int crush_no = 0;
    for (int i = 0; i < plane_no; i++) {
        if (((plane + i) -> x) != -1) {
            for (int j = 0; j < bullet_no; j++) {
                if (((bullets + j) -> x) != -1) {
                    if (abs(((bullets + j) -> x) - ((plane + i) -> x)) <= 1) {
                        (plane + i) -> x = -1;
                        crush_no += 1;
                    }
                }
            }
        }
    }
    return crush_no;
}

int main() {
    int scr_x = 80;
    int scr_y = 24;
    int v_plane = 1;
    int v_bullet = 3;
    int v_cannon = 1;

    struct point* cannon;
    cannon = (struct point*)malloc(sizeof(struct point*) * 1);
    cannon -> x = scr_x / 2;
    cannon -> y = scr_y - 2;

    int bullet_period = 1;
    int plane_period = 10;

    // MAX + DIS / V
    struct point* planes;
    int plane_no = scr_y / (v_plane * plane_period);
    planes = (struct point*)malloc(sizeof(struct point) * plane_no);
    for (int i = 0; i < plane_no; i++) {
        (planes + i) -> x = -1;
    }

    struct point* bullet;
    int bullet_no = scr_y / (v_bullet * bullet_period);
    bullet = (struct point*)malloc(sizeof(struct point) * bullet_no);
    for (int i = 0; i < bullet_no; i++) {
        (bullet + i) -> x = -1;
    }

    int score = 0;

    for (int Counter = 0; ; Counter ++) {
        show(cannon, planes, plane_no, bullet, bullet_no, scr_x, scr_y);

        if (getKey() == 97) { //Move the cannon
            
            cannon -> x -= v_cannon;
            if ((cannon -> x) <= 0) {
                cannon -> x = 1;
            }
        } else if (getKey() == 100) {
            cannon -> x += v_cannon;
            if ((cannon -> x) >= scr_x) {
                cannon -> x = scr_x - 2;
            }
        }

        if (Counter % bullet_period == 0) {// shoot a bullet

            for (int i = 0; i < bullet_no; i++) {

                if (((bullet + i) -> x) == -1) {
                    (bullet + i) -> x = cannon -> x;
                    (bullet + i) -> y = (cannon -> y) - 1;
                    break;
                }
            }

        }

        if (Counter % plane_period == 0) { // new a plane
            for (int i = 0; i < plane_no; i++) {
                if (((planes + i) -> x) == -1) {
                    // empty
                    (planes + i) -> x = randomInt(0, scr_x - 1);
                    (planes + i) -> y = 1;
                    break;
                }
            }
        }

        for (int i = 0; i < plane_no; i++) { // PLANE: IF EXCEED THEN DELETE ELSE MOVE
            if ((planes + i) -> x != -1) {
                if ((planes + i) -> y >= scr_y) {
                    (planes + i) -> x = -1;
                } else {
                    (planes + i) -> y += v_plane;
                }
            }
        }
        for (int i = 0; i < bullet_no; i++) { // BULLET: IF EXCEED THEN DELETE ELSE MOVE
            if ((bullet + i) -> x != -1) {
                if ((bullet + i) -> y <= 0) {
                    (bullet + i) -> x = -1;
                } else {
                    (bullet + i) -> y -= v_bullet;
                }
            }
        }


        score += crush(planes, plane_no, bullet, bullet_no);
        Sleep(100);
        if (getKey() == 27) {
            break;
        }
    }
    printf("%d, ", score);
    return 0;
}