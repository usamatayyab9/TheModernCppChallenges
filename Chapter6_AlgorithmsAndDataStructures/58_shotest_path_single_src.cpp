/**
 * @file 58_shotest_path_single_src.cpp
 * @author Usam Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * Compilation command : g++ -std=c++17 58_shotest_path_single_src.cpp
 * 
 *  This file is solution to "Problem 58. The shortest path between nodes"
 *  mentioned in "Chapter 6: Algorithms and Data Structures" of the book:
 * - The Modern C++ Challenge by Marius Bancilla (available at amazon https://www.amazon.com/Modern-Challenge-programmer-real-world-problems/dp/1788993861)
 *
 * Solution:
 * This problem solved using dijkstra's algorithm. To implement this solution
 * a class `Graph` which stores an undirected graph and provides mean to
 * add an edge to graph, access a vertex i.e. get a list of all other vetices it 
 * is connected, total number of vertices.
 * 
 * After building the graph object we can use the `Dijkstra` class to
 * compute shrotest path. The `Dijkstra` class does not store the graph.
 * Once an object of type `Dijkstra` is constructed all paths and distances
 * are computed in constructor there is no method of specifying a input
 * graph and source vertex again other than constructor. However
 * weights vector and distances vector are stored as long object is not
 * destructed.
 * 
 * See class and function comments for more details.
 * 
 * Driver code:
 * - Intializes a graph object and add edges to it.
 * - The consturcts a dijkstra object and passes the
 *      graph and source vertex to its constructor.
 * - Then prints all shortest paths from source vertex to every other vertices
 *  and cost of each path.
 *  
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <list>
#include <queue>
#include <exception>
#include <algorithm>

using std::numeric_limits;
using std::for_each;
using std::cbegin;
using std::cout;
using std::cend;
using std::endl;
using std::greater;
using std::map;
using std::string;
using std::vector;
using std::list;
using std::pair;
using std::priority_queue;
using std::size;
using std::unordered_map;


/**
 * @brief Class representing an undirected graph.
 * 
 * @tparam Vertex Type of the vertices in the graph.
 * @tparam Weight Type of the weights associated with edges.
 * 
 * Class should used like this:
 * auto undirected_graph = Graph<char, int>{};
 * undirected_graph.AddEdge('A', 'B', 7);
 * undirected_graph.AddEdge('A', 'F', 14);
 * undirected_graph.AddEdge('A', 'C', 9);
 * undirected_graph.AddEdge('B', 'C', 10);
 * undirected_graph.AddEdge('B', 'D', 15);
 * undirected_graph.AddEdge('C', 'D', 11);
 * undirected_graph.AddEdge('C', 'F', 2);
 * undirected_graph.AddEdge('D', 'E', 6);
 * undirected_graph.AddEdge('E', 'F', 9);
 * undirected_graph.Print();
 */
template<class Vertex = int, class Weight = int>
class Graph
{
    public:
    using vertex_type = Vertex;
    using weight_type = Weight;
    using vertices_vector = vector<vertex_type>;
    using neighbours_list = map<vertex_type, weight_type>;

    Graph() = default;

    void AddEdge(const vertex_type &vertexA, const vertex_type &vertexB, const weight_type &wt)
    {
        adj[vertexA][vertexB] = wt;
        adj[vertexB][vertexA] = wt;
    }

    void Print() const
    {
        for (const auto &[u, neighbours] : adj)
        {
            cout << u << " : ";
            for(const auto &[v, edge_weight] : neighbours)
            {
                cout << "{" << v << ", " << edge_weight << "} ";
            }
            cout << "\n";
        }
    }

    vertices_vector Vertices() const
    {
        auto vertices = vertices_vector{};
        vertices.reserve(size(adj));
        for (const auto &[vertex, neighbours] : adj)
        {
            vertices.push_back(vertex);
        }
        return vertices;
    }

    /**
     * @brief Returns the list of neighbours for a given vertex.
     *
     * @param u Vertex for which neighbours are requested.
     * @return Neighbours list for the specified vertex.
     * @throw std::out_of_range If the vertex 'u' is not in the graph.
     */
    const neighbours_list & operator[](const vertex_type &u) const
    {
        return adj.at(u);
    }

    size_t VertexCount()
    {
        return size(adj);
    }

    private:
    //graph representation
    map<vertex_type, neighbours_list> adj;
};

/**
 * @brief Dijkstra class for finding the shortest paths in a weighted graph.
 *
 * @tparam Vertex Type of the vertices in the graph.
 * @tparam Weight Type of the weights associated with edges.
 *  Usage of class:
 * auto undirected_graph = Graph<char, int>{};
 * //Add edges to graph
 * auto dijkstra_info = Dijkstra{undirected_graph, kSource};//Template parameters are auto deduced.
 * 
 * 
 */
