#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

const int inf = numeric_limits<int>::max();

// Define a struct to represent a graph edge.
struct Edge {
    int node;
    int distance;
    int time;
};

class Graph {
public:
    Graph(int numNodes) : numNodes(numNodes) {
        graph.resize(numNodes);
    }

    void addEdge(int from, int to, int distance, int time) {
        graph[from].push_back({to, distance, time});
    }

    const vector<Edge>& getEdges(int node) const {
        return graph[node];
    }

    int size() const {
        return numNodes;
    }

private:
    int numNodes;
    vector<vector<Edge>> graph;
};

class ShortestFastestPaths {
public:
    ShortestFastestPaths(const Graph& graph, int source, int destination)
        : graph(graph), source(source), destination(destination) {
        numNodes = graph.size();
        shortestDistance.assign(numNodes, inf);
        fastestTime.assign(numNodes, inf);
        shortestDistance[source] = 0;
        fastestTime[source] = 0;
        shortestPath.assign(numNodes, vector<int>());
        fastestPath.assign(numNodes, vector<int>());
    }

    void findPaths() {
        priority_queue<pair<int, int>> pq; // {distance, node}
        pq.push({0, source});

        while (!pq.empty()) {
            int curr_node = pq.top().second;
            int curr_distance = -pq.top().first;
            pq.pop();

            if (curr_distance > shortestDistance[curr_node] || curr_distance > fastestTime[curr_node]) {
                continue;
            }

            for (const Edge& edge : graph.getEdges(curr_node)) {
                int neighbor = edge.node;
                int new_distance = curr_distance + edge.distance;
                int new_time = curr_distance + edge.time;

                if (new_distance < shortestDistance[neighbor]) {
                    shortestDistance[neighbor] = new_distance;
                    pq.push({-new_distance, neighbor});
                    shortestPath[neighbor] = shortestPath[curr_node];
                    shortestPath[neighbor].push_back(neighbor);
                }

                if (new_time < fastestTime[neighbor]) {
                    fastestTime[neighbor] = new_time;
                    pq.push({-new_time, neighbor});
                    fastestPath[neighbor] = fastestPath[curr_node];
                    fastestPath[neighbor].push_back(neighbor);
                }
            }
        }
    }

    int getShortestDistance() const {
        return shortestDistance[destination];
    }

    const vector<int>& getShortestPath() const {
        return shortestPath[destination];
    }

    int getFastestTime() const {
        return fastestTime[destination];
    }

    const vector<int>& getFastestPath() const {
        return fastestPath[destination];
    }

private:
    const Graph& graph;
    int source;
    int destination;
    int numNodes;
    vector<int> shortestDistance;
    vector<int> fastestTime;
    vector<vector<int>> shortestPath;
    vector<vector<int>> fastestPath;
};

int main() {
    int numNodes = 6;
    int source = 2;
    int destination = 5;

    Graph graph(numNodes);

    graph.addEdge(0, 1, 2, 5);
    graph.addEdge(0, 2, 4, 3);
    graph.addEdge(1, 2, 1, 2);
    graph.addEdge(1, 3, 7, 4);
    graph.addEdge(2, 3, 1, 1);
    graph.addEdge(3, 4, 3, 2);
    graph.addEdge(4, 5, 2, 3);

    ShortestFastestPaths paths(graph, source, destination);
    paths.findPaths();

    cout << "Shortest Distance from Node " << source << " to Node " << destination << ": " << paths.getShortestDistance() << endl;
    cout << "Shortest Path: ";
    for (int node : paths.getShortestPath()) {
        cout << node << " -> ";
    }
    cout << destination << endl;

    cout << "Fastest Time from Node " << source << " to Node " << destination << ": " << paths.getFastestTime() << endl;
    cout << "Fastest Path: ";
    for (int node : paths.getFastestPath()) {
        cout << node << " -> ";
    }
    cout << destination << endl;

    return 0;
}
