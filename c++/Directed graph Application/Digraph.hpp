// Digraph.hpp
//
// ICS 46 Winter 2022
// Project #5: Rock and Roll Stops the Traffic
//
// This header file declares a class template called Digraph, which is
// intended to implement a generic directed graph.  The implementation
// uses the adjacency lists technique, so each vertex stores a linked
// list of its outgoing edges.
//
// Along with the Digraph class template is a class DigraphException
// and a couple of utility structs that aren't generally useful outside
// of this header file.
//
// In general, directed graphs are all the same, except in the sense
// that they store different kinds of information about each vertex and
// about each edge; these two types are the type parameters to the
// Digraph class template.

#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <exception>
#include <functional>
#include <list>
#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include <typeinfo>
#include <queue>



// DigraphExceptions are thrown from some of the member functions in the
// Digraph class template, so that exception is declared here, so it
// will be available to any code that includes this header file.

class DigraphException : public std::runtime_error
{
public:
    DigraphException(const std::string& reason);
};


inline DigraphException::DigraphException(const std::string& reason)
    : std::runtime_error{reason}
{
}



// A DigraphEdge lists a "from vertex" (the number of the vertex from which
// the edge points), a "to vertex" (the number of the vertex to which the
// edge points), and an EdgeInfo object.  Because different kinds of Digraphs
// store different kinds of edge information, DigraphEdge is a struct template.

template <typename EdgeInfo>
struct DigraphEdge
{
    int fromVertex;
    int toVertex;
    EdgeInfo einfo;
};



// A DigraphVertex includes two things: a VertexInfo object and a list of
// its outgoing edges.  Because different kinds of Digraphs store different
// kinds of vertex and edge information, DigraphVertex is a struct template.

template <typename VertexInfo, typename EdgeInfo>
struct DigraphVertex
{
    VertexInfo vinfo;
    std::list<DigraphEdge<EdgeInfo>> edges;
};



// Digraph is a class template that represents a directed graph implemented
// using adjacency lists.  It takes two type parameters:
//
// * VertexInfo, which specifies the kind of object stored for each vertex
// * EdgeInfo, which specifies the kind of object stored for each edge
//
// You'll need to implement the member functions declared here; each has a
// comment detailing how it is intended to work.
//
// Each vertex in a Digraph is identified uniquely by a "vertex number".
// Vertex numbers are not necessarily sequential and they are not necessarily
// zero- or one-based.

template <typename VertexInfo, typename EdgeInfo>
class Digraph
{
public:
    // The default constructor initializes a new, empty Digraph so that
    // contains no vertices and no edges.
    Digraph();

    // The copy constructor initializes a new Digraph to be a deep copy
    // of another one (i.e., any change to the copy will not affect the
    // original).
    Digraph(const Digraph& d);

    // The move constructor initializes a new Digraph from an expiring one.
    Digraph(Digraph&& d) noexcept;

    // The destructor deallocates any memory associated with the Digraph.
    ~Digraph() noexcept;

    // The assignment operator assigns the contents of the given Digraph
    // into "this" Digraph, with "this" Digraph becoming a separate, deep
    // copy of the contents of the given one (i.e., any change made to
    // "this" Digraph afterward will not affect the other).
    Digraph& operator=(const Digraph& d);

    // The move assignment operator assigns the contents of an expiring
    // Digraph into "this" Digraph.
    Digraph& operator=(Digraph&& d) noexcept;

    // vertices() returns a std::vector containing the vertex numbers of
    // every vertex in this Digraph.
    std::vector<int> vertices() const;

    // edges() returns a std::vector of std::pairs, in which each pair
    // contains the "from" and "to" vertex numbers of an edge in this
    // Digraph.  All edges are included in the std::vector.
    std::vector<std::pair<int, int>> edges() const;

    // This overload of edges() returns a std::vector of std::pairs, in
    // which each pair contains the "from" and "to" vertex numbers of an
    // edge in this Digraph.  Only edges outgoing from the given vertex
    // number are included in the std::vector.  If the given vertex does
    // not exist, a DigraphException is thrown instead.
    std::vector<std::pair<int, int>> edges(int vertex) const;

    // vertexInfo() returns the VertexInfo object belonging to the vertex
    // with the given vertex number.  If that vertex does not exist, a
    // DigraphException is thrown instead.
    VertexInfo vertexInfo(int vertex) const;

    // edgeInfo() returns the EdgeInfo object belonging to the edge
    // with the given "from" and "to" vertex numbers.  If either of those
    // vertices does not exist *or* if the edge does not exist, a
    // DigraphException is thrown instead.
    EdgeInfo edgeInfo(int fromVertex, int toVertex) const;

