#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _data(other._data) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if (this != &other)
        _data = other._data;
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

void BitcoinExchange::loadDataBase(const std::string &filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open())
        throw std::runtime_error("Error: could not open database file.");
    
    std::string line;
    bool firstLine = true;

    while (std::getline(file, line)) {
        if (firstLine) {
            firstLine = false;
            
            if (line != "date,exchange_rate") {
                throw std::runtime_error("Error: invalid database format.");
            }
            continue;
        }

        size_t commaPos = line.find(',');
        if (commaPos == std::string::npos)
            continue;
        
        std::string data = line.substr(0, commaPos);
        std::string rateStr = line.substr(commaPos + 1);

        std::istringstream iss(rateStr);
        double rate;
        if (iss >> rate) {
            _data[data] = rate;
        }
    }
    file.close();
}

void BitcoinExchange::processInput(const std::string &filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: could not open file." << std::endl;
        return;
    }
    std::string line;
    bool firstLine = true;

    while (std::getline(file, line)) {
        if (firstLine) {
            firstLine = false;
            if (line != "date | value") {
                std::cerr << "Error: invalid input format." << std::endl;
                file.close();
                return;
            }
            continue;
        }

        size_t pipePos = line.find('|');
        if (pipePos == std::string::npos) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        std::string date = trim(line.substr(0, pipePos));
        std::string valueStr = trim(line.substr(pipePos + 1));

        if (!validDate(date)) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        try {
            double value = parseValue(valueStr);

            if (!validValue(value)) {
                if (value < 0)
                    std::cerr << "Error: not a positive number." << std::endl;
                else
                    std::cerr << "Error: too large a number." << std::endl;
                continue;
            }

            double rate = getRate(date);
            double result = value * rate;

            std::cout << date << " => " << value << " = " << result << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: bad input => " << line << std::endl;
        }
    }
    file.close();
}

double BitcoinExchange::getRate(const std::string& date) const{
    std::map<std::string, double>::const_iterator it = _data.find(date);
    if (it != _data.end())
        return it->second;

    std::string closestDate = findClosestDate(date);
    it = _data.find(closestDate);
    if (it != _data.end())
        return it->second;
        
            return 0.0;
}

bool BitcoinExchange::validDate(const std::string& date) const {
    if (date.length() != 10)
        return false;

    if (date[4] != '-' || date[7] != '-')
        return false;
    
    for (size_t i = 0; i < date.length(); ++i) {
        if (i == 4 || i == 7)
            continue;
        if (!std::isdigit(date[i]))
            return false;
    }

    int year = atoi(date.substr(0, 4).c_str());
    int month = atoi(date.substr(5, 2).c_str());
    int day = atoi(date.substr(8, 2).c_str());

    if (year < 1000 || year > 9999)
        return false;
    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > 31)
        return false;
    
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeap ? 29 : 28))
            return false;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30)
            return false;
    }
    return true;
}

bool BitcoinExchange::validValue(double value) const {
    return value >= 0.0 && value <= 1000.0;
}

double BitcoinExchange::parseValue(const std::string& value) const {
    std::istringstream iss(value);
    double result;
    char extra;

    if (!(iss >> result))
        throw std::runtime_error("Error: invalid value.");
    if (iss >> extra)
        throw std::runtime_error("Error: invalid value.");
    return result;
}

std::string BitcoinExchange::trim(const std::string& str) const {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos)
        return "";
    
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

std::string BitcoinExchange::findClosestDate(const std::string& date) const {
    std::string closestDate;

    for (std::map<std::string, double>::const_iterator it = _data.begin();
        it != _data.end(); ++it) {
            if (it->first <= date) {
                if (closestDate.empty() || it->first > closestDate)
                    closestDate = it->first;
            }
        }
    return closestDate;
}