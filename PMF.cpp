#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

class Term {
public:
	int Cof;
	int Exp;
	Term* next;
	Term(int coef, int exp) : Cof(coef), Exp(exp), next(nullptr) {}
};
class Polynomial {
private:
	Term* head;

public:
	Polynomial() : head(nullptr) {}
	~Polynomial() {
		while (head) {
			Term* temp = head;
			head = head->next;
			delete temp;
		}
	}

	void insertTerm(int coef, int exp) {
		if (coef == 0) return;
		Term* newTerm = new Term(coef, exp);

		if (!head || exp > head->Exp) {
			newTerm->next = head;
			head = newTerm;
		}
		else {
			Term* current = head;
			while (current->next && current->next->Exp > exp) {
				current = current->next;
			}
			if (current->Exp == exp) {
				current->Cof += coef;
				if (current->Cof == 0) {
					if (current == head) {
						head = current->next;
					}
					else {
						Term* prev = head;
						while (prev->next != current) {
							prev = prev->next;
						}
						prev->next = current->next;
					}
					delete current;
				}
				delete newTerm;
			}
			else {
				newTerm->next = current->next;
				current->next = newTerm;
			}
		}
	}

	void readPolynomial() {
		std::string input;
		while (std::getline(std::cin, input) && !input.empty()) {
			std::istringstream iss(input);
			int coef, exp;
			char comma;
			if (iss >> coef >> comma >> exp) {
				insertTerm(coef, exp);
			}
		}
	}

	void printPolynomial() const {
		if (!head) {
			std::cout << "0" << std::endl;
			return;
		}

		Term* current = head;
		bool first = true;
		while (current) {
			if (current->Cof != 0) {
				if (!first && current->Cof > 0) {
					std::cout << "+";
				}
				if (current->Cof != 1 || current->Exp == 0) {
					std::cout << current->Cof;
				}
				if (current->Exp > 0) {
					std::cout << "x";
					if (current->Exp > 1) {
						std::cout << "^" << current->Exp;
					}
				}
				first = false;
			}
			current = current->next;
		}
		std::cout << std::endl;
	}

	static Polynomial add(const Polynomial& p1, const Polynomial& p2) {
		Polynomial result;
		Term* t1 = p1.head, * t2 = p2.head;

		while (t1 && t2) {
			if (t1->Exp > t2->Exp) {
				result.insertTerm(t1->Cof, t1->Exp);
				t1 = t1->next;
			}
			else if (t1->Exp < t2->Exp) {
				result.insertTerm(t2->Cof, t2->Exp);
				t2 = t2->next;
			}
			else {
				int sum = t1->Cof + t2->Cof;
				if (sum != 0) {
					result.insertTerm(sum, t1->Exp);
				}
				t1 = t1->next;
				t2 = t2->next;
			}
		}

		while (t1) {
			result.insertTerm(t1->Cof, t1->Exp);
			t1 = t1->next;
		}

		while (t2) {
			result.insertTerm(t2->Cof, t2->Exp);
			t2 = t2->next;
		}

		return result;
	}

	static Polynomial subtract(const Polynomial& p1, const Polynomial& p2) {
		Polynomial result;
		Term* t1 = p1.head, * t2 = p2.head;

		while (t1 && t2) {
			if (t1->Exp > t2->Exp) {
				result.insertTerm(t1->Cof, t1->Exp);
				t1 = t1->next;
			}
			else if (t1->Exp < t2->Exp) {
				result.insertTerm(-t2->Cof, t2->Exp);
				t2 = t2->next;
			}
			else {
				int diff = t1->Cof - t2->Cof;
				if (diff != 0) {
					result.insertTerm(diff, t1->Exp);
				}
				t1 = t1->next;
				t2 = t2->next;
			}
		}

		while (t1) {
			result.insertTerm(t1->Cof, t1->Exp);
			t1 = t1->next;
		}

		while (t2) {
			result.insertTerm(-t2->Cof, t2->Exp);
			t2 = t2->next;
		}

		return result;
	}

	double evaluate(double x) const {
		double result = 0;
		Term* current = head;

		while (current) {
			result += current->Cof * std::pow(x, current->Exp);
			current = current->next;
		}

		return result;
	}
};

int main() {
	Polynomial p1, p2;

	std::cout << "Enter the first polynomial (format: Cof,Exp):" << std::endl;
	p1.readPolynomial();

	std::cout << "Enter the second polynomial (format: Cof,Exp):" << std::endl;
	p2.readPolynomial();

	std::cout << "First polynomial: ";
	p1.printPolynomial();

	std::cout << "Second polynomial: ";
	p2.printPolynomial();

	Polynomial sum = Polynomial::add(p1, p2);
	std::cout << "Sum of polynomials: ";
	sum.printPolynomial();

	Polynomial diff = Polynomial::subtract(p1, p2);
	std::cout << "Difference of polynomials: ";
	diff.printPolynomial();

	double x;
	std::cout << "Enter a value for x to evaluate the polynomials: ";
	std::cin >> x;
	std::cout << "Evaluation of first polynomial: " << p1.evaluate(x) << std::endl;
	std::cout << "Evaluation of second polynomial: " << p2.evaluate(x) << std::endl;
	std::cout << "Evaluation of sum: " << sum.evaluate(x) << std::endl;
	std::cout << "Evaluation of difference: " << diff.evaluate(x) << std::endl;
	return 0;
}