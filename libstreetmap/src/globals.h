/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   globals.h
 * Author: duajasja
 *
 * Include this file in your source file if you need to use any of the variables defined below.
 * Consequently, whenever you define a global variable in any source file, add its extern 
 * declaration below.
 * 
 * Created on March 12, 2023, 5:38 a.m.
 */

#ifndef GLOBALS_H
#define GLOBALS_H
#pragma once

#include <iostream>
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "stdlib.h"
#include <sstream>
#include <stdbool.h>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "ezgl/application.hpp"
#include "ezgl/control.hpp"
#include "ezgl/graphics.hpp"
#include "m1_helper.h"
#include "m2_helper.h"
#include "m3_helper.h"

//Global variables
#define NO_EDGE -1
#define INF 9999999

enum FeatureGeometry{
    POINT = 1, LINE = 2, POLYGON = 3
};

typedef struct FeatureColour{
    int red;
    int green;
    int blue;
} FeatureColour;

typedef struct FeatureData{
    int feature_id;
    std::string feature_name;
    TypedOSMID feature_OSM_id;
    FeatureType feature_type;
    FeatureGeometry feature_geometry;
    std::vector<LatLon> feature_points;
    LatLon centroid;
    std::vector<ezgl::point2d> feature_points_xy;
    ezgl::point2d centroid_xy;
    FeatureColour colour;
    double area;
    int visible_at;
} FeatureData;

typedef struct POIData{
    int poi_id;
    ezgl::point2d location_xy;
    std::string poi_name;
    std::string poi_type;
    OSMID poi_osm_id;
    bool is_clustered;
    bool is_visible;
    LatLon location;
    std::string poi_class;
} POIData;

//structs
typedef struct Intersection_data {
    ezgl::point2d xy_loc;
    std::string name;
    bool highlight;
} Intersection_data;

typedef struct Street_segment_data {
    StreetSegmentInfo ssi;
    std:: string street_name;
    ezgl:: point2d start;
    ezgl:: point2d end;
    std:: vector<ezgl:: point2d> curve_points;
    std:: pair<ezgl:: point2d, ezgl:: point2d> longest_curve_interval;
    int red = 255;
    int green = 255;
    int blue = 255;
    bool highlight = false;

} Street_segment_data;


typedef struct City_data{
    std:: string city_name;
    bool isLoad = false;
    std:: string url;
} City_data;

typedef struct Node{
    int id;
    std::vector<StreetSegmentIdx> outgoing_edges;
    bool visited;
    int reaching_edge;
    float best_time;
    float best_f;
} Node;

typedef struct WaveElement{
    int node_id;
    int edge_id;
    float travel_time; //Total time to reach node
    float f;
    WaveElement(int n, int e, float t, float h){
        node_id = n;
        edge_id = e;
        travel_time = t;
        f = t + h;
    }
} WaveElement;


extern std::vector<double> street_segment_length;
extern std::vector<double> street_segment_travel_time;
extern std::unordered_map<std::string,std::vector<int>> all_streets;
extern std::vector<std::vector<StreetSegmentIdx>> street_segments;
extern std::vector<std::vector<StreetIdx>> streets_at_intersection;
extern std::vector<std::vector<IntersectionIdx>> intersections_along_street;
extern std::unordered_map<StreetIdx, std::vector<StreetSegmentIdx>> all_segments_in_street;
extern std::vector<std::vector<IntersectionIdx>> adjacent_intersections;
extern std::unordered_map<OSMID, std::vector<std::pair<std::string, std::string>>> OSM_id_tag_pairs;

extern bool show_intersection_data;
extern bool show_poi_data;
extern bool show_route;
extern bool set_route;
extern int click_counter;
extern bool draw_name;


extern std::vector<std::vector<FeatureData>> features;
extern ezgl::rectangle initial_world;
extern std::vector<POIData> pois;
extern std::vector<std::string> types_of_pois;

extern double max_lat;
extern double min_lat;
extern double max_lon;
extern double min_lon;

extern std:: vector<Intersection_data> intersections;
extern std:: vector<Street_segment_data> street_segs;
extern std:: vector<City_data> all_cities;
extern std:: vector<std::string> all_city_names;

extern std:: vector<StreetSegmentIdx> all_highways;
extern std:: unordered_map<OSMID , std:: vector< std:: pair<std:: string, std:: string>>> OSM_way_tags;
extern std:: vector<StreetSegmentIdx> motorways_trunks;
extern std:: vector<StreetSegmentIdx> primary_highways;
extern std:: vector<StreetSegmentIdx> secondary_highways;
extern std:: vector<StreetSegmentIdx> tertiary_highways;
extern std:: vector<StreetSegmentIdx> roads;
extern std:: vector<StreetSegmentIdx> unclassified;
extern std:: vector<StreetSegmentIdx> residential;
extern std:: vector<StreetSegmentIdx> living_street;
extern std:: vector<StreetSegmentIdx> service;
extern std:: vector<StreetSegmentIdx> pedestrian;
extern std:: vector<StreetSegmentIdx> other_highways;
extern GtkEntry *startSearchEntry;
// extern ezgl::renderer *g;

extern std::vector<Node> nodes;
extern std::vector<StreetSegmentInfo> street_segment_info;
extern std::vector<StreetSegmentIdx> path_to_show;
extern std::pair<IntersectionIdx, IntersectionIdx> navigation_points;
extern int start_inter;
extern int end_inter;
// extern float max_speed;


//m3
extern std:: vector<StreetSegmentIdx> path_created;
#endif /* GLOBALS_H */

