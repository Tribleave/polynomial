#pragma once
#ifndef POLYOMAIL_H
#define POLYNOMAIL_H
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

const double eps = 1e-6;

struct Term {
	double coef;
	int exp;
	Term * link;
	Term(double c, int e, Term* next = NULL)
	{
		coef = c; exp = e; link = next;
	}
	Term * InsertAfter(double c, int e);
	friend ostream& operator << (ostream&, const Term &);
};
class Polynomial {
public:
	Polynomial() { first = new Term(0, -1); maxExp = -1; }
	Polynomial(Polynomial& R);
	int maxOrder() { return maxExp; }
	Term* getHead() const { return first; }
private:
	Term * first;
	int maxExp;
	friend ostream& operator << (ostream&, const Polynomial&);
	friend istream& operator >> (istream&, Polynomial&);
	friend Polynomial operator +(Polynomial&, Polynomial&);
	friend Polynomial operator * (Polynomial&, Polynomial&);
};

Term* Term::InsertAfter(double c, int e){
	link = new Term(c, e, link);
	return link;
}

ostream& operator << (ostream& out, const Term& x) {
	if (x.coef == 0.0 )return out;
	else if (fabs(x.coef + 1) < eps)	out << "-";
	else if (fabs(x.coef - 1) >= eps)	out << x.coef;
	switch (x.exp) {
		case 0:
			break;
		case 1: out << "X";
			break;
		default: out << "X^" << x.exp;
			break;
	}
	return out;
}


Polynomial::Polynomial(Polynomial& R){
	first = new Term(0, -1);
	maxExp = R.maxOrder();
	Term * destptr = first, *srcptr = R.getHead()->link;
	while (srcptr != NULL) {
		destptr->InsertAfter(srcptr->coef, srcptr->exp);
		srcptr = srcptr->link;
		destptr = destptr->link;
	}
}

istream& operator >> (istream& in, Polynomial& x) {
	Term *rear = x.getHead();
	int c, e, mExp;
	while (in >> c >> e, c || e != -1) {
		if (fabs(c) < 0)	continue;
		rear = rear->InsertAfter(c, e);
		mExp = max(mExp, e);
	}
	x.maxExp = mExp;
	return in;
}

ostream& operator << (ostream& out, Polynomial& x) {
	Term *current = x.getHead()->link;
	bool h = true;
	if (current == NULL)
		printf("0");
	else
		while (current != NULL) {
			if (h == false && current->coef > 0.0)	out << "+";
			h = false;
			out << *current;
			current = current->link;
		}
	out << endl;
	return out;
}

Polynomial operator +(Polynomial& A, Polynomial& B) {
	Term *pa, *pb, *pc, *p;
	Polynomial C;
	pc = C.first;
	double temp;
	pa = A.getHead()->link;
	pb = B.getHead()->link;
	while (pa != NULL && pb != NULL) {
		if (pa->exp == pb->exp) {
			temp = pa->coef + pb->coef;
			if (fabs(temp) > eps)
				pc = pc->InsertAfter(temp, pa->exp);
			pa = pa->link;
			pb = pb->link;
		}
		else if (pa->exp > pb->exp) {
			pc = pc->InsertAfter(pa->coef, pa->exp);
			pa = pa->link;
		}
		else {
			pc = pc->InsertAfter(pb->coef, pb->exp);
			pb = pb->link;
		}
	}
	if (pa != NULL)	p = pa;
	else p = pb;
	while (p != NULL) {
		pc = pc->InsertAfter(p->coef, p->exp);
		p = p->link;
	}
	pc->link = NULL;
	if (C.first->link != NULL)
		C.maxExp = C.first->link->exp;
	else
		C.maxExp = -1;
	return C;
}

Polynomial operator*(Polynomial &A, Polynomial &B)
{
	Term *pa, *pb, *pc;
	Polynomial C;
	pc = C.getHead();
	if (A.maxOrder() != -1 || B.maxOrder() != -1) {
		C.maxExp = A.maxOrder() + B.maxOrder();
		double *result = new double[C.maxOrder() + 1];
		for (int i = 0; i <= C.maxOrder(); i++)
			result[i] = 0.0;
		pa = A.getHead()->link;
		while (pa != NULL) {
			pb = B.getHead()->link;
			while (pb != NULL) {
				int k = pa->exp + pb->exp;
				result[k] += pa->coef * pb->coef;
				pb = pb->link;
			}
			pa = pa->link;
		}
		for (int i = C.maxOrder(); i >= 0; i--)
			if (fabs(result[i]) >= eps)
				pc = pc->InsertAfter(result[i], i);
		delete[] result;
	}
	pc->link = NULL;
	return C;
}
#endif