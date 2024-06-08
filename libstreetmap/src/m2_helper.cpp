/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include "m2_helper.h"
#include "globals.h"
#include "m3_output_funcs.h"
#include "application.hpp"

std::vector<std::vector<FeatureData>> features;
ezgl::rectangle initial_world;
std::vector<POIData> pois;
std::vector<std::string> types_of_pois;

double max_lat;
double min_lat;
double max_lon;
double min_lon;
bool draw_name;


std:: vector<Intersection_data> intersections;
std:: vector<Street_segment_data> street_segs;
std:: vector<City_data> all_cities;
std:: vector<std::string> all_city_names;

bool show_intersection_data = false;
bool show_poi_data = false;
bool show_route = false;
int click_counter = 0;

ezgl:: application *appPtr;
int rgb[3];
std:: vector<StreetSegmentIdx> all_highways;
std:: unordered_map<OSMID , std:: vector< std:: pair<std::string, std::string>>> OSM_way_tags;
std:: vector<StreetSegmentIdx> motorways_trunks;
std:: vector<StreetSegmentIdx> primary_highways;
std:: vector<StreetSegmentIdx> secondary_highways;
std:: vector<StreetSegmentIdx> tertiary_highways;
std:: vector<StreetSegmentIdx> roads;
std:: vector<StreetSegmentIdx> unclassified;
std:: vector<StreetSegmentIdx> residential;
std:: vector<StreetSegmentIdx> living_street;
std:: vector<StreetSegmentIdx> service;
std:: vector<StreetSegmentIdx> pedestrian;
std:: vector<StreetSegmentIdx> other_highways;

std:: vector<StreetSegmentIdx> path_created;

//Loading Data Structures
void loadFeatureData(){
    features.resize(11);
    int num_features = getNumFeatures();
    for(int id = 0; id < num_features; id++){
        //new feature to be pushed into the vector
        FeatureData new_feature;
        int num_points_in_feature = getNumFeaturePoints(id);
        
        //basic information about the feature
        new_feature.feature_name = getFeatureName(id);
        new_feature.feature_id = id;
        new_feature.feature_OSM_id = getFeatureOSMID(id);
        for(int i = 0; i < num_points_in_feature; i++){ //list of latlon objects of points that make up the feature
            new_feature.feature_points.push_back(getFeaturePoint(id, i));
            ezgl::point2d new_point;
            new_point.x = xFromLon(getFeaturePoint(id, i).longitude());
            new_point.y = yFromLat(getFeaturePoint(id, i).latitude());
            new_feature.feature_points_xy.push_back(new_point);
        }
        
        //determining the geometry of the feature
        if(num_points_in_feature == 1){ //if the feature is just a point
            new_feature.feature_geometry = POINT;
            new_feature.area = 0;
        }
        else{ //if the feature is a line or a polygon
            LatLon first_point = getFeaturePoint(id, 0);
            LatLon last_point = getFeaturePoint(id, num_points_in_feature - 1);
            if(first_point.latitude() == last_point.latitude() && first_point.longitude() == last_point.longitude()){ //if it is closed way
                new_feature.feature_geometry = POLYGON;
                new_feature.area = findFeatureArea(new_feature.feature_id);
                new_feature.centroid_xy = calculateCentroid(new_feature.feature_points_xy);
            }
            else { //if it is a line
                new_feature.feature_geometry = LINE;
                new_feature.area = calculateLineFeatureLength(new_feature) * 0.01;
            }
        }

        //Determining the colour of the feature on the map
        FeatureType feature_type = getFeatureType(id);
        switch(feature_type){
            case 0: //unknown - black (for now)
                setRGBValues(new_feature.colour, 0, 0, 0);
                new_feature.visible_at = 1;
                break;
            case 1: //park - green
            // setRGBValues(new_feature.colour, 193, 208, 183);
                setRGBValues(new_feature.colour, 169, 222, 131);
                new_feature.visible_at = 3;
                break;
            case 2: //beach - yellow
            // setRGBValues(new_feature.colour, 255, 233, 166);
                setRGBValues(new_feature.colour, 249, 234, 154);
                new_feature.visible_at = 1;
                break;
            case 3: //lake - blue
            // setRGBValues(new_feature.colour, 168, 216, 234);
                setRGBValues(new_feature.colour, 167, 203, 227);
                new_feature.visible_at = 1;
                break;
            case 4: //river - blue
            // setRGBValues(new_feature.colour, 130, 207, 253);
                setRGBValues(new_feature.colour, 167, 203, 227);
                new_feature.visible_at = 4;
                break;
            case 5: //island - green
            // setRGBValues(new_feature.colour, 255, 244, 193);
                setRGBValues(new_feature.colour, 169, 222, 131);
                new_feature.visible_at = 1;
                break;
            case 6: //building - gray
            // setRGBValues(new_feature.colour, 216, 216, 216);
                setRGBValues(new_feature.colour, 208, 208, 208);
                new_feature.visible_at = 6;
                break;
            case 7: //greenspace - green
            //setRGBValues(new_feature.colour, 193, 208, 183);
                setRGBValues(new_feature.colour, 169, 222, 131);
                new_feature.visible_at = 2;
                break;
            case 8: //golfcourse - light green
            // setRGBValues(new_feature.colour, 214, 199, 166);
                setRGBValues(new_feature.colour, 198, 232, 177);
                new_feature.visible_at = 2;
                break;
            case 9: //stream - blue
            // setRGBValues(new_feature.colour, 130, 207, 253);
                setRGBValues(new_feature.colour, 167, 203, 227);
                new_feature.visible_at = 8;
                break;
            case 10: //glacier - light blue
            // setRGBValues(new_feature.colour, 255, 255, 255);
                setRGBValues(new_feature.colour, 167, 203, 227);
                new_feature.visible_at = 1;
                break;
            default:
                break;
        } 
        features[feature_type].push_back(new_feature);
    }
}

