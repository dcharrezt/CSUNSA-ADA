#ifndef QUADEDGE_H
#define QUADEDGE_H

#include "delaunay.h"
#include "point.h"

class QuadEdge;

class Quad{
public:
    Quad(){}
    QuadEdge* e[4];
};

class QuadEdge{
public:
    II r;
    Point* data;
    QuadEdge* next;
    Quad* group;

    QuadEdge();
    QuadEdge(int r, Point* data, Quad *group);

    QuadEdge* MakeEdge(Point* d0, Point* d1, Point* d2, Point* d3);

    void splice(QuadEdge *b);

    bool isPrimal()    {return r%2 == 0;}
    bool isCanonical() {return r == 0;}

    QuadEdge* canonical() {return group->e[0];}

    QuadEdge* Rot()    {cout<< this->r <<endl; return group->e[(r+1)%4];}
    QuadEdge* Onext()  {return next;}

    QuadEdge* Sym()    {cout << this->r <<endl; return Rot()->Rot();}
    QuadEdge* RotInv() {return Rot()->Rot()->Rot();}
    QuadEdge* Oprev()  {return Rot()->Onext()->Rot();}

    QuadEdge* Lnext() {return RotInv()->Onext()->Rot();}
    QuadEdge* Rnext() {return Rot()->Onext()->RotInv();}
    QuadEdge* Dnext() {return Sym()->Onext()->Sym();}

    QuadEdge* Lprev() {return Onext()->Sym();}
    QuadEdge* Rprev() {return Sym()->Onext();}
    QuadEdge* Dprev() {return RotInv()->Onext()->RotInv();}
};

QuadEdge::QuadEdge(){
}

QuadEdge::QuadEdge(int r, Point* data, Quad* group) {
  this->r = r;  this->data = data;  this->group = group;
}

QuadEdge* QuadEdge::MakeEdge(Point* d0, Point* d1, Point* d2, Point* d3) {
    Quad* g = new Quad();
    QuadEdge *e0, *e1, *e2, *e3;
    e0 = new QuadEdge(0, d0, g);
    e1 = new QuadEdge(1, d1, g);
    e2 = new QuadEdge(2, d2, g);
    e3 = new QuadEdge(3, d3, g);
    g->e[0] = e0;  g->e[1] = e1;  g->e[2] = e2;  g->e[3] = e3;
    e0->next = e0;
    e1->next = e3;
    e2->next = e2;
    e3->next = e1;
    cout << "e0" << e0->r << "      " << &(e0->r) <<endl;
    cout << "e1" << e1->r << "      " << &(e1->r) <<endl;
    cout << "e2" << e2->r << "      " << &(e2->r) <<endl;
    cout << "e3" << e3->r << "      " << &(e3->r) <<endl;
    return e0;
}

void QuadEdge::splice(QuadEdge* b) {

    cout << "splice" << endl;

    if (isPrimal()&&!b->isPrimal() || !isPrimal()&&b->isPrimal()) {
        cout << "f" << endl;
    }

    cout << "splice" << endl;
    QuadEdge *alpha, *beta, *temp;

    alpha = Onext()->Rot();
    beta = b->Onext()->Rot();

    // Swap(a.Onext(), b.Onext())
    temp = next;  next = b->next;  b->next = temp;

    // Swap(alpha.Onext(), beta.Onext())
    temp = alpha->next;  alpha->next = beta->next;  beta->next = temp;
}



#endif // QUADEDGE_H
