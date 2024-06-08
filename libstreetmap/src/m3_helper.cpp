#include "m3_helper.h"
#include "globals.h"
#include <unordered_map>
#include <queue>
#include <list>

struct Node; //forward declaration of struct
struct WaveElement;
struct CompareTravelTime;
std::vector<Node> nodes;
std::vector<int> visited_nodes;

class CompareTravelTime{
    public:
    bool operator()(const WaveElement& w1, const WaveElement& w2){
        return (w1.f > w2.f);
    }
};

void loadNodes(){
    int num_intersections = getNumIntersections();
    // std::cout << num_intersections << std::endl;
    nodes.resize(num_intersections);
    for(int i = 0; i < num_intersections; i++){
        Node new_node;
        //new_node.id = i;
        // new_node.outgoing_edges = streets_at_intersection[i];
       for(int j = 0; j < streets_at_intersection[i].size(); j++){
           /*
           if the street segment is a one way, then check if it starts at the current
           intersection, i.e. its from is the current intersection. if so, then that means
           it is an outgoing edge from the current intersection
           */
           if(getStreetSegmentInfo(streets_at_intersection[i][j]).oneWay){
               if(getStreetSegmentInfo(streets_at_intersection[i][j]).from == i){
                   new_node.outgoing_edges.push_back(streets_at_intersection[i][j]);
               }                                                                         
           }
           else{
               /*
               else if it is a two-way street then the street segment is always an outgoing 
               edge
               */
               new_node.outgoing_edges.push_back(streets_at_intersection[i][j]);
           }
       }
        new_node.visited = false;
        new_node.reaching_edge = -1;
        new_node.best_time = INF;
        new_node.best_f = INF;
        nodes[i] = new_node;
    }
}

std::vector<StreetSegmentIdx> searchPath(int source, int destination, double turn_penalty){
    //reset all the nodes visited during previous path search
    // Node * visited_node;
    int num_visited_nodes = visited_nodes.size();
    for(int i = 0; i < num_visited_nodes; i++){
        // visited_node = & (nodes[visited_nodes[i]]);
        nodes[visited_nodes[i]].best_time = INF;
        nodes[visited_nodes[i]].reaching_edge = -1;
        nodes[visited_nodes[i]].best_f = INF;
    }
    visited_nodes.clear();

    std::priority_queue<WaveElement, std::vector<WaveElement>, CompareTravelTime> wavefront;

    wavefront.push(WaveElement(source, NO_EDGE, 0, getHeuristic(source, destination)));

    while(!wavefront.empty()){
        WaveElement current_wave_element = wavefront.top();
        wavefront.pop();
        int current_node_id = current_wave_element.node_id;
        // Node * current_node = & (nodes[current_wave_element.node_id]);
        if(current_wave_element.f < nodes[current_node_id].best_f){
            
            visited_nodes.push_back(current_wave_element.node_id);
            nodes[current_node_id].best_time = current_wave_element.travel_time;
            nodes[current_node_id].best_f = current_wave_element.f;
            nodes[current_node_id].reaching_edge = current_wave_element.edge_id;


            if(current_wave_element.node_id == destination) {
                return (traceBack(destination));
            }

            for(int i = 0; i < nodes[current_node_id].outgoing_edges.size(); i++){
                int out_edge_id = nodes[current_node_id].outgoing_edges[i];

                if(out_edge_id == current_wave_element.edge_id) continue;

                StreetSegmentInfo next_info = street_segment_info[out_edge_id];
                StreetSegmentInfo current_info;

                float penalty = 0.0f;

                if(current_wave_element.edge_id != -1){
                    current_info = street_segment_info[current_wave_element.edge_id];
                    if (next_info.streetID != current_info.streetID){
                        penalty = (float)turn_penalty;
                    }
                    else penalty = 0.0f;
                }
                
                //assigning the value of to_node_id
                int to_node_id = (next_info.to == current_wave_element.node_id) ? next_info.from : next_info.to ;

                float heuristic = getHeuristic(to_node_id, destination);
                
                wavefront.push(WaveElement(to_node_id, out_edge_id, 
                    nodes[current_node_id].best_time + street_segment_travel_time[out_edge_id] + penalty, heuristic));
            }
        }
    }
    std::vector <int> empty_vector;
    return empty_vector;
}

std::vector<StreetSegmentIdx> traceBack(int destination){
    std::list<StreetSegmentIdx> path;
    int current_node = destination;
    int previous_edge = nodes[current_node].reaching_edge;
    std::vector<int> vec;

    while(previous_edge != NO_EDGE){
        path.push_front(previous_edge);
        int from = getStreetSegmentInfo(previous_edge).from;
        int to = getStreetSegmentInfo(previous_edge).to;
        current_node = (current_node == to) ? from : to;
        previous_edge = nodes[current_node].reaching_edge;
    }   
    int n = path.size();
    for(int i = 0; i < n; i++){
        vec.push_back(path.front());
        path.pop_front();
    }
    return (vec);
}