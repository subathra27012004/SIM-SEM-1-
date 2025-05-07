// Include the libraries and the header file
#include "Candlestick.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <limits>
#include <vector>
#include <cmath>
#include <string>

Candlestick Candlestick::compute(const std::string& date, const std::vector<double>& temperature, double prev_close) {
    // Open value: Average temperature from the previous time frame
    double open = prev_close;

    // High value: Highest temperature
    double high = *std::max_element(temperature.begin(), temperature.end());

    // Low value: Lowest temperature
    double low = *std::min_element(temperature.begin(), temperature.end());

    // Close value: Average temperature for the current time frame
    double close = std::accumulate(temperature.begin(), temperature.end(), 0.0) / temperature.size();

    return Candlestick(date, open, high, low, close);
};

void Candlestick::yearly_graph(const std::vector<Candlestick>& candlesticks, const std::string& countryCode) {
    // Generate text-based candlestick chart
        double yMax = -std::numeric_limits<double>::infinity();
        double yMin = std::numeric_limits<double>::infinity();

        std::cout << "==================================================================================================================================" << std::endl;
        std::cout << "                                           Candlestick chart for country code: "  << countryCode << "\n";
        std::cout << "==================================================================================================================================\n" << std::endl;

        // y-axis range from candlesticks
        for (const auto &candle : candlesticks)
        {
            yMax = std::max(yMax, candle.high);
            yMin = std::min(yMin, candle.low);
        }

        // Normalize y-axis range for display
        int range = (static_cast<int>(std::ceil(yMax)) + static_cast<int>(std::floor(yMin))) / 2;
        int yMin_Nor = range - 30;
        int yMax_Nor = range + 30;

        // Generate the graph with colors
        for (int y = yMax_Nor; y >= yMin_Nor; y -= 1)
        {
            // y-axis label to be printed next to the temperature
            std::cout << std::setw(6) << y << " | "; 
            for (const auto& candle : candlesticks)
            {
                // Round the values to display
                int H_round = static_cast<int>(std::round(candle.high));
                int L_round = static_cast<int>(std::round(candle.low));
                int O_round = static_cast<int>(std::round(candle.open));
                int C_round = static_cast<int>(std::round(candle.close));

                // Color based on Open (O) and Close (C)
                std::string color;
                if (C_round > O_round) {color = "\033[32m";} // Green for C > O, Red for O > C
                else {color = "\033[31m";}
                
                // Symbols for high,low,close,open
                if (y == H_round) {std::cout << color << "H " << " \033[0m";}
                else if (y == L_round) {std::cout << color << "L " << " \033[0m";}
                else if (y == O_round) {std::cout << color << "O " << " \033[0m";}
                else if (y == C_round) {std::cout << color << "C " << " \033[0m";}
                else if (y < H_round && y > L_round) {std::cout << color << "X " << " \033[0m";}
                else {std::cout << "   ";}
            }
            std::cout << std::endl;
        }

        std::cout << "-----------------------------------------------------------------------------------------------------------------------------------" << std::endl;

        // X-axis labels
        std::cout << "      ";
        for (int i = 0; i < candlesticks.size(); i++)
        {
            if (i % 10 == 0 || candlesticks[i].date == "1980" || candlesticks[i].date == "1990" || candlesticks[i].date == "2000" || candlesticks[i].date == "2019")
            {std::cout << std::setw(3) << candlesticks[i].date;}
            else{std::cout << std::setw(3) << "";}
        }
        std::cout << std::endl;
}

