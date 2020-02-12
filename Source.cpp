
#include<SFML/Graphics.hpp>
#include<iostream>
#include"Header.hpp"

using namespace sf;
using namespace std;






int main()
{
	
	backgr.loadFromFile("Textures//background.png");
	b1.loadFromFile("Textures//Button1.png");
	b2.loadFromFile("Textures//Button2.png");
	b3.loadFromFile("Textures//Button3.png");
	b4.loadFromFile("Textures//Button4.png");
	chessboard.loadFromFile("Textures//chessboard.png");
	redhighlight.loadFromFile("Textures//redhighlight.png");
	bluehighlight.loadFromFile("Textures//bluehighlight.png");
	yellowhighlight.loadFromFile("Textures//yellowhighlight.png");
	piecetex.loadFromFile("Textures//pieces.png");

	background.setTexture(backgr);
	button1.setTexture(b1);
	button2.setTexture(b2);
	button3.setTexture(b3);
	button4.setTexture(b4);
	Chessboard.setTexture(chessboard);
	highlightselect.setTexture(yellowhighlight);
	

	button1.setPosition(90, 90);
	button2.setPosition(90, 270);
	button3.setPosition(760, 40);
	button4.setPosition(760, 120);

	
	Sprite checkboard(redhighlight);
	

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			highlight[j][i].setPosition(offset+j * squaresize,offset+ i * squaresize);

	setup();
	
	while (window.isOpen())
	{
		
		Event e;
		
		Vector2i pos = Mouse::getPosition(window);
		
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();
			status = selectnumplayer(e, status, pos);
			
			if (status == 2)
			{
				
				if (e.type == Event::MouseButtonPressed)
					if (e.mouseButton.button == Mouse::Left)
					{
						playermove(pos);
						if (pieces[4].coordx < 0)
						{
							game=1;
						}
						if (pieces[28].coordx < 0)
						{
							game=-1;
						}
						if(selectpiece==-1) checkKing();
						undo(pos);
					}
			}
			if (status == 1)
			{
				if (e.type == Event::MouseButtonPressed)
					if (e.mouseButton.button == Mouse::Left)
					{
						playermove(pos);
						undo(pos);
					}
			}
	

		}
		if (status == 0) {
			window.clear();
			window.draw(background);
			window.draw(button1);
			window.draw(button2);
			window.display();
		}
		if (status !=0)
		{
			window.clear();
			window.draw(Chessboard);
			if (selectpiece == 1) {
				pieces[n].displaypositivemove();
				window.draw(highlightselect);
			};
			if (check == 1)
			{
				if (turn == -1)
					checkboard.setPosition(pieces[4].coordx * squaresize + offset, pieces[4].coordy * squaresize + offset);
				
				if(turn==1)
					checkboard.setPosition(pieces[28].coordx * squaresize + offset, pieces[28].coordy * squaresize + offset);

				window.draw(checkboard);
			}
			for (int i = 0; i < 32; i++)
				window.draw(pieces[i].piece);
			if (game != 0)
			{
				gameover(game);
			}
			window.draw(button3);
			window.draw(button4);
			window.display();
		}
		
	}
}

