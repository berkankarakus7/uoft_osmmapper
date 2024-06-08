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

#include <chrono>
#include "m2_helper.h"
#include "m3_output_funcs.h"
#include "globals.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include <string.h>


GtkEntry *startSearchEntry;
std::vector<StreetSegmentIdx> path_to_show;
std::pair<IntersectionIdx, IntersectionIdx> navigation_points = std::make_pair(-1, -1);
// ezgl::renderer *g;


//Callback Functions
void initial_setup( ezgl::application *application, bool /*new_window*/ );
void find_button(GtkWidget */*widget*/, ezgl::application *application);
void draw_main_canvas(ezgl:: renderer *g);
void toggleIntersectionData(GtkWidget* /*Widget*/, ezgl::application* application);
void findFunc(GtkWidget*, ezgl::application* application);
void togglePOIData(GtkWidget* /*Widget*/, ezgl::application* application);
void act_on_mouse_click(ezgl::application* app, GdkEventButton* event, double x, double y);

void searchChanged(GtkEntry *searchEntry);
std::vector<int> find_button_two_streets(std::string firstStreetPartial, std::string secondStreetPartial, ezgl::application *application);
void navigateButton(GtkWidget* /*Widget*/, ezgl::application* application);

int start_inter = -1;
int end_inter = -1;
int third_inter = -1;

bool tog = false;

void search_bar_callback(GtkComboBoxText */*widget*/, ezgl::application *application);
void findButton(ezgl::renderer *g);
void act_on_findButton(GtkWidget* /*Widget*/, ezgl::application* application);

std::vector<IntersectionIdx> UserClickedIntersec, UserInputIntersec;
int source = -1;
int destination = -1;

void drawMap() {
    ezgl::application::settings settings;
    settings.main_ui_resource = "libstreetmap/resources/main.ui";
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";
    ezgl::application application(settings);
    //g = application.get_renderer();
    application.add_canvas("MainCanvas", draw_main_canvas, initial_world); 
    application.run(initial_setup, act_on_mouse_click, nullptr, nullptr);
}


void draw_main_canvas(ezgl::renderer *g) {
    g -> set_color(230, 230, 230);
    ezgl::rectangle current_world = g -> get_visible_world();
    g -> fill_rectangle(current_world.m_first, current_world.m_second);
    drawFeatures(g);
    drawStreets(g);
    if(show_poi_data) drawPOIs(g);
    // if(path_to_show.size() != 0 && show_route){
    //     displayRoute(path_to_show, g);
    // }
    if(show_route) drawIntersections(g);
}


