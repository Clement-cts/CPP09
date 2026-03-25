#include "PmergeMe.hpp"

PmergeMe::PmergeMe(): _comp(0) {} 

PmergeMe::PmergeMe(const PmergeMe& other): _vector(other._vector), _deque(other._deque), _comp(other._comp) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        _vector = other._vector;
        _deque = other._deque;
        _comp = other._comp;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

double PmergeMe::getTimeElapsed() const {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    return currentTime.tv_sec * 1000000.0 + currentTime.tv_usec;
}

void PmergeMe::resetComparisonCount() {
    _comp = 0;
}

size_t PmergeMe::getComparisonCount() const {
    return _comp;
}

std::vector<size_t> PmergeMe::generateJacobsthalSequence(size_t size) {
    std::vector<size_t> jacobsthalNumbers;

    if (size == 0)
        return jacobsthalNumbers;
    
    jacobsthalNumbers.push_back(1);
    if (size == 1)
        return jacobsthalNumbers;
    
    jacobsthalNumbers.push_back(1);
    if (size == 2)
        return jacobsthalNumbers;
    
    size_t previousTwo = 1;
    size_t previousOne = 1;

    for (size_t i = 2; i < size; ++i) {
        size_t current = previousOne + 2 * previousTwo;
        jacobsthalNumbers.push_back(current);
        previousTwo = previousOne;
        previousOne = current;
    }
    return jacobsthalNumbers;
}

bool PmergeMe::isValidNumber(const std::string& str) const {
    if (str.empty())
        return false;
    
    for (size_t i = 0; i < str.length(); ++i) {
        if (!std::isdigit(str[i]))
            return false;
    }
    return true;
}

int PmergeMe::toInt(const std::string& str) const {
    std::istringstream iss(str);
    long value;

    iss >> value;
    if (iss.fail() || !iss.eof() || value < 0 || value > INT_MAX)
        throw std::runtime_error("Error");
    return static_cast<int>(value);
}

void PmergeMe::validateInput(int ac, char **av) {
    (void)av;
    if (ac < 2)
        throw std::runtime_error("Error");
}

void PmergeMe::loadArguments(int ac, char **av) {
    validateInput(ac, av);
    std::set<int> uniqueValues;

    for (int i = 1; i < ac; ++i) {
        std::string arg(av[i]);

        if (arg.empty())
            throw std::runtime_error("Error");

        size_t startIndex = 0;
        if (arg[0] == '+') {
            startIndex = 1;
            if (arg.length() == 1)
                throw std::runtime_error("Error");
        }
        for (size_t charIndex = startIndex; charIndex < arg.length(); ++charIndex) {
            if (!std::isdigit(arg[charIndex]))
                throw std::runtime_error("Error");
        }
        int value = toInt(arg);

        if (uniqueValues.find(value) != uniqueValues.end())
            throw std::runtime_error("Error");

        uniqueValues.insert(value);
        _vector.push_back(value);
        _deque.push_back(value);
    }
}

void PmergeMe::printSequence(const std::string& label, const std::vector<int>& sequence) const {
    std::cout << label;
    for (size_t i = 0; i < sequence.size(); ++i) {
        if (i > 0)
            std::cout << " ";
        std::cout << sequence[i];
    }
    std::cout << std::endl;
}

template<typename Container>
size_t PmergeMe::findInsertionIndex(const Container& sequence, int value, size_t begin, size_t end) {
    while (begin < end) {
        size_t mid = begin + (end - begin) / 2;
        _comp++;

        if (sequence[mid] < value)
            begin = mid + 1;
        else
            end = mid;
    }
    return begin;
}