    // addVertex() adds a vertex to the Digraph with the given vertex
    // number and VertexInfo object.  If there is already a vertex in
    // the graph with the given vertex number, a DigraphException is
    // thrown instead.
    void addVertex(int vertex, const VertexInfo& vinfo);

    // addEdge() adds an edge to the Digraph pointing from the given
    // "from" vertex number to the given "to" vertex number, and
    // associates with the given EdgeInfo object with it.  If one
    // of the vertices does not exist *or* if the same edge is already
    // present in the graph, a DigraphException is thrown instead.
    void addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo);

    // removeVertex() removes the vertex (and all of its incoming
    // and outgoing edges) with the given vertex number from the
    // Digraph.  If the vertex does not exist already, a DigraphException
    // is thrown instead.
    void removeVertex(int vertex);

    // removeEdge() removes the edge pointing from the given "from"
    // vertex number to the given "to" vertex number from the Digraph.
    // If either of these vertices does not exist *or* if the edge
    // is not already present in the graph, a DigraphException is
    // thrown instead.
    void removeEdge(int fromVertex, int toVertex);

    // vertexCount() returns the number of vertices in the graph.
    int vertexCount() const noexcept;

    // edgeCount() returns the total number of edges in the graph,
    // counting edges outgoing from all vertices.
    int edgeCount() const noexcept;

    // This overload of edgeCount() returns the number of edges in
    // the graph that are outgoing from the given vertex number.
    // If the given vertex does not exist, a DigraphException is
    // thrown instead.
    int edgeCount(int vertex) const;

    // isStronglyConnected() returns true if the Digraph is strongly
    // connected (i.e., every vertex is reachable from every other),
    // false otherwise.
    bool isStronglyConnected() const;

    // findShortestPaths() takes a start vertex number and a function
    // that takes an EdgeInfo object and determines an edge weight.
    // It uses Dijkstra's Shortest Path Algorithm to determine the
    // shortest paths from the start vertex to every other vertex
    // in the graph.  The result is returned as a std::map<int, int>
    // where the keys are vertex numbers and the value associated
    // with each key k is the predecessor of that vertex chosen by
    // the algorithm.  For any vertex without a predecessor (e.g.,
    // a vertex that was never reached, or the start vertex itself),
    // the value is simply a copy of the key.
    std::map<int, int> findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const;


private:
    // Add whatever member variables you think you need here.  One
    // possibility is a std::map where the keys are vertex numbers
    // and the values are DigraphVertex<VertexInfo, EdgeInfo> objects.
    std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> information;
    // You can also feel free to add any additional member functions
    // you'd like (public or private), so long as you don't remove or
    // change the signatures of the ones that already exist.
};



// You'll need to implement the member functions below.  There's enough
// code in place to make them compile, but they'll all need to do the
// correct thing instead.