void loadPOIData(){
    int num_pois = getNumPointsOfInterest();
    for(int i = 0; i < num_pois; i++){
        POIData new_poi;
        LatLon position = getPOIPosition(i);
        ezgl::point2d position_xy;
        position_xy.x = xFromLon(position.longitude());
        position_xy.y = yFromLat(position.latitude());
        new_poi.location = position;
        new_poi.location_xy = position_xy;
        new_poi.poi_id = i;
        new_poi.poi_name = getPOIName(i);
        new_poi.poi_osm_id = getPOIOSMNodeID(i);
        new_poi.poi_type = getPOIType(i);
        new_poi.is_clustered = false;
        pois.push_back(new_poi);
    }
}

void loadCities() {
    all_cities.resize( 19);
//    all_city_names.resize( 19);
    
    all_city_names.push_back( "City Names");
    
    all_city_names.push_back( "beijing");
    all_cities[0].city_name = "beijing";
    all_cities[0].isLoad = false;
    all_cities[0].url = "/cad2/ece297s/public/maps/beijing_china.streets.bin";
    
    all_city_names.push_back( "cairo");
    all_cities[1].city_name = "cairo";
    all_cities[1].isLoad = false;
    all_cities[1].url = "/cad2/ece297s/public/maps/cairo_egypt.streets.bin";
            
    all_city_names.push_back( "capeTown");
    all_cities[2].city_name = "capeTown";
    all_cities[2].isLoad = false;
    all_cities[2].url = "/cad2/ece297s/public/maps/cape-town_south-africa.streets.bin";
            
    all_city_names.push_back( "goldenHorseshoe");
    all_cities[3].city_name = "goldenHorseshoe";
    all_cities[3].isLoad = false;
    all_cities[3].url = "/cad2/ece297s/public/maps/golden-horseshoe_canada.streets.bin";
            
    all_city_names.push_back( "hamilton");
    all_cities[4].city_name = "hamilton";
    all_cities[4].isLoad = false;
    all_cities[4].url = "/cad2/ece297s/public/maps/hamilton_canada.streets.bin";
    
    all_city_names.push_back( "hongKong");
    all_cities[5].city_name = "hongKong";
    all_cities[5].isLoad = false;
    all_cities[5].url = "/cad2/ece297s/public/maps/hong-kong_china.streets.bin";
            
    all_city_names.push_back( "iceland");
    all_cities[6].city_name = "iceland";
    all_cities[6].isLoad = false;
    all_cities[6].url = "/cad2/ece297s/public/maps/iceland.streets.bin";
            
    all_city_names.push_back( "interlaken");
    all_cities[7].city_name = "interlaken";
    all_cities[7].isLoad = false;
    all_cities[7].url = "/cad2/ece297s/public/maps/interlaken_switzerland.streets.bin";
            
    all_city_names.push_back( "kyiv");
    all_cities[8].city_name = "kyiv";
    all_cities[8].isLoad = false;
    all_cities[8].url = "/cad2/ece297s/public/maps/kyiv_ukraine.streets.bin";
            
    all_city_names.push_back( "london");
    all_cities[9].city_name = "london";
    all_cities[9].isLoad = false;
    all_cities[9].url = "/cad2/ece297s/public/maps/london_england.streets.bin";
            
    all_city_names.push_back( "newDelhi");
    all_cities[10].city_name = "newDelhi";
    all_cities[10].isLoad = false;
    all_cities[10].url = "/cad2/ece297s/public/maps/new-delhi_india.streets";
            
    all_city_names.push_back( "newYork");
    all_cities[11].city_name = "newYork";
    all_cities[11].isLoad = false;
    all_cities[11].url = "/cad2/ece297s/public/maps/new-york_usa.streets.bin";
            
    all_city_names.push_back( "rioDeJaneiro");
    all_cities[12].city_name = "rioDeJaneiro";
    all_cities[12].isLoad = false;
    all_cities[12].url = "/cad2/ece297s/public/maps/rio-de-janeiro_brazil.streets.bin";
            
    all_city_names.push_back( "saintHelena");
    all_cities[13].city_name = "saintHelena";
    all_cities[13].isLoad = false;
    all_cities[13].url = "/cad2/ece297s/public/maps/saint-helena.streets.bin";
            
    all_city_names.push_back( "singapore");
    all_cities[14].city_name = "singapore";
    all_cities[14].isLoad = false;
    all_cities[14].url = "/cad2/ece297s/public/maps/singapore.streets.bin";
            
    all_city_names.push_back( "sydney");
    all_cities[15].city_name = "sydney";
    all_cities[15].isLoad = false;
    all_cities[15].url = "/cad2/ece297s/public/maps/sydney_australia.streets.bin";
            
    all_city_names.push_back( "tehran");
    all_cities[16].city_name = "tehran";
    all_cities[16].isLoad = false;
    all_cities[16].url = "/cad2/ece297s/public/maps/tehran_iran.streets.bin";
            
    all_city_names.push_back( "tokyo");
    all_cities[17].city_name = "tokyo";
    all_cities[17].isLoad = false;
    all_cities[17].url = "/cad2/ece297s/public/maps/tokyo_japan.streets.bin";
            
    all_city_names.push_back( "toronto");
    all_cities[18].city_name = "toronto";
    all_cities[18].isLoad = false;
    all_cities[18].url = "/cad2/ece297s/public/maps/toronto_canada.streets.bin";
}

void switch_cities( GtkComboBoxText* thisBox, ezgl::application* application) {
    
    for ( std:: size_t city_idx = 0; city_idx < all_cities.size(); ++city_idx ) {
        if ( gtk_combo_box_text_get_active_text(thisBox) == all_cities[city_idx].city_name ) {
            all_cities[city_idx].isLoad = true;
            changeMap( application);
            application->refresh_drawing();
        } 
    }    
}

