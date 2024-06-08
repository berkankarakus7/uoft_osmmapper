/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include "m3_output_funcs.h"
#include "globals.h"

/*
 * This function returns a vector of directions as a string for an entered path route
 * @param path_found vector of street segments for found path
 * @return vector of string of directions to take 
 */
std:: vector<std:: string> setPathDirections( std:: vector<StreetSegmentIdx> path_found) {
    std:: vector<std:: string> path_turn_directions;
    
    int cur_seg;
    int cur_street;
    int next_seg;
    int next_street;
    for ( int seg_idx = 0; seg_idx < path_found.size() - 1; seg_idx++ ) {
        cur_seg = path_found[seg_idx];
        cur_street = getStreetSegmentInfo( path_found[seg_idx]).streetID;
        next_seg = path_found[seg_idx + 1];
        next_street = getStreetSegmentInfo( path_found[seg_idx + 1]).streetID;
        
        if ( cur_street == next_street ) {
            path_turn_directions.push_back( "Go straight");
        } else {
            
            
            LatLon node_points[3];
            setVectorPointsPositions( node_points, cur_seg, next_seg);
            double determinant = calculateDeterminant( node_points);
            
            if ( determinant < 0 ) {
                path_turn_directions.push_back( "Turn Left");
            } else {
                path_turn_directions.push_back( "Turn Right");
            }        
        }
    }
    std:: reverse(path_turn_directions.begin(), path_turn_directions.end());
    return path_turn_directions;
    
}

/* 
 * This function creates two vectors to calculate the cross product
 * @param node points of intersection ids
 * @param current seg
 * @param next seg 
*/
void setVectorPointsPositions( LatLon *node_points, StreetSegmentIdx cur_seg, StreetSegmentIdx next_seg) {
    
    int cur_start = getStreetSegmentInfo( cur_seg).from;
    int cur_end = getStreetSegmentInfo( cur_seg).to;
    int next_start = getStreetSegmentInfo( next_seg).from;
    int next_end = getStreetSegmentInfo( next_seg).to;
    
    if ( cur_end == next_start ) {
        node_points[0] = getIntersectionPosition( cur_start);
        node_points[1] = getIntersectionPosition( cur_end);
        node_points[2] = getIntersectionPosition( next_end);
    }
    else if ( cur_end == next_end ) {
        node_points[0] = getIntersectionPosition( cur_start);
        node_points[1] = getIntersectionPosition( cur_end);
        node_points[2] = getIntersectionPosition( next_start);
    } else if ( cur_start == next_start ) {
        node_points[0] = getIntersectionPosition( cur_end);
        node_points[1] = getIntersectionPosition( cur_start);
        node_points[2] = getIntersectionPosition( next_end);
    } else if ( cur_start == next_end ) {
        node_points[0] = getIntersectionPosition( cur_end);
        node_points[1] = getIntersectionPosition( cur_start);
        node_points[2] = getIntersectionPosition( next_start);
    }
}

/* 
 * This function returns the last three intersection ids of a path found as node points for a current segment
 * @param node array of intersection ids
 * @param current seg
 * @param next_seg 
 */
void setVectorPointsIds( IntersectionIdx *node_points, StreetSegmentIdx cur_seg, StreetSegmentIdx next_seg) {
    
    int cur_start = getStreetSegmentInfo( cur_seg).from;
    int cur_end = getStreetSegmentInfo( cur_seg).to;
    int next_start = getStreetSegmentInfo( next_seg).from;
    int next_end = getStreetSegmentInfo( next_seg).to;
    
    if ( cur_end == next_start ) {
        node_points[0] =  cur_start;
        node_points[1] = cur_end;
        node_points[2] = next_end;
    }
    else if ( cur_end == next_end ) {
        node_points[0] = cur_start;
        node_points[1] = cur_end;
        node_points[2] = next_start;
    } else if ( cur_start == next_start ) {
        node_points[0] = cur_end;
        node_points[1] = cur_start;
        node_points[2] = next_end;
    } else if ( cur_start == next_end ) {
        node_points[0] = cur_end;
        node_points[1] = cur_start;
        node_points[2] = next_start;
    }
}

