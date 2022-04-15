#include <graphics.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
// Luu Thai Son comment code
struct Square //  tạo một struct lưu trữ giá trị của 1 ô để vẽ chữ UET
{
    int x, y;     // tọa độ x, y[i]
    bool touched; // trạng thái đã bị bóng cham hay chưa
};
typedef struct Square Square;
struct Check // tạo một struct để lưu giá trị trả về khi kiểm tra quả bóng chạm ô vuông
{
    int index;    // số thứ tự của ô vuông
    bool touched; // trạng thái
};
typedef struct Check Check;
Square create(int x, int y, bool touched) // tạo một ô vuông
{
    Square temp;
    temp.x = x;
    temp.y = y;
    temp.touched = touched;
    return temp;
}
/* tạo biến toàn cục (global) */
// số bóng(mới vào game là 1 bóng)
int countOfBall = 1;
// x, y[i] : tọa độ của bóng
// moveX[i], moveY[i] hướng di chuyển và tốc độ của bóng theo phương ngang và dọc
int *x = (int *)malloc(sizeof(int)),
    *y = (int *)malloc(sizeof(int)),
    *moveX = (int *)malloc(sizeof(int)),
    *moveY = (int *)malloc(sizeof(int));
int xPaddle; // xPaddle: tọa độ của thanh
// heart: số máu còn lại(mặc định lúc bắt đầu  game là 3 máu)
// countOfUET: số ô vuông cần thiết để vẽ chữ UET
// điểm = 0
// trạng thái (status) mặc định là bóng
int heart = 3, countOfUET = 48, score = 0, *status = (int *)malloc(sizeof(int));
time_t secondsStartFireBall = 0, presentTime = 0;            // thời gian kể từ lúc bắt đầu fire ball
bool result, finished = 0;                                   // finished: game kết thúc hay chưa(mặc định là chưa),  esult: kết quả(1: thắng, 2: thua)
Square *arr = (Square *)malloc(sizeof(Square) * countOfUET); // tạo con trỏ square để lưu trữ mảng gồm 48 (countOfUET) ô vuông

// khai báo tên hàm khá dễ hiểu, khỏi cần comment
void drawHeart();
int movePaddle();
void drawBall(int i);
bool moveBall(int i);
void newPaddle();
void newBall(int i);
void mainLoop();
void drawSquare(Square square);
void drawUET();
void drawScore(int score, int color);
Check checkBallToSquare_X(int i);
Check checkBallToSquare_Y(int i);
bool checkBallToSquare(int i);
bool checkWin();
void newGame();
void createUET();
void drawTutorial();
// quà tặng
void giveHeart();
void reduceSpeed(int i);
void addBall();
void fireBall(int i);

/*
    CÁCH DI CHUYỂN CỦA THANH VÀ BÓNG
    vẽ tại vị trí hiện tại của thanh(bóng) với màu trùng với màu nền
    thay đổi tọa độ của thanh(bóng)
    vẽ tại vị trí mới với màu của thanh(bóng)
 */
