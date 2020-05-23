#include <SFML/Graphics.hpp>
#include <iostream>

#define SCR_X 800
#define SCR_Y 800

const double PI = 3.141592653589793238463;


using namespace sf;

class Object : public sf::Transformable, public sf::Drawable
{
private:
	VertexArray lines;
public:
	float X1, X2, Y1, Y2;
	Object(float x1, float y1, float x2, float y2)//for line
	{
		X1 = x1;
		Y1 = y1;
		X2 = x2;
		Y2 = y2;
		lines = VertexArray(Lines, 2);
		lines[0].position = Vector2f(X1, Y1);
		lines[0].color = Color::White;
		lines[1].position = Vector2f(X2, Y2);
		lines[1].color = Color::White;
	}


	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
		states.transform *= getTransform();
		target.draw(lines, states);
	}
};

std::vector<Object> map;
std::vector<Vector2f> points;
int point_count=0;




class ray : public sf::Transformable, public sf::Drawable
{
private:
	float X1, X2, Y1, Y2;
	VertexArray lines;
		
public:
	float distans;
	bool ray_work=true;
	ray(float x,float y,float angle)//angle 0-360
	{
		X1 = x;
		Y1 = y;
		X2 = x + 2000*cos(angle*PI / 180);
		Y2 = y + 2000*sin(angle*PI / 180);
		lines = VertexArray(Lines, 2);
		lines[0].position = Vector2f(X1, Y1);
		lines[0].color = Color::White;
		lines[1].position = Vector2f(X2, Y2);
		lines[1].color = Color::White;

		for (Object &obj : map)
		{
			float t = ((X1 - obj.X1)*(obj.Y1 - obj.Y2) - (Y1 - obj.Y1)*(obj.X1 - obj.X2)) /
				((X1 - X2)*(obj.Y1 - obj.Y2) - (Y1 - Y2)*(obj.X1 - obj.X2));
			float u = ((Y1 - Y2)*(X1 - obj.X1) - (X1 - X2)*(Y1 - obj.Y1)) /
				((X1 - X2)*(obj.Y1 - obj.Y2) - (Y1 - Y2)*(obj.X1 - obj.X2));
			if (u >= 0 && u <= 1 && t > 0)
			{
				float nX = X1 + t * (X2 - X1);
				float nY = Y1 + t * (Y2 - Y1);

				float rad1 = (X1 - X2)*(X1 - X2) + (Y1 - Y2)*(Y1 - Y2);
				float rad2 = (X1 - nX)*(X1 - nX) + (Y1 - nY)*(Y1 - nY);


				if (rad1 > rad2) {
					X2 = nX;
					Y2 = nY;
					lines[1].color = Color::Red;
					lines[1].position = Vector2f(X2, Y2);
				distans = rad2;
				}
			}
		}

	}
	ray(float x1, float y1, float x2, float y2)
	{
		
		X1 = x1;
		Y1 = y1;
		X2 = x2;
		Y2 = y2;
		lines = VertexArray(Lines, 2);
		lines[0].position = Vector2f(X1, Y1);
		lines[0].color = Color::White;

		lines[1].position = Vector2f(X2, Y2);
		lines[1].color = Color::White;
		
		float nX = -1, nY = -1;
		for (Object &obj : map)
	//	if(!(myobj==&obj))
		{
			float t = ((X1 - obj.X1)*(obj.Y1 - obj.Y2) - (Y1 - obj.Y1)*(obj.X1 - obj.X2)) /
				((X1 - X2)*(obj.Y1 - obj.Y2) - (Y1 - Y2)*(obj.X1 - obj.X2));
			float u = ((Y1 - Y2)*(X1 - obj.X1) - (X1 - X2)*(Y1 - obj.Y1)) /
				((X1 - X2)*(obj.Y1 - obj.Y2) - (Y1 - Y2)*(obj.X1 - obj.X2));
			if (u >= 0 && u <= 1 && t > 0)
			{
				float tnX = X1 + t * (X2 - X1);
				float tnY = Y1 + t * (Y2 - Y1);

				float rad1 = (X1 - nX)* (X1 - nX)  + (Y1 - nY)* (Y1 - nY);
				float rad2 = (X1 - tnX)*(X1 - tnX) + (Y1 - tnY)*(Y1 - tnY);


				if (rad1 > rad2||(nX==-1&&nY==-1)) {
					nX = tnX;
					nY = tnY;
				}
			}
		}
		
		float rad1 = (X1 - X2)* (X1 - X2) + (Y1 - Y2)* (Y1 - Y2);
		float rad2 = (X1 - nX)*(X1 - nX) + (Y1 - nY)*(Y1 - nY);
		if (rad1<=rad2) {
		

			points.push_back(Vector2f(X2, Y2));
			point_count++;
		}
		else
		{
			ray_work = false;
		}
		
		if (nX == -1 && nY == -1) { ray_work = true; nX = X2; nY = Y2; }

		if (ray_work) {
			points.push_back(Vector2f(nX, nY));
			point_count++;
		}
		lines[1].color = Color::Red;
		lines[1].position = Vector2f(nX, nY);
	
	}
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
		states.transform *= getTransform();
		target.draw(lines, states);
	}
};

