#pragma once

#include <iostream>

template<typename... Text>
inline void send_error(Text... data){
    (std::cout << "ERROR: " << ... << data) << "\n";
}

template<typename... Text>
inline void send_log(Text... data){
    (std::cout << "LOG: " << ... << data) << "\n";
}

template<typename... Text>
inline void send_debug(Text... data){
    (std::cout << "DEBUG: " << ... << data) << "\n";
}