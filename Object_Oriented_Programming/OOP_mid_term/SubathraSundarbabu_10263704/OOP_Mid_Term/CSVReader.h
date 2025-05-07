// Include all the libraries
#pragma once
#include <vector>
#include <string>
#include <map>
#include "Candlestick.h"

class CSVReader {
public:
    CSVReader();
    // Read the CSV file and return its contents as a vector of strings
    static std::vector<std::string> readCSV(const std::string& filePath);
    
    // Tokenize single line from the CSV file
    static std::vector<std::string> tokenise(const std::string &line, char seperator);
    
    // Get the index of a specific column in the CSV file
    static int Col_Index(const std::string &col_Name);

    // Group temperatures by year
    static std::map<int, std::vector<double>> group_temp_year(const std::vector<std::string>& dataset, int col_Index);

    // Compute candlestick representation 
    static std::vector<Candlestick> computeCandlesticks(const std::map<int, std::vector<double>>& yearly_temps);

    // Group temperature data by year range
    static std::map<int, std::vector<double>> group_temp_year_range(const std::vector<std::string>& dataset,int col_Index,int startYear,int endYear);
    
    // process country codes and columns
    static std::map<std::string, int> CountryCode_processing(const std::string& input);
};
