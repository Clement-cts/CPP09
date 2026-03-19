#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <iostream>
# include <vector>
# include <deque>
# include <algorithm>
# include <string>
# include <sstream>
# include <algorithm>
# include <ctime>
# include <sys/time.h>
# include <stdexcept>
# include <set>
# include <climits>
# include <cctype>

class PmergeMe {
    public:
        PmergeMe();
        PmergeMe(const PmergeMe& other);
        PmergeMe& operator=(const PmergeMe& other);
        ~PmergeMe();

        std::vector<int> sortVector(double& elapsedTime);
        std::deque<int> sortDeque(double& elapsedTime);

        void printSequence(const std::string& label, const std::vector<int>& sequence) const;
        size_t getComparisonCount() const;
        void resetComparisonCount();

        void run(int ac, char **av);
        
    private:
        std::vector<int> _vector;
        std::deque<int> _deque;
        size_t _comp;

        std::vector<size_t> generateJacobsthalSequence(size_t size);

        bool isValidNumber(const std::string& str) const;
        int toInt(const std::string& str) const;

        template<typename Container>
        void fordJohnsonSort(Container& sequence);

        template<typename Container>
        size_t findInsertionIndex(const Container& sequence, int value, size_t begin, size_t end);

        void validateInput(int ac, char **av);
        void loadArguments(int ac, char **av);

        double getTimeElapsed() const;
};

# endif