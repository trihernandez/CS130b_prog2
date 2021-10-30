#include <utility>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <cfloat>
#include <algorithm>    // std::sort
//#include <time.h>       // to get the runtime for inputs

using namespace std;

#include <chrono>       // to get the runtime for inputs
using namespace std::chrono;



class Vertex{

    public:

    int identifier;
    double x;
    double y;
    std::vector<bool> connectivity;

    Vertex()
    {
        identifier = -1;
        x = 0;
        y = 0;
    }

    Vertex(int i, double x_pos, double y_pos, int vectorSize)
    {
        identifier = i;
        x = x_pos;
        y = y_pos;
        std::vector<bool> tmp(vectorSize, false);
        
        connectivity.swap(tmp);
        
        connectivity[i] = true;
    }

    Vertex( const Vertex &c )
    {
        identifier = c.identifier;
        x = c.x;
        y = c.y;
        connectivity = c.connectivity;
    }

    void printWithId()
    {
        cout << identifier << ", " << x << ", " << y << "\n";
    }
    
    void printConnectivity()
    {
        for(vector<bool>::iterator it = connectivity.begin(); it != connectivity.end(); it++)
        {
            if( *it == true )
                cout << "O";
            else
                cout << "-";
        }
    }
};



class Edge{

    public:

    int identifier;
    Vertex a;
    Vertex b;
    double distance;

    Edge(int i, Vertex c1, Vertex c2)
    {
        identifier = i;
        a = c1;
        b = c2;

        //calculate distance
        double x_diff = abs(b.x - a.x);
        double y_diff = abs(b.y - a.y);
        distance =  sqrt( pow(x_diff,2) + pow(y_diff,2) );
    }
    
    double getDistance()
    {
        return distance;
    }
    
    void printVertices()
    {
        cout << a.identifier << " " << b.identifier << "\n";
    }

    void printWithDistance()
    {
        cout << "Edge " << identifier << ":\n";
        a.printWithId();
        b.printWithId();
        cout << "Distance: " << getDistance() << "\n";
        cout << "\n";
    }
};



bool sortBySize(Edge a, Edge b)
{
    return a.getDistance() < b.getDistance();
}


bool sortByEdgeID(Edge a, Edge b)
{
    return a.identifier < b.identifier;
}


bool sortByVertexID(Vertex a, Vertex b)
{
    return a.identifier < b.identifier;
}


//add edge to solution, to include vertices that are part of our solution
vector<bool> combinedConnectivity(Edge e, int num_vertices, const vector< Vertex > &vertices)
{
    Vertex a = vertices[e.a.identifier];
    Vertex b = vertices[e.b.identifier];
    
    vector<bool> newConnectivity(num_vertices, false);
    
    int index = 0;
    
    //for a given index, if connectivity of a,newConnectivity[i] == true
    //or b.newConnectivity[i]=true, then newConnectivity[i] = true
    for(vector< bool >::iterator it = newConnectivity.begin(); it != newConnectivity.end(); ++it)
    {
        *it = a.connectivity[index] || b.connectivity[index];
        index++;
    }
    
    return newConnectivity;

}



int main()
{    
    auto start = high_resolution_clock::now();

    //obtain the coordinates of all points as a pair
    vector< Vertex > vertices;
    vector< Edge > edges;
    vector< Edge > solution;


    //temporary veriables for storing an entry
    std::string input;
    int num_vertices;
    int num_edges;
    double x_input;
    double y_input;
    int point_a_input;
    int point_b_input;

    //input the number of vertices
    cin >> input;
    num_vertices = stoi(input);

    //input each entry
    for(int i = 0; i<num_vertices; i++)
    {
        //input x
        cin >> input;
        x_input = stod(input);
        //input y
        cin >> input;
        y_input = stod(input);
        //combine the two into a coordinate
        Vertex v_input(i, x_input, y_input, num_vertices);

        vertices.push_back( v_input );

    }

    //input the number of edges
    cin >> input;
    num_edges = stoi(input);

    //input each entry
    if(num_edges == 0)
    {
        int total_index = 0;
        //if n=0, input all possible edges
        for(int i = 0; i<num_vertices; i++)
        {
            for(int j = i+1; j<num_vertices; j++)
            {
                Vertex point_a = vertices[i];
                Vertex point_b = vertices[j];
                Edge e_input(total_index, point_a, point_b);
                edges.push_back( e_input );

                total_index++;
            }
        }
    }
    else
        //otherwise, print the edges inputted
        for(int i = 0; i<num_edges; i++)
        {
            //input x
            cin >> input;
            point_a_input = stoi(input);
            //input y
            cin >> input;
            point_b_input = stoi(input);

            Vertex point_a = vertices[point_a_input];
            Vertex point_b = vertices[point_b_input];
            //combine stoi two into a coordinate
            Edge e_input(i, point_a, point_b);

            edges.push_back( e_input );
        }

    /*
    cout << "\n";
    for (vector< Vertex >::iterator it = vertices.begin(); it != vertices.end(); ++it)
    {
        it->printWithId();
    }
    cout << "\n";
    */
    
    //sort edges by size
    std::sort(edges.begin(), edges.end(), sortBySize);
    
    /*
    for (vector< Edge >::iterator it = edges.begin(); it != edges.end(); ++it)
    {
        it->printWithDistance();
    }
    cout << "\n";

    for(vector< Vertex >::iterator it = vertices.begin(); it != vertices.end(); ++it)
    {
        it->printConnectivity();
        cout << " e\n";
    }
    cout << "\n";
    */

    //now that we have the positions of each point, and we can successfully calculate the distance
    //We have all the sufficient info to commence kruskal's/prim's
    
    //adfjkglnf yake all edges by size and input until aaaalllllll aeree conennected
    for (vector< Edge >::iterator it = edges.begin(); it != edges.end(); ++it)
    {
        Vertex a = vertices[it->a.identifier];
        Vertex b = vertices[it->b.identifier];

        //if a is not already connected to b, add this edge to the solution
        //and update the connectivity to include the edges connected by b
        if( a.connectivity[b.identifier] == false )
        {
            solution.push_back( *it );
            
            vector< bool > newConnectivity = combinedConnectivity(*it, num_vertices, vertices);
            int index = 0;
            
            for(vector< bool >::iterator it = newConnectivity.begin(); it != newConnectivity.end(); ++it)
            {
                if( *it == true )
                {   
                    vertices[index].connectivity = newConnectivity;
                    //cout << "O";
                }
                /*
                else
                {
                    cout << "-";
                }
                */
                index++;
            }
            //cout << "\n";
        }
    }
    cout << "\n";
    
    /*
    for(vector< Vertex >::iterator it = vertices.begin(); it != vertices.end(); ++it)
    {
        it->printConnectivity();
        cout << " e\n";
    }
    */
    
    //sort solutions by identifier
    std::sort(solution.begin(), solution.end(), sortByEdgeID);
    
    //check solutions
    for (vector< Edge >::iterator it = solution.begin(); it != solution.end(); ++it)
    {
        it->printVertices();
    }
    cout << "\n";

    auto end = high_resolution_clock::now();
    auto total_runtime = duration_cast<microseconds>(end - start);
    double runtime_in_seconds = (double)total_runtime.count() / 1000000.0

    cout << "Runtime: " << total_runtime.count() << " s\n";
    
    return 0;
}