void changeMap( ezgl::application* application) {
    for ( std:: size_t city_idx = 0; city_idx < all_cities.size(); ++city_idx ) {
        if (  all_cities[city_idx].isLoad ) {
            std::cout << "You have now selected to load ... " << all_cities[city_idx].city_name << std::endl;
            all_cities[city_idx].isLoad = false;
            closeMap();
            loadMap(all_cities[city_idx].url);
            calculateBorder();
            ezgl:: rectangle change_initial_world({xFromLon(min_lon), yFromLat(min_lat)},{xFromLon(max_lon), yFromLat(max_lat)});
            application->change_canvas_world_coordinates(application->get_main_canvas_id(), change_initial_world);
            drawMap();
        } 
    }    
}


void loadIntersections() {
    
    intersections.resize(getNumIntersections());
    for (IntersectionIdx id = 0; id < getNumIntersections(); ++id ) { 

        intersections[id].xy_loc.x = xFromLon(getIntersectionPosition(id).longitude());
        intersections[id].xy_loc.y = yFromLat(getIntersectionPosition(id).latitude());
        intersections[id].name = getIntersectionName(id);
        intersections[id].highlight = false;
    }
}

void loadStreetSegments() {
    street_segs.resize(getNumStreetSegments());
    for ( StreetSegmentIdx idx = 0; idx < getNumStreetSegments(); idx++ ) {
        StreetSegmentInfo ssi_found = getStreetSegmentInfo(idx);
        street_segs[idx].ssi = ssi_found;
        street_segs[idx].start.x = xFromLon( getIntersectionPosition(ssi_found.from).longitude() );
        street_segs[idx].start.y = yFromLat( getIntersectionPosition(ssi_found.from).latitude() );
        street_segs[idx].end.x = xFromLon( getIntersectionPosition(ssi_found.to).longitude() );
        street_segs[idx].end.y = yFromLat( getIntersectionPosition(ssi_found.to).latitude() );
        
        street_segs[idx].street_name = getStreetName(street_segs[idx].ssi.streetID);
        
        street_segs[idx].curve_points.resize(ssi_found.numCurvePoints);
        for ( int numCurvePoints = 0; numCurvePoints < ssi_found.numCurvePoints; numCurvePoints++ ) {
            street_segs[idx].curve_points[numCurvePoints].x
                    =  xFromLon( getStreetSegmentCurvePoint(idx, numCurvePoints).longitude() );
            street_segs[idx].curve_points[numCurvePoints].y
                    =  yFromLat( getStreetSegmentCurvePoint(idx, numCurvePoints).latitude() );
        }   
        loadStreetTypes( idx);
        findLongestDistanceBetweenCurvePoints(idx);
    }
}

void loadStreetTypes( StreetSegmentIdx seg_idx) {
    
    std:: string tag_value = getOSMWayTagValue(street_segs[seg_idx].ssi.wayOSMID, "highway");
    if ( tag_value == "motorway" || tag_value == "motorway_link" 
        || tag_value == "trunk" || tag_value == "trunk_link" ) {
        street_segs[seg_idx].red = 180;
        street_segs[seg_idx].green = 180;
        street_segs[seg_idx].blue = 200;       
        motorways_trunks.push_back( seg_idx);
        return;
    } else if ( tag_value == "primary" || tag_value == "primary_link" ) {
        street_segs[seg_idx].red = 180;
        street_segs[seg_idx].green = 180;
        street_segs[seg_idx].blue = 200;  
        primary_highways.push_back( seg_idx);  
        return;
    } else if ( tag_value == "secondary" || tag_value == "secondary_link" ) {
        street_segs[seg_idx].red = 180;
        street_segs[seg_idx].green = 180;
        street_segs[seg_idx].blue = 200; 
        secondary_highways.push_back( seg_idx);
        return;
    } else if ( tag_value == "tertiary" || tag_value == "tertiary_link" ) {
        street_segs[seg_idx].red = 180;
        street_segs[seg_idx].green = 180;
        street_segs[seg_idx].blue = 200; 
        tertiary_highways.push_back( seg_idx);
        return;
    } else if ( tag_value == "road" ) {
        street_segs[seg_idx].red = 255;
        street_segs[seg_idx].green = 255;
        street_segs[seg_idx].blue = 255; 
        roads.push_back( seg_idx);
        return;
    } else if ( tag_value == "unclassified" || tag_value == "unclassified_link" ) {
        street_segs[seg_idx].red = 255;
        street_segs[seg_idx].green = 255;
        street_segs[seg_idx].blue = 255; 
        unclassified.push_back( seg_idx);
        return;
    } else if ( tag_value == "residential" || tag_value == "residential_link" ) {
        street_segs[seg_idx].red = 255;
        street_segs[seg_idx].green = 255;
        street_segs[seg_idx].blue = 255; 
        residential.push_back( seg_idx);
        return;
    } else if ( tag_value == "living_street" || tag_value == "living_street_link" ) {
        street_segs[seg_idx].red = 255;
        street_segs[seg_idx].green = 255;
        street_segs[seg_idx].blue = 255; 
        living_street.push_back( seg_idx);
        return;
    } else if ( tag_value == "service" || tag_value == "service_link" ) {
        street_segs[seg_idx].red = 255;
        street_segs[seg_idx].green = 255;
        street_segs[seg_idx].blue = 255; 
        service.push_back( seg_idx);
        return;
    } else if ( tag_value == "pedestrian" || tag_value == "pedestrian_link" ) {
        street_segs[seg_idx].red = 255;
        street_segs[seg_idx].green = 255;
        street_segs[seg_idx].blue = 255; 
        pedestrian.push_back( seg_idx);
        return;
    } else {
        street_segs[seg_idx].red = 255;
        street_segs[seg_idx].green = 255;
        street_segs[seg_idx].blue = 255; 
        other_highways.push_back( seg_idx);
        return;
    }
       
}

