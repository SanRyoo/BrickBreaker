#include <graphics.h>
#include <stdlib.h>
#include <time.h>
struct Square
{
    int x, y;
    bool touched;
};
typedef struct Square Square;
struct Check
{
    int index;
    bool touched;
};
typedef struct Check Check;
Square create(int x, int y, bool touched)
{
    Square temp;
    temp.x = x;
    temp.y = y;
    temp.touched = touched;
    return temp;
}

int x, y, xPaddle, moveX, moveY, heart = 3, countOfUET = 48;
bool result, finished = 0;

Square *arr = (Square *)malloc(sizeof(Square) * countOfUET);

void drawHeart();
void movePaddle();
void moveBall();
void newPaddle();
void newBall();
void mainLoop();
void drawSquare(Square square);
void drawUET();
Check checkBallToQuare_X();
Check checkBallToQuare_Y();
bool checkWin();
void newGame();
void createUET();

void drawHeart()
{
    if (heart == 3)
    {
        setcolor(1);
        setfillstyle(1, 10);
        fillellipse(20, 550, 10, 10);
        fillellipse(40, 550, 10, 10);
        fillellipse(60, 550, 10, 10);
    }
    else if (heart == 2)
    {
        setcolor(1);
        setfillstyle(1, 10);
        fillellipse(20, 550, 10, 10);
        fillellipse(40, 550, 10, 10);
        setcolor(4);
        setfillstyle(1, 0);
        fillellipse(60, 550, 10, 10);
    }
    else if (heart == 1)
    {
        setcolor(1);
        setfillstyle(1, 10);
        fillellipse(20, 550, 10, 10);
        setcolor(4);
        setfillstyle(1, 0);
        fillellipse(40, 550, 10, 10);
        fillellipse(60, 550, 10, 10);
    }
    else if (heart == 0)
    {
        setcolor(4);
        setfillstyle(1, 0);
        fillellipse(20, 550, 10, 10);
        fillellipse(40, 550, 10, 10);
        fillellipse(60, 550, 10, 10);
    }
}
void movePaddle()
{
    char keyPress = getch();
    if (keyPress == 75)
    {
        setcolor(0);
        setfillstyle(1, 0);
        bar(xPaddle - 50, 575, xPaddle + 50, 600);
        xPaddle -= 10;
        if (xPaddle <= 50)
            xPaddle = 50;
        setcolor(4);
        setfillstyle(1, 4);
        bar(xPaddle - 50, 575, xPaddle + 50, 600);
    }
    else if (keyPress == 77)
    {
        setcolor(0);
        setfillstyle(1, 0);
        bar(xPaddle - 50, 575, xPaddle + 50, 600);
        xPaddle += 10;
        if (xPaddle >= 850)
            xPaddle = 850;
        setcolor(4);
        setfillstyle(1, 4);
        bar(xPaddle - 50, 575, xPaddle + 50, 600);
    }
}
void moveBall()
{
    drawHeart();
    drawUET();

    setcolor(0);
    setfillstyle(1, 0);
    fillellipse(x, y, 10, 10);

    x += moveX;
    y += moveY;

    setcolor(15);
    setfillstyle(1, 15);
    fillellipse(x, y, 10, 10);

    Sleep(1);

    Check checkBallX = checkBallToQuare_X(), checkBallY = checkBallToQuare_Y();

    if (checkBallX.touched && checkBallY.touched)
    {
        (arr + checkBallX.index)->touched = 1;
        drawSquare(*(arr + checkBallX.index));
        moveX *= -1;
        moveY *= -1;
    }
    else if (checkBallX.touched && !checkBallY.touched)
    {
        (arr + checkBallX.index)->touched = 1;
        drawSquare(*(arr + checkBallX.index));
        moveX *= -1;
    }
    else if (!checkBallX.touched && checkBallY.touched)
    {
        (arr + checkBallY.index)->touched = 1;
        drawSquare(*(arr + checkBallY.index));
        moveY *= -1;
    }
    else
    {

        if (x >= 890 || x <= 10)
        {
            moveX *= -1;
        }
        else if (x <= xPaddle + 60 && x >= xPaddle - 60 && y >= 575)
        {
            moveX *= -1;
        }

        if (y <= 10)
        {
            moveY *= -1;
        }
        else if (y >= 565 && x >= xPaddle - 50 && x <= xPaddle + 50)
        {
            if (moveY < 3)
            {
                moveY++;
                if (moveX > 0)
                {
                    moveX++;
                }
                else
                {
                    moveX--;
                }
            }
            moveY *= -1;
        }
    }
    if (y >= 610)
    {
        newBall();
        heart--;
        drawHeart();
    }
}
void newPaddle()
{
    xPaddle = 450;
    setfillstyle(1, 4);
    bar(xPaddle - 50, 575, xPaddle + 50, 600);
}
void newBall()
{
    srand(time(NULL));
    x = rand() % 800 + 20;
    y = rand() % 100 + 20;
    if ((rand() % 2) == 0)
    {
        moveX = -1;
    }
    else
    {
        moveX = 1;
    }
    if ((rand() % 2) == 0)
    {
        moveY = -1;
    }
    else
    {
        moveY = 1;
    }
    setcolor(15);
    setfillstyle(1, 15);
    fillellipse(x, y, 10, 10);
}
void mainLoop()
{
    do
    {
        moveBall();
    } while (!kbhit() && heart > 0 && !checkWin());
    if (heart == 0)
    {
        finished = 1;
        result = 0;
        drawUET();
        setcolor(4);
        outtextxy(410, 400, (char *)"YOU LOSE");
        setcolor(15);
        outtextxy(410, 450, (char *)"1. NEW GAME");
        outtextxy(410, 470, (char *)"2. EXIT");
        while (1)
        {
            int keyPress = getch();
            if (keyPress == '2')
            {
                return;
            }
            else if (keyPress == '1')
            {
                heart = 3;
                setcolor(0);
                outtextxy(410, 400, (char *)"YOU LOSE");
                outtextxy(410, 450, (char *)"1. NEW GAME");
                outtextxy(410, 470, (char *)"2. EXIT");
                newGame();
                goto NEWGAME;
            }
        }
    }
    else if (checkWin())
    {
        finished = 1;
        result = 1;
        drawUET();
        setcolor(14);
        outtextxy(410, 400, (char *)"YOU WIN");
        setcolor(15);
        outtextxy(410, 450, (char *)"1. NEW GAME");
        outtextxy(410, 470, (char *)"2. EXIT");
        while (1)
        {
            int keyPress = getch();
            if (keyPress == '2')
            {
                return;
            }
            else if (keyPress == '1')
            {
                heart = 3;
                setcolor(0);
                outtextxy(410, 400, (char *)"YOU WIN");
                outtextxy(410, 450, (char *)"1. NEW GAME");
                outtextxy(410, 470, (char *)"2. EXIT");
                newGame();
                goto NEWGAME;
            }
        }
    }
    movePaddle();
NEWGAME:
    mainLoop();
}
void drawSquare(Square square)
{
    if (finished)
    {
        if (result == 1)
            setfillstyle(1, 14);
        else
            setfillstyle(1, 4);
    }
    else if (square.touched)
    {
        setfillstyle(1, 8); // 8
    }
    else
    {
        setfillstyle(1, 11);
    }
    bar(square.x, square.y, square.x + 19, square.y + 19);
}
void drawUET()
{
    for (int i = 0; i < countOfUET; i++)
    {
        drawSquare(*(arr + i));
    }
}
Check checkBallToQuare_X()
{
    Check temp;
    for (int i = 0; i < countOfUET; i++)
    {
        if (x >= (arr + i)->x - 10 && x <= (arr + i)->x + 30 && y >= (arr + i)->y && y <= (arr + i)->y + 20 && (arr + i)->touched == 0)
        {
            temp.touched = 1;
            temp.index = i;
            return temp;
        }
    }
    temp.touched = 0;
    temp.index = -1;
    return temp;
}
Check checkBallToQuare_Y()
{
    Check temp;
    for (int i = 0; i < countOfUET; i++)
    {
        if (y >= (arr + i)->y - 10 && y <= (arr + i)->y + 30 && x >= (arr + i)->x && x <= (arr + i)->x + 20 && (arr + i)->touched == 0)
        {
            temp.touched = 1;
            temp.index = i;
            return temp;
        }
    }
    temp.touched = 0;
    temp.index = -1;
    return temp;
}
bool checkWin()
{
    for (int i = 0; i < countOfUET; i++)
    {
        if ((arr + i)->touched == 0)
            return 0;
    }
    return 1;
}
void newGame()
{
    finished = 0;
    for (int i = 0; i < countOfUET; i++)
    {
        (arr + i)->touched = 0;
    }
    drawUET();
}
void createUET()
{
    /* U */
    *(arr + 0) = create(180, 180, 0);
    *(arr + 1) = create(180, 200, 0);
    *(arr + 2) = create(180, 220, 0);
    *(arr + 3) = create(180, 240, 0);
    *(arr + 4) = create(180, 260, 0);
    *(arr + 5) = create(180, 280, 0);
    *(arr + 6) = create(180, 300, 0);

    *(arr + 7) = create(200, 300, 0);
    *(arr + 8) = create(220, 300, 0);
    *(arr + 9) = create(240, 300, 0);
    *(arr + 10) = create(260, 300, 0);

    *(arr + 11) = create(260, 180, 0);
    *(arr + 12) = create(260, 200, 0);
    *(arr + 13) = create(260, 220, 0);
    *(arr + 14) = create(260, 240, 0);
    *(arr + 15) = create(260, 260, 0);
    *(arr + 16) = create(260, 280, 0);

    /* E */
    *(arr + 17) = create(410, 180, 0);
    *(arr + 18) = create(410, 200, 0);
    *(arr + 19) = create(410, 220, 0);
    *(arr + 20) = create(410, 240, 0);
    *(arr + 21) = create(410, 260, 0);
    *(arr + 22) = create(410, 280, 0);
    *(arr + 23) = create(410, 300, 0);

    *(arr + 24) = create(430, 180, 0);
    *(arr + 25) = create(450, 180, 0);
    *(arr + 26) = create(470, 180, 0);
    *(arr + 27) = create(490, 180, 0);

    *(arr + 28) = create(430, 240, 0);
    *(arr + 29) = create(450, 240, 0);
    *(arr + 30) = create(470, 240, 0);
    *(arr + 31) = create(490, 240, 0);

    *(arr + 32) = create(430, 300, 0);
    *(arr + 33) = create(450, 300, 0);
    *(arr + 34) = create(470, 300, 0);
    *(arr + 35) = create(490, 300, 0);

    /* T */
    *(arr + 36) = create(600, 180, 0);
    *(arr + 37) = create(620, 180, 0);
    *(arr + 38) = create(640, 180, 0);
    *(arr + 39) = create(660, 180, 0);
    *(arr + 40) = create(680, 180, 0);

    *(arr + 41) = create(640, 180, 0);
    *(arr + 42) = create(640, 200, 0);
    *(arr + 43) = create(640, 220, 0);
    *(arr + 44) = create(640, 240, 0);
    *(arr + 45) = create(640, 260, 0);
    *(arr + 46) = create(640, 280, 0);
    *(arr + 47) = create(640, 300, 0);
}

int main()
{
    initwindow(900, 600, (char *)"UET's Brick Breaker");
    setbkcolor(0);
    cleardevice();

    createUET();
    drawUET();
    drawHeart();

    newPaddle();
    newBall();

    mainLoop();

    closegraph();
    return 0;
}
