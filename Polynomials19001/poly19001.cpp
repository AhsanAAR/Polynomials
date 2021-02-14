#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

struct Polynomial {
	int degree = -1;
	int* coEfs = nullptr;
	void load(ifstream& reader) {
		reader >> degree;
		coEfs = new int[degree + 1];
		for (int i = degree; i >= 0; i--) {
			reader >> coEfs[i];
		}
	}

	int getCoef(int i) {
		if (i > degree)
			return 0;
		else
			return coEfs[i];
	}

	int eval(int x) {
		int ret = 0;
		for (int i = degree; i >= 0; i--) {
			int pow = 1;
			for (int a = 0; a < i; a++) {
				pow *= x;
			}
			ret += coEfs[i] * pow;
		}
		return ret;
	}

	void display() {
		if (degree < 0) {
			cout << 0; return;
		}
		cout << coEfs[degree];
		if (degree > 0)
			cout << 'x';
		if (degree > 1)
			cout << '^' << degree;
		for (int i = degree - 1; i >= 0; i--) {
			if (coEfs[i] == 0)
				continue;
			if (coEfs[i] > 0)
				cout << " + " << coEfs[i];
			else
				cout << " - " << coEfs[i] * -1;
			if (i > 0)
				cout << 'x';
			if (i > 1)
				cout << '^' << i;
		}
	}
};

int max(int A, int B) {
	if (A > B)
		return A;
	else
		return B;
}


void addPoly(Polynomial A, Polynomial B, Polynomial &Res) {
	Res.degree = max(A.degree, B.degree);
	delete[] Res.coEfs;
	Res.coEfs = new int[Res.degree + 1];
	bool start = false;
	int sum;
	for (int i = Res.degree; i >= 0; i--) {
		sum = A.getCoef(i) + B.getCoef(i);
		if (!start && sum == 0)
			Res.degree--;
		else
			start = true;
		Res.coEfs[i] = sum;
	}
	int* tempA = new int[Res.degree + 1];
	for (int i = 0; i <= Res.degree; i++)
		tempA[i] = Res.coEfs[i];
	delete[] Res.coEfs;
	Res.coEfs = tempA;
}

void SubtractPoly(Polynomial A, Polynomial B, Polynomial& Res) {
	Res.degree = max(A.degree, B.degree);
	delete[] Res.coEfs;
	Res.coEfs = new int[Res.degree + 1];
	bool start = false;
	int subtract;
	for (int i = Res.degree; i >= 0; i--) {
		subtract = A.getCoef(i) - B.getCoef(i);
		if (!start && subtract == 0)
			Res.degree--;
		else
			start = true;
		Res.coEfs[i] = subtract;
	}
	int* tempA = new int[Res.degree + 1];
	for (int i = 0; i <= Res.degree; i++)
		tempA[i] = Res.coEfs[i];
	delete[] Res.coEfs;
	Res.coEfs = tempA;
}

void MultiplyPoly(Polynomial A, Polynomial B, Polynomial& Res) {
	Res.degree = A.degree + B.degree;
	delete[] Res.coEfs;
	Res.coEfs = new int[Res.degree + 1];
	for (int i = 0; i <= Res.degree; i++)
		Res.coEfs[i] = 0;
	for (int iA = 0; iA <= A.degree; iA++)
		for (int iB = 0; iB <= B.degree; iB++)
			Res.coEfs[iA + iB] += A.coEfs[iA] * B.coEfs[iB];
}

void copyPoly(Polynomial &A, Polynomial &B) {
	A.degree = B.degree;
	delete[] A.coEfs;
	A.coEfs = new int[A.degree+1];
	for (int i = 0; i <= A.degree; i++)
		A.coEfs[i] = B.coEfs[i];
}

void DividePoly(Polynomial &A, Polynomial &B, Polynomial &Remainder, Polynomial& Quotient, Polynomial& divisor){
	Polynomial higher = A;
	Polynomial lower = B;
	if (lower.degree > higher.degree) {
		Polynomial T = higher;
		higher = lower;
		lower = T;
	}else if(lower.degree == higher.degree){
		for (int i = higher.degree; i >= 0; i--) {
			if (higher.coEfs[i] < lower.coEfs[i]) {
				Polynomial T = higher;
				higher = lower;
				lower = T;
				break;
			}
		}
	}
	divisor = lower;
	Polynomial dividend = higher;
	Quotient.degree = dividend.degree - divisor.degree;
	Quotient.coEfs = new int[Quotient.degree + 1];
	for (int i = 0; i <= Quotient.degree; i++)
		Quotient.coEfs[i] = 0;

	Polynomial multiple;
	do {
		Quotient.coEfs[dividend.degree - divisor.degree] = dividend.coEfs[dividend.degree] / divisor.coEfs[divisor.degree];
		Quotient.degree = dividend.degree - divisor.degree;
		MultiplyPoly(Quotient, divisor, multiple);
		SubtractPoly(dividend, multiple, Remainder);
		
		
		if (Remainder.getCoef(multiple.degree) == 0)
			if (Remainder.degree == -1 || Remainder.degree < divisor.degree)
				break;
			else
				copyPoly(dividend, Remainder);
		else
			break;
	} while (true);
	Quotient.degree = higher.degree - lower.degree;
}



int main() {
	ifstream reader("Poly.txt");
	Polynomial P1;
	P1.load(reader);
	cout << "P1 = ";
	P1.display();
	cout << endl << endl;


	Polynomial P2;
	P2.load(reader);
	cout << "P2 = ";
	P2.display();
	cout << endl << endl;


	Polynomial Res;
	addPoly(P1, P2, Res);
	cout << "P1 + P2 = ";
	Res.display();
	cout << endl << endl;
	
	SubtractPoly(P1, P2, Res);
	cout << "P1 - P2 = ";
	Res.display();
	cout << endl << endl;

	MultiplyPoly(P1, P2, Res);
	cout << "P1 x P2 = ";
	Res.display();
	cout << endl << endl;

	Polynomial R, Q, divisor;
	DividePoly(P1, P2, R, Q, divisor);
	cout << "P1 / P2 = ";
	Q.display();
	if (R.degree >= 0) {
		cout << " + (";
		R.display();
		cout << " / ";
		divisor.display();
		cout << " )";
	}
	cout << endl << endl;

	int x;
	reader >> x;
	cout << "P1(" << x << ") = " << P1.eval(x);
	cout << endl << endl;
	cout << "P2(" << x << ") = " << P2.eval(x);
	cout << endl << endl;
	
	return 0;
}
