 /*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   m3_output_funcs.h
 * Author: karakusb
 *
 * Created on March 26, 2023, 10:26 PM
 */

#ifndef M3_OUTPUT_FUNCS_H
#define M3_OUTPUT_FUNCS_H
#pragma once

#include "globals.h"

std:: vector<std:: string> setPathDirections( std:: vector<StreetSegmentIdx> path_found);

void writeDirections( std:: vector<StreetSegmentIdx> path_found);

std:: string writeDirectionsAsString( std:: vector<StreetSegmentIdx> path_found);

void setVectorPointsPositions( LatLon *node_points, StreetSegmentIdx cur_seg, StreetSegmentIdx next_seg);

void setVectorPointsIds( IntersectionIdx *node_points, StreetSegmentIdx cur_seg, StreetSegmentIdx next_seg);

int setLastVectorTipId( StreetSegmentIdx cur_seg, StreetSegmentIdx next_seg);

double calculateDeterminant( LatLon *node_points);

int findTravelSegmentLength( StreetSegmentIdx seg_idx, std:: vector<std:: string> directions, std:: vector<StreetSegmentIdx> path_found);

void displayDirectionsButton( GtkWidget* /*widget*/, ezgl:: application* application);

void displayHelpButton( GtkWidget* /*widget*/, ezgl:: application* application);


#endif /* M3_OUTPUT_FUNCS_H */