void drawHeart() // vẽ máu ở góc dưới bên phải màn hình
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
int movePaddle() // di chuyển thanh
{
    char keyPress = getch(); // tạo biến lưu giá trị đc nhấn từ bàn phím
    if (keyPress == 75)      // nếu là phím LEFT
    {
        // vẽ tại vị trí cũ -- màu đen
        setcolor(0);
        setfillstyle(1, 0);
        bar(xPaddle - 50, 575, xPaddle + 50, 600);
        // thay đổi tọa độ
        xPaddle -= 10;
        // nếu vượt ra ngoài màn hình thì đặt lại ngay sát góc
        if (xPaddle <= 50)
            xPaddle = 50;
        // vẽ tại vị trí mới -- màu đỏ
        setcolor(4);
        setfillstyle(1, 4);
        bar(xPaddle - 50, 575, xPaddle + 50, 600);
    }
    else if (keyPress == 77) // nếu là phím RIGHT (tương tự LEFT)
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
    return keyPress;
}
void drawBall(int i) // di chuyển bóng
{
    if (status[i] == 0)
    {
        setcolor(15);
        setfillstyle(1, 15);
    }
    else
    {
        setcolor(4);
        setfillstyle(1, 4);
    }
    fillellipse(x[i], y[i], 10, 10);
}
bool moveBall(int i)
{
    // vẽ tại vị trí cũ -- màu đen
    setcolor(0);
    setfillstyle(1, 0);
    fillellipse(x[i], y[i], 10, 10);

    // thay đổi tọa độ
    x[i] += moveX[i];
    y[i] += moveY[i];

    // vẽ tại vị trí cũ -- màu trắng
    drawBall(i);

    if (checkBallToSquare(i))
    {
        if (rand() % 4 == 0)
        {
            int present = rand() % 10;
            if (present == 0 || present == 1 || present == 2)
            {
                giveHeart();
            }
            else if (present == 3 || present == 4 || present == 5)
            {
                reduceSpeed(i);
            }
            else if (present == 6 || present == 7)
            {
                fireBall(i);
            }
            else if (present == 8 || present == 9)
            {
                if (countOfBall < 2)
                    addBall();
            }
        }
    }
    // nếu không chạm ô vuông
    else
    {
        // chạm hai cạnh thẳng đứng của màn hình
        if (x[i] >= 890 || x[i] <= 10)
        {
            moveX[i] *= -1;
        }
        // chạm 2 cạnh đứng của thanh (đổi hướng theo phương ngang rồi chết 1 mạng)
        else if (x[i] <= xPaddle + 60 && x[i] >= xPaddle - 60 && y[i] >= 575)
        {
            moveX[i] *= -1;
        }

        // chạm cạnh trên màn hình
        if (y[i] <= 10)
        {
            moveY[i] *= -1;
        }
        // chạm cạnh trên của thanh(-> đi lên)
        else if (y[i] >= 565 && y[i] <= 568 && x[i] >= xPaddle - 50 && x[i] <= xPaddle + 50)
        {
            if (moveY[i] < 3 && moveX[i] < 3)
            {
                moveY[i]++;
                if (moveX[i] > 0)
                    moveX[i]++;
                else
                    moveX[i]--;
            }
            moveY[i] *= -1;
        }
    }
    // nếu bóng xuống dưới
    if (y[i] >= 610)
    {
        heart--;     // chết 1 mạng
        drawHeart(); // vẽ lại máu

        if (countOfBall == 2)
        {
            countOfBall--;
            return 1;
        }
        else
        {
            newBall(i); // tạo bóng mới
        }
    }
    // kiểm tra đã hết thời gian fire ball chưa
    if (secondsStartFireBall != 0)
    {
        // nếu fire ball đã được 3 giây thì hết (trở về bóng bình thường)
        if (time((time_t *)0) - secondsStartFireBall >= 3)
        {
            status[i] = 0;
            secondsStartFireBall = 0; //đặt lại thời gian
        }
    }
    return 0;
}
void newPaddle() // tạo thanh mới(dùng khi mới vào game)
{
    // mặc định mới vào là thanh ở giữa
    xPaddle = 450;
    // vẽ
    setfillstyle(1, 4);
    bar(xPaddle - 50, 575, xPaddle + 50, 600);
}
void newBall(int i) // tạo bóng mới(dùng khi mới vào game hoặc mới chết xong)
{
    srand(time(NULL));
    // rand tọa độ
    x[i] = rand() % 800 + 20;
    y[i] = rand() % 100 + 20;
    // rand hướng
    if ((rand() % 2) == 0)
    {
        moveX[i] = -1;
    }
    else
    {
        moveX[i] = 1;
    }
    if ((rand() % 2) == 0)
    {
        moveY[i] = -1;
    }
    else
    {
        moveY[i] = 1;
    }
    // vẽ
    setcolor(15);
    setfillstyle(1, 15);
    fillellipse(x[i], y[i], 10, 10);
}
void mainLoop() // hàm main sử dụng đệ quy(hàm chính trong game)
{
    do
    {
        // trước khi di chuyển bóng thì vẽ lại UET và máu để tránh việc màu của bóng đè lên
        drawHeart();
        drawUET();
        drawTutorial();
        setcolor(15);
        line(900, 0, 900, 600);

        for (int i = 0; i < countOfBall; i++)
        {
            if (moveBall(i)) // thực hiện di chuyển bóng
            {
                if (i == 0)
                {
                    x[0] = x[1];
                    y[0] = y[1];
                    moveX[0] = moveX[1];
                    moveY[0] = moveY[1];
                }
            }
        }
        Sleep(1); // dừng màn hình một chút

        if (presentTime != 0)
        {
            if (time((time_t *)0) - presentTime >= 4)
            {
                setfillstyle(1, 0);
                bar(950, 400, 1100, 420);
            }
        }
    } while (!kbhit() && heart > 0 && !checkWin());
    // trong khi chưa nhấn phím nào đồng thời chưa thắng và chưa hết máu

    if (heart == 0) // nếu đã hết máu
    {
        finished = 1; // kết thúc game
        result = 0;   // kết quả là thua
        drawUET();    // vẽ UET màu đỏ

        // hiện 3 dòng chữ để thông báo và lựa chọn
        setcolor(4);
        outtextxy(410, 400, (char *)"YOU LOSE");
        setcolor(15);
        outtextxy(410, 450, (char *)"1. NEW GAME");
        outtextxy(410, 470, (char *)"2. EXIT");

        while (1) // vòng lặp vô hạn
        {
            int keyPress = getch();
            if (keyPress == '2') // cho tới khi nhấn phím 2
            {
                return; // thoát game
            }
            else if (keyPress == '1') // cho tới khi nhấn phím 1
            {
                // xóa 3 dòng chữ vưa hiện
                setcolor(0);
                outtextxy(410, 400, (char *)"YOU LOSE");
                outtextxy(410, 450, (char *)"1. NEW GAME");
                outtextxy(410, 470, (char *)"2. EXIT");

                drawScore(score, 0);
                newGame(); // tạo game mới
                drawScore(score, 15);
                goto NEWGAME; // thực hiện lại mainloop
            }
        }
    }
    else if (checkWin()) // nếu thắng thì tương tự như thua
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
                setcolor(0);
                outtextxy(410, 400, (char *)"YOU WIN");
                outtextxy(410, 450, (char *)"1. NEW GAME");
                outtextxy(410, 470, (char *)"2. EXIT");
                drawScore(score, 0);
                newGame();
                drawScore(score, 15);
                goto NEWGAME;
            }
        }
    }
    /* nếu không phải thắng, không phải thua thì là nhấn phím
    nhấn phím thì di chuyển thanh
    */
    if (movePaddle() == 27) // ESC
    {
        setcolor(15);
        outtextxy(410, 450, (char *)"1. CONTINUE");
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
                setcolor(0);
                outtextxy(410, 450, (char *)"1. CONTINUE");
                outtextxy(410, 470, (char *)"2. EXIT");
                break;
                goto NEWGAME;
            }
        }
    }