void loadOSMWays_new() {
    int num_ways = getNumberOfWays();
    for ( int way_idx = 0; way_idx < num_ways; way_idx++ ) {
//        std:: vector<std:: pair<std:: string, std:: string>> stored_tags;
        const OSMWay* way_osm = getWayByIndex( way_idx);
        for ( int tag_count = 0; tag_count < getTagCount( way_osm); tag_count++ ) {
            std:: pair<std:: string, std:: string> tag_pair_found = getTagPair( way_osm, tag_count);
            OSM_way_tags[way_osm->id()].push_back( getTagPair( way_osm, tag_count));
        }
        
    }

}

//Drawing the map
void drawFeatures(ezgl::renderer *g){
    int zoom = getCurrentZoom(g);
        if (zoom > 0){
            drawVectorOfFeatures(features[0], g); //unknown
            drawVectorOfFeatures(features[5], g); //islands
            drawVectorOfFeatures(features[10], g); //glaciers
        }
        if (zoom > 2){
            drawVectorOfFeatures(features[7], g); //greenspaces
            drawVectorOfFeatures(features[8], g); //golfcourses
        }
        if (zoom > 3){
            drawVectorOfFeatures(features[1], g); //parks
        }
        if (zoom > 2){
            drawVectorOfFeatures(features[2], g); //beaches
        }
        if (zoom > 0){
            drawVectorOfFeatures(features[4], g); //rivers
        }          
        if (zoom > 4){
            drawVectorOfFeatures(features[9], g); //streams
        } 
        if (zoom > 0){
            drawVectorOfFeatures(features[3], g); //lakes
        } 
        if (zoom > 6){
            drawVectorOfFeatures(features[6], g); //buildings
        }
}

void drawPOIs(ezgl::renderer *g){
    int zoom = getCurrentZoom(g);
    std::vector<ezgl::point2d> point_vec;
    point_vec.resize(1);
    if (zoom > 7){
        for(int i = 0; i < pois.size(); i++){
            point_vec[0] = pois[i].location_xy;
            if(!pointIsInVisibleWorld(point_vec[0], g)) continue;
            ezgl::surface *png_surface = ezgl::renderer::load_png("libstreetmap/resources/map_marker_generic.png");
            g -> draw_surface(png_surface, pois[i].location_xy);
            ezgl::renderer::free_surface(png_surface);
        }
    }
}

void drawVectorOfFeatures(std::vector<FeatureData> &features_to_draw, ezgl::renderer *g){
    int size = features_to_draw.size();
    FeatureData current_feature;
    for(int i = 0; i < size; i++){
        current_feature = features_to_draw[i];
        if(current_feature.feature_geometry == POLYGON){
            if(!polygonIsInVisibleWorld(current_feature.feature_points_xy, g)) continue;
            g -> set_color(current_feature.colour.red,  current_feature.colour.green, current_feature.colour.blue);
            g -> fill_poly(current_feature.feature_points_xy);
        }
        if(current_feature.feature_geometry == LINE){
            if(!polygonIsInVisibleWorld(current_feature.feature_points_xy, g)) continue;
            g -> set_color(current_feature.colour.red, current_feature.colour.green, current_feature.colour.blue);
            g -> set_line_width(1);
            for(int j = 0; j < current_feature.feature_points_xy.size() - 1; j ++){
                g -> draw_line(current_feature.feature_points_xy[j], current_feature.feature_points_xy[j+1]);
            }
        }
        if(current_feature.feature_geometry == POINT){
            if(!pointIsInVisibleWorld(current_feature.feature_points_xy[0], g)) continue;
            g -> set_color(ezgl::BLACK);
            g -> fill_rectangle(current_feature.feature_points_xy[0], 5, 5);
        }
        
    }
}

void drawIntersections( ezgl::renderer *g) {
    for ( std:: size_t inter_id = 0; inter_id < intersections.size(); ++inter_id ) {
        if (getCurrentZoom(g) > 7){
            if(pointIsInVisibleWorld(intersections[inter_id].xy_loc, g)){
                ezgl::point2d inter_loc;
                inter_loc.x = intersections[inter_id].xy_loc.x;
                inter_loc.y = intersections[inter_id].xy_loc.y;
                ezgl::surface *png_surface = ezgl::renderer::load_png("libstreetmap/resources/travellable_intersection.png");
                g -> draw_surface(png_surface, inter_loc);
                ezgl::renderer::free_surface(png_surface);
            }
        }
        
        if(intersections[inter_id].highlight){
            if (drawOnCurrentZoom( intersections[inter_id].xy_loc, g) ) {

                ezgl::point2d inter_loc;
                inter_loc.x = intersections[inter_id].xy_loc.x;
                inter_loc.y = intersections[inter_id].xy_loc.y;
                
                if(inter_id == end_inter){
                    ezgl::surface *png_surface = ezgl::renderer::load_png("libstreetmap/resources/end_point.png");
                    g -> draw_surface(png_surface, inter_loc);
                    ezgl::renderer::free_surface(png_surface);
                }
                else if (inter_id == start_inter){
                    ezgl::surface *png_surface = ezgl::renderer::load_png("libstreetmap/resources/start_point.png");
                    g -> draw_surface(png_surface, inter_loc);
                    ezgl::renderer::free_surface(png_surface);
                }            
            }
        }
    }
}

