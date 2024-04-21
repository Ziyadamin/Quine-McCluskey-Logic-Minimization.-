#include "Expression.h"
#include <list>

// struct variable ...................................................................................................
Expression::variable::variable(const char& _var, const unsigned short& _inx) : var(_var), inx(_inx) { }
bool Expression::variable::operator==(const variable& _var) const { return (var == _var.var) ? true : false; }

// struct literal ....................................................................................................
Expression::term::literal::literal(const char& _char00, const bool& _state00) : _char(_char00), _state(_state00) {}

// struct term .......................................................................................................
Expression::term::term(const std::string& _term, const std::vector<variable>& _variables) : binary_form("") {
	for(int i = 0; i < _term.size(); i++) {
		if(_term[i] != char(39)) // char(39) is the apostrophe character ' that represents inversion in user inputs and expression outputs
			literals.push_back(literal(_term[i]));
		else
			literals.rbegin()->_state = 0;
	}
	
//	std::sort(literals.begin(), literals.end());
	
	for(int i = 0, j = 0; i < _variables.size(); i++) { // in order for this to work we need the terms to be sorted alphabetically
		if(std::find(_variables.begin(), _variables.end(), variable(literals[j]._char)) != _variables.end()
		   && std::find(_variables.begin(), _variables.end(), variable(literals[j]._char))->inx == i)
			binary_form.push_back(literals[j++]._state + '0');
		else
			binary_form.push_back('-');
	}
}

void Expression::term::display() {
	for(int i = 0; i < literals.size(); i++) {
		std::cout << literals[i]._char;
		if(!literals[i]._state)
			std::cout << char(39); // char(39) is the apostrophe character ' that represents inversion in user inputs and expression outputs
	}
	
	std::cout << " (" << binary_form << ')';
}

void divide_string(std::vector<std::string>& word_vector, const std::string& str, const char& seperator) {
	std::list<std::string> word_list;
	for(int i = 0, start = 0, end = 0; i < str.size(); i++) {
		if(str[i] == seperator || i == str.size() - 1) {
			end = i;
			if(str[i] != seperator && i == str.size() - 1)
				end = i + 1;
			word_list.push_back("");
			word_list.rbegin()->resize(end - start);
			for(int j = start, c = 0; j < end; j++, c++)
				(*word_list.rbegin())[c] = str[j];
			start = end + 1;
		}
	}
	
	word_vector.resize(word_list.size());
	for(unsigned long i = 0; i < word_vector.size(); i++, word_list.pop_front())
		word_vector[i] = *word_list.begin();
}

// class Expression ..................................................................................................
Expression::Expression(const std::string& _expression, const char& _operation) : operation(_operation) {
	for (unsigned short i = 0, j = 0; i < _expression.size(); i++)
		if(_expression[i] != '+' && _expression[i] != '*' && _expression[i] != ' ' && _expression[i] != char(39)
		   && std::find(variables.begin(), variables.end(), _expression[i]) == variables.end())
			variables.push_back({ _expression[i], j++ });
	
	std::vector<std::string> words;
	divide_string(words, _expression, ' ');
	
//	for displaying the words vector:
//	std::cout << std::endl;
//	for(int i = 0; i < words.size(); i++)
//		std::cout << words[i] << std::endl;

	// now we seperate the terms from the operators and save them in the terms vector
	
	for(int i = 0; i < words.size(); i++)
		if(words[i] != "+") {
			std::vector<std::string> tmp;
			divide_string(tmp, words[i], operation);
			for(int inx = 0; inx < tmp.size(); inx++)
				if(tmp[inx] != "")
					terms.push_back(term(tmp[inx], variables));
		}
	
//	for examining the content of the terms vector:
//	std::cout << std::endl;
//	for(int i = 0; i < terms.size(); i++) {
//		terms[i].display();
//		std::cout << std::endl;
//	}
}

void Expression::display() {
	for(int i = 0; i < terms.size(); i++) {
		terms[i].display();
		if(i + 1 < terms.size())
			std::cout << " " << operation << " ";
	}
}

// this following block of code is mainly credited to GeeksforGeeks ****************************************************************
// .. it's a simple brute force algorithm to generate all possible binary strings of a certain length
// there were, however, some adjustments made by us to these two functions
// .. to allow the printing of the output of the boolean expression for every possible truth assignment combination of the variables

