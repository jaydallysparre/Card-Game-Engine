#include "TestCases.hpp"

void TestCases::registerAndGetPos(int ID, double x, double y) {
    testPosition.registerObjectPos(ID, x, y);
    std::pair<double, double> test = testPosition.getPos(ID);

    // Check if the test cases match, calculating difference just in case the testing decimal cases have long decimal places
    const double difference = 0.000001;
    bool xCorrect = std::abs(test.first - x) < difference;
    bool yCorrect = std::abs(test.second - y) < difference;

    assert(xCorrect && yCorrect && "The position was not set properly! Error within registerObjectPos or getPos");
    std::cout << "The position was set properly! No errors generated"; 
}