// Include all the libraries and the header file required
#include <iostream>
#include "CSVReader.h"
#include "MerkelMain.h"
#include "Weatherdata.h"

// Static members of MerkelMain class
std::string MerkelMain::header_Row;
std::vector<std::string> MerkelMain::dataset;
std::vector<std::string> MerkelMain::available_Codes;

// Constructor for Merkelmain
MerkelMain::MerkelMain() {}

// Init function of the program
void MerkelMain::init()
{
    // Stores user input
    int input;
    while (true)
    {
        // Show the menu options
        printMenu();
        // Get the user input
        std::cin >> input;
        // Process the selected option form the user
        processUserOption(input);
    }
}

// Menu function - Prints the different menu options
void MerkelMain::printMenu()
{
    std::cout << "===================================" << std::endl;
    std::cout << "               Menu"                 << std::endl;
    std::cout << "===================================" << std::endl;

    std::cout << "1: View Available Country Code" << std::endl;
    std::cout << "2: Compute candlestick data Yearly" << std::endl;
    std::cout << "3: Text Based Plot Yearly" << std::endl;
    std::cout << "4: Text Based Plot Using Filters" << std::endl;
    std::cout << "5: Future Prediction" << std::endl;
    std::cout << "6: Help Center" << std::endl;
    std::cout << "7: Exit" << std::endl;
    std::cout << "===================================" << std::endl;
}

// Show the available country codes for the inputs
void MerkelMain::show_codes()
{
    // Loading the dataset
    dataset = CSVReader::readCSV("weather_data_EU_1980-2019_temp_only.csv");
    // Temporary vector to store column headers
    std::vector<std::string> col_header;
    // Temporary string to parse headers
    std::string temporary;

    // Split the headerrow into individual column headers
    for (size_t i = 0; i < header_Row.length(); ++i)
    {
        if (header_Row[i] == ',')
        {
            col_header.push_back(temporary);
            temporary.clear();
        }
        else
        {
            temporary += header_Row[i];
        }
    }
    if (!temporary.empty())
    {
        col_header.push_back(temporary);
    }

    available_Codes.clear();
    // Extract country codes from headers with the temperature at the bacm
    for (size_t i = 1; i < col_header.size(); ++i)
    {
        if (col_header[i].find("_temperature") != std::string::npos)
        {
            available_Codes.push_back(col_header[i].substr(0, col_header[i].find("_temperature")));
        }
    }

    // Print the country codes
    std::cout << "Available Country Codes: ";
    for (size_t i = 0; i < available_Codes.size(); ++i) {
        std::cout << available_Codes[i];
        if (i != available_Codes.size() - 1) {
            std::cout << ",";
        }
    }
    std::cout << std::endl;
}

// Function to compute yearly candlestick data
void MerkelMain::compute_candlestick_yearly() {
    // Print the country codes first to see what codes are available for processing
    show_codes();
    // Non-static function, so created a variable called weather processor to access the function.
    computation_weather_data computation;
    computation.compute_candlestick_yearly(dataset); 
}

// Function to generate a yearly text based plot
void MerkelMain::text_plot_yearly() {
    // Print the country codes first to see what codes are available for processing
    show_codes();
    // Non-static function, so created a variable called weather processor to access the function.
    computation_weather_data computation;
    computation.text_plot_yearly(dataset);
}

// Function to generate a filtered text based plot
void MerkelMain::text_plot_Filters() {
    // Print the country codes first to see what codes are available for processing
    show_codes();
    // Non-static function, so created a variable called weather processor to access the function.
    computation_weather_data computation;
    computation.text_plot_Filters(dataset);
}

// Function to predict the future temperature and plot in a text based plot
void MerkelMain::Future_Prediction() {
    // Print the country codes first to see what codes are available for processing
    show_codes();
    // Non-static function, so created a variable called weather processor to access the function.
    computation_weather_data computation;
    computation.Future_Prediction(dataset);
}

// Function to instruct the user on how to use the menu options
void MerkelMain::Menuinstruction()
{
    std::cout << "===================================" << std::endl;
    std::cout << "             Instructions          " << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Welcome! This program leads you to explore and filter temperature data from various countries over different time periods." << std::endl;
    std::cout << std::endl;

    std::cout << "Overview:" << std::endl;
    std::cout << "1. View computations for all country codes across all years from 1980 to 2019. " << std::endl;
    std::cout << "2. Filter Data: Refine the dataset with various filter options:" << std::endl;
    std::cout << "   - By Country: Focus on temperature records for a specific country." << std::endl;
    std::cout << "   - By Year: Retrieve data for a specific year range." << std::endl;
    std::cout << "3. Predict Future Trends: Use historical temperature patterns to forecast future temperatures for selected countries." << std::endl;
    std::cout << std::endl;

    std::cout << "How to Use:" <<std::endl;
    std::cout << "   - Select options from the main menu by entering the menu options" << std::endl;
    std::cout << "   - Follow the instructions to filter or analyze the data as required." << std::endl;
    std::cout << std::endl;

    std::cout << "For the sample graph, please open the sample_graph.txt file to view the details of the graph." << std::endl;
    std::cout << std::endl;

    std::cout << "Thank you for using the program!" << std::endl;
}

// Function to exit the program
void MerkelMain::Exit()
{
    std::cout << "Thank you. Exiting now..." << std::endl;
    exit(0);
}

// Function to process the selected menu option
void MerkelMain::processUserOption(int userOption)
{
    if (userOption == 0 || userOption > 7)
    {
        std::cout << "Invalid choice. Choose 1-7" << std::endl;
    }
    if (userOption == 1)
    {
        show_codes();
    }
    if (userOption == 2)
    {
        compute_candlestick_yearly();
    }
    if (userOption == 3)
    {
        text_plot_yearly();
    }
    if (userOption == 4)
    {
        text_plot_Filters();
    }
    if (userOption == 5)
    {
        Future_Prediction();
    }
    if (userOption == 6)
    {
        Menuinstruction();
    }
    if (userOption == 7)
    {
        Exit();
    }
}