std::vector<ray> raycast;


int main()
{
	RenderWindow window(VideoMode(SCR_X, SCR_Y), "map");
	RenderWindow window2(VideoMode(SCR_X, SCR_Y), "game");



	map.push_back(Object(10, 10, 10, 790));
	map.push_back(Object(10, 10, 790, 10));
	map.push_back(Object(790, 790, 10, 790));
	map.push_back(Object(790, 790, 790, 10));

	srand((unsigned int)time(NULL));
	//srand(25256764675);
	float x1 = rand() % 700 + 50, y1 = rand() % 700 + 50, x2 = x1, y2 = y1;
	for (int i = 0; i < 30; i++) 
	map.push_back(Object(x1, y1, (x1 = x2, x2 = rand() % 700 + 50), (y1 = y2, y2 = rand() % 700 + 50)));



	float pX = 500, pY=150;
	float pAngle=0;
		Clock clc;
		float time=0;
		int fcount=0;

	std::vector <RectangleShape> scrnlines;


	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		Event event2;
		while (window2.pollEvent(event2))
		{
			if (event2.type == Event::Closed)
				window2.close();
		}


		if (Keyboard::isKeyPressed(Keyboard::Left))pAngle -= 0.2;
		if (Keyboard::isKeyPressed(Keyboard::Right))pAngle += 0.2;
		if (Keyboard::isKeyPressed(Keyboard::Up)) { pY += 0.3*sin(pAngle*PI/180);pX+= 0.3*cos(pAngle*PI / 180);	}
		if (Keyboard::isKeyPressed(Keyboard::Down)) { pY -= 0.3*sin(pAngle*PI/180); pX -= 0.3*cos(pAngle*PI / 180); }


		raycast.clear();


		int quality = 100;//кол-во лучей-1
		RectangleShape l;
		for (float i = -30; i <=30; i+=60.f/ quality)
		{
			ray r2(pX, pY,i+pAngle);
			raycast.push_back(r2);

			float hight = (500000 / (r2.distans* cos(abs(i)*PI / 180)));

			l = RectangleShape(Vector2f(SCR_X/ quality,2 * hight));
			l.setPosition(Vector2f(((i + 30) * SCR_X / 60.f)- SCR_X /(2.f* quality), SCR_Y/2 -hight));
			float col = 155*(hight/500)+100;
			if (col > 255)col = 255;
			l.setFillColor(Color::Color(col,col,col));

			scrnlines.push_back(l);
		}
		
	/*
	попытка оптимизации
	for (Object &obj : map)
		{
			ray r1(pX, pY, obj.X1, obj.Y1);
			ray r2(pX, pY, obj.X2, obj.Y2);
			if(r1.ray_work)
				raycast.push_back(r1);
			if (r2.ray_work)
				raycast.push_back(r2);
		}*/

		window.clear();
		window2.clear();

		for (RectangleShape &obj : scrnlines)
			window2.draw(obj);
		scrnlines.clear();
		for (Object &obj : map)
			window.draw(obj);
		for (ray &obj : raycast)
			window.draw(obj);
		for (Vector2f &obj : points)
		{
			CircleShape point(2.f);
			point.setFillColor(Color::Transparent);
			point.setOutlineThickness(2);
			point.setOutlineColor(Color::Green);
			point.setPosition(obj - Vector2f(2, 2));
			window.draw(point);
		}
		points.clear();
		CircleShape play(10.f);
		play.setFillColor(Color::Transparent);
		play.setOutlineThickness(-5);
		play.setOutlineColor(Color::Cyan);
		play.setPosition(Vector2f(pX, pY)-Vector2f(10,10));
		window.draw(play);

		window.display();
		window2.display();

		time += clc.getElapsedTime().asSeconds();
		fcount++;
		if (fcount > 30) {
			std::cout << "FPS:" << 1/(time/30)<<std::endl;
			time = 0;
			fcount = 0;
		}
			clc.restart();
	}
	return 0;
}
