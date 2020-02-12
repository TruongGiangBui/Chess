#include<SFML/Graphics.hpp>
#include<iostream>
#include"Header.hpp"

using namespace sf;
using namespace std;

int offset=40;
RenderWindow window(VideoMode(900, 720), "Chess");
Sprite background;
Sprite button1;
Sprite button2;
Sprite button3;
Sprite button4;
Sprite Chessboard;
string history = "";
int squaresize = 80;

int status = 0;
int check = 0;
int n = -1;
int selectpiece = -1;
int turn = 1;
int game=0;

Texture backgr, b1, b2, b3, b4, chessboard, piecetex, redhighlight, bluehighlight, yellowhighlight;
chesspieces pieces[32];
Sprite highlightselect;
int board[8][8]= 
{ -1,-6,0,0,0,0,6,1,
-2,-6,0,0,0,0,6,2,
-3,-6,0,0,0,0,6,3,
-4,-6,0,0,0,0,6,4,
-5,-6,0,0,0,0,6,5,
-3,-6,0,0,0,0,6,3,
-2,-6,0,0,0,0,6,2,
-1,-6,0,0,0,0,6,1,
};
Sprite highlight[8][8];
int selectnumplayer(Event e, int status,Vector2i Pos)
{
    // chọn 1 player status=1
    // chọn 2 player status=2
    // quit status=0 đưa bàn cò về trạng thái ban đầu
    if (e.type == Event::MouseButtonPressed)
    {
        if (e.mouseButton.button == Mouse::Left)
        {
            if (status == 0)   //cua so o trang thai lua chon 
            {
                if (button1.getGlobalBounds().contains(Pos.x, Pos.y))
                {
                    status=1;
                }
                if (button2.getGlobalBounds().contains(Pos.x, Pos.y))
                {
                    status = 2;
                }
            }
            if (status != 0)
            {
                if (button3.getGlobalBounds().contains(Pos.x, Pos.y))
                {
                    status = 0;
                    reset();
                    history = "";
                    turn = 1;
                    game = 0;
                }
            }
        }
    }
    return status;
}


Vector2i tocoord(char a, char b)
{
    //chuyển đổi từ tọa độ trên bàn cờ về tọa độ x y
    int x = int(a) - 97;
    int y = 7 - int(b) + 49;
    return Vector2i(x, y);
}

string tochessnode(int a, int b)
{
    //chuyển đổi từ tọa độ Oxy về tọa độ của bàn cờ
    string s = "";
    s += char(a + 97);
    s += char(56 - b);
    return s;
}

void setup()
{

    //cài đặt value cho các quân cờ
    //cài đặt texture cho quân cờ
    //cài đặt vị trí xuất phát cho các quân cờ
    int k = 0;

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            if (board[j][i] != 0)
            {
                pieces[k].setvalue(board[j][i]);
                pieces[k].setpieceTexture(piecetex);
                pieces[k].setpieceposition(j, i);
                k++;
            }
        }

}

void reset()
{
    //cài đặt bàn cờ về trạng thái ban đầu
    int tboard[8][8] =
    { -1,-6,0,0,0,0,6,1,
    -2,-6,0,0,0,0,6,2,
    -3,-6,0,0,0,0,6,3,
    -4,-6,0,0,0,0,6,4,
    -5,-6,0,0,0,0,6,5,
    -3,-6,0,0,0,0,6,3,
    -2,-6,0,0,0,0,6,2,
    -1,-6,0,0,0,0,6,1,
    };
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            board[j][i] = tboard[j][i];
        }
    }
    setup();
}

void compmove(string s)
{

    Vector2i oldpos = tocoord(s[0], s[1]);
    Vector2i newpos = tocoord(s[2], s[3]);
    int n;
    //Kiểm tra xem tọa độ đã cho ứng với quân cờ nào
    for (int i = 0; i < 32; i++)
    {
        if (pieces[i].coordx == oldpos.x && pieces[i].coordy == oldpos.y)
        {
            n = i;
        }
    }
    //kiểm tra xem vị trí đến có quân cờ nào khác không
    //Nếu có thì loại bỏ quân cờ vị trí đó
    for (int i = 0; i < 32; i++)
    {
        if (pieces[i].coordx == newpos.x && pieces[i].coordy == newpos.y)
        {
            pieces[i].piece.setPosition(-100, -100);
            pieces[i].coordx = -1;
            pieces[i].coordy = -1;
        }
    }
    pieces[n].Move(newpos.x, newpos.y);
    //thực hiện di chuyển quân cờ sang vị trí mới
}

