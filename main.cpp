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
    S.push_back(c);
    S.push_back(d);
    S.push_back(e);

    EdgePair *ep;
    Delaunay *f;
    ep = f->compute(S);

    cout << "I WANNA BE " << endl;

    /*cout << "EdgePair left x" << ep->left->group->e[0]->data->x << endl;
    cout << "EdgePair left y" << ep->left->group->e[0]->data->y << endl;
    cout << "EdgePair right x" << ep->right->group->e[2]->data->x << endl;
    cout << "EdgePair right y" << ep->right->group->e[2]->data->y << endl;*/

    return 0;
}
