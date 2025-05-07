// Include all the libraries required to run the program
#pragma once
#include "Candlestick.h"

class MerkelMain {
public:
    MerkelMain();
    void init();

    // Static functions
    static std::string header_Row;
    static std::vector<std::string> dataset;
    static std::vector<std::string> available_Codes;

    // Computation functions 
    void compute_candlestick_yearly();
    void text_plot_yearly();
    void text_plot_Filters();
    void Future_Prediction();
    
private:
    // Main menu
    void printMenu();
    // Process the user option
    void processUserOption(int userOption);
    // Display the country codes
    void show_codes();
    // Show the user how to use the application
    void Menuinstruction();
    // Exit option
    void Exit();
};