void drawStreets(ezgl::renderer *g){
    int zoom = getCurrentZoom(g);
    draw_name = false;
    g->set_line_width(0.5);
    if ( 2 >= zoom && zoom > 0){
        g->set_line_width( 6);
        displayStreetTypes( motorways_trunks, g);

        g->set_line_width( 5);
        displayStreetTypes( primary_highways, g);       
    }
    if ( 5 >= zoom && zoom > 2 ){
        if ( zoom >= 3 ) {
            draw_name = true;
        }
        g->set_line_width( 4);
        displayStreetTypes( motorways_trunks, g);
        
        g->set_line_width( 3.5);
        displayStreetTypes( primary_highways, g);
        
        g->set_line_width( 3.75);
        displayStreetTypes( secondary_highways, g);
    }
    if ( 8 >= zoom && zoom > 5 ){
        g->set_line_width( 5);
        displayStreetTypes( motorways_trunks, g);
        
        g->set_line_width( 4.5);
        displayStreetTypes( primary_highways, g);
        
        g->set_line_width( 4);
        displayStreetTypes( secondary_highways, g);
        
        g->set_line_width( 3.5);
        displayStreetTypes( tertiary_highways, g);
        
        if ( zoom >= 7 ) {
            g->set_line_width( 2.75);
            displayStreetTypes( roads, g);
            g->set_line_width( 2.5);
            displayStreetTypes( unclassified, g);
            g->set_line_width( 2.5);
            displayStreetTypes( residential, g);
            g->set_line_width( 2.5);
            displayStreetTypes( living_street, g);
            g->set_line_width( 2.5);
            displayStreetTypes( service, g);
            g->set_line_width( 2.25);
            displayStreetTypes( pedestrian, g);
            g->set_line_width( 2);
            displayStreetTypes( other_highways, g);
        }
    }
    if ( 11 >= zoom && zoom > 8 ){
        draw_name = true;
        g->set_line_width( 8);
        displayStreetTypes( motorways_trunks, g);
        
        g->set_line_width( 7);
        displayStreetTypes( primary_highways, g);
        
        g->set_line_width( 6);
        displayStreetTypes( secondary_highways, g);
        
        g->set_line_width( 5);
        displayStreetTypes( tertiary_highways, g);
        
        g->set_line_width( 4);
        displayStreetTypes( roads, g);
        g->set_line_width( 4);
        displayStreetTypes( unclassified, g);
        g->set_line_width( 3.5);
        displayStreetTypes( residential, g);
        g->set_line_width( 3.5);
        displayStreetTypes( living_street, g);
        g->set_line_width( 3.5);
        displayStreetTypes( service, g);
        g->set_line_width( 3.35);
        displayStreetTypes( pedestrian, g);
        g->set_line_width( 3);
        displayStreetTypes( other_highways, g);
    }
    if ( zoom > 11 ){
        g->set_line_width( 10);
        displayStreetTypes( motorways_trunks, g);
        
        g->set_line_width( 9);
        displayStreetTypes( primary_highways, g);
        
        g->set_line_width( 8);
        displayStreetTypes( secondary_highways, g);
        
        g->set_line_width( 7);
        displayStreetTypes( tertiary_highways, g);
        
        g->set_line_width( 5.5);
        displayStreetTypes( roads, g);
        g->set_line_width( 5.5);
        displayStreetTypes( unclassified, g);
        g->set_line_width( 5);
        displayStreetTypes( residential, g);
        g->set_line_width( 5);
        displayStreetTypes( living_street, g);
        g->set_line_width( 5);
        displayStreetTypes( service, g);
        g->set_line_width( 4.5);
        displayStreetTypes( pedestrian, g);
        g->set_line_width( 3.5);
        displayStreetTypes( other_highways, g);
    }
    g -> set_line_width(7);
        displayStreetTypes(path_created, g);
}

void displayStreetTypes( std:: vector<StreetSegmentIdx> street_type, ezgl::renderer *g ) {
    for ( std:: size_t idx = 0; idx < street_type.size(); idx++ ) {
        Street_segment_data seg_data = street_segs[street_type[idx]];
        g->set_color( seg_data.red, seg_data.green, seg_data.blue);
        if ( seg_data.highlight ) {
            g->set_color( 0, 102,102);
        }
        if ( seg_data.ssi.numCurvePoints == 0 ) {
            if ( drawOnCurrentZoom( seg_data.start, g)
                        || drawOnCurrentZoom( seg_data.end, g) ) {
                g->draw_line({seg_data.start.x, seg_data.start.y},
                {seg_data.end.x, seg_data.end.y} );
            }
        }
        else if ( seg_data.ssi.numCurvePoints == 1 ) {
            if ( drawOnCurrentZoom( seg_data.curve_points[0], g)
                    || drawOnCurrentZoom(seg_data.start, g) ) {
                g->draw_line({seg_data.start.x, seg_data.start.y},
                {seg_data.curve_points[0].x, seg_data.curve_points[0].y} );
            }
            if ( drawOnCurrentZoom( seg_data.curve_points[0], g)
                    || drawOnCurrentZoom(seg_data.end, g) ) {
                g->draw_line({seg_data.curve_points[0].x, seg_data.curve_points[0].y},
                {seg_data.end.x, seg_data.end.y} );
            }
        }
        else {
            if ( drawOnCurrentZoom( seg_data.curve_points[0], g)
                    || drawOnCurrentZoom(seg_data.start, g) ) {
                g->draw_line({seg_data.start.x, seg_data.start.y},
                {seg_data.curve_points[0].x, seg_data.curve_points[0].y} );
            }
            for ( std:: size_t i = 0; i < seg_data.curve_points.size() - 1; i++ ) {
                if ( drawOnCurrentZoom( seg_data.curve_points[i], g) 
                        || drawOnCurrentZoom( seg_data.curve_points[i+1], g) ) {
                    g->draw_line({seg_data.curve_points[i].x, seg_data.curve_points[i].y},
                    {seg_data.curve_points[i+1].x, seg_data.curve_points[i+1].y} );
                }
            }
            int curv_num = seg_data.ssi.numCurvePoints;
            if ( drawOnCurrentZoom( seg_data.curve_points[curv_num-1], g)
                    || drawOnCurrentZoom( seg_data.end, g) ) {
                g->draw_line({seg_data.curve_points[curv_num - 1].x, seg_data.curve_points[curv_num - 1].y},
                {seg_data.end.x, seg_data.end.y} );
            }
        }
        if ( draw_name == true ) {
            drawStreetNames_new( g, street_type[idx]);
        }
    }
}

