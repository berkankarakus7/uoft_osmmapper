/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include "globals.h"

#include <vector>
#include <string>

double computePathTravelTime(const std::vector<StreetSegmentIdx>& path, 
                             const double turn_penalty){
    int num_turns = 0;
    double travel_time = 0;
    for(int i = 0; i < path.size(); i++){
        travel_time += street_segment_travel_time[path[i]];
        if(i < path.size()-1){
            if(getStreetSegmentInfo(path[i]).streetID != getStreetSegmentInfo(path[i+1]).streetID) num_turns ++;
        }
    }
    travel_time += num_turns * turn_penalty;
    return travel_time;
}

std::vector<StreetSegmentIdx> findPathBetweenIntersections(const std::pair<IntersectionIdx, IntersectionIdx> intersect_ids,
                                                           const double turn_penalty){
    std::vector<int> path;
    path.clear();
    if(intersect_ids.first < 0 || intersect_ids.second < 0 ||
       intersect_ids.first >= getNumIntersections() || intersect_ids.second >= getNumIntersections()){
           return path;
       }
    if(intersect_ids.first == intersect_ids.second){
        return path;
    }
    return searchPath(intersect_ids.first, intersect_ids.second, turn_penalty);
}