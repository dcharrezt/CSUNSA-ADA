#include <iostream>

#include "delaunay.h"

using namespace std;

int main(int argc, char *argv[])
{
    points S,S2;
    Point* a = new Point(0,0);
    Point* b = new Point(1,2);
    Point* c = new Point(2,1);
    Point* d = new Point(3,4);
    Point* e = new Point(4,3);

    S.push_back(a);
    S.push_back(b);
    S2.push_back(c);
    S2.push_back(d);
    S2.push_back(e);

    EdgePair *ep;
    Delaunay *f;
    ep = f->compute(S2);

    cout << ep->left->data->x << endl;
    cout << ep->left->data->y << endl;
    cout << ep->right->data->x << endl;
    cout << ep->right->data->x << endl;

    return 0;
}