void act_on_mouse_click(ezgl::application * app, GdkEventButton * event, double x, double y) {
    if (show_route) {
        if (event-> button == 1) {
            //clear any paths that existed before
            for (StreetSegmentIdx seg_idx = 0; seg_idx < path_created.size(); seg_idx++) {
                street_segs[path_created[seg_idx]].highlight = false;
            }

            LatLon pos = LatLon(latFromY(y), lonFromX(x));
            int intersection_id = findClosestIntersection( pos);
            //if the new intersection is not the same as an already existing start/end point, 
            if(intersection_id != start_inter &&
               intersection_id != end_inter){
                //if the start point has not been set
                if (start_inter == -1){
                    start_inter = intersection_id;
                    intersections[start_inter].highlight = true;
                }
                //if the end point has not been set, set it and calculate path
                else if (start_inter != -1 && end_inter == -1){
                    end_inter = intersection_id;
                    // path_created = findPathBetweenIntersections(std::make_pair(start_inter, end_inter), 0.0f);
                    intersections[end_inter].highlight = true;
                }
                //if start and end point are both set
                //the end point is then changed to the newly selected position
                //calculate new path
                else if (start_inter != -1 && end_inter != -1){
                    intersections[end_inter].highlight = false;
                    end_inter = intersection_id;
                    intersections[end_inter].highlight = true;
                    // path_created = findPathBetweenIntersections(std::make_pair(start_inter, end_inter), 0.0f);
                }
            }
            //else if new point matches either the start/end point
            else {
                //if it matcches the start point, dont display the start point
                if(intersection_id == start_inter){
                    intersections[start_inter].highlight = false;
                    start_inter = -1;
                }
                //else, same for end point
                else if (intersection_id == end_inter){
                    intersections[end_inter].highlight = false;
                    end_inter = -1;
                }
                //clear any existing paths
                path_created.clear();
            }
            

            if(end_inter != -1 && start_inter != -1){
                path_created = findPathBetweenIntersections(std::make_pair(end_inter, start_inter), 0.0f);
                writeDirectionsAsString( path_created);

            }

            if(path_created.size() != 0){
                for ( StreetSegmentIdx seg_idx = 0; seg_idx < path_created.size(); seg_idx++) {
                    street_segs[path_created[seg_idx]].highlight = true;
                }
                double travel_time = computePathTravelTime(path_created, 0.0);
                int travel_time_seconds = static_cast<int>(travel_time);
                int minutes = travel_time_seconds / 60;
                int seconds = travel_time_seconds % 60;

                app->update_message(std::to_string(minutes) + " minutes and " + std::to_string(seconds) + " seconds to arrival point: "
                                            + getIntersectionName( end_inter)
                                            + ". Click Show Directions Button for detail!"); 
            }

            // if ( click_counter == 0 ) {
            //     intersections[intersection_id].highlight = !intersections[intersection_id].highlight;
            //     click_counter++;
            //     start_inter = intersection_id;

            // } else if ( click_counter == 1 ) {
            //     if ( intersections[intersection_id].highlight ) {
            //         click_counter--;
            //         start_inter = -1;
            //         end_inter = -1;
            //     } else {
            //         click_counter++;
            //         end_inter = intersection_id;
            //     }
            //     intersections[intersection_id].highlight = !intersections[intersection_id].highlight;
            // } else if ( click_counter == 2 ) {
            //     if ( intersections[intersection_id].highlight ) {
            //         click_counter--;
            //         intersections[intersection_id].highlight = !intersections[intersection_id].highlight;
            //         if ( intersection_id == start_inter ) {
            //             start_inter = end_inter;
            //             end_inter = -1;
            //         } else {
            //             end_inter = -1;
            //         }
            //     }
            // }
            
            // if ( start_inter != - 1 && end_inter != -1 ) {
            //     path_created = findPathBetweenIntersections(std::make_pair(end_inter, start_inter), 0);
            //     writeDirectionsAsString( path_created);
            // }
            // if ( click_counter == 2 ) {
            //     for ( StreetSegmentIdx seg_idx = 0; seg_idx < path_created.size(); seg_idx++) {
            //         street_segs[path_created[seg_idx]].highlight = true;
            //     }
            // } else {
            //     for ( StreetSegmentIdx seg_idx = 0; seg_idx < path_created.size(); seg_idx++) {
            //             street_segs[path_created[seg_idx]].highlight = false;
            //     }
            // }
        }
        app -> refresh_drawing();
    }
}



void initial_setup(ezgl::application *application, bool /*new_window*/){
    GtkWindow* main_window = (GtkWindow *)application -> get_object("MainWindow");
    gtk_window_set_title(main_window, "ECE297 Mapper Project cd045");
    application -> create_button("Search", 3, act_on_findButton);
    application -> create_button("Show POIs", 7, togglePOIData);
    application -> create_combo_box_text("Choose City", 8, switch_cities, all_city_names);
    application -> create_button("Show Directions", 10, displayDirectionsButton);
    application -> create_button("Help", 11, displayHelpButton);
    application -> create_button("Navigate - OFF", 12, navigateButton);
    application -> update_message("Click on the Help Button for further information");
}

void toggleIntersectionData(GtkWidget* /*Widget*/, ezgl::application* application){
    show_intersection_data = !show_intersection_data;
    if(!show_intersection_data){
        application -> change_button_text("Find Nearest Intersection - ON", "Find Nearest Intersection - OFF");
        application -> update_message("");
        for(int i = 0; i < intersections.size(); i++){
            intersections[i].highlight = false;
        }
    }
    else application -> change_button_text("Find Nearest Intersection - OFF" , "Find Nearest Intersection - ON");
    application -> refresh_drawing();
}

void togglePOIData(GtkWidget* /*Widget*/, ezgl::application* application){
    show_poi_data = !show_poi_data;
    if(show_poi_data){
        application -> change_button_text("Show POIs", "Hide POIs");
        application -> update_message("");
    }
    else {
        application -> change_button_text("Hide POIs", "Show POIs");
    }
    application -> refresh_drawing();
}

