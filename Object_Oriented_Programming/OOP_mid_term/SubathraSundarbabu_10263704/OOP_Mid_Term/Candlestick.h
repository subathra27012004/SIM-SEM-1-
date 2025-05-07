// Include all the required libraries
#pragma once
#include <string>
#include <vector>

class Candlestick {
public:
    // Candlestick variables
    // The year
    std::string date;
    // Opening temperature
    double open;
    // Highest temperature
    double high;
    // Lowest temperature
    double low;
    // Closing temperature
    double close;

    // Constructor to initialize a Candlestick object
    Candlestick(std::string _date, 
                double _open, 
                double _high, 
                double _low, 
                double _close)
        : date(_date), open(_open), high(_high), low(_low), close(_close) {}

    // Compute candlestick data
    static Candlestick compute(const std::string& date, const std::vector<double>& temperature, double prev_close);

    // Text based graphs
    static void generateTextBasedGraph(const std::vector<Candlestick>& candlesticks, const std::string& countryCode, bool isFuture);
    static void yearly_graph(const std::vector<Candlestick>& candlesticks, const std::string& countryCode);
    static void Filter_graph(const std::vector<Candlestick>& candlesticks, const std::string& countryCode);
    static void future_graph(const std::vector<Candlestick>& candlesticks, const std::string& countryCode);
};