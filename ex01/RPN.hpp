#ifndef RPN_HPP
# define RPN_HPP

# include <iostream>
# include <stack>
# include <cstdlib>
# include <iomanip>
# include <sstream>

class rpn {
    public:
        rpn();
        rpn(const rpn& a);
        rpn& operator=(const rpn& a);
        ~rpn();

        void ReadInput(const std::string& input);
        int CalculateResult(int n1, int n2, char op);
        int getResult() const;

    private:
        std::stack<int> _stack;
};

#endif