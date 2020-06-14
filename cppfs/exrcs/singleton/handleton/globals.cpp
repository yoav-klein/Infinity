#include "handleton.hpp"
#include "dog.hpp"
#include "holder.hpp"

using namespace ilrd;

#define TYPE Holder<float, int>

//using TYPE = Holder<float, int>

//InitHandleton(Dog)

template class Handleton<Dog>;

template class Handleton<Holder<float, int> >;
//InitHandleton(TYPE)
