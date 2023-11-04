#include <vector>
#include <string>
#include <iostream>
#include <queue>
#include <climits>
#include <algorithm>
#include <fstream>
using namespace std;

string graphFile = "test_data.txt";

class Graph {
public:
    Graph() {}
    Graph(int numVertices) : numVertices(numVertices) {
        adjacencyList.resize(numVertices);
    }

    void addEdge(char source, char destination, int distance, int time) {
        adjacencyList[source - 'A'].push_back({destination, distance, time});
    }
    //To calculate shortest path
    vector<char> shortestPath(char source, char destination) {
        return pathCalculator(source, destination, true);
    }
    //To calculate fastest path
    vector<char> fastestPath(char source, char destination) {
        return pathCalculator(source, destination, false);
    }
    
    //To check the existence of nodes
    bool isValidVertex(char vertex) {
        ifstream file(graphFile); 

        if (!file.is_open()) {
            cout << "Error: Could not open the file." << endl;
            return false;
        }

        char source, destination;
        int distance, time;
        bool found = false;

        while (file >> source >> destination >> distance >> time) {
            if (source == vertex || destination == vertex) {
                found = true;
                break;
            }
        }
        file.close();

        if (found) {
            return true;
        } else {
            return false;
        }
    }


    //To load graph data from a file
    void loadGraphFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open the file." << endl;
            return;
        }
        char source, destination;
        int distance, time;

        while (file >> source >> destination >> distance >> time) {
            addEdge(source, destination, distance, time);
        }
        file.close();
    }

    // Function to remove an edge between two vertices
    void removeEdge(char source, char destination) {
        for (int i = 0; i < adjacencyList[source - 'A'].size(); i++) {
            if (adjacencyList[source - 'A'][i].vertex == destination) {
                adjacencyList[source - 'A'].erase(adjacencyList[source - 'A'].begin() + i);
                break;
            }
        }
    }

    // Function to add a new vertex between two existing vertices 
    void addVertexBetween(char vertex, char source, char destination, int distance1, int time1, int distance2, int time2) {

        // Check if the new vertex already exists
        if (isValidVertex(vertex)) {
            cout << "The vertex " << vertex << " already exists. Choose a different vertex name." << endl;
            cout << endl;
            return;
        }
        // Remove the old edge and save the modified graph to the file
        removeEdge(source, destination);
        saveGraphToFile(graphFile);

        // Add the new vertex 
        addEdge(source, vertex, distance1, time1);
        addEdge(vertex, destination, distance2, time2);

        // Save the modified graph to the file
        saveGraphToFile(graphFile);
        cout << "Vertex " << vertex << " added between " << source << " and " << destination << endl;
    }
    // Function to save the modified graph to the file
    void saveGraphToFile(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open the file." << endl;
            return;
        }
        for (int i = 0; i < numVertices; i++) {
            char source = 'A' + i;
            for (const Edge& neighbor : adjacencyList[i]) {
                char destination = neighbor.vertex;
                int distance = neighbor.distance;
                int time = neighbor.time;
                file << source << " " << destination << " " << distance << " " << time << endl;
            }
        }
        file.close();
    }

    private:

    struct Edge {
        char vertex;
        int distance;
        int time;
    };
    struct CompareEdges {
        bool operator()(const Edge& e1, const Edge& e2) {
            return e1.distance > e2.distance;
        }
    };

    int numVertices;
    vector<vector<Edge>> adjacencyList;

    //Dijkstra Algo to Calculate Path
    vector<char> pathCalculator(char source, char destination, bool shortest) {
        vector<int> distance(numVertices, INT_MAX);
        vector<int> time(numVertices, INT_MAX);
        vector<char> previous(numVertices, 0);
        priority_queue<Edge, vector<Edge>, CompareEdges> pq;

        distance[source - 'A'] = 0;
        time[source - 'A'] = 0;
        pq.push({source, 0, 0});

        while (!pq.empty()) {
            Edge current = pq.top();
            pq.pop();

            char u = current.vertex;
            for (const Edge& neighbor : adjacencyList[u - 'A']) {
                char v = neighbor.vertex;
                int edgeDistance = neighbor.distance;
                int edgeTime = neighbor.time;

                if (shortest) {
                    if (distance[u - 'A'] + edgeDistance < distance[v - 'A']) {
                        distance[v - 'A'] = distance[u - 'A'] + edgeDistance;
                        time[v - 'A'] = time[u - 'A'] + edgeTime;
                        previous[v - 'A'] = u;
                        pq.push({v, distance[v - 'A'], time[v - 'A']});
                    }
                } else {
                    if (time[u - 'A'] + edgeTime < time[v - 'A']) {
                        distance[v - 'A'] = distance[u - 'A'] + edgeDistance;
                        time[v - 'A'] = time[u - 'A'] + edgeTime;
                        previous[v - 'A'] = u;
                        pq.push({v, distance[v - 'A'], time[v - 'A']});
                    }
                }
            }
        }

        if (distance[destination - 'A'] == INT_MAX) {
            return {};  // An empty path indicates no valid path
        }

        vector<char> path;
        char current = destination;
        while (current != source) {
            path.push_back(current);
            current = previous[current - 'A'];
        }
        path.push_back(source);
        reverse(path.begin(), path.end());
        return path;
    }
};