/* 
 * This function return the intersection id of the last segment in the path drawn
 * @param current segment
 * @param next segment
 * @return intersection id of last node
 */
int setLastVectorTipId( StreetSegmentIdx cur_seg, StreetSegmentIdx next_seg) {
    
    int cur_start = getStreetSegmentInfo( cur_seg).from;
    int cur_end = getStreetSegmentInfo( cur_seg).to;
    int next_start = getStreetSegmentInfo( next_seg).from;
    int next_end = getStreetSegmentInfo( next_seg).to;
    
    if ( cur_end == next_start ) {
        return next_end;
    }
    else if ( cur_end == next_end ) {
        return next_start;
    } else if ( cur_start == next_start ) {
        return next_end;
    } else if ( cur_start == next_end ) {
        return next_start;
    }
    
    return 0;
}

/* 
 * This function calculated determinant for cross products
 * @param latlon node points
 * @return the result
 */
double calculateDeterminant( LatLon *node_points) {
    
    double v1_x = node_points[1].longitude() - node_points[0].longitude();
    double v1_y = node_points[1].latitude() - node_points[0].latitude();
    double v2_x = node_points[2].longitude() - node_points[1].longitude();
    double v2_y = node_points[2].latitude() - node_points[1].latitude();

    double determinant = (v1_x*v2_y) - (v1_y*v2_x);
    return determinant;
}

/*
 * This function write directions and the length
 * for a path as a string
 * @param path_found vector of street segments for path found
 * @return string of directions
 */
std:: string writeDirectionsAsString( std:: vector<StreetSegmentIdx> path_found) {
    std:: string dir_message = "";
    std:: vector<std:: string> directions = setPathDirections( path_found);
    
    if ( path_found.empty() ) {
        dir_message = "no route found\n";
        return dir_message;
    }
    
    
    if ( path_found.size() == 1 ) {
        dir_message = "destination soon " + street_segs[path_found[0]].street_name + "\n";
        return dir_message;
    }
    
    int is_straight_counter = 0;
    for ( StreetSegmentIdx seg_idx = 0, seg_idx_end = path_found.size() - 1; seg_idx < path_found.size() - 1 && seg_idx_end > 0;
            seg_idx++, seg_idx_end-- ) {
        if ( street_segs[path_found[seg_idx_end]].street_name != "unknown" ) {
            if ( directions[seg_idx] == "Go straight") {
                if (is_straight_counter == 0 ) {
                    int sub_path_length = findTravelSegmentLength( seg_idx, directions, path_found);
                    dir_message = dir_message + directions[seg_idx];
                    dir_message = dir_message + " through " + street_segs[path_found[seg_idx_end]].street_name;
                    dir_message = dir_message + " " + std:: to_string(sub_path_length) + " meters";
                    dir_message = dir_message + "   \n";
                }
                is_straight_counter++;
            } else {
                is_straight_counter = 0;
                dir_message = dir_message + directions[seg_idx];
                dir_message = dir_message + " from " + street_segs[path_found[seg_idx_end]].street_name;
                dir_message = dir_message + " to " + street_segs[path_found[seg_idx_end - 1]].street_name;
                dir_message = dir_message + "   \n";
            }
     
            if ( seg_idx_end == 1 ) {
                IntersectionIdx a;
                a = setLastVectorTipId( path_found[seg_idx_end], path_found[seg_idx_end - 1]);
                dir_message = dir_message + "Arrived to destination intersection: " + getIntersectionName(a);
                dir_message = dir_message + "   \n";
            }
        }
    }
    
    return dir_message;
}