void checkKing()
{
    int temp = 0;
    for (int i = 0; i < 32; i++)
    {
        pieces[i].positivemove();
        for (int a = 0; a < 8; a++)
            for (int b = 0; b < 8; b++)
            {
                if (pieces[i].positivemoveboard[b][a] == 2 && board[b][a] / turn == 5)
                {
                    check = 1;
                    temp = 1;
                }
            }
    }
    if (temp == 0) check = 0;
    for (int i = 0; i < 32; i++)
    {
        pieces[i].resetpositivemove();
    }
}

void playermove(Vector2i pos)
{
    //click chuột vào quân cờ các vị trí có thể đi tới sẽ hiện ra
    //click vào 1 trong các vị trí có thể đi quân cờ sẽ di chuyển đến đó
    //selectpiece=1 quân cờ đã được chọn
    //kiểm tra đã có quân cờ nào được chọn chưa
    if (selectpiece == -1) {
        //chưa có quân cờ nào được chọn
        //kiểm tra vị trí click chuột ứng với quân cờ nào
        //kiểm tra màu quân cờ có đúng lượt đi của nó hay không
        //đánh dấu các vị trí di chuyển được của quân cờ được chọn
        //đánh dấu chỉ số quân cờ được chọn
        for (int i = 0; i < 32; i++)
            if (pieces[i].piece.getGlobalBounds().contains(pos.x, pos.y))
                if (pieces[i].getvalue() / turn > 0)
                {
                    selectpiece = 1;
                    pieces[i].positivemove();
                    highlightselect.setPosition(offset + pieces[i].coordx * squaresize, offset + pieces[i].coordy * squaresize);
                    n = i;

                }
    }
    else
        if (n >= 0) {
            //quân cờ đã được chọn
            //kiểm tra vị trí được click có nằm trong các vị trí có thể đi tới của quân cờ hay không
            //nếu thỏa mãn điều kiện thực hiện di chuyển quân cờ đến vị trí đích
            //lưu lại bước đi vào history
            //chuyển lượt cho quân màu ngược lại
            if (pieces[n].canmove((pos.x - offset) / squaresize, (pos.y - offset) / squaresize)) {
                for (int i = 0; i < 32; i++)
                {
                    if (pieces[i].coordx == (pos.x - offset) / squaresize && pieces[i].coordy == (pos.y - offset) / squaresize && i != n)
                    {

                        pieces[i].piece.setPosition(-100, -100);
                        pieces[i].coordx = -1;
                        pieces[i].coordy = -1;
                    }
                }
                history +=tochessnode(pieces[n].coordx, pieces[n].coordy);//oldpos
                pieces[n].Move((pos.x - offset) / squaresize, (pos.y - offset) / squaresize);
                history += tochessnode(pieces[n].coordx, pieces[n].coordy);//newpos
                cout << history << endl;
                // cf[n].temp();
               
                pieces[n].resetpositivemove();
                pieces[n].Promotion();

                turn = -turn;
            }
            
            selectpiece = -1;
            n = -1;
        }
       // checkKing();
}

void gameover(int a)
{
    Texture wins;
    Sprite winner;
    if (a == 1)
    {
        wins.loadFromFile("Textures//player1wins.png");
        winner.setTexture(wins);
        window.draw(winner);
    }
    if (a == -1)
    {
        wins.loadFromFile("Textures//player2wins.png");
        winner.setTexture(wins);
        window.draw(winner);
    }
}

void undo(Vector2i pos)
{
    // trở lại nước đi trước đó
    //xóa lịch sử nước đi vừa xong
    //đưa bàn cờ về trạng thái ban đầu
    //thực hiện di chuyển theo các bước trong lịch sử
    if (button4.getGlobalBounds().contains(pos.x, pos.y))
        if (history.length() >= 4)
        {
            history.erase(history.length() - 4, 4);
            cout << history << endl;
            reset();
            for (int i = 0; i < history.length(); i += 4)
            {
                compmove(history.substr(i, 4));
            }
            turn = -turn;
            checkKing();
        }
    
}

void chesspieces::setpieceposition(int x, int y)
{
    //set vị trí cho quân cờ dựa vào tọa độ x,y
    coordx = x;
    coordy = y;
    piece.setPosition(offset+squaresize * coordx, offset+squaresize * coordy);

}