void drawStreetNames_new(ezgl:: renderer *g, StreetSegmentIdx seg_idx) {
    
    ezgl:: point2d point_middle;
    ezgl:: point2d from_seg, to_seg;

    if ( street_segs[seg_idx].street_name != "<unknown>" && seg_idx % 2 == 0 ) {
        from_seg = street_segs[seg_idx].start;
        to_seg = street_segs[seg_idx].end;
        
        LatLon c1( latFromY(street_segs[seg_idx].start.y), lonFromX(street_segs[seg_idx].start.x));
        LatLon c2( latFromY(street_segs[seg_idx].end.y), lonFromX(street_segs[seg_idx].end.x));
        double longest_distance = findDistanceBetweenTwoPoints( c1, c2);
        if ( street_segs[seg_idx].ssi.numCurvePoints > 0) {
            LatLon c3( latFromY(street_segs[seg_idx].longest_curve_interval.first.y), lonFromX(street_segs[seg_idx].longest_curve_interval.first.x));
            LatLon c4( latFromY(street_segs[seg_idx].longest_curve_interval.second.y), lonFromX(street_segs[seg_idx].longest_curve_interval.second.x));
            if ( longest_distance < findDistanceBetweenTwoPoints( c3, c4) ) {
                from_seg = street_segs[seg_idx].longest_curve_interval.first;
                to_seg = street_segs[seg_idx].longest_curve_interval.second;
            }
        }

        ezgl:: rectangle bounds(from_seg, to_seg);
        point_middle = bounds.center();
        
        if ( !drawOnCurrentZoom(point_middle, g) ) {
            return;
        }

        setRotationOfStreetNames( g, from_seg, to_seg);

        g->set_color (ezgl:: BLACK);
        double text_length = sqrt( pow( from_seg.y-to_seg.y, 2) + pow( from_seg.x-to_seg.x, 2));
        if ( street_segs[seg_idx].ssi.oneWay ) {
            if ( to_seg.x > from_seg.x ) {
                g->draw_text( point_middle, "-->" + street_segs[seg_idx].street_name + "-->", text_length, text_length);
            } else {
                g->draw_text( point_middle, "<--" + street_segs[seg_idx].street_name + "<--", text_length, text_length);
            }
        } else {
            g->draw_text( point_middle, street_segs[seg_idx].street_name, text_length, text_length);
        }
    }
}


//Helper Functions
int getCurrentZoom(ezgl::renderer *g) {
    double initial_area = initial_world.area();
    double current_area = g -> get_visible_world().area();
    if(current_area > initial_area) current_area = initial_area;
    int zoom_level = floor(log(initial_area / current_area) / log(3)) + 1;
    // Limit zoom level to 1-15
    zoom_level = std::max(1, std::min(15, zoom_level));
    return zoom_level;
}

ezgl::point2d calculateCentroid(std::vector<ezgl::point2d> &vec){
    double sum_x = 0;
    double sum_y = 0;
    for(int i = 0; i < vec.size(); i++){
        sum_x += vec[i].x;
        sum_y += vec[i].y;
    }
    ezgl::point2d centroid;
    centroid.x = sum_x / vec.size();
    centroid.y = sum_y / vec.size();
    return centroid;
}

bool pointIsInVisibleWorld(ezgl::point2d point, ezgl::renderer * g){
    ezgl::point2d bottom_left = g -> get_visible_world().m_first;
    ezgl::point2d top_right = g -> get_visible_world().m_second;
    if((point.x > bottom_left.x) &&
       (point.x < top_right.x) &&
       (point.y > bottom_left.y) &&
       (point.y < top_right.y)){
        return true;
    }
    return false;
}

bool polygonIsInVisibleWorld(std::vector<ezgl::point2d>& points_xy, ezgl::renderer *g){
    ezgl::point2d bottom_left = g -> get_visible_world().m_first;
    ezgl::point2d top_right = g -> get_visible_world().m_second;
    for(int i = 0; i < points_xy.size(); i++){
        if((points_xy[i].x > bottom_left.x) &&
           (points_xy[i].x < top_right.x) &&
           (points_xy[i].y > bottom_left.y) &&
           (points_xy[i].y < top_right.y)){
            return true;
        }
    }
    return false;
}

ezgl::rectangle calculatePolygonBorder(std::vector<ezgl::point2d> &vec) {
    double max_y = vec[0].y;
    double min_y = max_y;
    double max_x = vec[0].x;
    double min_x = max_x;

    for(int i = 0; i < vec.size(); i++){
        max_y = std::max(max_y, vec[i].y);
        max_x = std::max(max_x, vec[i].x);
        min_y = std::min(min_y, vec[i].y);
        min_x = std::min(min_x, vec[i].x);
    }
    ezgl::rectangle border;
    border.m_first.x = min_x;
    border.m_first.y = min_y;
    border.m_second.x = max_x;
    border.m_second.y =  max_y;
    return border;
}

double calculateLineFeatureLength(FeatureData &feature){
    double length = 0;
    for(int i = 0; i < getNumFeaturePoints(feature.feature_id) - 1; i ++){
        length += findDistanceBetweenTwoPoints(getFeaturePoint(feature.feature_id, i), getFeaturePoint(feature.feature_id, i+1));
    }
    return length;
}

inline void setRGBValues(FeatureColour &colour, int red, int green, int blue){
    colour.red = red;
    colour.green = green;
    colour.blue = blue;
}

double xFromLon(double lon) {
    double lat_average = 0.5 * (kDegreeToRadian * min_lat + kDegreeToRadian * max_lat);   
    double x_coor = kEarthRadiusInMeters * kDegreeToRadian * lon * cos(lat_average);
    return x_coor; 
}

double yFromLat(double lat) {
    double y_coor = kEarthRadiusInMeters * lat * kDegreeToRadian;
    return y_coor;  
}