void navigateButton(GtkWidget* /*Widget*/, ezgl::application* application){
    show_route = !show_route;
    if(!show_route){
        application -> change_button_text("Navigate - ON", "Navigate - OFF");
        application -> update_message("");
        for (StreetSegmentIdx seg_idx = 0; seg_idx < path_created.size(); seg_idx++) {
            street_segs[path_created[seg_idx]].highlight = false;
        }
        intersections[start_inter].highlight = false;
        intersections[end_inter].highlight = false;
        start_inter = -1;
        end_inter = -1;
        application -> refresh_drawing();
        path_created.clear();
        click_counter = 0;
    }
    else {
        application -> change_button_text("Navigate - OFF", "Navigate - ON");
        application -> refresh_drawing();
    }
}

void act_on_findButton(GtkWidget* /*Widget*/, ezgl::application* application) {
    // Get the text written in the widget
    GObject *text_entry1 = application->get_object("search1");
    GtkEntry* search1 = GTK_ENTRY(text_entry1);
    const gchar* text1 = gtk_entry_get_text(search1);
    
    GObject *text_entry2 = application->get_object("search2");
    GtkEntry* search2 = GTK_ENTRY(text_entry2);
    const gchar* text2 = gtk_entry_get_text(search2);
    
    std::cout << text1<< std::endl;
    std::cout << text2<< std::endl;
    
    std::vector<StreetIdx> streetName1;
    std::vector<StreetIdx> streetName2;
    streetName1 = findStreetIdsFromPartialStreetName(text1);
    streetName2 = findStreetIdsFromPartialStreetName(text2);

        //If vector empty-no street exists
        if (streetName1.size() == 0) {
            std::cout << text1 << " is not a valid street name. Please try again by pressing find" << std::endl;
        }

        if (streetName2.size() == 0) {
            std::cout << text2 << " is not a valid street name. Please try again by pressing find" << std::endl;
        }
    
        //a street id corresponding to the street name is found
        
        std::vector<IntersectionIdx> highlightedIntersections;
    if (streetName1.size() == 1 && streetName2.size() == 1) {
        highlightedIntersections = findIntersectionsOfTwoStreets(streetName1[0], streetName2[0]);
    } else if (streetName1.size() > 1 && streetName2.size() == 1) {
        // Use the first street index in case more than one street index is found for streetName1
        highlightedIntersections = findIntersectionsOfTwoStreets(streetName1[0], streetName2[0]);
    } else if (streetName1.size() == 1 && streetName2.size() > 1) {
        // Use the first street index in case more than one street index is found for streetName2
        highlightedIntersections = findIntersectionsOfTwoStreets(streetName1[0], streetName2[0]);
    } else if (streetName1.size() > 1 && streetName2.size() > 1) {
        // Use the first street index for both streetName1 and streetName2 in case more than one street index is found for both
        highlightedIntersections = findIntersectionsOfTwoStreets(streetName1[0], streetName2[0]);
    }
    

    // Highlight the found intersections by setting their `highlight` property to true.
    // for (int i = 0; i < highlightedIntersections.size(); i++){
    //     intersections[highlightedIntersections[i]].highlight = true;
    // }
    intersections[highlightedIntersections[0]].highlight = true;
    application -> refresh_drawing();
    
    if (start_inter == -1){
        start_inter = highlightedIntersections[0];
    }
    else if (start_inter != -1 && end_inter == -1){
        end_inter = highlightedIntersections[0];
        path_created = findPathBetweenIntersections(std::make_pair(end_inter, start_inter), 0.0);
        writeDirectionsAsString(path_created);
        double travel_time = computePathTravelTime(path_created, 0.0);
        int travel_time_seconds = static_cast<int>(travel_time);
        int minutes = travel_time_seconds / 60;
        int seconds = travel_time_seconds % 60;

        application->update_message(std::to_string(minutes) + " minutes and " + std::to_string(seconds) + " seconds to arrival point: "
                                    + getIntersectionName( end_inter)
                                    + ". Click Show Directions Button for detail!"); 
        application -> refresh_drawing();
    }
}