template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph()
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(const Digraph& d)
{
    this->information = d.information;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(Digraph&& d) noexcept
{
    std::swap(this->information, d.information);
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::~Digraph() noexcept
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(const Digraph& d)
{
    this->information = d.information;
    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(Digraph&& d) noexcept
{
    std::swap(this->information, d.information);
    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<int> Digraph<VertexInfo, EdgeInfo>::vertices() const
{
    std::vector<int> vertexList;
    for(auto x: information)
    {
        vertexList.push_back(x.first);
    }
    return vertexList;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges() const
{
    std::vector<std::pair<int, int>> vertexList;
    for(auto x: information)
    {
        for(auto y: x.second.edges)
        {
            std::pair p(y.fromVertex, y.toVertex);
            vertexList.push_back(p);
        }
        
    }
    return vertexList;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges(int vertex) const
{
    std::vector<std::pair<int, int>> vertexList;
    if(information.find(vertex) != information.end())
    {
        for(auto x: information.find(vertex)->second.edges)
        {
            std::pair p(x.fromVertex, x.toVertex);
            vertexList.push_back(p);
            
        }
    }
    else
    {
        throw DigraphException{"Vertex does not exist"};
    }
    
    return vertexList;
}


template <typename VertexInfo, typename EdgeInfo>
VertexInfo Digraph<VertexInfo, EdgeInfo>::vertexInfo(int vertex) const
{
    if(information.find(vertex) != information.end())
    {
        try
        {
            return information.at(vertex).vinfo;
        }
        catch(...)
        {
            throw DigraphException{"Vertex does not exist"};
        }
        
        
    }
    else
    {
        throw DigraphException{"Vertex does not exist"};
    }
}


template <typename VertexInfo, typename EdgeInfo>
EdgeInfo Digraph<VertexInfo, EdgeInfo>::edgeInfo(int fromVertex, int toVertex) const
{

    for(auto x: information)
    {
        for(auto y: x.second.edges)
        {
            if (fromVertex == y.fromVertex && toVertex == y.toVertex)
            {
                return y.einfo;
            }
        }
    }

    throw DigraphException{"Vertex does not exist"};

}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addVertex(int vertex, const VertexInfo& vinfo)
{
    if(information.find(vertex) == information.end())
    {
        information.insert(std::pair<int, DigraphVertex<VertexInfo,EdgeInfo>> (vertex, DigraphVertex<VertexInfo,EdgeInfo>{vinfo}));

    }
    else
    {
        throw DigraphException{"Exists already"};
    }
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo)
{
    std::pair<int,int> p(fromVertex,toVertex);
    if(information.count(fromVertex) != 0 && information.count(toVertex) != 0)
    {
        for(auto x: edges(fromVertex))
        {
            if(x == p)
            {
                throw DigraphException{"Exists already"};
            }
            
        }
        information[fromVertex].edges.push_back(DigraphEdge<EdgeInfo>{fromVertex,toVertex, einfo});
    }
    else
    {
        throw DigraphException{"Exists already"};
    }

}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeVertex(int vertex)
{
    if(information.find(vertex) != information.end())
    {
        information.erase(vertex);
    }
    else
    {
        throw DigraphException{"Vertex does not exist"};
    }
    
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeEdge(int fromVertex, int toVertex)
{
    std::pair<int,int> p(fromVertex,toVertex);
    if(information.count(fromVertex) != 0 && information.count(toVertex) != 0)
    {
        for(typename std::list<DigraphEdge<EdgeInfo>>::iterator i =information[fromVertex].edges.begin(); i!=information[fromVertex].edges.end(); i++)
        {
            if(i->toVertex == toVertex)
            {
                information[fromVertex].edges.erase(i);
                break;
            }
        }
        
    }
    else
    {
        throw DigraphException{"Edge does not exist"};
    }
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::vertexCount() const noexcept
{
    return information.size();
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount() const noexcept
{
    int count = 0;
    for(auto x: information)
    {
        count += x.second.edges.size();
    }
    return count;
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount(int vertex) const
{
    return edges(vertex).size();
}


template <typename VertexInfo, typename EdgeInfo>
bool Digraph<VertexInfo, EdgeInfo>::isStronglyConnected() const
{
    std::vector<int> reached_edges;
    for(auto x: information)
    {
        for(auto y: x.second.edges)
        {
            reached_edges.push_back(y.toVertex);
        }
    }
    for(auto x: vertices())
    {
        if(std::count(reached_edges.begin(), reached_edges.end(), x) == 0)
        {
            return false;
        }
    }
    return true;


}


template <typename VertexInfo, typename EdgeInfo>
std::map<int, int> Digraph<VertexInfo, EdgeInfo>::findShortestPaths(
    int startVertex,
    std::function<double(const EdgeInfo&)> edgeWeightFunc) const
{
    std::map<int, double> Dshortest;
    std::map<int, int> Pshortest;
    std::map<int, bool> isShortest;
    for(auto x: information)
    {
        if(x.first != startVertex)
        {
            
            Dshortest[x.first] = 1000000000;
            isShortest[x.first] = false;
        }
        else
        {
            Pshortest[x.first] = x.first;
            Dshortest[x.first] = 0;
            isShortest[x.first] = false;
        }
        
    }
    std::priority_queue<int> pq;
    pq.push(startVertex);
    while(! pq.empty())
    {
        int v = pq.top();
        pq.pop();
        if(isShortest[v] == false)
        {
            isShortest[v] = true;
            for(auto x: information.at(v).edges)
            {
                if(Dshortest[x.toVertex]> Dshortest[x.fromVertex] + edgeWeightFunc(x.einfo))
                {
                    Dshortest[x.toVertex] = Dshortest[x.fromVertex] + edgeWeightFunc(x.einfo);
                    Pshortest[x.toVertex] = v;
                    pq.push(x.toVertex);
                }
            }
        }
    }
    return Pshortest;
    //  for each vertex v
    // {
    //     set kv to false
    //     set pv to unknown (or none, if v is the start vertex)
    //     set dv to ∞ (or 0, if v is the start vertex)
    // }

    // let pq be an empty priority queue
    // enqueue the start vertex into pq with priority 0
    
    // while (pq is not empty)
    // {
    //     vertex v = the vertex in pq with the smallest priority
    //     dequeue the smallest-priority vertex from pq
        
    //     if (kv is false)
    //     {
    //         kv = true

    //         for each vertex w such that edge v → w exists
    //         {
    //             if (dw > dv + C(v, w))
    //             {
    //                 dw = dv + C(v, w)
    //                 pw = v
    //                 enqueue w into pq with priority dw
    //             }
    //         }
    //     }
    // }

}



#endif

