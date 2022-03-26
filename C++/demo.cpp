#include <graphics.h>
#include <stdlib.h>
#include <time.h>
// tạo một struct lưu trữ giá trị của 1 ô để vẽ chữ UET
struct Square
{
    int x, y;     // tọa độ x, y
    bool touched; // trạng thái đã bị bóng cham hay chưa
};
typedef struct Square Square;
// tạo một struct để lưu giá trị trả về khi kiểm tra quả bóng chạm ô vuông
struct Check
{
    int index;    // số thứ tự của ô vuông
    bool touched; // trạng thái
};
typedef struct Check Check;
// tạo một ô vuông
Square create(int x, int y, bool touched)
{
    Square temp;
    temp.x = x;
    temp.y = y;
    temp.touched = touched;
    return temp;
}

// tạo biến toàn cục (global)
// x, y : tọa độ của bóng
// xPaddle: tọa độ của thanh
// moveX, moveY hướng di chuyển và tốc độ của bóng theo phương ngang và dọc
// heart: số máu còn lại(mặc định lúc bắt đầu  game là 3 máu)
// countOfUET: số ô vuông cần thiết để vẽ chữ UET
int x, y, xPaddle, moveX, moveY, heart = 3, countOfUET = 48;
// finished: game kết thúc hay chưa(mặc định là chưa)
// result: kết quả(1: thắng, 2: thua)
bool result, finished = 0;
// tạo con trỏ square để lưu trữ mảng gồm 48 (countOfUET) ô vuông
Square *arr = (Square *)malloc(sizeof(Square) * countOfUET);

// khai báo tên hàm khá dễ hiểu, khỏi cần comment
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

// vẽ máu ở góc dưới bên phải màn hình
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
/*
    CÁCH DI CHUYỂN CỦA THANH VÀ BÓNG
    vẽ tại vị trí hiện tại của thanh(bóng) với màu trùng với màu nền
    thay đổi tọa độ của thanh(bóng)
    vẽ tại vị trí mới với màu của thanh(bóng)
 */

// di chuyển thanh
void movePaddle()
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
}
// di chuyển bóng
void moveBall()
{
    // trước khi di chuyển bóng thì vẽ lại UET và máu để tránh việc màu của bóng đè lên
    drawHeart();
    drawUET();
    // vẽ tại vị trí cũ -- màu đen
    setcolor(0);
    setfillstyle(1, 0);
    fillellipse(x, y, 10, 10);
    // thay đổi tọa độ
    x += moveX;
    y += moveY;
    // vẽ tại vị trí cũ -- màu trắng
    setcolor(15);
    setfillstyle(1, 15);
    fillellipse(x, y, 10, 10);

    Sleep(1); // dừng màn hình một chút

    // kiểm tra bóng vs 48 ô vuông
    Check checkBallX = checkBallToQuare_X(), checkBallY = checkBallToQuare_Y();

    // nếu chạm theo cả 2 phương(chạm chéo)
    if (checkBallX.touched && checkBallY.touched)
    {
        // set trạng thái của ô vuông là đã chạm
        (arr + checkBallX.index)->touched = 1;
        drawSquare(*(arr + checkBallX.index));
        //đổi hướng cả hai phương
        moveX *= -1;
        moveY *= -1;
    }
    // neu chỉ chạm theo phương ngang
    else if (checkBallX.touched && !checkBallY.touched)
    {
        (arr + checkBallX.index)->touched = 1;
        drawSquare(*(arr + checkBallX.index));
        moveX *= -1;
    }
    // neu chỉ chạm theo phương dọc
    else if (!checkBallX.touched && checkBallY.touched)
    {
        (arr + checkBallY.index)->touched = 1;
        drawSquare(*(arr + checkBallY.index));
        moveY *= -1;
    }
    // nếu không chạm ô vuông
    else
    {
        // chạm hai cạnh thẳng đứng của màn hình
        if (x >= 890 || x <= 10)
        {
            moveX *= -1;
        }
        // chạm cạnh trên của thanh(-> đi lên)
        else if (x <= xPaddle + 60 && x >= xPaddle - 60 && y >= 575)
        {
            moveX *= -1;
        }
        // chạm cạnh trên
        if (y <= 10)
        {
            moveY *= -1;
        }
        // chạm cạnh phải của thanh (đổi hướng theo phương ngang rồi chết 1 mạng)
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
    // nếu bóng xuống dưới
    if (y >= 610)
    {
        newBall();   // tạo bóng mới
        heart--;     // chết 1 mạng
        drawHeart(); // vẽ lại máu
    }
}
// tạo thanh mới(dùng khi mới vào game)
void newPaddle()
{
    // mặc định mới vào là thanh ở giữa
    xPaddle = 450;
    // vẽ
    setfillstyle(1, 4);
    bar(xPaddle - 50, 575, xPaddle + 50, 600);
}
// tạo bóng mới(dùng khi mới vào game hoặc mới chết xong)
void newBall()
{
    srand(time(NULL));
    // rand tọa độ
    x = rand() % 800 + 20;
    y = rand() % 100 + 20;
    // rand hướng
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
    // vẽ
    setcolor(15);
    setfillstyle(1, 15);
    fillellipse(x, y, 10, 10);
}
// hàm main sử dụng đệ quy(hàm chính trong game)
void mainLoop()
{
    do
    {
        moveBall(); // thực hiện di chuyển bóng
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

                newGame();    // tạo game mới
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
                newGame();
                goto NEWGAME;
            }
        }
    }
    /* nếu không phải thắng, không phải thua thì là nhấn phím
    nhấn phím thì di chuyển thanh
    */
    movePaddle();
NEWGAME:
    mainLoop(); // gọi lại hàm(đệ quy)
}
// vẽ một ô vuông
void drawSquare(Square square)
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
void drawUET()
{
    for (int i = 0; i < countOfUET; i++)
    {
        drawSquare(*(arr + i));
    }
}
// kiểm tra bóng chạm ô vuông theo phương ngang
Check checkBallToQuare_X()
{
    Check temp; // tạo biến để lưu giá trị trả về
    // kiểm tra từng ô
    for (int i = 0; i < countOfUET; i++)
    {
        // kiểm tra tọa độ đồng thời ô đó chưa bị chạm lần nào
        if (x >= (arr + i)->x - 10 && x <= (arr + i)->x + 30 && y >= (arr + i)->y && y <= (arr + i)->y + 20 && (arr + i)->touched == 0)
        {
            temp.touched = 1;
            temp.index = i;
            return temp; // trả về bị chạm và vị trí của ô vuông
        }
    }
    temp.touched = 0;
    temp.index = -1;
    return temp; // trả về chưa bị chạm
}
// tương tự X
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
// nếu UET bị chạm hết rồi thì thắng
bool checkWin()
{
    for (int i = 0; i < countOfUET; i++)
    {
        if ((arr + i)->touched == 0)
            return 0;
    }
    return 1;
}
// game mới
void newGame()
{
    finished = 0; // chưa kết thúc
    heart = 3;    //đặt lại máu
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
