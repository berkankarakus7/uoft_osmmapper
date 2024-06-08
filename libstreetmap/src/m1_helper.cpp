/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include "m1_helper.h"

float max_speed;
float min_pace;
std::vector<StreetSegmentInfo> street_segment_info;


// Return the value associated with this key on the specified OSMNode.
// If this OSMNode does not exist in the current map, or the specified key is 
// not set on the specified OSMNode, return an empty string.
// Speed Requirement --> high
std::string getOSMNodeTagValue (OSMID OSMid, std::string key) {
    std:: string found_value = "";
//    std:: unordered_map<OSMID , std:: vector< std:: pair<std:: string, std:: string>>> osm_node_map = OSMIdNodeTags();
    
    std:: vector< std:: pair< std:: string, std:: string>> tag_pairs_found = OSM_id_tag_pairs[OSMid];
    for ( int i = 0; i < tag_pairs_found.size(); i++ ) {
        if ( tag_pairs_found[i].first == key ) {
            found_value = tag_pairs_found[i].second;
            return tag_pairs_found[i].second;
        }
    } 
    return found_value;

}


/*
 * This function creates an unordered_map containing vectors with pair of two strings.
 *each vector stores all tag values for a specific node ID.
 * In each pair first value stores the OSMId and second string is the value of each Id.
 * Key of unordered map is OSMIDs .
 */
std:: unordered_map<OSMID , std:: vector< std:: pair<std:: string, std:: string>>> LoadOSMIdNodeTags() {
    std:: unordered_map<OSMID , std:: vector< std:: pair<std:: string, std:: string>>> OSM_nodes_map;
    
    for ( int node_id = 0; node_id < getNumberOfNodes(); node_id++ ) {
//        std:: vector<std:: pair<std:: string, std:: string>> stored_tags;
        const OSMNode* node_osm = getNodeByIndex( node_id);
        for ( int tag_count = 0; tag_count < getTagCount( node_osm); tag_count++ ) {
            std:: pair<std:: string, std:: string> tag_pair_found = getTagPair( node_osm, tag_count);
            OSM_nodes_map[node_osm->id()].push_back( getTagPair( node_osm, tag_count));
        }
        
    }
    return OSM_nodes_map;
    
}

/*
creates an unordered map stores all street segments in a given street ID.
key values are street IDs
and values are vectors storing all the street segment IDs in the same street
*/
std:: unordered_map<StreetIdx, std:: vector<StreetSegmentIdx>> allSegmentsInSameStreet() {
    std:: unordered_map<StreetIdx, std:: vector<StreetSegmentIdx>> streets_created_by_segs;
    
    // loop through each street segment
    for ( StreetSegmentIdx i= 0; i < getNumStreetSegments(); i++ ) {
        StreetSegmentInfo segment = getStreetSegmentInfo( i);
        StreetIdx insert_street_idx = segment.streetID; // get the street id of segment
        streets_created_by_segs[insert_street_idx].push_back( i); // insert to vector in unordered map
    }
    return streets_created_by_segs;
}


/*
Preloads all the street names from 0 to each character of substring ex:
"Bloor" is "B", "Bl", "Blo" ... 
returns an unordered_map of all street subprefixes. Keys are subprefix strings and
values are vectors holding integer Street IDs share the same prefix
*/
std:: unordered_map<std:: string, std:: vector<int>> loadStreetsName() {
    std:: unordered_map<std:: string, std:: vector<int>> all_streets_in;
    
    //loop through each street name
    for ( int i = 0; i < getNumStreets(); i++) {
        std:: string street = getStreetName( i);
        correctedStreetPrefix( street); //format name
        //loop through each substring from 0 to length - 1 for every street name
        for ( int up_to_character = 1; up_to_character <= street.length(); up_to_character++ ) {
            std:: string street_to_char = street.substr( 0, up_to_character);    
            all_streets_in[street_to_char].push_back( i);
        }
    }
    
    return all_streets_in;
    
}

// deletes all white spaces and makes all cases to lower
void correctedStreetPrefix( std::string &street_prefix) {
    
    street_prefix.erase(std::remove_if(street_prefix.begin(), street_prefix.end(), ::isspace),
    street_prefix.end()); //delete all white spaces
    
    std:: transform(street_prefix.begin(), street_prefix.end(), street_prefix.begin(), ::tolower); // all cases to lower
    
    return;
    
}

/*
Helper function that loops through all intersections and creates a vector for each intersection of all its adjacent
intersections.
*/
void helper_adjacentIntersections(){
    int num_intersections = getNumIntersections();
    StreetSegmentInfo current;
    adjacent_intersections.resize(num_intersections);
    std::vector<StreetSegmentIdx> segments_on_intersection;
    for(int i = 0; i < num_intersections; i++){
        segments_on_intersection = streets_at_intersection[i];
        for(int j = 0; j < segments_on_intersection.size(); j++){
            current = getStreetSegmentInfo(segments_on_intersection[j]);
            if(current.oneWay || current.to == current.from){
                if(current.to != i) adjacent_intersections[i].push_back(current.to);
            }
            else{
                if(current.to != i) adjacent_intersections[i].push_back(current.to);
                if(current.from != i) adjacent_intersections[i].push_back(current.from);
            }
        }
    }
    for(int i = 0; i < num_intersections; i++){
        helper_removeDuplicates(adjacent_intersections[i]);
    }
}

