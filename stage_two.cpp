
#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <boost/random/linear_congruential.hpp>
#include <boost/progress.hpp>
#include <boost/shared_ptr.hpp>

#include <emscripten/bind.h>
#include <cstddef>
#include <cstdlib>

#include "json.hpp"
using json = nlohmann::json;

typedef boost::rectangle_topology<> topology_type;

typedef topology_type::point_type point_type;

typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, 
                       boost::property<boost::vertex_name_t, std::string> > Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

typedef std::map<std::string, Vertex> NameToVertex;


Vertex get_vertex(const std::string& name, Graph& g, NameToVertex& names)
{
  NameToVertex::iterator i = names.find(name);
  if (i == names.end())
    i = names.insert(std::make_pair(name, add_vertex(name, g))).first;
  return i->second;
}


Graph g;
NameToVertex names;


std::string calculateGraph(std::string dataIn, double width, double height, double iterations) {


    // double width = 800;
    // double height = 600;
    json j = json::parse(dataIn);

    for(json::iterator it = j.begin(); it != j.end(); ++it) {
        json rowString = *it;
        add_edge(get_vertex(rowString[0], g, names), get_vertex(rowString[1], g, names), g);
    }
    typedef std::vector<point_type> PositionVec;
    PositionVec position_vec(num_vertices(g));
    typedef boost::iterator_property_map<PositionVec::iterator, 
                                    boost::property_map<Graph, boost::vertex_index_t>::type>
        PositionMap;
    PositionMap position(position_vec.begin(), get(boost::vertex_index, g));

    boost::minstd_rand gen;
    topology_type topo(gen, -width/2, -height/2, width/2, height/2);
    random_graph_layout(g, position, topo);
    fruchterman_reingold_force_directed_layout
        (g, position, topo,
        cooling(boost::linear_cooling<double>(iterations)));

    json jsonOut = json::object();

    boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
        json node = json::object();
        // node["name"] = get(boost::vertex_name, g, *vi);
        node["x"] = position[*vi][0];
        node["y"] = position[*vi][1];
        // jsonOut.push_back(node);
        jsonOut[get(boost::vertex_name, g, *vi)] = node;

    }

    std::string outStr = jsonOut.dump();
    return outStr;
}


EMSCRIPTEN_BINDINGS(stage_two_example)
{
	emscripten::function("calculateGraph", &calculateGraph);
}
