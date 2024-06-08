/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   m2_helper.h
 * Author: duajasja
 *
 * Created on March 24, 2023, 11:12 PM
 */

#ifndef M2_HELPER_H
#define M2_HELPER_H
#pragma once

#include "globals.h"

//Forward declaration of structs
struct FeatureColour;
struct FeatureData;
struct POIData;
struct Intersection_data;
struct Street_segment_data;
struct Street_data;
struct City_data;


//Loading Data Structures
void loadFeatureData();
void loadPOIData();
void loadCities();
void loadIntersections();
void loadStreetSegments();
void loadStreetTypes(StreetSegmentIdx seg_idx);
void loadOSMWays_new();


//Drawing the map
void drawFeatures(ezgl::renderer *);
void drawPOIs(ezgl::renderer*);
void drawVectorOfFeatures(std::vector<FeatureData> &, ezgl::renderer*);
void drawIntersections( ezgl::renderer *g);
void drawStreets(ezgl::renderer *g);
void displayStreetTypes( std:: vector<StreetSegmentIdx> street_type, ezgl::renderer *g );
void drawStreetNames_new(ezgl:: renderer *g, StreetSegmentIdx seg_idx);


//Helper functions
void switch_cities( GtkComboBoxText* thisBox, ezgl::application* application);
void changeMap( ezgl::application* application);
int getCurrentZoom(ezgl::renderer *g);
ezgl::point2d calculateCentroid(std::vector<ezgl::point2d> &);
bool pointIsInVisibleWorld(ezgl::point2d, ezgl::renderer *);
bool polygonIsInVisibleWorld(std::vector<ezgl::point2d>&, ezgl::renderer*);
ezgl::rectangle calculatePolygonBorder(std::vector<ezgl::point2d> &); 
double calculateLineFeatureLength(FeatureData &feature);
void setRGBValues(FeatureColour &colour, int red, int green, int blue);
double xFromLon(double lon);
double yFromLat(double lat);
double lonFromX(double x);
double latFromY(double y);
void calculateBorder();
std::string getOSMWayTagValue (OSMID wayOSMid, std::string key);
void setRotationOfStreetNames(ezgl:: renderer *g, ezgl:: point2d &from_seg, ezgl:: point2d &to_seg);
bool checkStreetSegmentCurvesInZoomLevel( ezgl:: renderer *g, StreetSegmentIdx seg_idx);
bool drawOnCurrentZoom(ezgl:: point2d p_street_seg, ezgl::renderer * g);
void findLongestDistanceBetweenCurvePoints(StreetSegmentIdx idx);

//m3

#endif /* M2_HELPER_H */

