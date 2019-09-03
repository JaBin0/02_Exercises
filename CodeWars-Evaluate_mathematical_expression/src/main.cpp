/* Evaluate mathematical expression
    ====== Description ======
    Given a mathematical expression as a string you must return the result as a number.
	Number may be both whole numbers and/or decimal numbers. The same goes for the returned result.
	Operators
	You need to support the following mathematical operators:
		Multiplication *
		Division / (as true division)
		Addition +
		Subtraction -
		Operators are always evaluated from left-to-right, and * and / must be evaluated before + and -.
		Parentheses: You need to support multiple levels of nested parentheses, ex. (2 / (2 + 3.33) * 4) - -6
		Whitespace : There may or may not be whitespace between numbers and operators.
	
	An addition to this rule is that the minus sign (-) used for negating numbers and parentheses will never be separated by whitespace. I.e., all of the following are valid expressions.
	
	1-1    // 0
	1 -1   // 0
	1- 1   // 0
	1 - 1  // 0
	1- -1  // 2
	1 - -1 // 2
	
	6 + -(4)   // 2
	6 + -( -4) // 10
	And the following are invalid expressions
	
	1 - - 1    // Invalid
	1- - 1     // Invalid
	6 + - (4)  // Invalid
	6 + -(- 4) // Invalid
	Validation
	You do not need to worry about validation - you will only receive valid mathematical expressions following the above rules.
*/
#include <iostream>
#include <string.h>
#include <vector>

double calc(std::string expression);

int main(int argc, char** argv ) {
	std::cout << "Code Wars: Evaluate mathematical expression" << std::endl;
	std::string expr_1 = "42 + 11 - 82.24 * 4 / 521 + 0.15 - 18 * 4.258";
	std::cout << expr_1 << "=" << calc(expr_1) << std::endl;
    return 0;
}

double calc(std::string expression) {
	std::string num; 
	std::vector<std::string> exprVec;
	for(auto it = expression.begin(); it != expression.end(); ++it) {
		if('0' <= *it && *it <= '9') {
			num += *it;
			while(('0' <= *(it+1) && *(it+1) <= '9') || *(it+1) == '.') {
				++it;
				num += *it;
			}
			std::cout <<"Number: " << num.data() << std::endl;
			exprVec.push_back(num);
			num.clear();
		}
		else if(*it == '+' || *it == '-' || *it == '*' || *it=='/' || *it=='(' || *it==')') {
			exprVec.push_back({*it});
			std::cout <<"Other: " << *it << std::endl;
		}
	}

	// look for priority 1
	for(auto )

	return 0;
}