#include <stdio.h>
#include "Poly.h"
#include "PolyNode.h"
#include <cctype>
#include <cstring>
#include <cstdlib>

//-------------------------------------------------
// Creates a polynomial from an expression.
// Returns a pointer to the first PolyNode in the list (head of the list)
//
// Expression will be of the following sort:
// Ex1: 2.3x^4 + 5x^3 - 2.64x - 4
// Ex2: -4.555x^10 - 45.44
// Ex3: x^6 + 2x^4 - x^3 - 6.3x + 4.223 
// Ex4: 34
// Ex5: -x+1
// Ex6: -3x^4    +   4x
// Ex7: -2x  - 5
//
PolyNode* CreatePoly(char* expr) {
    PolyNode* head = nullptr;
    PolyNode* current = nullptr;
    int i = 0;

    while (expr[i] != '\0') {
        // Add code to skip spaces
        while (isspace(expr[i])) {
            i++;
        }

        // Check for the sign of the term
        int coefSign = 1;
        if (expr[i] == '-') {
            coefSign = -1;
            i++;
        }
        else if (expr[i] == '+') {
            i++; // Skip the '+' sign
        }
        while (isspace(expr[i])) {
            i++;
        }

        // Parse the coefficient and exponent for the term
        double coef = 1.0;
        int exp = 0;

        while (isdigit(expr[i]) || expr[i] == '.') {
            if (expr[i] == '.') {
                i++;
                double decimalPlace = 10.0;
                while (isdigit(expr[i])) {
                    coef += (expr[i] - '0') / decimalPlace;
                    decimalPlace *= 10.0;
                    i++;
                }
            }
            else {
                coef = (coef == 1.0) ? (expr[i] - '0') : coef * 10 + (expr[i] - '0');
                i++;
            }
        }
        coef = coef*coefSign;

        if (expr[i] == 'x') {
            i++;
            if (expr[i] == '^') {
                i++;
                while (isdigit(expr[i])) {
                    exp = exp * 10 + (expr[i] - '0');
                    i++;
                }
            }
            else {
                exp = 1;
            }
        }

        // Create a new PolyNode for the term
        PolyNode* newNode = new PolyNode;
        newNode->coef = coef;
        newNode->exp = exp;
        newNode->next = nullptr;

        // Link the new PolyNode to the polynomial linked list
        if (current == nullptr) {
            head = newNode;
            current = newNode;
        }
        else {
            current->next = newNode;
            current = newNode;
        }

        // Skip spaces after the term
        while (isspace(expr[i])) {
            i++;
        }
    }

    return head;
}



/// -------------------------------------------------
/// Walk over the poly nodes & delete them
///
void DeletePoly(PolyNode* poly) {
    while (poly != nullptr) {
        PolyNode* current = poly;
        poly = poly->next;
        delete current;
    }
}
	
//-------------------------------------------------
// Adds a node (coefficient, exponent) to poly. If there already 
// is a node with the same exponent, then you simply add the coefficient
// to the existing coefficient. If not, you add a new node to polynomial
// Returns a pointer to the possibly new head of the polynomial.
//
PolyNode* AddNode(PolyNode* head, double coef, int exp) {
    if (coef == 0.0) {
        return head; // Skip terms with a coefficient of 0
    }

    PolyNode* newNode = new PolyNode;
    newNode->coef = coef;
    newNode->exp = exp;
    newNode->next = nullptr;

    if (head == nullptr || exp > head->exp) {
        newNode->next = head;
        return newNode;
    }

    PolyNode* current = head;
    while (current->next != nullptr && exp < current->next->exp) {
        current = current->next;
    }

    if (exp == current->exp) {
        current->coef += coef;
        delete newNode;
    }
    else {
        newNode->next = current->next;
        current->next = newNode;
    }

    return head;
}

PolyNode* Add(PolyNode* poly1, PolyNode* poly2) {
    PolyNode* result = nullptr;

    while (poly1 != nullptr || poly2 != nullptr) {
        if (poly1 != nullptr && poly2 != nullptr) {
            if (poly1->exp == poly2->exp) {
                result = AddNode(result, poly1->coef + poly2->coef, poly1->exp);
                poly1 = poly1->next;
                poly2 = poly2->next;
            }
            else if (poly1->exp > poly2->exp) {
                result = AddNode(result, poly1->coef, poly1->exp);
                poly1 = poly1->next;
            }
            else {
                result = AddNode(result, poly2->coef, poly2->exp);
                poly2 = poly2->next;
            }
        }
        else if (poly1 != nullptr) {
            result = AddNode(result, poly1->coef, poly1->exp);
            poly1 = poly1->next;
        }
        else {
            result = AddNode(result, poly2->coef, poly2->exp);
            poly2 = poly2->next;
        }
    }

    return result;
}



//-------------------------------------------------
// Subtracts poly2 from poly1 and returns the resulting polynomial
// Computes: poly3 = poly1 - poly2 and returns poly3
//
PolyNode *Subtract(PolyNode *poly1, PolyNode *poly2){
	// Fill this in
	return NULL;
} //end-Substract

//-------------------------------------------------
// Multiplies poly1 and poly2 and returns the resulting polynomial
// Computes: poly3 = poly1 * poly2 and returns poly3
//
PolyNode *Multiply(PolyNode *poly1, PolyNode *poly2){
	// Fill this in
	return NULL;
} //end-Multiply

//-------------------------------------------------
// Evaluates the polynomial at a particular "x" value and returns the result
//
double Evaluate(PolyNode *poly, double x){
	// Fill this in
	return 0;
} //end-Evaluate

//-------------------------------------------------
// Computes the derivative of the polynomial and returns it
// Ex: poly(x) = 3x^4 - 2x + 1-->Derivative(poly) = 12x^3 - 2
//
PolyNode *Derivative(PolyNode *poly){
	// Fill this in
	return NULL;
} //end-Derivative

//-------------------------------------------------
// Plots the polynomial in the range [x1, x2].
// -39<=x1<x2<=39
// -12<=y<=12
// On the middle of the screen you gotta have x & y axis plotted
// During evaluation, if "y" value does not fit on the screen,
// then just skip it. Otherwise put a '*' char depicting the curve
//
void Plot(PolyNode *poly, int x1, int x2){
	// Fill this in	
} //end-Plot