void chesspieces::Move(int x, int y)
{
    // di chuyển quân cờ từ vị trí cũ sang vị trí x,y mới
    board[coordx][coordy] = 0;
    
    coordx = x;
    coordy = y;
    
    board[x][y] = value;
    piece.setPosition(offset+squaresize * coordx, offset + squaresize * coordy);
}

void chesspieces::setpieceTexture(Texture tm)
{
    //cài texture cho các quân cờ
    t = tm;
    piece.setTexture(t);
    int x = abs(value) - 1;
    int y = value > 0 ? 1 : 0;
    piece.setTextureRect(IntRect(squaresize * x, squaresize * y, squaresize, squaresize));
    piece.setPosition(offset + squaresize * coordx, offset + squaresize * coordy);
}

void chesspieces::setvalue(int val)
{
    //cài đặt value cho các quân cờ
    this->value = val;
}

int chesspieces::getvalue()
{
    // trả lại giá trị quân cờ
    return value;
}

void chesspieces::positivemoveKing(int x,int y)
{
    int movex[] = { 1,1,1,0,0,-1,-1,-1 };
    int movey[] = { 1,0,-1,1,-1,1,0,-1 };
    int color = value / abs(value);

    //kiểm tra 8 ô xung quanh ô nào quân tướng có thể tới
    //nếu là ô trống thì đánh dấu là 1
    //nếu là 1 quân đen thì đánh dấu là 2
    for (int i = 0; i < 8; i++)
    {
        if ((x + movex[i]) >= 0 && (x + movex[i]) < 8 && (y + movey[i] >= 0) && (y + movey[i] )< 8)
        {
            if(board[x+movex[i]][y+movey[i]]/color==0)
            positivemoveboard[x + movex[i]][y + movey[i]] = 1;
            if (board[x + movex[i]][y + movey[i]] / color < 0)
            positivemoveboard[x + movex[i]][y + movey[i]] = 2;
        }
    }
}

void chesspieces::positivemoveQueen(int x, int y)
{
    //các vị trí quân hậu có thể tới
    //tổng hợp của xe và tượng
    positivemoveBishop(x, y);
    positivemoveRook(x, y);
}

void chesspieces::positivemoveRook(int x, int y)
{
    //kiểm tra 2 đường ngang và dọc
    int color = value / abs(value);
    int a = x;
    int b = y;
    for (int i = b+1; i < 8; i++)
    {
        if (board[a][i] / color > 0)
        {
            break;
        }
        else if (board[a][i] / color < 0)
        {
            positivemoveboard[a][i] = 2;
            break;
        }
        else positivemoveboard[a][i] = 1;
    }
    for (int i = b - 1; i >= 0; i--)
    {
        if (board[a][i] / color > 0)
        {
            break;
        }
        else if (board[a][i] / color < 0)
        {
            positivemoveboard[a][i] = 2;
            break;
        }
        else positivemoveboard[a][i] = 1;
    }
    for (int i = a - 1; i >= 0; i--)
    {
        if (board[i][b] / color > 0)
        {
            break;
        }
        else if (board[i][b] / color < 0)
        {
            positivemoveboard[i][b] = 2;
            break;
        }
        else positivemoveboard[i][b] = 1;
    }
    for (int i = a + 1; i < 8; i++)
    {
        if (board[i][b] / color > 0)
        {
            break;
        }
        else if (board[i][b] / color < 0)
        {
            positivemoveboard[i][b] = 2;
            break;
        }
        else positivemoveboard[i][b] = 1;
    }
}

void chesspieces::positivemoveBishop(int x, int y)
{
    //kiểm tra 2 đường chéo
    int a, b;
    int color = value / abs(value);

    a = x + 1;
    b = y + 1;
    while (a < 8 && b < 8)
    {
        if (board[a][b] / color > 0)
        {
            break;
        }
        else if (board[a][b] / color < 0)
        {
            positivemoveboard[a][b] = 2;
            break;
        }
        else positivemoveboard[a][b] = 1;
        a++;
        b++;
    }
    a = x - 1;
    b = y - 1;
    while (a >=0 && b >= 0)
    {
        if (board[a][b] / color > 0)
        {
            break;
        }
        else if (board[a][b] / color < 0)
        {
            positivemoveboard[a][b] = 2;
            break;
        }
        else positivemoveboard[a][b] = 1;
        a--;
        b--;
    }
    a = x + 1;
    b = y - 1;
    while (a <8  && b >= 0)
    {
        if (board[a][b] / color > 0)
        {
            break;
        }
        else if (board[a][b] / color < 0)
        {
            positivemoveboard[a][b] = 2;
            break;
        }
        else positivemoveboard[a][b] = 1;
        a++;
        b--;
    }
    a = x - 1;
    b = y + 1;
    while (a >= 0 && b <8)
    {
        if (board[a][b] / color > 0)
        {
            break;
        }
        else if (board[a][b] / color < 0)
        {
            positivemoveboard[a][b] = 2;
            break;
        }
        else positivemoveboard[a][b] = 1;
        a--;
        b++;
    }
}

