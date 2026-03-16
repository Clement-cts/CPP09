#include "RPN.hpp"

rpn::rpn() {}

rpn::rpn(const rpn& a) {
    *this = a;
}

rpn& rpn::operator=(const rpn& a) {
    if (this != &a) {
        this->_stack = a._stack;
    }
    return *this;
}

rpn::~rpn() {}

int rpn::getResult() const {
    return this->_stack.top();
}

int rpn::CalculateResult(int n1, int n2, char op) {
    switch (op) {
        case '+':
            return n2 + n1;
        case '-':
            return n2 - n1;
        case '*':
            return n2 * n1;
        case '/': {
            if (n1 == 0)
                throw std::runtime_error("Error: Division by zero");
            return n2 / n1;
        }
    }
    throw std::runtime_error("Error: Invalid operator");
}

void rpn::ReadInput(const std::string& input) {
    std::istringstream iss(input);
    std::string token;

    while (iss >> token) {
        if (token.length() == 1 && std::isdigit(token[0])) {
            this->_stack.push(token[0] - '0');
        }
        else if (token.length() == 1 && std::string("+-*/").find(token[0]) != std::string::npos) {
            if (this->_stack.size() < 2)
                throw std::runtime_error("Error");
            
            int n1 = this->_stack.top(); this->_stack.pop();
            int n2 = this->_stack.top(); this->_stack.pop();

            this->_stack.push(CalculateResult(n1, n2, token[0]));
        }
        else {
            throw std::runtime_error("Error");
        }
    }
    if (this->_stack.size() != 1)
        throw std::runtime_error("Error");
}