double lonFromX(double x) {
    double lat_average = 0.5 * (kDegreeToRadian * min_lat + kDegreeToRadian * max_lat);   
    double lon = x / (kEarthRadiusInMeters * kDegreeToRadian * cos(lat_average));
    return lon;
}

double latFromY(double y) { 
    double lat = y / (kEarthRadiusInMeters * kDegreeToRadian);
    return lat;
}

void calculateBorder() {
    max_lat = getIntersectionPosition(0).latitude();
    min_lat = max_lat;
    max_lon = getIntersectionPosition(0).longitude();
    min_lon = max_lon;
    
    for (IntersectionIdx id = 0; id < getNumIntersections(); ++id ) { 
        
        max_lat = std::max(max_lat, getIntersectionPosition(id).latitude());
        min_lat = std::min(min_lat, getIntersectionPosition(id).latitude());
        max_lon = std::max(max_lon, getIntersectionPosition(id).longitude());
        min_lon = std::min(min_lon, getIntersectionPosition(id).longitude());
        
    }
}

std::string getOSMWayTagValue (OSMID wayOSMid, std::string key) {
    std:: string found_value = "";
//    std:: unordered_map<OSMID , std:: vector< std:: pair<std:: string, std:: string>>> osm_node_map = OSMIdNodeTags();
    
    std:: vector< std:: pair< std:: string, std:: string>> tag_pairs_found = OSM_way_tags[wayOSMid];
    for ( int i = 0; i < tag_pairs_found.size(); i++ ) {
        if ( tag_pairs_found[i].first == key ) {
            found_value = tag_pairs_found[i].second;
            return tag_pairs_found[i].second;
        }
    } 
    return found_value;

}

void setRotationOfStreetNames(ezgl:: renderer *g, ezgl:: point2d &from_seg, ezgl:: point2d &to_seg) {
    
    double angle = 0;
    g->set_text_rotation(0);

    if ( from_seg.x > to_seg.x ) {
        ezgl:: point2d temp_seg = from_seg;
        from_seg = to_seg;
        to_seg = temp_seg;
    }
    
    angle = atan( (to_seg.y - from_seg.y)/(to_seg.x - from_seg.x))*(180/M_PI);  
    
    if (angle < 0 ) {
        angle = angle + 360;
    }
    if ( angle < 360 && angle > 0 ) {
        g->set_text_rotation(angle);
    }
}

bool checkStreetSegmentCurvesInZoomLevel( ezgl:: renderer *g, StreetSegmentIdx seg_idx) {
    
    for ( std:: size_t curv_pts = 0; curv_pts < street_segs[seg_idx].ssi.numCurvePoints; curv_pts++ ) {
       if ( drawOnCurrentZoom( street_segs[seg_idx].curve_points[curv_pts], g) ) {
           return true;
       }
    }
    return false;
}

bool drawOnCurrentZoom(ezgl:: point2d p_street_seg, ezgl::renderer * g){
    ezgl::point2d bottom_left = g -> get_visible_world().m_first;
    ezgl::point2d top_right = g -> get_visible_world().m_second;
    //std::cout << "Current world is " << bottom_left.x << " " << bottom_left.y << std::endl;
    if((p_street_seg.x > bottom_left.x) &&
            (p_street_seg.x < top_right.x) &&
            (p_street_seg.y > bottom_left.y) &&
            (p_street_seg.y < top_right.y)){
            return true;
    }
    return false;
}

void findLongestDistanceBetweenCurvePoints(StreetSegmentIdx idx) {
    
    int curv_pts = street_segs[idx].ssi.numCurvePoints;
    if ( curv_pts != 0 ) {
        if ( curv_pts == 1 ) {
            double longest_dist;
            LatLon c1( latFromY(street_segs[idx].curve_points[0].y), lonFromX(street_segs[idx].curve_points[0].x));
            LatLon c2( latFromY(street_segs[idx].start.y), lonFromX(street_segs[idx].start.x));
            longest_dist = findDistanceBetweenTwoPoints( c1, c2); //initialize
            street_segs[idx].longest_curve_interval.first = street_segs[idx].start;
            street_segs[idx].longest_curve_interval.second = street_segs[idx].curve_points[0];

            c2 = LatLon( latFromY(street_segs[idx].end.y), lonFromX(street_segs[idx].end.x));
            if ( longest_dist < findDistanceBetweenTwoPoints(c1, c2) ) {
                longest_dist = findDistanceBetweenTwoPoints(c1, c2);
                street_segs[idx].longest_curve_interval.first = street_segs[idx].curve_points[0];
                street_segs[idx].longest_curve_interval.second = street_segs[idx].end;
            }
        } else {
            double longest_dist;
            LatLon c1( latFromY(street_segs[idx].curve_points[0].y), lonFromX(street_segs[idx].curve_points[0].x));
            LatLon c2( latFromY(street_segs[idx].start.y), lonFromX(street_segs[idx].start.x));
            longest_dist = findDistanceBetweenTwoPoints( c1, c2); //initialize
            street_segs[idx].longest_curve_interval.first = street_segs[idx].start;
            street_segs[idx].longest_curve_interval.second = street_segs[idx].curve_points[0];

            for ( std:: size_t i = 0; i < street_segs[idx].curve_points.size() - 1; i++ ) {
                c1 = LatLon( latFromY(street_segs[idx].curve_points[i].y), lonFromX(street_segs[idx].curve_points[i].x));
                c2 = LatLon( latFromY(street_segs[idx].curve_points[i+1].y), lonFromX(street_segs[idx].curve_points[i+1].x));
                if ( longest_dist < findDistanceBetweenTwoPoints(c1, c2) ) {
                    longest_dist = findDistanceBetweenTwoPoints(c1, c2);
                    street_segs[idx].longest_curve_interval.first = street_segs[idx].curve_points[i];
                    street_segs[idx].longest_curve_interval.second = street_segs[idx].curve_points[i+1];
                }
            }

            c1 = LatLon( latFromY(street_segs[idx].curve_points[curv_pts-1].y), lonFromX(street_segs[idx].curve_points[curv_pts -1].x));
            c2 = LatLon( latFromY(street_segs[idx].end.y), lonFromX(street_segs[idx].end.x));
            if ( longest_dist < findDistanceBetweenTwoPoints(c1, c2) ) {
                longest_dist = findDistanceBetweenTwoPoints(c1, c2);
                street_segs[idx].longest_curve_interval.first = street_segs[idx].curve_points[curv_pts-1];
                street_segs[idx].longest_curve_interval.second = street_segs[idx].end;
            }

        }
    }
            
}