/* 
 * This function calculates the segment length to be traveled
 * @param seg_idx current segment idx to be calcualted
 * @param directions a vector of directions string
 * @param path_found path to be found of street segments
 * @return int length of segment
 */
int findTravelSegmentLength( StreetSegmentIdx seg_idx, std:: vector<std:: string> directions, std:: vector<StreetSegmentIdx> path_found) {
    
    std:: string cur_dir = directions[0];
    // int i = 0;
    int street_length = findStreetSegmentLength(path_found[seg_idx]);
    for(int i = seg_idx; directions[i] == directions[i+1]; i++){
        street_length = street_length + findStreetSegmentLength( path_found[i+1]);
    }
    return street_length;
}

/*This function displays directions for an entered route. It also displays how long are the
 * street segment paths for the User Interface
 * @param application
 * @param widget 
 */
void displayDirectionsButton( GtkWidget* /*widget*/, ezgl:: application* application) {    
//    application->update_message(writeDirectionsAsString(path).c_str());
    
//    GtkWidget create_popup_message( "Directions", writeDirectionsAsString(path).c_str());
    application -> refresh_drawing();
    if ( path_created.empty() ) {
        application->update_message("Sorry, there is no path to display");
        return;
    } else {
        application->update_message("Directions for the path displayed");
    }
    GtkWidget *popup_window;

    popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(popup_window), "Directions To");//title position
    gtk_window_set_default_size(GTK_WINDOW(popup_window),300,250);//size
    gtk_window_set_position(GTK_WINDOW(popup_window), GTK_WIN_POS_CENTER);

    gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
    g_signal_connect (G_OBJECT (popup_window),
                      "focus-out-event",
                      NULL,
                      NULL);

    GtkWidget* directions_label = gtk_label_new( writeDirectionsAsString( path_created).c_str());
    gtk_label_set_justify(GTK_LABEL(directions_label), GTK_JUSTIFY_CENTER);
    gtk_container_add(GTK_CONTAINER(popup_window), directions_label);
    
    gtk_widget_show_all (popup_window);
    gtk_widget_grab_focus (popup_window);
    
    
}

/*This function displays help button help button for the User Interface
 * @param application
 * @param widget 
 */
void displayHelpButton( GtkWidget* /*widget*/, ezgl:: application* application) {
    
    application->update_message("Help button is displayed");
    GtkWidget *popup_window;
    application -> refresh_drawing();
    popup_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(popup_window), "Help Page");//title position
    gtk_window_set_default_size(GTK_WINDOW(popup_window),300,250);//size
    gtk_window_set_position(GTK_WINDOW(popup_window), GTK_WIN_POS_CENTER);

    gtk_widget_set_events (popup_window, GDK_FOCUS_CHANGE_MASK);
    g_signal_connect (G_OBJECT (popup_window),
                      "focus-out-event",
                      NULL,
                      NULL);

    std::stringstream help_message;
        
    help_message    << "Welcome. We Hate Google Maps!\n"
                    << std:: endl << "=> To see Points of Interests, click on the \"Show POIs\" button."
                    << std:: endl << "=> To find a route between two intersections, "
                    << std:: endl << "      1) Turn Navigate to ON. It is set to OFF by default."
                    << std:: endl << "      2) Intersections that you can select will be highlighted."
                    << std:: endl << "      3) Click on any highlighted intersections to select the source and destination. A route will be displayed. "
                    << std:: endl << "      4) Clicking a third intersection will change the destination. "
                    << std:: endl << "      5) To see travel directions, click on \"Show Directions\"."

                    << std:: endl << "=> Alternatively,"
                    << std:: endl << "      1) Enter two streets whose intersections are to be the source. Click \"SEARCH\""
                    << std:: endl << "      2) Enter two streets whose intersections are to be the destination. Click \"SEARCH\""
                    << std:: endl << "      3) To see travel directions, click on \"Show Directions\"." 
                    << std::endl;
    
    GtkWidget* directions_label = gtk_label_new( help_message.str().c_str());
    gtk_container_add(GTK_CONTAINER(popup_window), directions_label);
    
    gtk_widget_show_all (popup_window);
    gtk_widget_grab_focus (popup_window);
}

