#include "States.hpp"


States currentState = {
    sf::View(),
    0,
    DRAW,
    nullptr,
    sf::Color::Black,
    nullptr,
    32,
    32,
    {3, 3},
    sf::Color::White,
    true,
    true
};
//sf::Clock clock;
animationStates animationState = {
    false,
    /*clock*/
};