int main() {
    vector<char> path;
    Graph graph(100);

    // Load graph data from a file
    graph.loadGraphFromFile(graphFile);

     while (true) {
        cout << "------------------------------" << endl;

        cout << "Menu:\n";
        cout << "1. Calculate Path\n";
        cout << "2. Add Vertex\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        // Check if input is a valid number
        while (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
            cout << "Invalid input. Please enter a number.\n";
            cout << endl;
            cout << "Enter your choice: ";
            cin >> choice;
        }
        cout << "------------------------------" << endl;

        if (choice == 1) {
            char source, destination;
            string choice;

            cout << "Enter source vertex: ";
            cin >> source;

            if (!graph.isValidVertex(source)) {
                cout << "Vertex does not exist in the graph." << endl;
                cout << endl;
                continue;
            }
            cout << endl;
            cout << "Enter destination vertex: ";
            cin >> destination;

            if (!graph.isValidVertex(destination)) {
                cout << "Vertex does not exist in the graph." << endl;
                cout << endl;
                continue;
            }
            cout << endl;
            cout << "Enter choice (shortest/fastest): ";
            cin >> choice;
            cout << endl;

            if (choice == "shortest" || choice == "SHORTEST") {
                path = graph.shortestPath(source, destination);
                if (path.empty()) {
                    cout << "No valid shortest path between " << source << " and " << destination << endl;
                } else {
                    cout << "Shortest path: ";
                    for (char vertex : path) {
                        cout << vertex << "->";
                    }
                    cout << "end" << endl;
                    cout << endl;
                    cout << "------------------------------" << endl;
                }
            } else if (choice == "fastest" || choice == "FASTEST") {
                path = graph.fastestPath(source, destination);
                if (path.empty()) {
                    cout << "No valid fastest path between " << source << " and " << destination << endl;
                    cout << endl;
                } else {
                    cout << "Fastest path: ";
                    for (char vertex : path) {
                        cout << vertex << "->";
                    }
                    cout << "end" << endl;
                    cout << endl;
                    cout << "------------------------------" << endl;
                }
            } else {
                cout << "Invalid choice. Please enter 'shortest' or 'fastest'." << endl;
                cout << endl;
            }
        } else if (choice == 2) {
            char source, destination, vertex;
            int distance1, distance2;
            int time1, time2;

            cout << "Enter source vertex: ";
            cin >> source;
            if (!graph.isValidVertex(source)) {
                cout << "Vertex does not exist in the graph." << endl;
                cout << endl;
                continue;
            }
            cout <<endl;
            
            cout << "Enter destination vertex: ";
            cin >> destination;
            if (!graph.isValidVertex(destination)) {
                cout << "Vertex does not exist in the graph." << endl;
                cout << endl;
                continue;
            }
            cout <<endl;

            cout << "Enter the new vertex name: ";
            cin >> vertex;
            cout << "Enter the distance from " << source << " to " << vertex << ": ";
            cin >> distance1;
            cout << "Enter the time from " << source << " to " << vertex << ": ";
            cin >> time1;
            cout << "Enter the distance from " << vertex << " to " << destination << ": ";
            cin >> distance2;
            cout << "Enter the time from " << vertex << " to " << destination << ": ";
            cin >> time2;

            graph.addVertexBetween(vertex, source, destination, distance1, time1, distance2, time2);
        } else if (choice == 3) {
            break;
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
