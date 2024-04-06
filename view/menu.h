#ifndef PROJECTDA_MENU_H
#define PROJECTDA_MENU_H
/**
* @file Menu.h
* @brief This file contains the Menu header.
*/

// Project headers
#include "../controller/manager.h"
// Standard Library Headers
#include <stack>

using namespace std;

/**
 * @class Menu
 * @brief This is the menu interface to access funcionalities on manager.
 *
 * This class represents a menu.
 */

class Menu{
    /**
    * @brief A stack that stores pointers to visited Menus.
    */
    stack<void(Menu::*)()> menuStack;
    /**
     * @brief Pointer to a manager instance.
     */
    Manager* manager;

    /* Auxiliary functions */
    /**
    * @brief Removes leading and trailing spaces from a string.
    * @param input The input string to process.
    * @return A string with leading and trailing spaces removed.
    */
    string removeLeadingTrailingSpaces(const string& input);

    /* Timer functions */
    /**
    * @brief Starts a timer.
    * @param start_real Reference to the start time in real time.
    * @param start_cpu Reference to the start time in CPU time.
    */
    void startTimer(timespec& start_real, timespec& start_cpu) const;
    /**
    * @brief Stops a timer and calculates elapsed time.
     * @param start_real Reference to the start time in real time.
     * @param start_cpu Reference to the start time in CPU time.
     * @param elapsed_real Reference to store the elapsed real time.
     * @param elapsed_cpu Reference to store the elapsed CPU time.
     */
    void stopTimer(timespec& start_real, timespec& start_cpu, double& elapsed_real, double& elapsed_cpu) const;

    /* Validity of input */
    /**
     * @brief Checks the validity of a pipe between two vertexes.
     * @param code1 Reference to the code of the first vertex.
     * @param code2 Reference to the code of the second vertex.
     * @return Pointer to the Edge if the pipe is valid, nullptr otherwise.
     */
    Edge* validPipe(string& code1, string& code2);
    /**
     * @brief Checks the validity of a city code.
     * @param code Reference to the city code.
     * @return True if the city code is valid, false otherwise.
    */
    bool validCity(string& code);
    /**
     * @brief Checks the validity of a station code.
     * @param code Reference to the station code.
     * @return True if the station code is valid, false otherwise.
    */
    bool validStation(string& code);
    /**
     * @brief Checks the validity of a reservoir code.
     * @param code Reference to the reservoir code.
     * @return True if the reservoir code is valid, false otherwise.
    */
    bool validReservoir(string& code);

    /**
     * @brief Calls the other validity functions by type.
     * @param type The type of the Vertex.
     * @param code Reference to the code of the Vertex.
     * @return True if the vertex type and code combination is valid, false otherwise.
     */
    bool isValidInterface(const string& type,string& code);

    /* Get input */
        /**
     * @brief Retrieves vertex codes of a specific vertex type from a given unordered map.
     * @param vertices The unordered map containing vertices.
     * @param itemType The type of vertex for which vertices are retrieved.
     * @return A vector of vertex codes of the specified item type.
     */
     vector<string> getVertex(const unordered_map<string, Vertex*>& vertices, const string& itemType);

    /**
     * @brief Retrieves codes of all cities.
     * @return A vector of city codes.
     */
    vector<string> getCities();

    /**
     * @brief Retrieves codes of all stations.
     * @return A vector of station codes.
     */
    vector<string> getStations();

    /**
     * @brief Retrieves codes of all reservoirs.
     * @return A vector of reservoir codes.
     */
    vector<string> getReservoirs();

    /**
     * @brief Retrieves pointers to all pipes.
     * @return A vector of pointers to Edge objects representing pipes.
    */
    vector<Edge*> getPipes();

    /**
     * @brief Retrieves a number input within a specified range.
     * @param minInput The minimum allowed input value.
     * @param maxInput The maximum allowed input value.
     * @param option Pointer to store the selected option.
     * @return True if the input is successfully obtained, false otherwise.
     */
    bool getNumberInput(int minInput, int maxInput, int* option);


    /* Get examples of input */
    /**
     * @brief Retrieves examples of vertexes with a specific label(output) from a given map.
     * @param items The unordered map containing items.
     * @param label Output customization.
     */
    void getItemExamples(const unordered_map<string, Vertex*>& items, const string& label);

    /**
     * @brief Retrieves examples of cities.
     */
    void getCityExamples();

    /**
     * @brief Retrieves examples of stations.
     */
    void getStationExamples();

    /**
     * @brief Retrieves examples of reservoirs.
     */
    void getReservoirExamples();

    /**
     * @brief Retrieves examples of vertexes depending on the type.
     * @param type The type of vertex for which examples are retrieved.
     */
    void getExamplesInterface(const string& type);



public:
    /**
    * @brief Destructor for Menu.
    */
    ~Menu();

    /**
     * @brief Constructor for Menu.
     * @param manager Pointer to a Manager instance.
     */
        explicit Menu(Manager* manager);

    /**
     * @brief Displays the main menu.
     */
        void mainMenu();

    /**
     * @brief Shutdowns menu.
     */
        void exitMenu();

    /**
     * @brief Returns to the previous menu.
     */
        void goBack();

    /**
     * @brief Displays the algorithm menu.
     */
        void algorithmMenu();

    /**
     * @brief Executes exercise 2.1.
     */
        void exercise21();

    /**
     * @brief Executes exercise 2.2.
     */
        void exercise22();

    /**
     * @brief Executes exercise 2.3.
     */
        void exercise23();

    /**
     * @brief Executes exercise 3.1.
     */
        void exercise31();

    /**
     * @brief Executes exercise 3.2.
     */
        void exercise32();

    /**
     * @brief Executes exercise 3.3.
     */
        void exercise33();

    /**
     * @brief Displays the extra menu with other algorithms.
     */
        void extraMenu();

};

#endif //PROJECTDA_MENU_H