//void writeDirections( std:: vector<StreetSegmentIdx> path_found) {
//    
//    std:: vector<std:: string> directions = setPathDirections( path_found);
//    
//    if ( path_found.empty() ) {
//        std:: cout << "no route found" << std::endl;
//        return;
//    }
//    
//    
//    if ( path_found.size() == 1 ) {
//        std:: cout << "destination soon" << street_segs[path_found[0]].street_name << std:: endl;
//        return;
//    }
//    
//    int is_straight_counter = 0;
//    for ( StreetSegmentIdx seg_idx = 0, seg_idx_end = path_found.size() - 1; seg_idx < path_found.size() - 1 && seg_idx_end > 0;
//            seg_idx++, seg_idx_end-- ) {
//        if ( street_segs[path_found[seg_idx_end]].street_name != "unknown" ) {
//            if ( directions[seg_idx] == "Go straight") {
//                if (is_straight_counter == 0 ) {
//                    int sub_path_length = findTravelSegmentLength( seg_idx, directions, path_found);
//                    std:: cout << directions[seg_idx];
//                    std:: cout << " through " << street_segs[path_found[seg_idx_end]].street_name;
//                    std:: cout << " " << sub_path_length << " meters";
//                    std:: cout << std:: endl;
//                }
//                is_straight_counter++;
//            } else {
//                is_straight_counter = 0;
//                std:: cout << directions[seg_idx];
//                std:: cout << " from " << street_segs[path_found[seg_idx_end]].street_name;
//                std:: cout << " to " << street_segs[path_found[seg_idx_end - 1]].street_name;
//                std:: cout << std:: endl;
//            }
//     
//            if ( seg_idx_end == 1 ) {
//                IntersectionIdx a;
//                a = setLastVectorTipId( path_found[seg_idx_end], path_found[seg_idx_end - 1]);
//                std:: cout << "Arrived to destination intersection: " << getIntersectionName(a);
//                std:: cout << std:: endl;
//            }
//        }
//    }
//}

// gboolean
// on_popup_focus_out (GtkWidget *widget,
//                     GdkEventFocus *event,
//                     gpointer data)
// 
//   return TRUE;
// }

//void displayDirectionsButton( GtkWidget* /*widget*/, ezgl:: application* application) {
//    std:: vector<StreetSegmentIdx> path = {118396, 4178, 4179, 4180, 4181, 4182, 156305, 153398, 191095, 153392, 153393, 153394};
//    
////    application->update_message(writeDirectionsAsString(path).c_str());
//    
//    GObject* window = application->get_object(application->get_main_window_id().c_str());
//    if ( !GTK_IS_WINDOW( window ) ) {
//        g_print( "The window is wrong\n");
//        return;
//    }
//    
//    GtkWidget* dialog = gtk_dialog_new_with_buttons( 
//            "Directions Display",
//            (GtkWindow*) window,
//            GTK_DIALOG_MODAL,
//            "Close",
//            GTK_RESPONSE_REJECT,
//            NULL);
//    
//    g_signal_connect_swapped (dialog,
//                           "response",
//                           G_CALLBACK (gtk_widget_destroy),
//                           dialog);
//    
//    const gchar* directions_message = writeDirectionsAsString( path).c_str();
//    GtkWidget* dialog_content = gtk_dialog_get_content_area(GTK_DIALOG( dialog));
//    GtkWidget* directions_label = gtk_label_new( writeDirectionsAsString( path).c_str());
//    gtk_container_add(GTK_CONTAINER(dialog_content), directions_label);
//    gtk_widget_show_all( dialog);
//    
//}