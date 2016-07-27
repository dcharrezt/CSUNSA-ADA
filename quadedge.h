#ifndef QUADEDGE_H
#define QUADEDGE_H

#include "delaunay.h"
#include "point.h"

class QuadEdge;

class Quad{
public:
    Quad(){}

    QuadEdge *e[4];
};

class QuadEdge{
public:
    II r;
    Point* data;
    QuadEdge* next;
    Quad* group;

    QuadEdge();
    QuadEdge(int r, Point* data);
    QuadEdge(QuadEdge* q);

    QuadEdge* MakeEdge(Point* d0, Point* d1, Point* d2, Point* d3);

    void splice(QuadEdge *b);

    bool isPrimal()    {return r%2 == 0;}
    bool isCanonical() {return r == 0;}

    QuadEdge* canonical() {return group->e[0];}

    QuadEdge* Rot()    {return (r < 3) ? this->group->e[r+1] : this->group->e[r-3];}
    QuadEdge* Onext()  {return next;}

    QuadEdge* Sym()    {return Rot()->Rot();}
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

QuadEdge::QuadEdge(int r, Point* data) {
    this->r = r;
    this->data = data;
}

QuadEdge::QuadEdge(QuadEdge *q)
{
    this->r = q->r;
    this->data = q->data;
    this->group = q->group;
    this->next = q->next;
}

QuadEdge* QuadEdge::MakeEdge(Point* d0, Point* d1, Point* d2, Point* d3) {
    //Quad* g = new Quad();
    //QuadEdge *e0, *e1, *e2, *e3;
    this->group = new Quad();
    this->group->e[0] = new QuadEdge(0, d0);
    cout << "cat " << endl;
    this->group->e[1] = new QuadEdge(1, d1);
    cout << "cat " << endl;
    this->group->e[2] = new QuadEdge(2, d2);
    cout << "cat " << endl;
    this->group->e[3] = new QuadEdge(3, d3);
    cout << "cat " << endl;

    //this->group->e[1] = e1;  this->group->e[2] = e2;  this->group->e[3] = e3;

    this->group->e[0]->group = this->group;
    this->group->e[1]->group = this->group;
    this->group->e[2]->group = this->group;
    this->group->e[3]->group = this->group;

    this->group->e[0]->next = this;
    this->group->e[1]->next = this->group->e[3];
    this->group->e[2]->next = this->group->e[2];
    this->group->e[3]->next = this->group->e[1];

   /* e0->next = e0;
    e1->next = e3;
    e2->next = e2;
    e3->next = e1;*/
    //cout << "g e 0 " << g->e[0]->r << endl;
    //cout << "e0 " << e0->r << endl;
    /*cout << "e0 " << &e0 << "\t" << e0->r << "\t" << &(e0->r) << "e0 " <<e0<<endl;
    cout << "e1 " << &e1 << "\t" << e1->r << "\t" << &(e1->r) <<"e1 " <<e1<<endl;
    cout << "e2 " << &e2 << "\t" << e2->r << "\t" << &(e2->r) <<"e2 " <<e2<<endl;
    cout << "e3 " << &e3 << "\t" << e3->r << "\t" << &(e3->r) <<"e3 " <<e3<<endl;*/
    return group->e[0];
}

void QuadEdge::splice(QuadEdge* b) {

    cout << "splice" << endl;

    if (isPrimal()&&!b->isPrimal() || !isPrimal()&&b->isPrimal()) {
        cout << "f" << endl;
    }

    cout << "splice2" << endl;
    QuadEdge *alpha, *beta, *temp;

    alpha = Onext()->Rot();



    beta = b->group->e[0]->Onext()->Rot();

    cout << "splice3" << endl;
    cout << beta->r << endl;

    // Swap(a.Onext(), b.Onext())
    temp = next;  next = b->next;  b->next = temp;

    // Swap(alpha.Onext(), beta.Onext())
    temp = alpha->next;  alpha->next = beta->next;  beta->next = temp;
}



#endif // QUADEDGE_H