template<class Vertex, class Weight>
class Dijkstra
{
    public:
    /**
     * @brief Initializes Dijkstra's algorithm with a given graph and source vertex.
     *
     * @param G Graph on which Dijkstra's algorithm will be applied.
     * @param src Source vertex for computing shortest paths.
     */
    Dijkstra(const Graph<Vertex, Weight> &G, const Vertex &src)
    {
        Init(G, src);
    }

    /**
     * @brief Initializes Dijkstra's algorithm with a given graph and source vertex.
     *
     * This function initializes the Dijkstra's algorithm for finding the shortest paths
     * in a weighted graph. It sets up the distance values, assigns infinity to all vertices
     * except the source, and populates a priority queue for processing vertices in order of
     * their current tentative distances from the source. The main loop iterates until the
     * priority queue is empty, exploring neighboring vertices and updating their tentative
     * distances whenever a shorter path is found.
     *
     * @param G Graph on which Dijkstra's algorithm will be applied.
     * @param src Source vertex for computing shortest paths.
     *
     * The function uses a priority queue to efficiently select the vertex with the minimum
     * tentative distance at each step. It updates the distance values and the previous vertex
     * map during the exploration process.
     *
     * @note If any exception occurs during the process, the function catches the exception,
     * clears the distance and previous values, and continues. The reason for the exception
     * may include invalid vertex access or arithmetic overflow when updating distances.
     *
     * @param G Graph on which Dijkstra's algorithm will be applied.
     * @param src Source vertex for computing shortest paths.
    */
    void Init(const Graph<Vertex, Weight> &G, const Vertex &src)
    {
        distances_.clear();
        previous_.clear();
        try
        {
            /*! assign distance values to infinity */
            const auto kVertices = G.Vertices();
            for (const auto &u : kVertices)
            {
                distances_[u] = numeric_limits<Weight>::max();
            }
            /*! source to source distance is 0 */
            distances_[src] = Weight{ 0 };
            
            /*! heap used in dijkstra algorithm */
            auto PQ = priority_queue<pair<Weight, Vertex>, vector<pair<Weight, Vertex>>, greater<pair<Weight, Vertex>>>{};
            PQ.push(pair{ distances_[src], src });

            while (!PQ.empty())
            {
                const auto [weight, u] = PQ.top();
                PQ.pop();
                cout << "Exploring " << u << ", " << distances_[u] << endl;
                /*! traverse all neighbours of vertex u */
                for (const auto &[v, wt] : G[u]) //forall (u,v) e E
                {
                    // cout << "\tv: " << v << " distances_[v] = " << distances_[v] << " wt = " << wt << endl;
                    // cout << "\tu: " << u << " distances_[u] = " << distances_[u] << endl;
                    if (distances_[v] > distances_[u] + wt)
                    {
                         
                        distances_[v] = distances_[u] + wt;
                        previous_[v]  = u;
                        PQ.push(pair{ distances_[v], v });
                    }
                }
            }
        }
        catch(const std::exception &e)
        {
            //if anything bad happens. Clear distance and previous values
            cout << e.what();
            distances_.clear();
            previous_.clear(); 
        }
    }
    /**
     * returns the distance from source to provided destination
     * if dest is not in the map then throws exception
     **/
    Weight DistanceTo(const Vertex &dest) const
    {
        return distances_.at(dest);
    }
    /**
     * Returns the path from source to destination
     * if destination does not exist in the map return an empty list
     **/
    list<Vertex> PathTo(const Vertex &dest)
    {
        auto current_vertex = dest;
        auto path           = list<Vertex> { current_vertex };
        while (previous_.find(current_vertex) != cend(previous_))
        {
            const auto &next_vertex = previous_[current_vertex];
            path.push_front(next_vertex);
            current_vertex = next_vertex;
        }
        return path;
    }

    private:
    unordered_map<Vertex, Weight> distances_;
    unordered_map<Vertex, Vertex> previous_;
};

int main()
{
    auto undirected_graph = Graph<char, int>{};
    undirected_graph.AddEdge('A', 'B', 7);
    undirected_graph.AddEdge('A', 'F', 14);
    undirected_graph.AddEdge('A', 'C', 9);
    undirected_graph.AddEdge('B', 'C', 10);
    undirected_graph.AddEdge('B', 'D', 15);
    undirected_graph.AddEdge('C', 'D', 11);
    undirected_graph.AddEdge('C', 'F', 2);
    undirected_graph.AddEdge('D', 'E', 6);
    undirected_graph.AddEdge('E', 'F', 9);

    cout << "Graph: \n"; 
    undirected_graph.Print();

    const auto kSource = 'A';
    auto dijkstra_info = Dijkstra{undirected_graph, kSource};
    
    for (const auto &kDest : undirected_graph.Vertices())
    {
        cout << kSource << " -> " << kDest << " : " << dijkstra_info.DistanceTo(kDest) << " ";
        const auto kPath = dijkstra_info.PathTo(kDest);
        cout << kPath.front();
        for_each(++cbegin(kPath), cend(kPath), [](const auto kVertex){
            cout << " -> " << kVertex;
        });

        cout << endl;
    }

    return 0;
}