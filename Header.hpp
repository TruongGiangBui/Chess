#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>


using namespace sf;
using namespace std;
extern int offset;
extern RenderWindow window;   // cửa sổ game
extern Sprite background;        
extern Sprite button1;       //1 player game
extern Sprite button2;       //2 player game
extern Sprite button3;       //quit
extern Sprite button4;       //undo
extern Sprite Chessboard;
extern int squaresize;
extern string history;       //lich su cua tran dau
extern Texture backgr, b1, b2, b3, b4, chessboard, piecetex, redhighlight, bluehighlight, yellowhighlight;


extern int status;   
//trang thai
// 1: 1player
// 2: 2player
// 0: select window
extern int check;
extern int n;
// danh dau quan co duoc chon
extern int selectpiece;
// quan co da chon hay chua
extern int turn;
//luot choi
//1 trắng
//-1 đen
extern int game;
//game=1 player 1 wins
//game=-1 player 2 wins
extern int board[8][8];   //bảng  lưu vị trí các quân cờ
extern Sprite highlight[8][8];   //đánh dấu 
extern Sprite highlightselect;   //đánh dấu vị trí được chọn


int selectnumplayer(Event e, int status, Vector2i Pos);// lựa chọn số người chơi

void setup(); //khởi tạo vị trí banđầu cho các quân cờ
void reset(); //reset các quân cờ về vị trí ban đầu

string tochessnode(int, int);  //đổi từ tọa độ số sang thành tọa độ bàn cờ
Vector2i tocoord(char a, char b);  //đổi tọa độ bàn cờ sang tọa độ số


void compmove(string s);    //di chuyển từ tọa độ này sang tọa độ kia 
void playermove(Vector2i pos);  //di chuyển quân cờ từ vị trí này sang vị trí kia(click chuột)
void undo(Vector2i pos);      //trở lại nước đi trước đó

void checkKing();          //kiểm tra xem có chiếu tướng hay không
void gameover(int a);      //kết thúc game
class chesspieces
{
private:
	int value; // value: giá trị quân cờ
	//class quân cờ
public:
	Sprite piece;      
	Texture t; 
	
	// value=1: Castle
	// value=2: Knight
	// value=3: Bishop
	// value=4: Queen
	// value=5: King
	// value=6: Pawn
	// value>0: White  value<0 Black
	int coordx, coordy;  // tọa độ hiện tại quân cờ
	int positivemoveboard[8][8] =// bảng lưu vị trí có thể đến của quân cờ trong nước đi
	{ 0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0 };
	
	void setpieceposition(int x, int y); //đặt quân cờ vào tọa độ x y
	void setpieceTexture(Texture);       //cài đặt texture cho quân cờ
	void setvalue(int);                  //cài giá trị cho quân cờ
	int getvalue();
	void Move(int x,int y);              //di chuyển từ tọa độ cũ sang tọa độ mới
	void positivemove();      //khả năng di chuyển của quân cờ 
	void positivemoveKing(int x,int y);   //khả năng di chuyển của King
	void positivemoveQueen(int x, int y); //khả năng di chuyển của Queen
	void positivemoveBishop(int x, int y);  //khả năng di chuyển của Bishop
	void positivemoveKnight(int x, int y);  //khả năng di chuyển của Knight
	void positivemoveRook(int x, int y);   //khả năng di chuyển của Rook
	void positivemovePawn(int x, int y);   //khả năng di chuyển của Pawn               
	void Promotion();                      //Phong hậu
	bool canmove(int x, int y);            //Kiểm tra xem vị trí x,y có đến được không
	void resetpositivemove();              //reset lại bảng các vị trí có thể đi
	void displaypositivemove();            //Hiển thị các vị trí có thể đi của quân cờ
	void temp();
};

extern chesspieces pieces[32];   //mảng các quân cờ