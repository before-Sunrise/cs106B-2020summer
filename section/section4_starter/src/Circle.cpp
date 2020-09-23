#include "Circle.h"
using namespace std;
#include "testing/SimpleTest.h"
#include "gmath.h"
#include <sstream>

/**
 constructs a new circle with the given radius
 */
Circle::Circle(double r) {
    this->r = r;
}


/**
 *returns the area occupied by the circle
 */
double Circle::area() const {
    return PI * r * r;
}

/**
 * returns the distance around the circle
 */
double Circle::circumference() const{
    return 2 * PI * r;
}

/**
 * returns the radius as a real number
 */
double Circle::getRadius() const{
    return r;
}

/**
 * returns a string representation such as "Circle{radius=2.5}"
 */
string Circle::toString() const{
    ostringstream oss;
    oss << r;
    return "Circle{radius=" + oss.str() + "}";
}

/* Provided Tests Below This Line */

PROVIDED_TEST("Simple Example"){
    Circle c(2.5);
    EXPECT_EQUAL(c.getRadius(), 2.5);
    EXPECT_EQUAL(c.circumference(), 2 * PI * 2.5);
    EXPECT_EQUAL(c.area(), PI * 2.5 * 2.5);
    EXPECT_EQUAL(c.toString(), "Circle{radius=2.5}");
}
