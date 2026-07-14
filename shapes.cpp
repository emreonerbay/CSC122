#include <iostream>
#include <cmath>
 
using namespace std;
 
class Shape {
protected:
	string name;
public:
	Shape(string n) {
		name = n;
	}
 
	//step 1 just had these return 0, but step 3 says to make shape abstract
	//so theyre pure virtual now instead
	virtual double getArea() = 0;
	virtual double getPerimeter() = 0;
 
	string getName() {
		return name;
	}
};
 
class Circle : public Shape {
private:
	double radius;
public:
	Circle(double r) : Shape("Circle") {
		radius = r;
	}
 
	double getArea() {
		double pi = 3.14159;
		return pi * radius * radius;
	}
 
	double getPerimeter() {
		double pi = 3.14159;
		return 2 * pi * radius;
	}
};
 
class Rectangle : public Shape {
protected:
	double width;
	double height;
public:
	Rectangle(double w, double h) : Shape("Rectangle") {
		width = w;
		height = h;
	}
 
	double getArea() {
		return width * height;
	}
 
	double getPerimeter() {
		return 2 * (width + height);
	}
};
 
class RightTriangle : public Shape {
protected:
	double base;
	double height;
public:
	RightTriangle(double b, double h) : Shape("Right Triangle") {
		base = b;
		height = h;
	}
 
	double getArea() {
		return (base * height) / 2;
	}
 
	double getPerimeter() {
		double hyp = sqrt(base * base + height * height);
		return base + height + hyp;
	}
};
 
//square is just a rectangle where width and height are the same
//dont really need to rewrite getArea/getPerimeter, the rectangle ones just work
class Square : public Rectangle {
public:
	Square(double s) : Rectangle(s, s) {
		name = "Square"; //without this it kept printing "Rectangle" for squares too
	}
};
 
//same idea here, isoceles right triangle is just a right triangle with both legs equal
class IsocelesRightTriangle : public RightTriangle {
public:
	IsocelesRightTriangle(double leg) : RightTriangle(leg, leg) {
		name = "Isoceles Right Triangle"; //same deal as square, otherwise it says Right Triangle
	}
};
 
void printAreaToScreen(Shape *s) {
	cout << "The " << s->getName() << " has an area of " << s->getArea() << endl;
}
 
int main() {
 
	Circle c = Circle(5);
	Rectangle r = Rectangle(4, 6);
	RightTriangle t = RightTriangle(3, 4);
	Square sq = Square(4);
	IsocelesRightTriangle it = IsocelesRightTriangle(5);
 
	printAreaToScreen(&c);
	printAreaToScreen(&r);
	printAreaToScreen(&t);
	printAreaToScreen(&sq);
	printAreaToScreen(&it);
 
 
}
 
