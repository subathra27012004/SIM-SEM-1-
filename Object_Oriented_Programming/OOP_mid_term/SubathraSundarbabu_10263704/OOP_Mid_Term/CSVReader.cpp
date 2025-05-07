// Include all the libraries and header file
#include "CSVReader.h"
#include "MerkelMain.h"
#include <fstream>
#include <iostream>

CSVReader::CSVReader()
{

}

// Reads a CSV file and returns its lines as a vector of strings
std::vector<std::string> CSVReader::readCSV(const std::string &filePath)
{
    std::ifstream file(filePath);
    
    // Vector to store each line from the csv
    std::vector<std::string> lines;
    
    // String to hold the current line
    std::string line;

    // If file is not open
    if (!file.is_open())
    {
        std::cout << "Error: Could not open file: " << filePath << std::endl;
    }

    // Read the header row
    if (std::getline(file, line))
    {
        // Store the header row in the static member `headerRow` of MerkelMain
        MerkelMain::header_Row = line;
    }

    // Read the lines and add them to the vector
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }
    
    // Close the file after reading
    file.close(); 
    return lines;
}

// Splits a string into tokens based on ","
std::vector<std::string> CSVReader::tokenise(const std::string &line, char seperator)
{
    // Vector to store the tokens
    std::vector<std::string> tokens;
    // Start of the current token
    int start = 0;
    // End of the current token
    int end = 0;

    // Loop to find each token
    while ((end = line.find(seperator, start)) != std::string::npos)
    {
        // Extract the token and add it to the vector
        tokens.push_back(line.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(line.substr(start));
    return tokens;
}

// Finds the index in the CSV header row
int CSVReader::Col_Index(const std::string &col_Name)
{
    // Tokenise the header row to get individual column names
    std::vector<std::string> tokens = tokenise(MerkelMain::header_Row, ',');
    for (int i = 0; i < tokens.size(); ++i)
    {
        // Compare the current token with the column name
        if (tokens[i] == col_Name)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// Function to group temperatures by year
std::map<int, std::vector<double>> CSVReader::group_temp_year(
    const std::vector<std::string>& dataset, int col_Index) {
    // Store temperatures grouped by year
    std::map<int, std::vector<double>> yearly_temps;

    for (const auto& line : dataset) {
        // Tokenise using ","
        auto tokens = CSVReader::tokenise(line, ',');
        if (tokens.size() <= col_Index) continue;

        // Extract the year from the timpstamp
        int year = std::stoi(tokens[0].substr(0, 4));

        // Convert the temperature value to number
        double temperature = std::stod(tokens[col_Index]);

        // Push the temperature to the particular year
        yearly_temps[year].push_back(temperature);
    }

    return yearly_temps;
}

// Function to compute candlesticks
std::vector<Candlestick> CSVReader::computeCandlesticks(
    const std::map<int, std::vector<double>>& yearly_temps) {
    // Store candlestick values
    std::vector<Candlestick> candlesticks;
    double prev_close = 0.0;

    for (const auto& entry : yearly_temps) {
        // Get the temperature for the current year
        const auto& temps = entry.second;
        // Compute candlestick for the current year using compute function in candlestick.cpp
        auto candle = Candlestick::compute(std::to_string(entry.first), temps, prev_close);
        // Add it to the vector
        candlesticks.emplace_back(candle);
        prev_close = candle.close;
    }

    return candlesticks;
}

// Function to group temperature by year range
std::map<int, std::vector<double>> CSVReader::group_temp_year_range(const std::vector<std::string>& dataset,int col_Index,int startYear,int endYear) {
    // Store temperature grouped by year within range
    std::map<int, std::vector<double>> yearly_temps;

    for (const auto& line : dataset) {
        // tokenise using ","
        auto tokens = CSVReader::tokenise(line, ',');
        if (tokens.size() <= col_Index) continue;

        // Get the timestamp
        std::string timestamp = tokens[0];
        // Get the year
        int year = std::stoi(timestamp.substr(0, 4));

        // Only compute for the specified year range
        if (year >= startYear && year <= endYear) { 
            double temperature = std::stod(tokens[col_Index]);
            yearly_temps[year].push_back(temperature);
        }
    }

    return yearly_temps;
}

// Function to process the country code for the filter function
std::map<std::string, int> CSVReader::CountryCode_processing(const std::string& input) {
    std::vector<std::string> code;
    std::map<std::string, int> code_to_index;
    std::string seperator = ",";
    int pos = 0;
    std::string input_clean = input;

    // Extract and process country codes
    while ((pos = input_clean.find(seperator)) != std::string::npos) {
        code.push_back(input_clean.substr(0, pos));
        input_clean.erase(0, pos + seperator.length());
    }
    code.push_back(input_clean);

    for (std::string& countryCode : code) {
        // Trim spaces
        countryCode.erase(countryCode.find_last_not_of(" \t\n\r\f\v") + 1);

        // Construct column name
        std::string col_Name = countryCode + "_temperature";
        int col_Index = CSVReader::Col_Index(col_Name);

        // Country code not found print messages
        if (col_Index == -1) {
            std::cout << "Error: Country code not found: " << countryCode << std::endl;
        } else {
            code_to_index[countryCode] = col_Index;
        }
    }

    return code_to_index;
}
