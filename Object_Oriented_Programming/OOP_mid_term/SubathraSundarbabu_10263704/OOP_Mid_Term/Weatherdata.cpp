// Include function to add in the libraries and the header files needed to work on the functions
#include "weatherdata.h"
#include "MerkelMain.h"
#include "CSVReader.h"
#include "Candlestick.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <cmath>

// Compute yearly candlestick data from the given dataset
void computation_weather_data::compute_candlestick_yearly(const std::vector<std::string>& dataset) {
    // User input of the country code
    std::cout << "Enter the country code: ";
    std::string Code;
    std::cin >> Code;

    // Add _temperature with the country code to find the column
    std::string col_name = Code + "_temperature";
    // Find the index of the column
    int col_Index = CSVReader::Col_Index(col_name);

    // If the country code is not found, print the message
    if (col_Index == -1)
    {
        std::cout << "Wrong Input! Country code is not from the list stated above" << std::endl;
        return;
    }

    // Utils functions to group data and compute candlesticks.
    auto Temps_yearly = CSVReader::group_temp_year(dataset, col_Index);
    auto candlesticks = CSVReader::computeCandlesticks(Temps_yearly);

    // Print the values of the candlestick
    std::cout << "======================================================================" << std::endl;
    std::cout << "                    Candlestick data for " << Code << std::endl;
    std::cout << "======================================================================" << std::endl;

    for (const auto &candle : candlesticks)
    {
        std::cout << "Date: " << candle.date << "| Open: " << candle.open << "| High: " << candle.high << "| Low: " << candle.low << "| Close: " << candle.close << std::endl;
    }
};

void computation_weather_data::text_plot_yearly(const std::vector<std::string>& dataset) {
    // User input multiple country codes
    std::cout << "Enter country codes (comma-separated): ";

    std::string input;
    std::cin.ignore();
    std::getline(std::cin, input);

    // Use the helper function
    auto code_to_index = CSVReader::CountryCode_processing(input);

    // Process each country code
    for (const auto& pair : code_to_index) {
        const std::string& countryCode = pair.first;
        int col_index = pair.second;

        // Utils functions to group data and compute candlesticks.
        auto Temps_yearly = CSVReader::group_temp_year(dataset, col_index);
        auto candlesticks = CSVReader::computeCandlesticks(Temps_yearly);
        Candlestick::yearly_graph(candlesticks, countryCode);
    }
}

void computation_weather_data::text_plot_Filters(const std::vector<std::string>& dataset) {
    // User input for country codes
    std::cout << "Enter country codes (comma-separated): ";
    std::string input;
    std::cin.ignore();
    std::getline(std::cin, input);

    // User input for year range
    std::string year_Range;
    std::cout << "Enter the year range (e.g., 1980-1983): ";
    std::cin >> year_Range;

    // Parse the year range
    int dash = year_Range.find('-');
    if (dash == std::string::npos) {
        std::cout << "Error: Invalid year range format!" << std::endl;
        return;
    }

    int startYear = std::stoi(year_Range.substr(0, dash));
    int endYear = std::stoi(year_Range.substr(dash + 1));

    // Error message for invalid year range
    if (startYear > endYear) {
        std::cout << "Error: Start year cannot be greater than end year!" << std::endl;
        return;
    }

    // Process country codes and find their column indices
    std::map<std::string, int> code_to_index = CSVReader::CountryCode_processing(input);

    // Iterate over processed country codes and their column indices
    for (const auto& pair : code_to_index) {
        const std::string& countryCode = pair.first;
        int col_index = pair.second;

        // Group temperatures by year within the range
        auto Temps_yearly = CSVReader::group_temp_year_range(dataset, col_index, startYear, endYear);

        // Compute candlesticks from grouped temperatures
        auto candlesticks = CSVReader::computeCandlesticks(Temps_yearly);

        Candlestick::Filter_graph(candlesticks, countryCode);
     }
}


