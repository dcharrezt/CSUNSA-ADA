#ifndef POINT_H
#define POINT_H

typedef int II;

class Point
{
public:
    II x;
    II y;

    Point();
    Point(II x, II y);
    Point operator+(const Point& b);
    Point operator-(const Point& b);
    bool operator==(const Point& b);
    bool isCCW(Point* a, Point* b, Point* c);
    bool isInCircle(Point* a, Point* b, Point* c, Point* d);
};

Point::Point(){
    this->x = 0;
    this->y = 0;
}


Point::Point(II x, II y){
    this->x = x;
    this->y = y;
}

Point Point::operator+(const Point& b){
    Point p;
    p.x = this->x + b.x;
    p.y = this->y + b.y;
    return p;
}

Point Point::operator-(const Point& b){
    Point p;
    p.x = this->x - b.x;
    p.y = this->y - b.y;
    return p;
}

bool Point::operator==(const Point& b){
    return (this->x == b.x && this->y == b.y) ? true : false;
}

bool Point::isCCW(Point* a, Point* b, Point* c){
    // Check if the sequence of vertices is counter-clockwise.
    // Assumes the vertices are in cartesian coordinates.
    // | Ax Ay 1 |
    // | Bx By 1 | > 0 ?
    // | Cx Cy 1 |

    return (a->x*b->y + a->y*c->x + b->x*c->y - a->x*c->y - a->y*b->x - b->y*c->x) > 0;
}

bool Point::isInCircle(Point* a, Point* b, Point* c, Point* d){
    // Check if the Point D is inside the circle defined by A, B, C.
    // | Ax Ay (Ax^2 + Ay^2) 1 |
    // | Bx By (Bx^2 + By^2) 1 | > 0 && isCCCW(A,B,C)
    // | Cx Cy (Cx^2 + Cy^2) 1 |     OR
    // | Dx Dy (Dx^2 + Dy^2) 1 | < 0 && !isCCW(A,B,C)

    II af, bf, cf, df, detA11, detA12, detA13, detA14, det;
    af = a->x*a->x + a->y*a->y;
    bf = b->x*b->x + b->y*b->y;
    cf = c->x*c->x + c->y*c->y;
    df = d->x*d->x + d->y*d->y;

    detA11 = b->y*cf  + bf*d->y  + c->y*df  - b->y*df  - bf*c->y  - cf*d->y;
    detA12 = b->x*cf  + bf*d->x  + c->x*df  - b->x*df  - bf*c->x  - cf*d->x;
    detA13 = b->x*c->y + b->y*d->x + c->x*d->y - b->x*d->y - b->y*c->x - c->y*d->x;
    detA14 = b->x*c->y*df + b->y*cf*d->x + bf*c->x*d->y
          - b->x*cf*d->y - b->y*c->x*df - bf*c->y*d->x;

    det = a->x*detA11 - a->y*detA12 + af*detA13 - detA14;

    return (isCCW(a,b,c) && det>0) || (!isCCW(a,b,c) && det<0);
}

#endif // POINT_H
