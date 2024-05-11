#pragma once

#include <stdint.h>

static uint32_t current_id = -1;

inline uint32_t getNextRuntimeId(){
    current_id += 1;
    return current_id;
}