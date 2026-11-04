#pragma once
#include <string>
namespace qol {
inline std::string wallet(int bolts){ return "Vendor - "+std::to_string(bolts)+" bolts"; }
inline std::string acquired(int id,int bolts){ return "Item "+std::to_string(id)+" acquired - "+std::to_string(bolts)+" bolts"; }
}
