#include <iostream>
#include <cmath>
using namespace std;

class Point {
private:
    double x;
    double y;
public:
    // constructor
    Point(double x_value, double y_value) {
        x = x_value;
        y = y_value;
    }

    // distance between two points
    double operator-(Point other) {
        double x_diff = x - other.x;
        double y_diff = y - other.y;
        double distance = sqrt((x_diff * x_diff) + (y_diff * y_diff));
        return distance;
    }

    bool operator==(Point other) {
        if (x == other.x && y == other.y) {
            return true;
        }
        else {
            return false;
        }
    }

    bool operator!=(Point other) {
        if (x != other.x || y != other.y) {
            return true;
        }
        else {
            return false;
        }
    }

    // midpoint between two points
    Point operator/(Point other) {
        double mid_x = (x + other.x) / 2;
        double mid_y = (y + other.y) / 2;
        Point midpoint(mid_x, mid_y);
        return midpoint;
    }

    // multiply point by a number
    Point operator*(double num) {
        double new_x = x * num;
        double new_y = y * num;
        Point result(new_x, new_y);
        return result;
    }

    // 0 returns x, 1 returns y
    double operator[](int index) {
        if (index == 0) {
            return x;
        }
        else {
            return y;
        }
    }

    // prefix ++ increments x
    Point operator++() {
        x++;
        Point result(x, y);
        return result;
    }

    // postfix ++ increments y
    Point operator++(int) {
        y++;
        Point result(x, y);
        return result;
    }

    // prefix -- decrements x
    Point operator--() {
        x--;
        Point result(x, y);
        return result;
    }

    // postfix -- decrements y
    Point operator--(int) {
        y--;
        Point result(x, y);
        return result;
    }

    // getters
    double getX() { return x; }
    double getY() { return y; }

    // setters
    void setX(double x_value) { x = x_value; }
    void setY(double y_value) { y = y_value; }
};

// print point in (x, y) format
ostream& operator<<(ostream& out, Point p) {
    out << "(" << p.getX() << ", " << p.getY() << ")";
    return out;
}

// read point from input in (x, y) format
istream& operator>>(istream& in, Point& p) {
    char c;
    double x_val, y_val;
    in >> c >> x_val >> c >> y_val >> c;
    p.setX(x_val);
    p.setY(y_val);
    return in;
}

int main() {
    Point p1(3.2, 9.8);
    Point p2(5.5, -1.2);
    Point p3(3.2, 9.8);

    // program output
    cout << "Distance: " << p1 - p2 << endl;
    cout << "p1 == p2: " << (p1 == p2) << endl;
    cout << "p1 != p2: " << (p1 != p2) << endl;
    cout << "Midpoint: " << p1 / p2 << endl;
    cout << "p1 == p3: " << (p1 == p3) << endl;
    cout << "p1 != p3: " << (p1 != p3) << endl;
    cout << "p1 * 2: " << p1 * 2 << endl;
    cout << "p1 * 2.0: " << p1 * 2.0 << endl;
    cout << "p1 * -3: " << p1 * -3 << endl;
    cout << "p1[0]: " << p1[0] << endl;
    cout << "p1[1]: " << p1[1] << endl;

    // test ++ and --
    ++p1;
    cout << "After ++p1: " << p1 << endl;
    p1++;
    cout << "After p1++: " << p1 << endl;
    --p1;
    cout << "After --p1: " << p1 << endl;
    p1--;
    cout << "After p1--: " << p1 << endl;

    // test >>
    Point p4(0, 0);
    cout << "Enter a point (x, y): ";
    cin >> p4;
    cout << "You entered: " << p4 << endl;

    return 0;
}