/*
helper_streetsAtIntersections - finds the intersections incident at all street segments
*/
void helper_streetsAtIntersections(){
    int num_intersections = getNumIntersections();
    //int num_streets = getNumStreets();
    int num_street_segments_intersection;
    StreetSegmentIdx current_segment;
    StreetSegmentInfo info;
    streets_at_intersection.resize(num_intersections);
    for(int intersection_id = 0; intersection_id < num_intersections; intersection_id++){
        num_street_segments_intersection = getNumIntersectionStreetSegment(intersection_id);
        for(int street_segment = 0; street_segment < num_street_segments_intersection; street_segment++){
            current_segment = getIntersectionStreetSegment(intersection_id, street_segment);
            streets_at_intersection[intersection_id].push_back(current_segment);
            if(street_segment == num_street_segments_intersection - 1){
                helper_removeDuplicates(streets_at_intersection[intersection_id]);
            }
        }
    }
}


//removeDuplicates removes all the duplicate entries in a vector using unordered sets
void helper_removeDuplicates(std::vector<int> &vec){ 
    std::vector<int>::iterator iter = vec.begin(); //pointer to the (end + 1)th element of the unique vector
    std::unordered_set<int> s;
    for(auto curr = vec.begin(); curr != vec.end(); curr++){
        if(s.insert(*curr).second){ //insert the current element of the vector into the newly created unordered set
                                    //if element inserted was unique (insert's return value's second member should be true)
            *iter++ = *curr;       //if unique element is inserted, overwrite the value at iter with the current value. 
                                    //increase iter by 1
        }
    }
    vec.erase(iter, vec.end()); //erase everything between iter and the end of the vector
}

/*
helper_streetSegments performs the following -
1. Finds the length of all street segments - stores in street_segment_length
2. Finds the travel time of all street segments - stores in street_segment_travel_time
3. Finds the intersections on a street - stores in intersections_along_street
4. Finds all the street segments that make up a street - stores in street_segments
*/
void helper_streetSegments(){
    double distance = 0;
    max_speed = 0;
    std::vector<LatLon> street_segment_sections;
    int num_curve_points = 0;
    int num_streets = getNumStreets();
    int num_street_segments = getNumStreetSegments();
    intersections_along_street.resize(num_streets);
    street_segments.resize(num_streets);
    street_segment_info.resize(num_street_segments);
    StreetSegmentInfo info;
      
    //loop that runs through all street segments; i++){
    for(int i = 0; i < num_street_segments; i++){

        info = getStreetSegmentInfo(i);   //get the StreetSegmentInfo struct variable of the i-th segment
        if(info.speedLimit > max_speed) max_speed = info.speedLimit;
        street_segment_info[i] = info;
        intersections_along_street[info.streetID].push_back(info.to); //store the intersection ids at corresponding street id index
        intersections_along_street[info.streetID].push_back(info.from);

        //code to find intersections at a street segment excluding duplicates)
        street_segments[info.streetID].push_back(info.from); //store the intersections on the street segment to the 2-d street-segment vector
        street_segments[info.streetID].push_back(info.to);

        distance = 0; //set length of the current street segment to 0
        num_curve_points = info.numCurvePoints;     //get number of curve points
        street_segment_sections.clear(); //clear the street_segment_sections

        street_segment_sections.push_back(getIntersectionPosition(info.from)); //first LatLon object is always the from intersection
        for(int j = 0; j < num_curve_points; j++){
            street_segment_sections.push_back(getStreetSegmentCurvePoint(i, j)); //push all the LatLons of curve points into the vector
        }

        street_segment_sections.push_back(getIntersectionPosition(info.to)); //last LatLon object is always the to intersection

        for(int k = 0 ; k <= num_curve_points; k++){
            distance = distance + findDistanceBetweenTwoPoints(street_segment_sections[k], street_segment_sections[k+1]); //find the distance of each street segment by 
                                                                                          //iterating through its "sections"
        }

        street_segment_length.push_back(distance);
        street_segment_travel_time.push_back(distance/info.speedLimit); //push the final length of the street segment calculated into the vector of all
                                                                        //street segment lengths
    }

    for(int i = 0; i < num_streets; i++){
        helper_removeDuplicates(intersections_along_street[i]);
    }
    min_pace = 1 / max_speed;

}

/*This function retruns a pair two x-y coordinats. Each pair stores two different pair that 
    stores the x-y coordinates converted from LatLon*/
std:: pair<std:: pair<double, double>, std:: pair<double, double>> convertLatLon( LatLon point_1, LatLon point_2) {
    double lat_average = 0.5 * (point_1.latitude() + point_2.latitude()) * kDegreeToRadian;

    //convert Lat to x coordinates
    double x1 = kEarthRadiusInMeters * point_1.longitude() * kDegreeToRadian * cos(lat_average);
    double x2 = kEarthRadiusInMeters * point_2.longitude() * kDegreeToRadian * cos(lat_average);

    //convert Lon to y coordinates
    double y1 = kEarthRadiusInMeters * point_1.latitude() * kDegreeToRadian;
    double y2 = kEarthRadiusInMeters * point_2.latitude() * kDegreeToRadian;

    std:: pair<double, double> first{x1,y1};
    std:: pair<double, double> second{x2,y2};
    
    std:: pair<std:: pair<double, double>, std:: pair<double, double>> two_converted_points {first, second};
    
    return two_converted_points;
}