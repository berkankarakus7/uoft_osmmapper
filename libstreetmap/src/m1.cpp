/* 
 * Copyright 2023 University of Toronto
 *
 * Permission is hereby granted, to use this software and associated 
 * documentation files (the "Software") in course work at the University 
 * of Toronto, or for personal use. Other uses are prohibited, in 
 * particular the distribution of the Software either publicly or to third 
 * parties.
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "globals.h"
#include "m1_helper.h"

//Global Vectors
std::vector<double> street_segment_length;
std::vector<double> street_segment_travel_time;
std::unordered_map<std::string,std::vector<int>> all_streets;
std::vector<std::vector<StreetSegmentIdx>> street_segments;
std::vector<std::vector<StreetIdx>> streets_at_intersection;
std::vector<std::vector<IntersectionIdx>> intersections_along_street;
std::unordered_map<StreetIdx, std::vector<StreetSegmentIdx>> all_segments_in_street;
std::vector<std::vector<IntersectionIdx>> adjacent_intersections;
std::unordered_map<OSMID, std::vector<std::pair<std::string, std::string>>> OSM_id_tag_pairs;



bool loadMap(std::string map_streets_database_filename) {
    std::cout << "loadMap: " << std::endl;
    bool load_successful = loadStreetsDatabaseBIN(map_streets_database_filename);
    if (!load_successful) {
        return false;
    }
    
    map_streets_database_filename.replace(map_streets_database_filename.find(".streets."), 9, ".osm.");
    bool load_osm_successful = loadOSMDatabaseBIN(map_streets_database_filename);
    if (!load_osm_successful) {
        return false;
    }
    
    //M1 Cache
    all_streets = loadStreetsName();
    all_segments_in_street = allSegmentsInSameStreet();
    OSM_id_tag_pairs = LoadOSMIdNodeTags();
    helper_streetSegments();
    // std::cout << "max speed is " << max_speed << std::endl;
    helper_streetsAtIntersections();
    helper_adjacentIntersections();
    
    //M2 Cache
    calculateBorder();  
    loadOSMWays_new();
    loadCities();
    loadIntersections();
    loadStreetSegments();
    loadFeatureData();
    loadPOIData();
    
    //M3 Cache
    loadNodes();
    
    //Initial World initialization
    initial_world.m_first.x = xFromLon(min_lon);
    initial_world.m_first.y = yFromLat(min_lat);
    initial_world.m_second.x = xFromLon(max_lon);
    initial_world.m_second.y = yFromLat(max_lat);
    return load_successful;
}

void closeMap() {
    std::cout << "closeMap() is called" << std::endl;
    
    //Clearing global vectors
    street_segment_length.clear();
    street_segment_travel_time.clear();
    street_segments.clear();
    streets_at_intersection.clear();
    intersections_along_street.clear();
    adjacent_intersections.clear();
    all_streets.clear();
    all_segments_in_street.clear();
    OSM_id_tag_pairs.clear();
    
    //M2 Cache
    features.clear();
    intersections.clear();
    street_segs.clear();

    //M3 Cache clear
    nodes.clear();

    
    //M3 cache
//    nodes.clear();
    
    //Closing the Streets and OSM Database
    closeStreetDatabase(); 
    closeOSMDatabase();
}

//Function to find distance between to LatLon points
double findDistanceBetweenTwoPoints(LatLon point_1, LatLon point_2){
    double lat_average = 0.5 * (point_1.latitude() + point_2.latitude()) * kDegreeToRadian;

    double x1 = kEarthRadiusInMeters * point_1.longitude() * kDegreeToRadian * cos(lat_average);
    double x2 = kEarthRadiusInMeters * point_2.longitude() * kDegreeToRadian * cos(lat_average);

    double y1 = kEarthRadiusInMeters * point_1.latitude() * kDegreeToRadian;
    double y2 = kEarthRadiusInMeters * point_2.latitude() * kDegreeToRadian;

    double distance = sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
    return distance;
}

//Function that returns the length of a street segment with id street_segment_id
double findStreetSegmentLength (StreetSegmentIdx street_segment_id){
    return (street_segment_length[street_segment_id]); 
}

//Function that returns the travel time of a street segment with id street_segment_id
double findStreetSegmentTravelTime(StreetSegmentIdx street_segment_id){
    return (street_segment_travel_time[street_segment_id]);
}

// Returns the length of a given street in meters
// Speed Requirement --> high 
double findStreetLength(StreetIdx street_id) {
//    std:: unordered_map<StreetIdx, std:: vector<StreetSegmentIdx>> m1 = allSegmentsInSameStreet();
    std:: vector<StreetSegmentIdx> segments_of_street = all_segments_in_street[street_id];
    double street_length = 0;
    
    for ( auto i_segment : segments_of_street ) {
        street_length = street_length + findStreetSegmentLength( i_segment);
    }
    
    return street_length;
}

//Function that returns the ID of the closest intersection to the LatLon object my_position
IntersectionIdx findClosestIntersection (LatLon my_position){
    double shortest_distance = findDistanceBetweenTwoPoints(my_position, getIntersectionPosition(0));
    LatLon current_intersection;
    IntersectionIdx closest_intersection = -1;
    int num_intersections = getNumIntersections();
    int current_distance;


    for (int i = 1; i < num_intersections; i++){
        current_intersection = getIntersectionPosition(i);
        current_distance = findDistanceBetweenTwoPoints(my_position, current_intersection);
        if(current_distance < shortest_distance){
            shortest_distance = current_distance;
            closest_intersection = i;
        }
    }
    return closest_intersection;
}

//Function that returns a vector of all streets that are incident at an intersection with id intersection_id
std::vector<StreetSegmentIdx> findStreetSegmentsOfIntersection(IntersectionIdx intersection_id) {
    return (streets_at_intersection[intersection_id]);
}

//Function that returns a vector of all intersections that are incident on a street with street_id
std::vector<IntersectionIdx> findIntersectionsOfStreet(StreetIdx street_id) {
    return (intersections_along_street[street_id]);
}

//Function that returns all the intersections of two streets with ids street_id1 and street_id2
std::vector<IntersectionIdx> findIntersectionsOfTwoStreets(StreetIdx street_id1, StreetIdx street_id2) {
    std::vector<IntersectionIdx> common_intersections;
    std::vector<IntersectionIdx> intersections_street1 = intersections_along_street[street_id1];
    std::vector<IntersectionIdx> intersections_street2 = intersections_along_street[street_id2];
    int num_intersections1 = intersections_street1.size();
    int num_intersections2 = intersections_street2.size();
    for(int i = 0; i < num_intersections1; i++){
        for(int j = 0; j < num_intersections2; j++){
            if (intersections_street1[i] == intersections_street2[j]){
                common_intersections.push_back(intersections_street1[i]);
            }
        }
    }
    helper_removeDuplicates(common_intersections);
    return common_intersections;

}       

std::vector<IntersectionIdx> findAdjacentIntersections(IntersectionIdx intersection_id){
    return(adjacent_intersections[intersection_id]);
}


// Returns all street ids corresponding to street names that start with the 
// given prefix 
// The function should be case-insensitive to the street prefix. 
// The function should ignore spaces.
//  For example, both "bloor " and "BloOrst" are prefixes to 
// "Bloor Street East".
// If no street names match the given prefix, this routine returns an empty 
// (length 0) vector.
// You can choose what to return if the street prefix passed in is an empty 
// (length 0) string, but your program must not crash if street_prefix is a 
// length 0 string.
// Speed Requirement --> high 
std::vector<StreetIdx> findStreetIdsFromPartialStreetName(std::string street_prefix) {  
    std:: vector<StreetIdx> streets_found;
 
    // returns empty vector if length is 0
    if (street_prefix.length() == 0) {
        return streets_found;
    }
    
    //format prefix
    correctedStreetPrefix( street_prefix);
    
    //find the prefix from preloaded streets unordered map and store in vector created
    streets_found = all_streets[street_prefix];
    
    return streets_found;  
}


POIIdx findClosestPOI(LatLon my_position, std::string POItype){
    double shortest_distance = 99999999;
    double current_distance = 0;
    POIIdx closest_poi = -1;
    std::string poi_type;
    int num_poi = getNumPointsOfInterest();
    for(int poi = 0; poi < num_poi; poi++){
        poi_type = getPOIType(poi);
        correctedStreetPrefix(poi_type);
        correctedStreetPrefix(POItype);
        if(poi_type == POItype){
            LatLon current_poi = getPOIPosition(poi);
            current_distance = findDistanceBetweenTwoPoints(my_position, current_poi);
            if(current_distance < shortest_distance){
                closest_poi = poi;
                shortest_distance = current_distance;
            }
        }
    }  
    return closest_poi;
}

// Returns the area of the given closed feature in square meters
// Assume a non self-intersecting polygon (i.e. no holes)
// Return 0 if this feature is not a closed polygon.
// Speed Requirement --> moderate
double findFeatureArea(FeatureIdx feature_id){ 
    double area = 0;
    
    // loop through each feature for all feature points
    for ( int pointNum = 0; pointNum < getNumFeaturePoints(feature_id) - 1; pointNum++ ) {
        LatLon first = getFeaturePoint( feature_id, pointNum);
        LatLon second = getFeaturePoint( feature_id, pointNum + 1);
        
        // return 0 if not a polygon
        if ( getFeaturePoint( feature_id, 0).latitude() != getFeaturePoint( feature_id, getNumFeaturePoints( feature_id)-1).latitude()
        || getFeaturePoint( feature_id, 0).longitude() != getFeaturePoint( feature_id, getNumFeaturePoints( feature_id)-1).longitude()) {
            return 0;
        }
        
        
        std:: pair<std::pair<double, double>, std::pair<double, double>> latlon_converted;
        latlon_converted = convertLatLon(first, second); //converted lat lon pair
        
        double x1 = latlon_converted.first.first;
        double y1 = latlon_converted.first.second;
        double x2 = latlon_converted.second.first;
        double y2 = latlon_converted.second.second;
        
        // area calculated of closed polygon
        double small_area = ( ((y1-y2) * ((x1+x2))/2.0) );
        area = area + small_area;
    }
    
    // if area is negative multiply by -1
    if ( area < 0 ) {
        return -area;
    }
    

    return area;

}