template<typename Container>
void PmergeMe::fordJohnsonSort(Container& sequence) {
    size_t size = sequence.size();
    if (size <= 1)
        return;
    
    if (size == 2) {
        _comp++;
        if (sequence[0] > sequence[1])
            std::swap(sequence[0], sequence[1]);
        return;
    }
    if (size == 3) {
        _comp++;
        if (sequence[0] > sequence[1])
            std::swap(sequence[0], sequence[1]);
        
        _comp++;
        if (sequence[1] > sequence[2]) {
            std::swap(sequence[1], sequence[2]);
            _comp++;
            if (sequence[0] > sequence[1])
                std::swap(sequence[0], sequence[1]);
        }
        return;
    }

    bool hasStraggler = (size % 2 == 1);
    int straggler = hasStraggler ? sequence[size - 1] : 0;
    size_t numberOfPairs = size / 2;

    std::vector< std::pair<int, int> > valuePairs;
    for (size_t pairIndex = 0; pairIndex < numberOfPairs; ++pairIndex) {
        _comp++;
        if (sequence[2 * pairIndex] > sequence[2 * pairIndex + 1])
            valuePairs.push_back(std::make_pair(sequence[2 * pairIndex], sequence[2 * pairIndex + 1]));
        else
            valuePairs.push_back(std::make_pair(sequence[2 * pairIndex + 1], sequence[2 * pairIndex]));
    }

    if (valuePairs.size() > 1) {
        Container maxElems;
        for (size_t pairIndex = 0; pairIndex < valuePairs.size(); ++pairIndex)
            maxElems.push_back(valuePairs[pairIndex].first);
        
        fordJohnsonSort(maxElems);

        std::vector< std::pair<int, int> > orderedPairs(valuePairs.size());
        std::vector<bool> pairAlreadyUsed(valuePairs.size(), false);

        for (size_t maxIndex = 0; maxIndex < maxElems.size(); ++maxIndex) {
            for (size_t pairIndex = 0; pairIndex < valuePairs.size(); ++pairIndex) {
                if (!pairAlreadyUsed[pairIndex] && valuePairs[pairIndex].first == maxElems[maxIndex]) {
                    orderedPairs[maxIndex] = valuePairs[pairIndex];
                    pairAlreadyUsed[pairIndex] = true;
                    break;
                }
            }
        }
        valuePairs = orderedPairs;
    }
    Container sortedChain;

    if (!valuePairs.empty())
        sortedChain.push_back(valuePairs[0].second);

    for (size_t pairIndex = 0; pairIndex < valuePairs.size(); ++pairIndex)
        sortedChain.push_back(valuePairs[pairIndex].first);

    if (valuePairs.size() > 1) {
        std::vector<size_t> jacobsthalNumbers = generateJacobsthalSequence(valuePairs.size() + 2);
        std::vector<bool> alreadyInserted(valuePairs.size(), false);
        alreadyInserted[0] = true;

        for (size_t jacobIndex = 2; jacobIndex < jacobsthalNumbers.size(); ++jacobIndex) {
            size_t currentJacobsthal = jacobsthalNumbers[jacobIndex];
            size_t previousJacobsthal = jacobsthalNumbers[jacobIndex - 1];

            for (size_t pos = std::min(currentJacobsthal, valuePairs.size()); pos > previousJacobsthal && pos > 0; --pos) {
                size_t pairIndex = pos - 1;
                if (pairIndex < valuePairs.size() && !alreadyInserted[pairIndex]) {
                    size_t searchLimit = sortedChain.size();
                    for (size_t chainIndex = 0; chainIndex < sortedChain.size(); ++chainIndex) {
                        if (sortedChain[chainIndex] == valuePairs[pairIndex].first) {
                            searchLimit = chainIndex;
                            break;
                        }
                    }
                    
                    size_t insertIndex = findInsertionIndex(sortedChain, valuePairs[pairIndex].second, 0, searchLimit);
                    sortedChain.insert(sortedChain.begin() + insertIndex, valuePairs[pairIndex].second);
                    alreadyInserted[pairIndex] = true;
                }
            }
        }
        for (size_t pairIndex = 1; pairIndex < valuePairs.size(); ++pairIndex) {
            if (!alreadyInserted[pairIndex]) {
                size_t searchLimit = sortedChain.size();
                for (size_t chainIndex = 0; chainIndex < sortedChain.size(); ++chainIndex) {
                    if (sortedChain[chainIndex] == valuePairs[pairIndex].first) {
                        searchLimit = chainIndex;
                        break;
                    }
                }
                size_t insertIndex = findInsertionIndex(sortedChain, valuePairs[pairIndex].second, 0, searchLimit);
                sortedChain.insert(sortedChain.begin() + insertIndex, valuePairs[pairIndex].second);
            }
        }
    }
    if (hasStraggler) {
        size_t insertIndex = findInsertionIndex(sortedChain, straggler, 0, sortedChain.size());
        sortedChain.insert(sortedChain.begin() + insertIndex, straggler);
    }
    
    sequence = sortedChain;
}

std::vector<int> PmergeMe::sortVector(double& elapsedTime) {
    double startTime = getTimeElapsed();

    std::vector<int> sortedVector = _vector;
    fordJohnsonSort(sortedVector);

    double endTime = getTimeElapsed();
    elapsedTime = endTime - startTime;

    return sortedVector;
}

std::deque<int> PmergeMe::sortDeque(double& elapsedTime) {
    double startTime = getTimeElapsed();

    std::deque<int> sortDeque = _deque;
    fordJohnsonSort(sortDeque);

    double endTime = getTimeElapsed();
    elapsedTime = endTime - startTime;

    return sortDeque;
}

void PmergeMe::run(int ac, char **av) {
    loadArguments(ac, av);

    printSequence("Before: ", _vector);

    resetComparisonCount();
    double vectorElapsedTime;
    std::vector<int> sortedVector = sortVector(vectorElapsedTime);
    size_t vectorComp = getComparisonCount();

    resetComparisonCount();
    double dequeElapsedTime;
    std::deque<int> sortedDeque = sortDeque(dequeElapsedTime);
    size_t dequeComp = getComparisonCount();

    printSequence("After: ", sortedVector);

    std::cout << "Time to process a range of " << _vector.size()
                << " elements with std::vector : " << vectorElapsedTime << " us" << std::endl;
    std::cout << "Time to process a range of " << _deque.size()
                << " elements with std::deque : " << dequeElapsedTime << " us" << std::endl;

    std::cout << "comp: " << vectorComp << std::endl;
    std::cout << "comp deque: " << dequeComp << std::endl;
}

template void PmergeMe::fordJohnsonSort<std::vector<int> >(std::vector<int>& sequence);
template void PmergeMe::fordJohnsonSort<std::deque<int> >(std::deque<int>& sequence);
template size_t PmergeMe::findInsertionIndex<std::vector<int> >(const std::vector<int>& sequence, int value, size_t begin, size_t end);
template size_t PmergeMe::findInsertionIndex<std::deque<int> >(const std::deque<int>& sequence, int value, size_t begin, size_t end);