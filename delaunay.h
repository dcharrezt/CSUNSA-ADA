#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <iostream>
using namespace std;

#include "point.h"
#include "quadedge.h"
#include <vector>


typedef std::vector<Point*> points;

class EdgePair {
public:
    QuadEdge* left, *right;

    EdgePair();
    EdgePair(QuadEdge *left, QuadEdge *right);
};

EdgePair::EdgePair(){

}

EdgePair::EdgePair(QuadEdge *left, QuadEdge *right){
    this->left = left;
    this->right = right;
}

class Delaunay
{

public:
    Delaunay();

    // Return the vector with indices [start, end).
    points subVector(points p, II start, II end);

    QuadEdge* MakeQuadEdge(Point* org, Point* dst);

    Point* Org(QuadEdge* e);
    Point* Dest(QuadEdge* e);

    bool isRightOf(Point *x, QuadEdge *e);
    bool isLeftOf(Point *x, QuadEdge* e);

    QuadEdge *Connect(QuadEdge *a, QuadEdge *b);
    void DeleteEdge(QuadEdge* e);
    points preprocess(points p);
    QuadEdge *process(points p);
    EdgePair *compute(points p);
};

QuadEdge* Delaunay::MakeQuadEdge(Point* org, Point* dst) {
    QuadEdge* q;
    q->MakeEdge(org, 0, dst, 0);
    return q;
}

bool Delaunay::isRightOf(Point* x, QuadEdge* e) {
    Point *p;
    return p->isCCW(x, Dest(e), Org(e));
}

bool Delaunay::isLeftOf(Point* x, QuadEdge* e) {
    Point *p;
    return p->isCCW(x, Org(e), Dest(e));
}

Point* Delaunay::Org(QuadEdge* e){
    return e->data;
}
Point* Delaunay::Dest(QuadEdge* e){
    return e->Sym()->data;
}

QuadEdge* Delaunay::Connect(QuadEdge* a, QuadEdge* b) {
    QuadEdge* e = MakeQuadEdge(a->Sym()->data, b->data);
    e->splice(a->Lnext());
    e->Sym()->splice(b);
    return e;
}

void Delaunay::DeleteEdge(QuadEdge* e) {
    e->splice(e->Oprev());
    e->Sym()->splice(e->Sym()->Oprev());
}

//-------------------------------------------------------------------------
// Divide and conquer algorithm for computing the Delaunay diagram.

QuadEdge* Delaunay::process(points p) {
    points sites = preprocess(p);
    EdgePair* pair = compute(sites);
    return pair->left;
}

// Sort by x and y coordinates. (They should already be unique!)
points Delaunay::preprocess(points point) {
    points temp;

 return temp;
}

// Recursively compute the Delaunay triangulation of a point set S and return
// two edges, le and re, which are the counterclockwise convex hull edge out
// of the leftmost vertex and the clockwise convex hull edge out of the
// rightmost vertex, respectively.
EdgePair* Delaunay::compute(points S) {
    if (S.size() == 2) {
        QuadEdge* a = MakeQuadEdge(S[0], S[1]);
        return new EdgePair(a, a->Sym());
    }
    else if (S.size() == 3) {
        QuadEdge *a, *b;
        a = MakeQuadEdge(S[0], S[1]);
        b = MakeQuadEdge(S[1], S[2]);
        cout << "a" << a->r << "    " <<a->data->x <<"\t" << a->data->y<< endl;
        cout << "b" << b->r << "    " <<b->data->x <<"\t" << b->data->y<< endl;
        a->Sym()->splice(b);
        cout << "asd" << endl;
        // Close the triangle.
        QuadEdge* c;
        Point* t;
        if (t->isCCW(S[0], S[1], S[2])) {
            c = Connect(b, a);
            return new EdgePair(a, b->Sym());
        }
        else if (t->isCCW(S[0], S[2], S[1])) {
            c = Connect(b, a);
            return new EdgePair(c->Sym(), c);
        }
        else  // the three points are collinear
            return new EdgePair(a, b->Sym());
    }

    // At this point, we have more that three points, so divide and conquer.
    EdgePair* Lpair = compute(subVector(S, 0, S.size()/2));
    EdgePair* Rpair = compute(subVector(S, S.size()/2, S.size()));

    // Extract the edges from the pairs.
    QuadEdge* ldo = Lpair->left, *ldi = Lpair->right;
    QuadEdge* rdi = Rpair->left, *rdo = Rpair->right;

    // Compute the lower common tangent of L and R.
    while (true) {
        if (isLeftOf(Org(rdi), ldi))
            ldi = ldi->Lnext();
        else if (isRightOf(Org(ldi), rdi))
            rdi = rdi->Rprev();
        else
            break;
    }

    // Create a first cross-edge base1 from rdi.Org to ldi.Org.
    QuadEdge* base1 = Connect(rdi->Sym(), ldi);
    if (Org(ldi) == (Org(ldo)))
        ldo = base1->Sym();
    if (Org(rdi) == (Org(rdo)))
        rdo = base1;

    // This is the merge loop.
    while (true) {
        //  Locate the first L point (lcand.Dest) to be
        // encountered by the rising bubble, and delete L edges out of
        // base1.Dest that fail the circle test.
        QuadEdge* lcand = base1->Sym()->Onext();
        Point *t;
        if (isRightOf(Dest(lcand), base1)) {
            while (t->isInCircle(Dest(base1), Org(base1), Dest(lcand),
                                    Dest(lcand->Onext()))) {
                QuadEdge* temp = lcand->Onext(); DeleteEdge(lcand); lcand = temp;
            }
        }
        // Symmetrically, locate first R point to be hit, and delete R edges.
        QuadEdge* rcand = base1->Oprev();
        if (isRightOf(Dest(rcand), base1)) {
            while (t->isInCircle(Dest(base1), Org(base1), Dest(rcand),
                                    Dest(rcand->Oprev()))) {
                QuadEdge* temp = rcand->Oprev(); DeleteEdge(rcand); rcand = temp;
            }
        }
        // If both are invalid, then base1 is the upper common tangent.
        if (!isRightOf(Dest(lcand), base1) && !isRightOf(Dest(rcand), base1))
            break;

        // The next cross edge is to be connected to either lcand.Dest or
        // rcand.Dest.  If both are valid, then choose the appropriate one
        // using the InCircle test.
        if (!isRightOf(Dest(lcand), base1) ||
            (isRightOf(Dest(rcand), base1) &&
            t->isInCircle(Dest(lcand), Org(lcand), Org(rcand), Dest(rcand))))
            // Add cross edge base1 from rcand.Dest to base1.Dest.
            base1 = Connect(rcand, base1->Sym());
        else
            // Add cross edge base1 from base1.Org to lcand.Dest.
            base1 = Connect(base1->Sym(), lcand->Sym());
    }

    return new EdgePair(ldo, rdo);
}

// Return the subarray with indices [start, end).
points Delaunay::subVector(points p, int start, int end) {
    points p1;

    return p1;
}



#endif // DELAUNAY_H
