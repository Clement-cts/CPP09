#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <map>
# include <string>
# include <cstdlib>
# include <stdexcept>
# include <cctype>


class BitcoinExchange {
    public:
        BitcoinExchange();
        BitcoinExchange(const BitcoinExchange& other);
        BitcoinExchange& operator=(const BitcoinExchange& other);
        ~BitcoinExchange();

        void loadDataBase(const std::string &filename);
        void processInput(const std::string &filename);

        double getRate(const std::string &date) const;

    private:
        std::map<std::string, double> _data;

        bool validDate(const std::string &date) const;
        bool validValue(double value) const;
        double parseValue(const std::string &value) const;
        std::string trim(const std::string& str) const;
        std::string findClosestDate(const std::string& date) const;
};

#endif