// Function responsible for every row in truth table
bool Expression::printTheArray(int arr[], int n) {
	std::cout << std::endl;
	for (int i = 0; i < n; i++)
		std::cout << " " << arr[i];
	
	std::vector<bool> flags(terms.size(), true);
	for (int j = 0; j < terms.size(); j++)
		for (int i = 0; i < n; i++)
			if(arr[i] + '0' != terms[j].binary_form[i] && terms[j].binary_form[i] != '-') {
				flags[j] = false;
				break;
			}
	
	for(int i = 0; i < terms.size(); i++)
		if(flags[i])
			return true;
	
	return false;
}

// Function to generate all binary strings
void Expression::generateAllBinaryStrings(int n, int arr[], int i) {
	if (i == n) {
		bool output_flag = printTheArray(arr, n);
		std::cout << " |   " << output_flag;
		return;
	}
 
	// First assign "0" at ith position
	// and try for all other permutations
	// for remaining positions
	arr[i] = 0;
	generateAllBinaryStrings(n, arr, i + 1);
 
	// And then assign "1" at ith position
	// and try for all other permutations
	// for remaining positions
	arr[i] = 1;
	generateAllBinaryStrings(n, arr, i + 1);
}

// *********************************************************************************************************************************

void Expression::print_truthTable() {
	const int n = (int)variables.size();
	int arr[n];
	
	std::cout << "\n\n";
	for(std::vector<variable>::iterator it = variables.begin(); it != variables.end(); it++)
		std::cout << ' ' << it->var;
	std::cout << " | output";
	std::cout << "\n ";
	for(int i = 0; i < 2 * n + 8; i++)
		std::cout << '-';
	
	generateAllBinaryStrings(n, arr);
}

void Expression::group(std::vector<std::vector<std::string>>& _groups, const std::vector<std::string>& binary_terms) {
	for(int i = 0; i < binary_terms.size(); i++) {
		int counter = 0;
		
		for(int j = 0; j < variables.size(); j++)
			if(binary_terms[i][j] == '1')
				counter++;
		
		_groups[counter].push_back(binary_terms[i]);
	}
}

void Expression::QM(std::vector<std::vector<std::vector<std::string>>>& groups, const std::vector<std::string>& binary_terms, std::vector<std::string*>& starred)
{
	int inx = 0;
	for(int col = 0; col < groups.size(); col++) {
		for (int group_num = 0; group_num < groups[col].size() - 1; group_num++) {
			for(int j01 = 0; j01 < groups[col][group_num].size(); j01++) {
				for(int j02 = 0; j02 < groups[col][group_num + 1].size(); j02++) {
					std::vector<unsigned short> occurences;
					for(int k = 0; k < variables.size(); k++)
						if(groups[col][group_num][j01][k] != groups[col][group_num + 1][j02][k])
							occurences.push_back(k);
					
					if(occurences.size() > 1) break;
					else if(occurences.size() == 1) { // if num of occurences is 1
						std::string tmp;
						tmp = groups[col][group_num][j01];
						tmp[*occurences.begin()] = '-';
						
						inx = col + 1;
						group(groups[col + 1], std::vector<std::string>(1, tmp));
					}
					else // when the number of occurences of differences is zero
						starred.push_back(&groups[col][group_num][j01]);
				}
			}
		}
	}
	
	for (int i = 0; i < groups[inx].size(); i++)
		for(int j = 0; j < groups[inx][i].size(); j++)
			starred.push_back(&groups[inx][i][j]);
}

void Expression::print_PIs() { 
	std::vector<std::vector<std::vector<std::string>>> groups(variables.size() + 1, std::vector<std::vector<std::string>>(variables.size() + 1));
	std::vector<std::string> binary_strings(terms.size());
	
	for(int i = 0; i < terms.size(); i++)
		binary_strings[i] = terms[i].binary_form;

//	testing the binary_strings vector..
//	for(int i = 0; i < terms.size(); i++)
//		std::cout << "\n " << binary_strings[i];
	
	std::vector<std::string*> starred;
	group(groups[0], binary_strings);
	QM(groups, binary_strings, starred);
	
//	testing the implication table..
//	for(int column = 0; column < groups.size(); column++) {
//		std::cout << "\n Column0" << column << std::endl;
//		for(int group_num = 0; group_num < groups[column].size(); group_num++) {
//			std::cout << "\n > Group" << group_num << ':';
//			for(int row = 0; row < groups[column][group_num].size(); row++)
//				std::cout << ' ' << groups[column][group_num][row] << ',';
//		}
//		std::cout << std::endl;
//	}
	
	std::cout << "\n Prime Implicants:";
	for(int i = 0; i < starred.size(); i++)
		std::cout << "\n * " << *starred[i];
}
