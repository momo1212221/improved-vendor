#include <cassert>
#include "logic.hpp"
int main(){ assert(qol::wallet(500)=="Vendor - 500 bolts"); assert(qol::acquired(7,300).find("7")!=std::string::npos); }
