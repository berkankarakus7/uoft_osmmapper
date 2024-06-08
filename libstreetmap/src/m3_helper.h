#ifndef M3_HELPER_H
#define M3_HELPER_H
#pragma once

#include "globals.h"
#define AVERAGE_SPEED 30.0f
extern float max_speed;
extern float min_pace;

void loadNodes();
void findAverageSpeed();
std::vector<StreetSegmentIdx> searchPath(int source, int destination, double turn_penalty);
std::vector<StreetSegmentIdx> traceBack(int destination);

inline float getHeuristic(int current_node, int destination_node){
    LatLon pos1 = getIntersectionPosition(current_node);
    LatLon pos2 = getIntersectionPosition(destination_node);
    float heuristic = findDistanceBetweenTwoPoints(pos1, pos2) * min_pace;
    return heuristic;
}

#endif