void chesspieces::positivemoveKnight(int x, int y)
{
    //kiểm tra 8 vị trí có thể đi được theo chữ L
    int movex[] = { 1,1,-1,-1,2,2,-2,-2 };
    int movey[] = { 2,-2,2,-2,1,-1,1,-1 };
    int color = value / abs(value);
    for (int i = 0; i < 8; i++)
    {
        if ((x + movex[i]) >= 0 && (x + movex[i]) < 8 && (y + movey[i] >= 0) && (y + movey[i]) < 8)
        {
            if (board[x + movex[i]][y + movey[i]] / color == 0)
                positivemoveboard[x + movex[i]][y + movey[i]] = 1;
            if (board[x + movex[i]][y + movey[i]] / color < 0)
                positivemoveboard[x + movex[i]][y + movey[i]] = 2;
        }
    }
}

void chesspieces::positivemovePawn(int x, int y)
{
    int color = value / abs(value);
    if(y==1||y==6)
    for (int i = 1; i <= 2; i++)
    {
        if (board[x][y-i*color] / color > 0)
        {
            break;
        }
        else if (board[x][y - i * color] / color < 0)
        {
            break;
        }
        else positivemoveboard[x][y - i * color] = 1;
    }
    else if (board[x][y -  color] / color == 0)
         positivemoveboard[x][y - color] = 1;
    if (board[x + 1][y - color] / color < 0) positivemoveboard[x + 1][y - color] = 2;
    if (board[x - 1][y - color] / color < 0) positivemoveboard[x - 1][y - color] = 2;
}

void chesspieces::positivemove()
{
    switch (abs(value))
    {
    case 1: {
        positivemoveRook(coordx, coordy);
        break;
    }
    case 2: {
        positivemoveKnight(coordx, coordy);
        break;
    }
    case 3: {
        positivemoveBishop(coordx, coordy);
        break;
    }
    case 4: {
        positivemoveQueen(coordx, coordy);
        break;
    }
    case 5: {
        positivemoveKing(coordx, coordy);
        break;
    }
    case 6: {
        positivemovePawn(coordx, coordy);
        break;
          }
    }
}

void chesspieces::Promotion()
{
    //phong hậu cho tốt
    if (value == 6 && coordy == 0)
    {
        this->value = 4;
        piece.setTextureRect(IntRect(squaresize * 3, squaresize , squaresize, squaresize));
        board[coordx][coordy] = value;
    }
    if (value == -6 && coordy == 7)
    {
        this->value = -4;
        piece.setTextureRect(IntRect(squaresize * 3, 0, squaresize, squaresize));
        board[coordx][coordy] = value;
    }
}

void chesspieces::temp()
{
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++)
        {
            printf("%3d ", board[j][i] );
        }
        cout << endl;
    }cout << endl;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++)
        {
            cout << positivemoveboard[j][i] << " ";
        }
        cout << endl;
    }cout << endl;
}

void chesspieces::resetpositivemove()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            positivemoveboard[i][j] = 0;
        }
   }
}

void chesspieces::displaypositivemove()
{
    //hiện thị các vị trí có thể tới khi quân cờ được chọn
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if(positivemoveboard[j][i]==1)
            highlight[j][i].setTexture(bluehighlight);
            if(positivemoveboard[j][i] == 2)
            highlight[j][i].setTexture(redhighlight);
            
            if (positivemoveboard[j][i] !=0) window.draw(highlight[j][i]);
        }
    }
}

bool chesspieces::canmove(int x, int y)
{
    //kiểm tra xem vị trí x,y có thể đi tới được không
    if (positivemoveboard[x][y] != 0)
        return 1;
    else return 0;
}
