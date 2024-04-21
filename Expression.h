#ifndef Expression_h
#define Expression_h

#include <iostream>
#include <vector>

class Expression {
	struct variable {
		char var;
		unsigned short inx;
		
		variable(const char& _var, const unsigned short& _inx = 0);
		bool operator==(const variable& _var) const;
	};
	
	struct term {
		struct literal {
			char _char;
			bool _state; // inverted/complement or not (e.g. if _state = 1 then it's written as: a, otherwise it's: a')
			
			literal(const char& _char00, const bool& _state00 = 1);
		};
		
		std::vector<literal> literals;
		std::string binary_form;
		
		term(const std::string& _term, const std::vector<variable>& _variables); // takes _term and identifies its literals and stores their _char value and _state
		void display();
	};
	
	bool printTheArray(int arr[], int n);
	void generateAllBinaryStrings(int n, int arr[], int i = 0);
	
	void group(std::vector<std::vector<std::string>>& groups, const std::vector<std::string>& binary_terms);
	void QM(std::vector<std::vector<std::vector<std::string>>>& groups, const std::vector<std::string>& binary_terms, std::vector<std::string*>& starred);
	
public:
	std::vector<term> terms; // holds the implicants/terms of the expression,
	// e.g. for an expression:= abc + b'c'd, the terms vector would hold <abc> and <b'c'd>.
	char operation; // product '*' or sum '+' (for later on expandability by also using PoS)
	
	// this map will  be used to hold all distinct latin characters used to represent the boolean function's expression
	std::vector<variable> variables; // this map holds every variable and its order index as input by the user in their expression
	// ... so for an expression:= b'c + ad', we have the map:= b --> 0, c --> 1, a --> 2, d --> 3.
	
	Expression(const std::string& _expression, const char& _operation = '+');
	// ^^ the constructor takes the _expression and identifies its terms and operation (+/*) and stores their literals
	
	void display();
	void print_truthTable();	
	void print_PIs();
};

#endif /* Expression_h */
