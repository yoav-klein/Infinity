
#include "../singleton/handleton/handleton.hpp"
#include "factory.hpp"
#include "animals.hpp"

using namespace ilrd;

#define FACTORY_ANIMAL Factory<Animal, std::string, int>

InitHandleton(FACTORY_ANIMAL)

