///*
// * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
// * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
// */
//
///* 
// * File:   m2_berkan_funcs.h
// * Author: karakusb
// *
// * Created on March 6, 2023, 2:13 a.m.
// */
//
//#ifndef M2_BERKAN_FUNCS_H
//#define M2_BERKAN_FUNCS_H
//
//#include <iostream>
//
//#include "StreetsDatabaseAPI.h"
//#include "stdlib.h"
//#include "OSMDatabaseAPI.h"
//#include "ezgl/application.hpp"
//#include "ezgl/graphics.hpp"
//#include <math.h>
//#include <stdbool.h>
//#include "m1_helper.h"
//#include "globals.h"
//#include "m1_helper.h"
//#include "m2_helper.h"
//
////external vars
//
//
////global vars
//extern std:: vector<Intersection_data> intersections;
//extern std:: vector<Street_segment_data> street_segs;
//extern std:: vector<Street_data> streets;
//extern std:: unordered_map<OSMID, std::string> OSM_highway_tags;
//extern std:: vector<City_data> all_cities;
////extern std:: vector<std:: string> all_city_names;
//extern ezgl:: application *appPtr;
//int rgb[3] = {0, 0, 0};
//extern std:: vector<std:: string> all_city_names;
//std:: vector<StreetSegmentIdx> all_highways;
//std:: unordered_map<OSMID , std:: vector< std:: pair<std:: string, std:: string>>> OSM_way_tags;
//std:: vector<StreetSegmentIdx> motorways_trunks;
//std:: vector<StreetSegmentIdx> primary_highways;
//std:: vector<StreetSegmentIdx> secondary_highways;
//std:: vector<StreetSegmentIdx> tertiary_highways;
//std:: vector<StreetSegmentIdx> roads;
//std:: vector<StreetSegmentIdx> unclassified;
//std:: vector<StreetSegmentIdx> residential;
//std:: vector<StreetSegmentIdx> living_street;
//std:: vector<StreetSegmentIdx> service;
//std:: vector<StreetSegmentIdx> pedestrian;
//std:: vector<StreetSegmentIdx> other_highways;
//
//
//double max_lat;
//double min_lat;
//double max_lon;
//double min_lon;
//bool high_res;
//bool highmid_res;
//bool mid_res;
//bool midlow_res;
//bool low_res;
//bool draw_name;
//int street_width_factor;
////ezgl::rectangle initial_world;
//
////helper funcs
//void loadCities();
//void loadIntersections();
//void drawIntersections(ezgl::renderer *g);
//void loadStreetSegments();
//void drawStreetSegments(ezgl::renderer *g);
//void loadOSMWays();
//void setResolution(ezgl:: renderer *g);
//void setStreetSegmentOnZoomLevel( ezgl:: renderer *g, StreetSegmentIdx seg_idx);
//void calculateBorder();
//void drawStreetNames(ezgl:: renderer *g, StreetSegmentIdx seg_idx);
//void setRotationOfStreetNames(ezgl:: renderer *g, ezgl:: point2d &from_seg, ezgl:: point2d &to_seg);
//void findLongestDistanceBetweenCurvePoints(StreetSegmentIdx idx);
//bool drawOnCurrentZoom(ezgl:: point2d p_street_seg, ezgl::renderer * g);
//bool checkStreetSegmentCurvesInZoomLevel( ezgl:: renderer *g, StreetSegmentIdx seg_idx);
//void switch_cities( GtkComboBoxText* thisBox, ezgl::application* application);
//void changeMap( ezgl:: application *application);
//
//
//void drawStreets(ezgl::renderer *g);
//void loadOSMWays_new();
//std::string getOSMWayTagValue (OSMID wayOSMid, std::string key);
//void loadStreetTypes( StreetSegmentIdx seg_idx);
//void displayStreetTypes( std:: vector<StreetSegmentIdx> street_type, ezgl::renderer *g );
//void drawStreetNames_new(ezgl:: renderer *g, StreetSegmentIdx seg_idx);
//
//// void loadCities() {
////     all_cities.resize( 19);
//// //    all_city_names.resize( 19);
//    
//    
////     all_cities[0].city_name = "beijing";
////     all_cities[0].isLoad = false;
////     all_cities[0].url = "/cad2/ece297s/public/maps/beijing_china.streets.bin";
//    
////     all_cities[1].city_name = "cairo";
////     all_cities[1].isLoad = false;
////     all_cities[1].url = "/cad2/ece297s/public/maps/cairo_egypt.streets.bin";
//            
////     all_cities[2].city_name = "capeTown";
////     all_cities[2].isLoad = false;
////     all_cities[2].url = "/cad2/ece297s/public/maps/cape-town_south-africa.streets.bin";
//            
////     all_cities[3].city_name = "goldenHorseshoe";
////     all_cities[3].isLoad = false;
////     all_cities[3].url = "/cad2/ece297s/public/maps/golden-horseshoe_canada.streets.bin";
//            
////     all_cities[4].city_name = "hamilton";
////     all_cities[4].isLoad = false;
////     all_cities[4].url = "/cad2/ece297s/public/maps/hamilton_canada.streets.bin";
//            
////     all_cities[5].city_name = "hongKong";
////     all_cities[5].isLoad = false;
////     all_cities[5].url = "/cad2/ece297s/public/maps/hong-kong_china.streets.bin";
//            
////     all_cities[6].city_name = "iceland";
////     all_cities[6].isLoad = false;
////     all_cities[6].url = "/cad2/ece297s/public/maps/iceland.streets.bin";
//            
////     all_cities[7].city_name = "interlaken";
////     all_cities[7].isLoad = false;
////     all_cities[7].url = "/cad2/ece297s/public/maps/interlaken_switzerland.streets.bin";
//            
////     all_cities[8].city_name = "kyiv";
////     all_cities[8].isLoad = false;
////     all_cities[8].url = "/cad2/ece297s/public/maps/kyiv_ukraine.streets.bin";
//            
////     all_cities[9].city_name = "london";
////     all_cities[9].isLoad = false;
////     all_cities[9].url = "/cad2/ece297s/public/maps/london_england.streets.bin";
//            
////     all_cities[10].city_name = "newDelhi";
////     all_cities[10].isLoad = false;
////     all_cities[10].url = "/cad2/ece297s/public/maps/new-delhi_india.streets";
//            
////     all_cities[11].city_name = "newYork";
////     all_cities[11].isLoad = false;
////     all_cities[11].url = "/cad2/ece297s/public/maps/new-york_usa.streets.bin";
//            
////     all_cities[12].city_name = "rioDeJaneiro";
////     all_cities[12].isLoad = false;
////     all_cities[12].url = "/cad2/ece297s/public/maps/rio-de-janeiro_brazil.streets.bin";
//            
////     all_cities[13].city_name = "saintHelena";
////     all_cities[13].isLoad = false;
////     all_cities[13].url = "/cad2/ece297s/public/maps/saint-helena.streets.bin";
//            
////     all_cities[14].city_name = "singapore";
////     all_cities[14].isLoad = false;
////     all_cities[14].url = "/cad2/ece297s/public/maps/singapore.streets.bin";
//            
////     all_cities[15].city_name = "sydney";
////     all_cities[15].isLoad = false;
////     all_cities[15].url = "/cad2/ece297s/public/maps/sydney_australia.streets.bin";
//            
////     all_cities[16].city_name = "tehran";
////     all_cities[16].isLoad = false;
////     all_cities[16].url = "/cad2/ece297s/public/maps/tehran_iran.streets.bin";
//            
////     all_cities[17].city_name = "tokyo";
////     all_cities[17].isLoad = false;
////     all_cities[17].url = "/cad2/ece297s/public/maps/tokyo_japan.streets.bin";
//            
////     all_cities[18].city_name = "toronto";
////     all_cities[18].isLoad = false;
////     all_cities[18].url = "/cad2/ece297s/public/maps/toronto_canada.streets.bin";
//    
//// //    for ( std:: size_t city_idx = 0; city_idx < all_cities.size(); city_idx++ ) {
//// //        all_city_names.push_back( all_cities[city_idx].city_name);
//// //    }
//// }
//
//// void switch_cities( GtkComboBoxText* thisBox, ezgl::application* application) {
//    
////     for ( std:: size_t city_idx = 0; city_idx < all_cities.size(); ++city_idx ) {
////        if ( gtk_combo_box_text_get_active_text(thisBox) == all_cities[city_idx].city_name ) {
////             all_cities[city_idx].isLoad = true;
////             application->refresh_drawing();
////         } 
////     }
//    
//// }
//
//// void changeMap() {
////     for ( std:: size_t city_idx = 0; city_idx < all_cities.size(); ++city_idx ) {
////         if (  all_cities[city_idx].isLoad ) {
////             std::cout << "You have now selected to load ... " << all_cities[city_idx].city_name << std::endl;
////             all_cities[city_idx].isLoad = false;
////             closeMap();
////             loadMap(all_cities[city_idx].url);
////             calculateBorder();
////             ezgl:: rectangle change_initial_world({xFromLon(min_lon), yFromLat(min_lat)},{xFromLon(max_lon), yFromLat(max_lat)});
////             appPtr->change_canvas_world_coordinates(appPtr->get_main_canvas_id(), change_initial_world);
////             appPtr->refresh_drawing();
////             drawMap();
////         } 
////     }
//    
//    
//// }
//
//void loadIntersections() {
//    
//    intersections.resize(getNumIntersections());
//    for (IntersectionIdx id = 0; id < getNumIntersections(); ++id ) { 
//
//        intersections[id].xy_loc.x = xFromLon(getIntersectionPosition(id).longitude());
//        intersections[id].xy_loc.y = yFromLat(getIntersectionPosition(id).latitude());
//        intersections[id].name = getIntersectionName(id);
//        intersections[id].highlight = false;
//    }
//}
//
//void calculateBorder() {
//    max_lat = getIntersectionPosition(0).latitude();
//    min_lat = max_lat;
//    max_lon = getIntersectionPosition(0).longitude();
//    min_lon = max_lon;
//    
//    for (IntersectionIdx id = 0; id < getNumIntersections(); ++id ) { 
//        
//        max_lat = std::max(max_lat, getIntersectionPosition(id).latitude());
//        min_lat = std::min(min_lat, getIntersectionPosition(id).latitude());
//        max_lon = std::max(max_lon, getIntersectionPosition(id).longitude());
//        min_lon = std::min(min_lon, getIntersectionPosition(id).longitude());
//        
//    }
//}
//
//void drawIntersections( ezgl::renderer *g) {
//    for ( std:: size_t inter_id = 0; inter_id < intersections.size(); ++inter_id ) {
//        if(intersections[inter_id].highlight){
//        if ( drawOnCurrentZoom( intersections[inter_id].xy_loc, g) ) {
//
//            ezgl::point2d inter_loc;
//            inter_loc.x = intersections[inter_id].xy_loc.x;
//            inter_loc.y = intersections[inter_id].xy_loc.y;
//            
//            ezgl::surface *png_surface = ezgl::renderer::load_png("libstreetmap/resources/map_marker_intersection.png");
//            g -> draw_surface(png_surface, inter_loc);
//            ezgl::renderer::free_surface(png_surface);
//        }
//        }
//    }
//}
//
//void loadStreetSegments() {
//    street_segs.resize(getNumStreetSegments());
//    for ( StreetSegmentIdx idx = 0; idx < getNumStreetSegments(); idx++ ) {
//        StreetSegmentInfo ssi_found = getStreetSegmentInfo(idx);
//        street_segs[idx].ssi = ssi_found;
//        street_segs[idx].start.x = xFromLon( getIntersectionPosition(ssi_found.from).longitude() );
//        street_segs[idx].start.y = yFromLat( getIntersectionPosition(ssi_found.from).latitude() );
//        street_segs[idx].end.x = xFromLon( getIntersectionPosition(ssi_found.to).longitude() );
//        street_segs[idx].end.y = yFromLat( getIntersectionPosition(ssi_found.to).latitude() );
//        
//        street_segs[idx].street_name = getStreetName(street_segs[idx].ssi.streetID);
//        
//        street_segs[idx].curve_points.resize(ssi_found.numCurvePoints);
//        for ( int numCurvePoints = 0; numCurvePoints < ssi_found.numCurvePoints; numCurvePoints++ ) {
//            street_segs[idx].curve_points[numCurvePoints].x
//                    =  xFromLon( getStreetSegmentCurvePoint(idx, numCurvePoints).longitude() );
//            street_segs[idx].curve_points[numCurvePoints].y
//                    =  yFromLat( getStreetSegmentCurvePoint(idx, numCurvePoints).latitude() );
//            if (numCurvePoints > 1) {
//                int middle_curv_pt1 = (int) ((numCurvePoints/2)-1);
//                int middle_curv_pt2 = (int) (numCurvePoints/2);
//                street_segs[idx].middle_curve_pts.first.x =  xFromLon( getStreetSegmentCurvePoint(idx, middle_curv_pt1).longitude() );
//                street_segs[idx].middle_curve_pts.first.y =  yFromLat( getStreetSegmentCurvePoint(idx, middle_curv_pt1).latitude() );
//                street_segs[idx].middle_curve_pts.second.x =  xFromLon( getStreetSegmentCurvePoint(idx, middle_curv_pt2).longitude() );
//                street_segs[idx].middle_curve_pts.second.y =  yFromLat( getStreetSegmentCurvePoint(idx, middle_curv_pt2).latitude() );
//            }
//        }   
//        loadStreetTypes( idx);
//    }
//}
//
//void drawStreetSegments( ezgl::renderer *g) {
//    g->set_color (ezgl::WHITE);
//    for ( std:: size_t idx = 0; idx < street_segs.size(); idx++ ) {
//        street_segs[idx].draw = false;
//        setStreetSegmentOnZoomLevel( g, idx);
//        if ( street_segs[idx].draw ) {
//            if ( street_segs[idx].ssi.numCurvePoints == 0 ) {
//    //            g->set_color (ezgl::RED);
//                if ( drawOnCurrentZoom( street_segs[idx].start, g)
//                            || drawOnCurrentZoom(street_segs[idx].end, g) ) {
//                    g->draw_line({street_segs[idx].start.x, street_segs[idx].start.y},
//                    {street_segs[idx].end.x, street_segs[idx].end.y} );
//                }
//                if ( draw_name ) {
//                        drawStreetNames(g, idx);
//                }
//            }
//            else if ( street_segs[idx].ssi.numCurvePoints == 1 ) {
//    //            g->set_color (ezgl::GREEN);
//    //            findLongestDistanceBetweenCurvePoints(idx);
//                if ( drawOnCurrentZoom( street_segs[idx].curve_points[0], g)
//                        || drawOnCurrentZoom(street_segs[idx].start, g) ) {
//                    g->draw_line({street_segs[idx].start.x, street_segs[idx].start.y},
//                    {street_segs[idx].curve_points[0].x, street_segs[idx].curve_points[0].y} );
//                }
//                if ( drawOnCurrentZoom( street_segs[idx].curve_points[0], g)
//                        || drawOnCurrentZoom(street_segs[idx].end, g) ) {
//                    g->draw_line({street_segs[idx].curve_points[0].x, street_segs[idx].curve_points[0].y},
//                    {street_segs[idx].end.x, street_segs[idx].end.y} );
//                    }
//                    if ( draw_name ) {
//                        drawStreetNames(g, idx);
//                    }
//                }
//            else {
//    //            g->set_color (ezgl::BLUE);
//    //            findLongestDistanceBetweenCurvePoints(idx);
//                if ( drawOnCurrentZoom( street_segs[idx].curve_points[0], g)
//                        || drawOnCurrentZoom(street_segs[idx].start, g) ) {
//                    g->draw_line({street_segs[idx].start.x, street_segs[idx].start.y},
//                    {street_segs[idx].curve_points[0].x, street_segs[idx].curve_points[0].y} );
//                    }
//
//                for ( std:: size_t i = 0; i < street_segs[idx].curve_points.size() - 1; i++ ) {
//                    if ( drawOnCurrentZoom( street_segs[idx].curve_points[i], g) 
//                            || drawOnCurrentZoom( street_segs[idx].curve_points[i+1], g) ) {
//                        g->draw_line({street_segs[idx].curve_points[i].x, street_segs[idx].curve_points[i].y},
//                        {street_segs[idx].curve_points[i+1].x, street_segs[idx].curve_points[i+1].y} );
//                        }
//                    }
//
//                int curv_num = street_segs[idx].ssi.numCurvePoints;
//                if ( drawOnCurrentZoom( street_segs[idx].curve_points[curv_num-1], g)
//                        || drawOnCurrentZoom(street_segs[idx].end, g) ) {
//                    g->draw_line({street_segs[idx].curve_points[curv_num - 1].x, street_segs[idx].curve_points[curv_num - 1].y},
//                    {street_segs[idx].end.x, street_segs[idx].end.y} );
//                    }
//                    if ( draw_name ) {
//                        drawStreetNames(g, idx);
//                    }
//                }
//            }
//        }
//}
//
//
//void drawStreets(ezgl::renderer *g){
//    int zoom = getCurrentZoom(g);
//    
//    draw_name = false;
//    g->set_line_width( 0.5);
//    if ( 2 >= zoom && zoom > 0){
//        
//        if ( zoom >= 2 ) {
//            draw_name = true;
//        }
//        g->set_color(180, 180, 200);
//        g->set_line_width( 6);
//        displayStreetTypes( motorways_trunks, g);
//        
//        g->set_color(180, 180, 200);
//        g->set_line_width( 5);
//        displayStreetTypes( primary_highways, g);
//        
////        g->set_color(180, 180, 200);
////        g->set_color(ezgl:: RED);
////        g->set_line_width( 2);
////        displayStreetTypes( secondary_highways, g);
//        
//    }
//    if ( 5 >= zoom && zoom > 2 ){
//        g->set_color(180, 180, 200);
//        g->set_line_width( 4);
//        displayStreetTypes( motorways_trunks, g);
//        
//        g->set_color(180, 180, 200);
//        g->set_line_width( 3.5);
//        displayStreetTypes( primary_highways, g);
//        
//        g->set_color(180, 180, 200);
//        g->set_line_width( 3.75);
//        displayStreetTypes( secondary_highways, g);
//    }
//    if ( 8 >= zoom && zoom > 5 ){
//        g->set_color(180, 180, 200);
//        g->set_line_width( 5);
//        displayStreetTypes( motorways_trunks, g);
//        
//        g->set_color(180, 180, 200);
//        g->set_line_width( 4.5);
//        displayStreetTypes( primary_highways, g);
//        
//        g->set_color(180, 180, 200);
//        g->set_line_width( 4);
//        displayStreetTypes( secondary_highways, g);
//        
//        g->set_color(180, 180, 200);
//        g->set_line_width( 3.5);
//        displayStreetTypes( tertiary_highways, g);
//        
//        if ( zoom >= 7 ) {
//            g->set_color( ezgl:: WHITE );
//            g->set_line_width( 2.75);
//            displayStreetTypes( roads, g);
//            g->set_line_width( 2.5);
//            displayStreetTypes( unclassified, g);
//            g->set_line_width( 2.5);
//            displayStreetTypes( residential, g);
//            g->set_line_width( 2.5);
//            displayStreetTypes( living_street, g);
//            g->set_line_width( 2.5);
//            displayStreetTypes( service, g);
//            g->set_line_width( 2.25);
//            displayStreetTypes( pedestrian, g);
//        }
//    }
//    if ( 11 >= zoom && zoom > 8 ){
//        g->set_color(180, 180, 200);
//        g->set_line_width( 8);
//        displayStreetTypes( motorways_trunks, g);
//        
//        g->set_color(180, 180, 200);
//        g->set_line_width( 7);
//        displayStreetTypes( primary_highways, g);
//        
//        g->set_color(180, 180, 200);
//        g->set_line_width( 6);
//        displayStreetTypes( secondary_highways, g);
//        
//        g->set_color(180, 180, 200);
//        g->set_line_width( 5);
//        displayStreetTypes( tertiary_highways, g);
//        
//        g->set_color( ezgl:: WHITE );
//        g->set_line_width( 4);
//        displayStreetTypes( roads, g);
//        g->set_line_width( 4);
//        displayStreetTypes( unclassified, g);
//        g->set_line_width( 3.5);
//        displayStreetTypes( residential, g);
//        g->set_line_width( 3.5);
//        displayStreetTypes( living_street, g);
//        g->set_line_width( 3.5);
//        displayStreetTypes( service, g);
//        g->set_line_width( 3.35);
//        displayStreetTypes( pedestrian, g);
//    }
//    if ( zoom > 11 ){
//        g->set_color(180, 180, 200);
//        g->set_line_width( 10);
//        displayStreetTypes( motorways_trunks, g);
//        
//        g->set_color(180, 180, 200);
//        g->set_line_width( 9);
//        displayStreetTypes( primary_highways, g);
//        
//        g->set_color(180, 180, 200);
//        g->set_line_width( 8);
//        displayStreetTypes( secondary_highways, g);
//        
//        g->set_color(180, 180, 200);
//        g->set_line_width( 7);
//        displayStreetTypes( tertiary_highways, g);
//        
//        g->set_color( ezgl:: WHITE );
//        g->set_line_width( 5.5);
//        displayStreetTypes( roads, g);
//        g->set_line_width( 5.5);
//        displayStreetTypes( unclassified, g);
//        g->set_line_width( 5);
//        displayStreetTypes( residential, g);
//        g->set_line_width( 5);
//        displayStreetTypes( living_street, g);
//        g->set_line_width( 5);
//        displayStreetTypes( service, g);
//        g->set_line_width( 4.5);
//        displayStreetTypes( pedestrian, g);
//        
//        
//    }
//    
//    
////    g->set_line_width( 0.5);
////    if (zoom > 0){
////        g->set_color(180, 180, 200);
////        g->set_line_width( 3);
////        rgb[0] = 180;
////        rgb[1] = 180;
////        rgb[2] = 200;
////        displayStreetTypes( motorways_trunks, g);
////        
////        g->set_color(180, 180, 200);
////        g->set_line_width( 2.75);
////        rgb[0] = 180;
////        rgb[1] = 180;
////        rgb[2] = 200;
////        displayStreetTypes( primary_highways, g);
////        
////        g->set_color(192, 192, 192);
////        g->set_line_width( 2);
////        rgb[0] = 192;
////        rgb[1] = 192;
////        rgb[2] = 192;
////        displayStreetTypes( secondary_highways, g);
////        
////    }
////    if (zoom > 2){
////        g->set_color(180, 180, 200);
////        g->set_line_width( 2.5);
////        displayStreetTypes( motorways_trunks, g);
////        
////        g->set_color(180, 180, 200);
////        g->set_line_width( 2.25);
////        displayStreetTypes( primary_highways, g);
////        
////        g->set_color(192, 192, 192);
////        g->set_line_width( 1.75);
////        displayStreetTypes( secondary_highways, g);
////    }
////    if (zoom > 3){
////        
////        g->set_color(180, 180, 200);
////        g->set_line_width( 2);
////        displayStreetTypes( motorways_trunks, g);
////        
////        g->set_color(180, 180, 200);
////        g->set_line_width( 2);
////        displayStreetTypes( primary_highways, g);
////        
////        g->set_color(192, 192, 192);
////        g->set_line_width( 1.75);
////        displayStreetTypes( secondary_highways, g);
////        g->set_color( ezgl:: WHITE );
////        g->set_line_width( 1.35);
////        displayStreetTypes( tertiary_highways, g);
////    }
////    if ( zoom <= 6 && zoom > 4 ){
////
////        g->set_color( ezgl:: WHITE );
////        g->set_line_width( 1.35);
////        displayStreetTypes( tertiary_highways, g);
////        
////        g->set_color( ezgl:: WHITE );
////        g->set_line_width( 1.25);
////        displayStreetTypes( roads, g);
////        g->set_line_width( 1);
////        displayStreetTypes( unclassified, g);
////        g->set_line_width( 1);
////        displayStreetTypes( residential, g);
////        g->set_line_width( 1);
////        displayStreetTypes( living_street, g);
////        g->set_line_width( 1);
////        displayStreetTypes( service, g);
////        g->set_line_width( 1);
////        displayStreetTypes( pedestrian, g);
////    }  
////    if ( zoom <= 8 && zoom > 6 ){
////        g->set_color( ezgl:: WHITE );
////        g->set_line_width( 0.75);
////        displayStreetTypes( roads, g);
////        g->set_line_width( 0.5);
////        displayStreetTypes( unclassified, g);
////        g->set_line_width( 0.5);
////        displayStreetTypes( residential, g);
////        g->set_line_width( 0.5);
////        displayStreetTypes( living_street, g);
////        g->set_line_width( 0.5);
////        displayStreetTypes( service, g);
////        g->set_line_width( 0.5);
////        displayStreetTypes( pedestrian, g);
////        
////    }
////    if ( zoom <= 10 && zoom > 8 ){
////        g->set_color( ezgl:: WHITE );
////        g->set_line_width( 1.275);
////        displayStreetTypes( roads, g);
////        g->set_line_width( 1.25);
////        displayStreetTypes( unclassified, g);
////        g->set_line_width( 1);
////        displayStreetTypes( residential, g);
////        g->set_line_width( 1);
////        displayStreetTypes( living_street, g);
////        g->set_line_width( 1);
////        displayStreetTypes( service, g);
////        g->set_line_width( 1);
////        displayStreetTypes( pedestrian, g);
////        
////        g->set_line_width( 0.5);
////        displayStreetTypes( other_highways, g);
////        
////    }   
////    if ( zoom <= 12 && zoom > 10 ){
////        g->set_color( ezgl:: WHITE );
////        g->set_line_width( 2.75);
////        displayStreetTypes( roads, g);
////        g->set_line_width( 2.5);
////        displayStreetTypes( unclassified, g);
////        g->set_line_width( 2.5);
////        displayStreetTypes( residential, g);
////        g->set_line_width( 2.5);
////        displayStreetTypes( living_street, g);
////        g->set_line_width( 2.5);
////        displayStreetTypes( service, g);
////        g->set_line_width( 2.5);
////        displayStreetTypes( pedestrian, g);
////        
////        g->set_line_width( 1.5);
////        displayStreetTypes( other_highways, g);
////        
////    }
//}
//
//
//void loadStreetTypes( StreetSegmentIdx seg_idx) {
//    
//    std:: string tag_value = getOSMWayTagValue(street_segs[seg_idx].ssi.wayOSMID, "highway");
//    if ( tag_value == "motorway" || tag_value == "motorway_link" 
//        || tag_value == "trunk" || tag_value == "trunk_link" ) {
//       
//        motorways_trunks.push_back( seg_idx);
//        return;
//    } else if ( tag_value == "primary" || tag_value == "primary_link" ) {
//        primary_highways.push_back( seg_idx);
//        return;
//    } else if ( tag_value == "secondary" || tag_value == "secondary_link" ) {
//        secondary_highways.push_back( seg_idx);
//        return;
//    } else if ( tag_value == "tertiary" || tag_value == "tertiary_link" ) {
//        tertiary_highways.push_back( seg_idx);
//        return;
//    } else if ( tag_value == "road" ) {
//        roads.push_back( seg_idx);
//        return;
//    } else if ( tag_value == "unclassified" || tag_value == "unclassified_link" ) {
//        unclassified.push_back( seg_idx);
//        return;
//    } else if ( tag_value == "residential" || tag_value == "residential_link" ) {
//        residential.push_back( seg_idx);
//        return;
//    } else if ( tag_value == "living_street" || tag_value == "living_street_link" ) {
//        living_street.push_back( seg_idx);
//        return;
//    } else if ( tag_value == "service" || tag_value == "service_link" ) {
//        service.push_back( seg_idx);
//        return;
//    } else if ( tag_value == "pedestrian" || tag_value == "pedestrian_link" ) {
//        pedestrian.push_back( seg_idx);
//        return;
//    } else {
//        other_highways.push_back( seg_idx);
//        return;
//    }
//       
//}
//
//void displayStreetTypes( std:: vector<StreetSegmentIdx> street_type, ezgl::renderer *g ) {
//    for ( std:: size_t idx = 0; idx < street_type.size(); idx++ ) {
//        Street_segment_data seg_data = street_segs[street_type[idx]];
//        if ( seg_data.ssi.numCurvePoints == 0 ) {
//            if ( drawOnCurrentZoom( seg_data.start, g)
//                        || drawOnCurrentZoom( seg_data.end, g) ) {
//                g->draw_line({seg_data.start.x, seg_data.start.y},
//                {seg_data.end.x, seg_data.end.y} );
//            }
//        }
//        else if ( seg_data.ssi.numCurvePoints == 1 ) {
//            if ( drawOnCurrentZoom( seg_data.curve_points[0], g)
//                    || drawOnCurrentZoom(seg_data.start, g) ) {
//                g->draw_line({seg_data.start.x, seg_data.start.y},
//                {seg_data.curve_points[0].x, seg_data.curve_points[0].y} );
//            }
//            if ( drawOnCurrentZoom( seg_data.curve_points[0], g)
//                    || drawOnCurrentZoom(seg_data.end, g) ) {
//                g->draw_line({seg_data.curve_points[0].x, seg_data.curve_points[0].y},
//                {seg_data.end.x, seg_data.end.y} );
//            }
//        }
//        else {
//            if ( drawOnCurrentZoom( seg_data.curve_points[0], g)
//                    || drawOnCurrentZoom(seg_data.start, g) ) {
//                g->draw_line({seg_data.start.x, seg_data.start.y},
//                {seg_data.curve_points[0].x, seg_data.curve_points[0].y} );
//            }
//            for ( std:: size_t i = 0; i < seg_data.curve_points.size() - 1; i++ ) {
//                if ( drawOnCurrentZoom( seg_data.curve_points[i], g) 
//                        || drawOnCurrentZoom( seg_data.curve_points[i+1], g) ) {
//                    g->draw_line({seg_data.curve_points[i].x, seg_data.curve_points[i].y},
//                    {seg_data.curve_points[i+1].x, seg_data.curve_points[i+1].y} );
//                }
//            }
//            int curv_num = seg_data.ssi.numCurvePoints;
//            if ( drawOnCurrentZoom( seg_data.curve_points[curv_num-1], g)
//                    || drawOnCurrentZoom( seg_data.end, g) ) {
//                g->draw_line({seg_data.curve_points[curv_num - 1].x, seg_data.curve_points[curv_num - 1].y},
//                {seg_data.end.x, seg_data.end.y} );
//            }
//        }
////        if ( draw_name == true ) {
////            drawStreetNames_new( g, street_type[idx]);
////        }
//    }
//}
//
//
////int getCurrentZoom(ezgl::renderer *g) {
////    double initial_area = initial_world.area();
////    double current_area = g -> get_visible_world().area();
////    if(current_area > initial_area) current_area = initial_area;
////    double ratio = (initial_area / current_area);
////    int zoom_level = floor(log(initial_area / current_area) / log(3)) + 1;
////
////
////    // Limit zoom level to 1-15
////    zoom_level = std::max(1, std::min(15, zoom_level));
////    return zoom_level;
////}
//
//void loadOSMWays_new() {
//    int num_ways = getNumberOfWays();
//    for ( int way_idx = 0; way_idx < num_ways; way_idx++ ) {
////        std:: vector<std:: pair<std:: string, std:: string>> stored_tags;
//        const OSMWay* way_osm = getWayByIndex( way_idx);
//        for ( int tag_count = 0; tag_count < getTagCount( way_osm); tag_count++ ) {
//            std:: pair<std:: string, std:: string> tag_pair_found = getTagPair( way_osm, tag_count);
//            OSM_way_tags[way_osm->id()].push_back( getTagPair( way_osm, tag_count));
//        }
//        
//    }
//
//}
//
//std::string getOSMWayTagValue (OSMID wayOSMid, std::string key) {
//    std:: string found_value = "";
////    std:: unordered_map<OSMID , std:: vector< std:: pair<std:: string, std:: string>>> osm_node_map = OSMIdNodeTags();
//    
//    std:: vector< std:: pair< std:: string, std:: string>> tag_pairs_found = OSM_way_tags[wayOSMid];
//    for ( int i = 0; i < tag_pairs_found.size(); i++ ) {
//        if ( tag_pairs_found[i].first == key ) {
//            found_value = tag_pairs_found[i].second;
//            return tag_pairs_found[i].second;
//        }
//    } 
//    return found_value;
//
//}
//
//
//void loadOSMWays() {
//    int num_ways = getNumberOfWays();
//    for ( int way_idx = 0; way_idx < num_ways; way_idx++ ) {
//        const OSMWay * way_found = getWayByIndex( way_idx);
//        
//        int num_tags = getTagCount( way_found);
//        for ( int tag_idx = 0; tag_idx < num_tags; tag_idx++ ) {
//            std:: pair<std:: string, std:: string> tag_found = getTagPair( way_found, tag_idx);
//            
//            if ( tag_found.first == "highway" ) {
//                std:: pair<OSMID, std:: string> insert_tag_highway = {way_found->id(), tag_found.second};
//                OSM_highway_tags[way_found->id()] = tag_found.second;
////                OSM_way_id_tag_pairs.insert( std:: pair<OSMID, std:: string>(way_found->id(), tag_found.second));
//            }
//        }
//    }
//}
//
////void loadStreets() {
////    
////    // loop through each street segment
////    streets.resize( getNumStreets());
////    for ( std:: size_t idx = 0; idx < street_segs.size(); idx++ ) {
////        StreetIdx insert_street_idx = street_segs[idx].ssi.streetID; // get the street id of segment
////        streets[insert_street_idx].street_name = getStreetName(street_segs[idx].ssi.streetID); //insert street name
////        streets[insert_street_idx].segments.push_back( street_segs[idx]); // insert to vector of street all street segments
////    }
////}
//
//void drawStreetNames_new(ezgl:: renderer *g, StreetSegmentIdx seg_idx) {
//    
//    ezgl:: point2d point_middle;
//    ezgl:: point2d from_seg, to_seg;
//
//    if ( street_segs[seg_idx].street_name != "<unknown>" ) {
//        if ( drawOnCurrentZoom( street_segs[seg_idx].start, g) || drawOnCurrentZoom( street_segs[seg_idx].end, g)) {
//            from_seg = street_segs[seg_idx].start;
//            to_seg = street_segs[seg_idx].end;
//            if ( street_segs[seg_idx].ssi.numCurvePoints > 0) {
//                from_seg = street_segs[seg_idx].middle_curve_pts.first;
//                from_seg = street_segs[seg_idx].middle_curve_pts.second;
//            }
//
//            ezgl:: rectangle bounds(from_seg, to_seg);
//            point_middle = bounds.center();
//
//            setRotationOfStreetNames( g, from_seg, to_seg);
//
//            g->set_color (ezgl:: BLACK);
//            double text_length = sqrt( pow( from_seg.y-to_seg.y, 2) + pow( from_seg.x-to_seg.x, 2));
//            if ( street_segs[seg_idx].ssi.oneWay ) {
//                if ( to_seg.x > from_seg.x ) {
//                    g->draw_text( point_middle, "-->" + street_segs[seg_idx].street_name + "-->", text_length, text_length);
//                } else {
//                    g->draw_text( point_middle, "<--" + street_segs[seg_idx].street_name + "<--", text_length, text_length);
//                }
//            } else {
//                g->draw_text( point_middle, street_segs[seg_idx].street_name, text_length, text_length);
//            }
//            g->set_color (rgb[0], rgb[1], rgb[2]);
//        }
//    }
//}
//
//void drawStreetNames(ezgl:: renderer *g, StreetSegmentIdx seg_idx) {
//    
//    ezgl:: point2d point_middle;
//    ezgl:: point2d from_seg, to_seg;
//
//    if ( street_segs[seg_idx].street_name != "<unknown>" ) {
//        if (seg_idx % 2 == 0 && (drawOnCurrentZoom( street_segs[seg_idx].start, g) 
//            || drawOnCurrentZoom( street_segs[seg_idx].start, g) || checkStreetSegmentCurvesInZoomLevel( g, seg_idx)) ) {
////            && street_segs[seg_idx].draw ) {
//            setStreetSegmentOnZoomLevel( g, seg_idx);
//            from_seg = street_segs[seg_idx].start;
//            to_seg = street_segs[seg_idx].end;
//            if ( street_segs[seg_idx].ssi.numCurvePoints > 0) {
//                from_seg = street_segs[seg_idx].middle_curve_pts.first;
//                from_seg = street_segs[seg_idx].middle_curve_pts.second;
//            }
//
//            ezgl:: rectangle bounds(from_seg, to_seg);
//            point_middle = bounds.center();
//
//            setRotationOfStreetNames( g, from_seg, to_seg);
//
//            g->set_color (ezgl:: BLACK);
//            double text_length = sqrt( pow( from_seg.y-to_seg.y, 2) + pow( from_seg.x-to_seg.x, 2));
//            if ( street_segs[seg_idx].ssi.oneWay ) {
//                if ( to_seg.x > from_seg.x ) {
//                    g->draw_text( point_middle, "-->" + street_segs[seg_idx].street_name + "-->", text_length, text_length);
//                } else {
//                    g->draw_text( point_middle, "<--" + street_segs[seg_idx].street_name + "<--", text_length, text_length);
//                }
//            } else {
//                g->draw_text( point_middle, street_segs[seg_idx].street_name, text_length, text_length);
//            }
//            g->set_color (rgb[0], rgb[1], rgb[2]);
//        }
//    }
//    
////    for ( std:: size_t st_idx = 0; st_idx < streets.size(); st_idx++ ) {
////        
////        if ( streets[st_idx].street_name != "<unknown>" ) {
////            
////            std:: vector<Street_segment_data> segs= streets[st_idx].segments;
////            for ( std:: size_t seg_idx = 0; seg_idx < segs.size(); seg_idx++ ) {
////                setStreetSegmentOnZoomLevel( g, seg_idx);
////                if (seg_idx % 3 == 0 && (drawOnCurrentZoom( segs[seg_idx].start, g) 
////                        || drawOnCurrentZoom( segs[seg_idx].start, g) || checkStreetSegmentCurvesInZoomLevel( g, seg_idx)) 
////                            && street_segs[seg_idx].draw ) {
////                    from_seg = segs[seg_idx].start;
////                    to_seg = segs[seg_idx].end;
////                    if ( segs[seg_idx].ssi.numCurvePoints > 0) {
////                        from_seg = segs[seg_idx].middle_curve_pts.first;
////                        from_seg = segs[seg_idx].middle_curve_pts.second;
////                    }
////                    
////                    ezgl:: rectangle bounds(from_seg, to_seg);
////                    point_middle = bounds.center();
////
////                    setRotationOfStreetNames( g, from_seg, to_seg, seg_idx);
////
////                    g->set_color( ezgl:: BLACK);
////                    double text_length = sqrt( pow( from_seg.y-to_seg.y, 2) + pow( from_seg.x-to_seg.x, 2));
////                    g->draw_text( point_middle, streets[st_idx].street_name, text_length, text_length);
////                }
////            }
////        }
////    }
//}
//
//void setRotationOfStreetNames(ezgl:: renderer *g, ezgl:: point2d &from_seg, ezgl:: point2d &to_seg) {
//    
//    double angle = 0;
//    g->set_text_rotation(0);
//
//        
//    if ( from_seg.x > to_seg.x ) {
//        ezgl:: point2d temp_seg = from_seg;
//        from_seg = to_seg;
//        to_seg = temp_seg;
//    }
//    
//    angle = atan( (to_seg.y - from_seg.y)/(to_seg.x - from_seg.x))*(180/M_PI);  
//    
////    if ( segs[seg_idx].ssi.numCurvePoints == 0 ) {
////
////        angle = atan( (to_seg.y - from_seg.y)/(to_seg.x - from_seg.x))*(180/M_PI);  
////    } else {
////        
////    }
////    else {
////        
//////        double dx = segs[seg_idx].longest_curve_interval.second.x - segs[seg_idx].longest_curve_interval.first.x;
//////        double dy = segs[seg_idx].longest_curve_interval.second.y - segs[seg_idx].longest_curve_interval.first.y;
//////        angle = atan(dy/dx)*(180/M_PI);
//////        
//////        point_middle.x = 0.5*(segs[seg_idx].longest_curve_interval.first.x
//////                + segs[seg_idx].longest_curve_interval.second.x);
//////        point_middle.y = 0.5*(segs[seg_idx].longest_curve_interval.first.y 
//////                + segs[seg_idx].longest_curve_interval.second.y);
////    }
//    
//    if (angle < 0 ) {
//        angle = angle + 360;
//    }
//    if ( angle < 360 && angle > 0 ) {
//        g->set_text_rotation(angle);
//    }
//}
//
//bool checkStreetSegmentCurvesInZoomLevel( ezgl:: renderer *g, StreetSegmentIdx seg_idx) {
//    
//    for ( std:: size_t curv_pts = 0; curv_pts < street_segs[seg_idx].ssi.numCurvePoints; curv_pts++ ) {
//       if ( drawOnCurrentZoom( street_segs[seg_idx].curve_points[curv_pts], g) ) {
//           return true;
//       }
//    }
//    return false;
//}
//
//void setResolution( ezgl:: renderer *g) {
//    double current_world_area = g -> get_visible_world().area();
//    double total_world_area = initial_world.area();
//    double ratio = current_world_area / total_world_area;
//    
//    high_res = false;
//    mid_res = false;
//    low_res = false;
//
//    if (ratio < 0.05){
//        high_res = true;
//        mid_res = false;
//        low_res = false;
//    } else if (ratio >= 0.05 && ratio < 0.25){
//        high_res = false;
//        mid_res = true;
//        low_res = false;
//    }  else if (ratio >= 0.25 && ratio < 0.5){
//        high_res = false;
//        mid_res = false;
//        low_res = true;
//    }
//}
//
//inline void setStreetSegmentOnZoomLevel( ezgl:: renderer *g, StreetSegmentIdx seg_idx ) {
//    
//    street_width_factor = 1;
//    std:: string segment_highway_name = OSM_highway_tags[street_segs[seg_idx].ssi.wayOSMID];
//    if ( segment_highway_name == "motorway" || segment_highway_name == "motorway_link" 
//            || segment_highway_name == "trunk" || segment_highway_name == "trunk_link" ) {
//            street_width_factor = 3;
//            if ( high_res ) {
//                street_width_factor = 2;
//        }
//        g->set_color(180, 180, 212);
//        rgb[0] = 180;
//        rgb[1] = 180;
//        rgb[2] = 212;
//        street_segs[seg_idx].draw = true;
//        draw_name = true;
//        } else if ( segment_highway_name == "secondary" || segment_highway_name == "secondary_link" 
//                || segment_highway_name == "tertiary" || segment_highway_name == "tertiary_link" ) {
//            street_width_factor = 3;
//            if ( high_res ) {
//            street_width_factor = 2;
//        }
//        g->set_color(180, 180, 212);
//        rgb[0] = 180;
//        rgb[1] = 180;
//        rgb[2] = 212;
//        street_segs[seg_idx].draw = true;
//        draw_name = true;
//    } else if ( segment_highway_name == "primary" || segment_highway_name == "primary_link" ) {
//            street_width_factor = 3;
//            if ( high_res ) {
//                street_width_factor = 2;
//        }
//        g->set_color(180, 180, 212);
//        rgb[0] = 180;
//        rgb[1] = 180;
//        rgb[2] = 212;
//        street_segs[seg_idx].draw = true;
//        draw_name = true;
//        } else if ( segment_highway_name == "service" ) {
//            if ( high_res ) {
//                street_segs[seg_idx].draw = true;
//                street_width_factor = 2;
//        }
//        g->set_color(255, 255, 255);
//        rgb[0] = 255;
//        rgb[1] = 255;
//        rgb[2] = 255;
//        } else {
//            if (high_res) {
//                street_segs[seg_idx].draw = true;
//        }
//        g->set_color (ezgl::WHITE);
//        rgb[0] = 255;
//        rgb[1] = 255;
//        rgb[2] = 255;
//    }
//    
//                
//    g->set_line_width( street_width_factor);
//}
//
//bool drawOnCurrentZoom(ezgl:: point2d p_street_seg, ezgl::renderer * g){
//    ezgl::point2d bottom_left = g -> get_visible_world().m_first;
//    ezgl::point2d top_right = g -> get_visible_world().m_second;
//    //std::cout << "Current world is " << bottom_left.x << " " << bottom_left.y << std::endl;
//    if((p_street_seg.x > bottom_left.x) &&
//            (p_street_seg.x < top_right.x) &&
//            (p_street_seg.y > bottom_left.y) &&
//            (p_street_seg.y < top_right.y)){
//            return true;
//    }
//    return false;
//}
//
////void findLongestDistanceBetweenCurvePoints(StreetSegmentIdx idx) {
////    
////    int curv_pts = street_segs[idx].ssi.numCurvePoints;
////    if ( curv_pts == 1 ) {
////        double longest_dist;
////        LatLon c1( latFromY(street_segs[idx].curve_points[0].y), lonFromX(street_segs[idx].curve_points[0].x));
////        LatLon c2( latFromY(street_segs[idx].start.y), lonFromX(street_segs[idx].start.x));
////        longest_dist = findDistanceBetweenTwoPoints( c1, c2); //initialize
////        street_segs[idx].longest_curve_interval.first = street_segs[idx].start;
////        street_segs[idx].longest_curve_interval.second = street_segs[idx].curve_points[0];
////
////        c2 = LatLon( latFromY(street_segs[idx].end.y), lonFromX(street_segs[idx].end.x));
////        if ( longest_dist < findDistanceBetweenTwoPoints(c1, c2) ) {
////            longest_dist = findDistanceBetweenTwoPoints(c1, c2);
////            street_segs[idx].longest_curve_interval.first = street_segs[idx].curve_points[0];
////            street_segs[idx].longest_curve_interval.second = street_segs[idx].end;
////        }
////    } else {
////        double longest_dist;
////        LatLon c1( latFromY(street_segs[idx].curve_points[0].y), lonFromX(street_segs[idx].curve_points[0].x));
////        LatLon c2( latFromY(street_segs[idx].start.y), lonFromX(street_segs[idx].start.x));
////        longest_dist = findDistanceBetweenTwoPoints( c1, c2); //initialize
////        
////        for ( std:: size_t i = 0; i < street_segs[idx].curve_points.size() - 1; i++ ) {
////            c1 = LatLon( latFromY(street_segs[idx].curve_points[i].y), lonFromX(street_segs[idx].curve_points[i].x));
////            c2 = LatLon( latFromY(street_segs[idx].curve_points[i+1].y), lonFromX(street_segs[idx].curve_points[i+1].x));
////            if ( longest_dist < findDistanceBetweenTwoPoints(c1, c2) ) {
////                longest_dist = findDistanceBetweenTwoPoints(c1, c2);
////                street_segs[idx].longest_curve_interval.first = street_segs[idx].curve_points[i];
////                street_segs[idx].longest_curve_interval.second = street_segs[idx].curve_points[i+1];
////            }
////        }
////        
////        c1 = LatLon( latFromY(street_segs[idx].curve_points[curv_pts-1].y), lonFromX(street_segs[idx].curve_points[curv_pts -1].x));
////        c2 = LatLon( latFromY(street_segs[idx].end.y), lonFromX(street_segs[idx].end.x));
////        if ( longest_dist < findDistanceBetweenTwoPoints(c1, c2) ) {
////            longest_dist = findDistanceBetweenTwoPoints(c1, c2);
////            street_segs[idx].longest_curve_interval.first = street_segs[idx].curve_points[curv_pts-1];
////            street_segs[idx].longest_curve_interval.second = street_segs[idx].end;
////        }
////        
////    }
////            
////}
//
//
//
//
//#endif /* M2_BERKAN_FUNCS_H */
//
