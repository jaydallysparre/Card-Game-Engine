#include "RenderPosition.hpp"
#include <cassert>
#include <cmath>
#include <iostream> 

class TestCases{
    private: 
    RenderPosition testPosition;
    public:
    void registerAndGetPos(int ID, double x, double y);
};