#include "Expression.h"
using namespace std;

// validifying an expression:
// - no repeated characters in any term
// - no ending with operator
// - no duplicate operator
// - no starting with operator
// - no stand alone apostrophe
// - no starting term with apostrophe
// - every term has to have its variables sorted alphabetically

bool is_valid(const string& s) {
	// check if the string is empty
	if (s.empty()) {
		return false;
	}
	// check if the string contains only valid characters
	for (auto c : s) {
		// function can contain a to z , A to Z , + , ’ , space
		if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '+' || c == '\'' || c == ' ' ) {
			continue;
		} else {
			return false;
		}
	}
	// check if the string contains only valid terms
	vector<string> terms;
	string term;
	for (char c : s) {
		if (c == '+') {
			terms.push_back(term);
			term.clear();
		} else {
			term += c;
		}
	}
	
	return true;
}

char *removeDubs(char c[], int n) {
	int index = 0;
	for (int i = 0; i <n ; i++) {
		int j;
		for (j= 0; j<i; j++)
			if (c[i] == c[j])
				break;
		if (j==i)
			c[index++] = c[i];
	}
	return c;
}

/*bool no_beging_or_end_withOP(int n , char c[])
{
	char b, e;int w,q;
	char op[] = {'+', '!', '.', '^'};
	for (w=0; w<4; w++)
	{
		while (q=n-1)
		if ((c[0] == op[w]) || (c[q] == op[w]))
		return true;
		else
		return false;
	}
}*/

//translate the next to be the validation function
void validation() {
	const int k = 1024;
	// first removing duplicates
	std::cout << "please enter characters: ";
	char c[k];
	std::cin >> c;
	int n = sizeof (c)/sizeof (c[0]);
	/*if(no_beging_or_end_withOP(n,c))
	cout<<"invalide";
	else*/
	std::cout << removeDubs(c, n);
}

int main() {
	string exp;
	
	cout << "\n Enter a boolean function in SOP form (e.g. abc + a'b'c): ";
	getline(cin, exp);
	
//	if(is_valid(exp)) cout << "Valid SOP expression" << endl;
//		else { cout << "Invalid SOP expression" << endl ; return 0; }
	
//	we assume the input expression is valid from here...
	Expression sop(exp);
	
	std::cout << " The SoP expression you entered: ";
	sop.display();
	
//	tests:
//	ab + ab'
//	abc + a'b'c
//	a'b'cd + a'bcd + ab'cd
	
	sop.print_truthTable();
	std::cout << "\n\n ";
	sop.print_PIs();
	
	std::cout << "\n\n ";
	return 0;
}

//std::pair<unsigned short, std::string> get_minterm(const std::string& binary, const std::vector<Expression::variable>& variables);
// returns index of minterm and its expression
// note: the length of the binary string is the same size as the variables vector
