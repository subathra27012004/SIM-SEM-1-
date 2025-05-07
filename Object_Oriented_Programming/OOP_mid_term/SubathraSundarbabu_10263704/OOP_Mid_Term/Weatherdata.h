// Include all the libraries and the header file
#pragma once
#include "Candlestick.h"

class computation_weather_data{
    public:
        // Compute yearly candlestick data
        void compute_candlestick_yearly(const std::vector<std::string>& dataset);
        // Generate a text-based plot of yearly weather data
        void text_plot_yearly(const std::vector<std::string>& dataset);
        // Generate a text-based plot of filtered weather data based on the selected countries and year range
        void text_plot_Filters(const std::vector<std::string>& dataset);
        // Predict future weather data trends
        void Future_Prediction(const std::vector<std::string>& dataset);

    private:
        // Store candlestick data as a vector
        std::vector<Candlestick> candlesticks;
};