void computation_weather_data::Future_Prediction(const std::vector<std::string>& dataset) {
    // User input of country code for the future prediction
    std::cout << "Enter the country code for future temperature prediction: ";
    std::string countryCode;
    std::cin >> countryCode;

    // Add the country code with _temperature and look for it in the index of the dataset
    std::string col_name = countryCode + "_temperature";
    int col_index = CSVReader::Col_Index(col_name);

    // Wrong input
    if (col_index == -1) {
        std::cout << "Error: Wrong Country Code" << std::endl;
        return;
    }

    // User input the number of years to predict
    std::cout << "Enter the number of years to predict (e.g., 6 for 2020 to 2025): ";
    int years_predicted;
    std::cin >> years_predicted;

    // If the user input negative number error message is printed
    if (years_predicted <= 0) {
        std::cout << "Error: The number of years to predict must be greater than 0." << std::endl;
        return;
    }

    // Function to group temperature data by year
    auto Temps_yearly = CSVReader::group_temp_year(dataset, col_index);

    // Store year statistics and list of years
    std::vector<int> years;
    std::map<int, double> avg_temps;
    std::map<int, double> yearlyHighs;
    std::map<int, double> yearlyLows;

    // Calculate average, high, low temperatures for each year
    for (const auto& entry : Temps_yearly) {
        double averageTemp = std::accumulate(entry.second.begin(), entry.second.end(), 0.0) / entry.second.size();
        double yearlyHigh = *std::max_element(entry.second.begin(), entry.second.end());
        double yearlyLow = *std::min_element(entry.second.begin(), entry.second.end());

        avg_temps[entry.first] = averageTemp;
        yearlyHighs[entry.first] = yearlyHigh;
        yearlyLows[entry.first] = yearlyLow;
        years.push_back(entry.first);
    }

    // Sort the years in ascending order
    std::sort(years.begin(), years.end());

    // Linear regression to predict future temperatures
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    int n = years.size();

    for (int i = 0; i < n; i++) {
        int x = years[i];
        // Average temperature for the year
        double y = avg_temps[years[i]];

        sumX += x;
        sumY += y;
        sumXY += x * y;
        sumX2 += x * x;
    }

    // Calculate the slope
    double m = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);

    // Values for candlestick between year 2015 to 2019, the last 5 years
    std::vector<Candlestick> prev_candlestickdata;
    double totalHighDiff = 0, totalLowDiff = 0;
    int count = 0;

    // Generate candlesticks for 2015 to 2019
    for (int i = 0; i < years.size(); i++) {
        if (years[i] >= 2015 && years[i] <= 2019) {
            // Temperature data for the year
            auto& temps = Temps_yearly[years[i]];
            double prev_close = (i == 0) ? 0.0 : avg_temps[years[i - 1]];
            Candlestick candle = Candlestick::compute(std::to_string(years[i]), temps, prev_close);

            // Difference in highs
            totalHighDiff += (yearlyHighs[years[i]] - yearlyHighs[years[i - 1]]);
            // Difference in low
            totalLowDiff += (yearlyLows[years[i]] - yearlyLows[years[i - 1]]);
            count++;

            prev_candlestickdata.push_back(candle);
        }
    }

    // Average difference in highs and lows
    double avgHighDiff = totalHighDiff / count;
    double avgLowDiff = totalLowDiff / count;

    std::vector<Candlestick> future_Candlesticks;
    double prev_close = prev_candlestickdata.back().close;

    // Generate predicted candlesticks
    for (int i = 1; i <= years_predicted; i++) {
        int futureYear = 2019 + i;
        // Predict average temperature
        double Temp_predicted = avg_temps[2019] + m * i;

        double open = prev_close;
        double close = Temp_predicted;

        // Predict high and low temperature
        double high = yearlyHighs[2019] + avgHighDiff * i;
        double low = yearlyLows[2019] + avgLowDiff * i;

        Candlestick futureCandle(std::to_string(futureYear), open, high, low, close);
        future_Candlesticks.push_back(futureCandle);

        prev_close = close;
    }

    // Combine past and future candlesticks for the chart
    std::vector<Candlestick> combined_Candlesticks = prev_candlestickdata;
    combined_Candlesticks.insert(combined_Candlesticks.end(), future_Candlesticks.begin(), future_Candlesticks.end());

    // Display values for past and future candlesticks
    for (const auto& candle : combined_Candlesticks) {
        std::cout << "Year: " << candle.date << " | Open: " << candle.open << " | High: " << candle.high << " | Low: " << candle.low << " | Close: " << candle.close << std::endl;
    }

    // Future graph
    Candlestick::future_graph(combined_Candlesticks, countryCode);
}