//358,359,360
//396525,396526,396527,396528,396529,396530,193,194,355146,
//320429,320430,160687,160688,258035,258036,240508,240522,240523,240524,240525,240526,240527,196691,664,314238,320431,250927,250928,
//280442,280443,280444,280445,280441,393243,65,66,67,68,282976,282977,282974,282975,308721,416593,184414,416583,413252,416591,416592,282973,
//411232,411233,411234,74117,74118,74119,74120,177996,177997,411230,
//411231,74132,74133,74134,74135,74136,74137,74138,74139,196701,196700,196699,196698,196697,196696,196695,196694,
//196693,196692,74131,74121,74122,74123,74124,74125,74126,74127,74128,74129,74130,259198,259199,
//259200,259190,241235,173325,173326,114537,114536,427392,210938,262579,103517,427393,267349,103518,262565,325196,
//267351,267348,267346,267347,156567,411678,103516,75,156568,262587,262567,1529,154571,294948,
//103541,103542,294950,103543,1313,179402,249652,241234,103546,294939,103547,173434,173435,427622,427623,427619,35566,
//133807,35567,427616,427615,35565,133808,133811,361806,294947,35536,35544,214257,35543,214256,
//35542,35541,202371,202372,35537,33737,294943,191640,35531,35530,35529,294942,35528,214328,35522,35521,294945,33739,1186,
//342522,342452,342521,342442,342523,342527,342443,184919,184920,184918,184909,184911,
//342461,184912,202932,202933,127146,149092,184928,184929,342531,342532,202931,145411,145409,342439,344592,344591,249679,
//344590,249678,118645,164688,249675,184748,184752,184757,184758,179796,342565,179795,
//342564,342560,342563,306786,163903,202112,416302,248182,28213,293529,195665,24942,317336,262276,262270,
//262271,131415,129728,129729,262274,262283,24891,317335,262221,262272,30749,129087,129088,129089,
//129090,129091,369130,369131,181364,181363,181362,181361,181360,181359,369129,369128,369127,369126,369125,369124,
//32906,32905,32904,32903,32902,32901,32900,32899,181009,181010,181011,181012,181013,27454,27455,
//27456,27457,122960,122961,122962,122963,122964,122965,122966,122967,122968,122969,129517,
//37895,37896,37897,35104,35105};

// void displayRoute( std:: vector<StreetSegmentIdx> street_type, ezgl::renderer *g ) {
//     for ( std:: size_t idx = 0; idx < street_type.size(); idx++ ) {
//         Street_segment_data seg_data = street_segs[street_type[idx]];
//         g->set_color( 255,105,180);
//         g->set_line_width( 12);
// //        g->set_line_dash( ezgl::line_dash(10));
//         if ( seg_data.ssi.numCurvePoints == 0 ) {
//             if ( drawOnCurrentZoom( seg_data.start, g)
//                         || drawOnCurrentZoom( seg_data.end, g) ) {
//                 g->draw_line({seg_data.start.x, seg_data.start.y},
//                 {seg_data.end.x, seg_data.end.y} );
//             }
//         }
//         else if ( seg_data.ssi.numCurvePoints == 1 ) {
//             if ( drawOnCurrentZoom( seg_data.curve_points[0], g)
//                     || drawOnCurrentZoom(seg_data.start, g) ) {
//                 g->draw_line({seg_data.start.x, seg_data.start.y},
//                 {seg_data.curve_points[0].x, seg_data.curve_points[0].y} );
//             }
//             if ( drawOnCurrentZoom( seg_data.curve_points[0], g)
//                     || drawOnCurrentZoom(seg_data.end, g) ) {
//                 g->draw_line({seg_data.curve_points[0].x, seg_data.curve_points[0].y},
//                 {seg_data.end.x, seg_data.end.y} );
//             }
//         }
//         else {
//             if ( drawOnCurrentZoom( seg_data.curve_points[0], g)
//                     || drawOnCurrentZoom(seg_data.start, g) ) {
//                 g->draw_line({seg_data.start.x, seg_data.start.y},
//                 {seg_data.curve_points[0].x, seg_data.curve_points[0].y} );
//             }
//             for ( std:: size_t i = 0; i < seg_data.curve_points.size() - 1; i++ ) {
//                 if ( drawOnCurrentZoom( seg_data.curve_points[i], g) 
//                         || drawOnCurrentZoom( seg_data.curve_points[i+1], g) ) {
//                     g->draw_line({seg_data.curve_points[i].x, seg_data.curve_points[i].y},
//                     {seg_data.curve_points[i+1].x, seg_data.curve_points[i+1].y} );
//                 }
//             }
//             int curv_num = seg_data.ssi.numCurvePoints;
//             if ( drawOnCurrentZoom( seg_data.curve_points[curv_num-1], g)
//                     || drawOnCurrentZoom( seg_data.end, g) ) {
//                 g->draw_line({seg_data.curve_points[curv_num - 1].x, seg_data.curve_points[curv_num - 1].y},
//                 {seg_data.end.x, seg_data.end.y} );
//             }
//         }
//         g->format_font("monospace", ezgl::font_slant::normal, ezgl::font_weight::bold, 15);
//         drawStreetNames_new( g, street_type[idx]);
//     }
// //    writeDirections( london_path);
// }