void Candlestick::Filter_graph(const std::vector<Candlestick>& candlesticks, const std::string& countryCode) {
        // Generate text-based candlestick chart
        double yMax = -std::numeric_limits<double>::infinity();
        double yMin = std::numeric_limits<double>::infinity();

        for (const auto& candle : candlesticks) {
            yMax = std::max(yMax, candle.high);
            yMin = std::min(yMin, candle.low);
        }

        int range = (static_cast<int>(std::ceil(yMax)) + static_cast<int>(std::floor(yMin))) / 2;
        int yMin_Nor = range - 30;
        int yMax_Nor = range + 30;

        std::cout << "==================================================================================================================================" << std::endl;
        std::cout << "                                           Candlestick chart for country code: " << countryCode << "\n";
        std::cout << "==================================================================================================================================\n" << std::endl;

        for (int y = yMax_Nor; y >= yMin_Nor; y -= 1) {
            std::cout << std::setw(6) << y << " | ";
            for (const auto& candle : candlesticks) {
                int H_round = static_cast<int>(std::round(candle.high));
                int L_round = static_cast<int>(std::round(candle.low));
                int O_round = static_cast<int>(std::round(candle.open));
                int C_round = static_cast<int>(std::round(candle.close));

                // Color based on Open (O) and Close (C)
                std::string color;
                if (C_round > O_round) {color = "\033[32m";} // Green for C > O, Red for O > C
                else {color = "\033[31m";}

                if (y == H_round) std::cout << color << "H" << "\033[0m\t";
                else if (y == L_round) std::cout << color << "L" << "\033[0m\t";
                else if (y == O_round) std::cout << color << "O" << "\033[0m\t";
                else if (y == C_round) std::cout << color << "C" << "\033[0m\t";
                else if (y < H_round && y > L_round) std::cout << color << "X" << "\033[0m\t";
                else std::cout << " \t";
            }
            std::cout << std::endl;
        }

        std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
        std::cout << "  ";
        for (const auto& candle : candlesticks) {
            std::cout << std::setw(8) << candle.date;
        }
        std::cout << std::endl;

}

void Candlestick::future_graph(const std::vector<Candlestick>& candlesticks, const std::string& countryCode) {
    double yMax = -std::numeric_limits<double>::infinity();
    double yMin = std::numeric_limits<double>::infinity();

    // Determine Y-axis range
    for (const auto& candle : candlesticks) {
        yMax = std::max(yMax, candle.high);
        yMin = std::min(yMin, candle.low);
    }

    int range = (static_cast<int>(std::ceil(yMax)) + static_cast<int>(std::floor(yMin))) / 2;
    int yMin_Nor = range - 30;
    int yMax_Nor = range + 30;

    std::cout << "\n==================================================================================================================================\n";
    std::cout << "                                           Candlestick Chart for Country Code: " << countryCode << "\n";
    std::cout << "==================================================================================================================================\n";

    for (int y = yMax_Nor; y >= yMin_Nor; y -= 1) {
        std::cout << std::setw(6) << y << " | ";
        for (const auto& candle : candlesticks) {
            int H_round = static_cast<int>(std::round(candle.high));
            int L_round = static_cast<int>(std::round(candle.low));
            int O_round = static_cast<int>(std::round(candle.open));
            int C_round = static_cast<int>(std::round(candle.close));

            // Determine the color for Open/Close
            std::string color;
            if (candle.date < "2020") {  // Historical data
                if (C_round > O_round) color = "\033[32m";  // Green for C > O
                else color = "\033[31m";  // Red for O > C
            } else {  // Future predictions
                if (C_round > O_round) color = "\033[34m";  // Blue for C > O
                else color = "\033[33m";  // Yellow for O > C
            }

            if (y == H_round) std::cout << color << "H " << "\033[0m\t";
            else if (y == L_round) std::cout << color << "L " << "\033[0m\t";
            else if (y == O_round) std::cout << color << "O " << "\033[0m\t";
            else if (y == C_round) std::cout << color << "C " << "\033[0m\t";
            else if (y < H_round && y > L_round) std::cout << color << "X " << "\033[0m\t";
            else std::cout << "  \t";
        }
        std::cout << std::endl;
    }

    // X-axis labels
    std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
    std::cout << "  ";
    for (const auto& candle : candlesticks) {
        std::cout << std::setw(8) << candle.date;
    }
    std::cout << std::endl;
}