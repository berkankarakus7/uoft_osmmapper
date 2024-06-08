/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   helper.h
 * Author: duajasja
 *
 * Created on March 23, 2023, 2:25 a.m.
 */

#ifndef M1_HELPER_H
#define M1_HELPER_H
#pragma once

#include "globals.h"

void helper_streetSegments();
void helper_removeDuplicates(std::vector<StreetSegmentIdx> &vec);
void helper_streetsAtIntersections();
void helper_intersectionsAlongStreet();
void helper_intersectionOfTwoStreets(StreetIdx&, StreetIdx&);
void helper_adjacentIntersections();
void correctedStreetPrefix(std::string &street_prefix);
std:: unordered_map<StreetIdx, std:: vector<StreetSegmentIdx>> allSegmentsInSameStreet();
std:: pair<std:: pair<double, double>, std:: pair<double, double>> convertLatLon( LatLon point_1, LatLon point_2);
std:: unordered_map<std:: string, std:: vector<int>> loadStreetsName();
std:: unordered_map<OSMID , std:: vector< std:: pair<std:: string, std:: string>>> LoadOSMIdNodeTags();

#endif /* HELPER_H */