NEWGAME:
    mainLoop(); // gọi lại hàm(đệ quy)
}
void drawSquare(Square square) // vẽ một ô vuông
{
    // nếu đã kết thúc game
    if (finished)
    {
        if (result == 1)
            setfillstyle(1, 14); // nếu thắng thì vẽ màu vàng
        else

            setfillstyle(1, 4); // thua thì vẽ màu đỏ
    }
    // nếu chưa kết thúc
    else if (square.touched)
    {
        setfillstyle(1, 8); // chạm rồi thì vẽ màu đen xám
    }
    else
    {
        setfillstyle(1, 11); // chưa chạm thì vẽ màu xanh
    }
    // vẽ
    bar(square.x, square.y, square.x + 19, square.y + 19);
}
char *intToString(int a)
{
    char *s = (char *)malloc(sizeof(char) * 10);
    for (int i = 0; i <= 8; i++)
    {
        s[i] = '0';
    }
    s[9] = '\0';

    int i = 8;
    while (a > 0)
    {
        s[i] = a % 10 + '0';
        a /= 10;
        i--;
    }
    while (s[0] == '0')
    {
        for (int j = 0; j < strlen(s) - 1; j++)
        {
            s[j] = s[j + 1];
        }
        s[strlen(s) - 1] = '\0';
    }
    if (strlen(s) == 0)
    {
        s[0] = '0';
        s[1] = '\0';
    }
    return s;
}
void drawScore(int score, int color) // vẽ điểm
{
    setfillstyle(1, 8);
    bar(980, 90, 999, 109);

    char *temp = (char *)malloc(sizeof(char) * 15);

    temp = intToString(score);
    strcat(temp, (char *)"/48");

    setcolor(color);
    outtextxy(1010, 93, temp);
}
void drawUET()
{
    for (int i = 0; i < countOfUET; i++)
    {
        drawSquare(*(arr + i));
    }
}
Check checkBallToSquare_X(int i) // kiểm tra bóng chạm ô vuông theo phương ngang
{
    Check temp; // tạo biến để lưu giá trị trả về
    // kiểm tra từng ô
    for (int j = 0; j < countOfUET; j++)
    {
        // kiểm tra tọa độ đồng thời ô đó chưa bị chạm lần nào
        if (x[i] >= (arr + j)->x - 10 && x[i] <= (arr + j)->x + 30 && y[i] >= (arr + j)->y && y[i] <= (arr + j)->y + 20 && (arr + j)->touched == 0)
        {
            temp.touched = 1;
            temp.index = j;
            return temp; // trả về bị chạm và vị trí của ô vuông
        }
    }
    temp.touched = 0;
    temp.index = -1;
    return temp; // trả về chưa bị chạm
}
Check checkBallToSquare_Y(int i) // tương tự X
{
    Check temp;
    for (int j = 0; j < countOfUET; j++)
    {
        if (y[i] >= (arr + j)->y - 10 && y[i] <= (arr + j)->y + 30 && x[i] >= (arr + j)->x && x[i] <= (arr + j)->x + 20 && (arr + j)->touched == 0)
        {
            temp.touched = 1;
            temp.index = j;
            return temp;
        }
    }
    temp.touched = 0;
    temp.index = -1;
    return temp;
}
bool checkBallToSquare(int i)
{
    // kiểm tra bóng vs 48 ô vuông
    Check checkBallX = checkBallToSquare_X(i), checkBallY = checkBallToSquare_Y(i);
    // nếu chạm theo cả 2 phương(chạm chéo)
    if (checkBallX.touched && checkBallY.touched)
    {
        // set trạng thái của ô vuông là đã chạm
        (arr + checkBallX.index)->touched = 1;

        drawSquare(*(arr + checkBallX.index));

        drawScore(score, 0);
        score++;
        drawScore(score, 15);
        // nếu là bóng trắng thì đổi hướng cả hai phương
        // còn fire ball thì đi thẳng
        if (status[i] == 0)
        {
            moveX[i] *= -1;
            moveY[i] *= -1;
        }
        return 1;
    }
    // neu chỉ chạm theo phương ngang
    else if (checkBallX.touched && !checkBallY.touched)
    {
        (arr + checkBallX.index)->touched = 1;
        drawSquare(*(arr + checkBallX.index));
        drawScore(score, 0);
        score++;
        drawScore(score, 15);
        if (status[i] == 0) // nếu là bóng trắng
            moveX[i] *= -1;
        return 1;
    }
    // neu chỉ chạm theo phương dọc
    else if (!checkBallX.touched && checkBallY.touched)
    {
        (arr + checkBallY.index)->touched = 1;
        drawSquare(*(arr + checkBallY.index));
        drawScore(score, 0);
        score++;
        drawScore(score, 15);
        if (status[i] == 0) // nếu là bóng trắng
            moveY[i] *= -1;
        return 1;
    }
    return 0;
}
bool checkWin() // nếu UET bị chạm hết rồi thì thắng
{
    for (int i = 0; i < countOfUET; i++)
    {
        if ((arr + i)->touched == 0)
            return 0;
    }
    return 1;
}
void newGame() // game mới
{
    cleardevice();
    finished = 0; // chưa kết thúc
    heart = 3;    //đặt lại máu
    score = 0;
    countOfBall = 1;

    realloc(x, countOfBall);
    realloc(y, countOfBall);
    realloc(moveX, countOfBall);
    realloc(moveY, countOfBall);

    for (int i = 0; i < countOfUET; i++)
    {
        (arr + i)->touched = 0; //đặt lại tất cả ô vuông đều chưa bị chạm
    }
    drawUET(); // vẽ lại uet
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
void drawTutorial()
{
    setcolor(15);
    outtextxy(950, 200, (char *)"LEFT: Move Left");
    outtextxy(950, 220, (char *)"RIGHT: Move Right");
    outtextxy(950, 240, (char *)"ESC: Pause");
}
void giveHeart()
{
    if (heart < 3)
    {
        heart++;
        drawHeart();
        setfillstyle(1, 0);
        bar(950, 400, 1100, 420);
        setcolor(15);
        outtextxy(950, 400, (char *)"+1 Heart");
        presentTime = time((time_t *)0);
    }
}
void reduceSpeed(int i)
{
    if (abs(moveX[i]) != 1 && abs(moveY[i]) != 1)
    {
        if (moveY[i] > 0)
            moveY[i]--;
        else
            moveY[i]++;

        if (moveX[i] > 0)
            moveX[i]--;
        else
            moveX[i]++;

        setfillstyle(1, 0);
        bar(950, 400, 1100, 420);
        setcolor(15);
        outtextxy(950, 400, (char *)"Reduce Speed");
        presentTime = time((time_t *)0);
    }
}
void addBall()
{
    countOfBall++;

    realloc(x, countOfBall);
    realloc(y, countOfBall);
    realloc(moveX, countOfBall);
    realloc(moveY, countOfBall);
    newBall(countOfBall - 1);

    realloc(status, countOfBall);
    status[1] = 0;

    setfillstyle(1, 0);
    bar(950, 400, 1100, 420);
    setcolor(15);
    outtextxy(950, 400, (char *)"+1 Ball");
    presentTime = time((time_t *)0);
}
void fireBall(int i)
{
    status[i] = 1;
    secondsStartFireBall = time((time_t *)0);
    setfillstyle(1, 0);
    bar(950, 400, 1100, 420);
    setcolor(15);
    outtextxy(950, 400, (char *)"Fire ball");
    presentTime = time((time_t *)0);
}
int main()
{
    initwindow(1100, 600, (char *)"UET's Brick Breaker");
    setbkcolor(0);
    cleardevice();

    createUET();
    drawUET();
    drawHeart();
    drawScore(score, 15);
    drawTutorial();

    newPaddle();
    newBall(0);

    status[0] = 0;

    mainLoop();

    closegraph();
    return 0;
}