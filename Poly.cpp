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
    PolyNode* newNode = new PolyNode;
    newNode->coef = coef;
    newNode->exp = exp;
    newNode->next = nullptr;

    if (head == nullptr || exp > head->exp) {
        newNode->next = head;
        return newNode;
    }
    if (exp == head->exp) {
        head->coef += coef;
        delete newNode;
        return head;
    }

    PolyNode* current = head;
    while (current->next != nullptr && current->next->exp > exp) {
        current = current->next;
    }

    if (current->next != nullptr && current->next->exp == exp) {
        current->next->coef += coef;
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
PolyNode* Subtract(PolyNode* poly1, PolyNode* poly2) {
    PolyNode* result = nullptr;

    while (poly1 != nullptr || poly2 != nullptr) {
        if (poly1 != nullptr && poly2 != nullptr) {
            if (poly1->exp == poly2->exp) {
                result = AddNode(result, poly1->coef - poly2->coef, poly1->exp);
                poly1 = poly1->next;
                poly2 = poly2->next;
            }
            else if (poly1->exp > poly2->exp) {
                result = AddNode(result, poly1->coef, poly1->exp);
                poly1 = poly1->next;
            }
            else {
                result = AddNode(result, -poly2->coef, poly2->exp);
                poly2 = poly2->next;
            }
        }
        else if (poly1 != nullptr) {
            result = AddNode(result, poly1->coef, poly1->exp);
            poly1 = poly1->next;
        }
        else {
            result = AddNode(result, -poly2->coef, poly2->exp);
            poly2 = poly2->next;
        }
    }

    return result;
}


//-------------------------------------------------
// Multiplies poly1 and poly2 and returns the resulting polynomial
// Computes: poly3 = poly1 * poly2 and returns poly3
//
PolyNode* Multiply(PolyNode* poly1, PolyNode* poly2) {
    PolyNode* result = nullptr;

    // Iterate through the terms of poly1
    for (PolyNode* term1 = poly1; term1 != nullptr; term1 = term1->next) {
        // Iterate through the terms of poly2
        for (PolyNode* term2 = poly2; term2 != nullptr; term2 = term2->next) {
            // Multiply coefficients and add exponents
            double coef = term1->coef * term2->coef;
            int exp = term1->exp + term2->exp;

            // Add the term to the result polynomial
            result = AddNode(result, coef, exp);
        }
    }

    return result;
}


//-------------------------------------------------
// Evaluates the polynomial at a particular "x" value and returns the result
//
double Evaluate(PolyNode* poly, double x) {
    double result = 0.0;
    PolyNode* current = poly;

    while (current != nullptr) {
        result += current->coef * pow(x, current->exp);
        current = current->next;
    }

    return result;
}

//-------------------------------------------------
// Computes the derivative of the polynomial and returns it
// Ex: poly(x) = 3x^4 - 2x + 1-->Derivative(poly) = 12x^3 - 2
//
PolyNode* Derivative(PolyNode* poly) {
    PolyNode* derivative = nullptr;
    PolyNode* current = poly;

    while (current) {
        if (current->exp > 0) {
            double coef = current->exp * current->coef;
            int exp = current->exp - 1;
            derivative = AddNode(derivative, coef, exp);
        }
        current = current->next;
    }

    return derivative;
}


//-------------------------------------------------
// Plots the polynomial in the range [x1, x2].
// -39<=x1<x2<=39
// -12<=y<=12
// On the middle of the screen you gotta have x & y axis plotted
// During evaluation, if "y" value does not fit on the screen,
// then just skip it. Otherwise put a '*' char depicting the curve
//
void Plot(PolyNode* poly, int x1, int x2) {
    const int minY = -12;
    const int maxY = 12;
    const int xAxis = 0;
    const char axisChar = '+';
    const char plotChar = '*';

    for (int y = maxY; y >= minY; y--) {
        for (int x = x1; x <= x2; x++) {
            double result = Evaluate(poly, x);
            if (result >= y - 0.5 && result < y + 0.5) {
                if (x == xAxis && y == 0) {
                    putchar(axisChar);
                }
                else if (y == 0) {
                    putchar(axisChar);
                }
                else if (x == xAxis) {
                    putchar(axisChar);
                }
                else {
                    putchar(plotChar);
                }
            }
            else if (y == minY && x == xAxis) {
                putchar(axisChar);
            }
            else if (y == 0) {
                putchar('-');
            }
            else if (x == xAxis) {
                putchar('|');
            }
            else {
                putchar(' ');
            }
        }
        putchar('\n');
    }
}

