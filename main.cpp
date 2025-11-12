// Map navigation
#include <bits/stdc++.h>
using namespace std;

/* -----------------------------------------------------------
   IIT Jodhpur Campus Navigation System (Graph + Linked Lists)
   -----------------------------------------------------------
   Features:
   • Hostels, Messes, Departments, and Landmarks as Nodes
   • Adjacency List built using Linked Lists
   • BFS → Shortest path by number of hops
   • Dijkstra → Shortest route by total distance
----------------------------------------------------------- */

struct AdjNode {
    string neighbor;
    double dist;
    AdjNode* next;
    AdjNode(const string& n, double d) : neighbor(n), dist(d), next(nullptr) {}
};

class CampusGraph {
    unordered_map<string, AdjNode*> adjHeads;
    unordered_set<string> vertices;

public:
    void addVertex(const string& v) {
        if (!vertices.count(v)) {
            vertices.insert(v);
            adjHeads[v] = nullptr;
        }
    }

    void addEdge(const string& a, const string& b, double dist, bool bidir = true) {
        addVertex(a);
        addVertex(b);
        AdjNode* nodeB = new AdjNode(b, dist);
        nodeB->next = adjHeads[a];
        adjHeads[a] = nodeB;
        if (bidir) {
            AdjNode* nodeA = new AdjNode(a, dist);
            nodeA->next = adjHeads[b];
            adjHeads[b] = nodeA;
        }
    }

    void displayMap() {
        cout << "\n📍 Campus Adjacency Lists (Each adjacency list shows connected locations):\n";
        for (auto &v : vertices) {
            cout << "  " << v << " -> ";
            AdjNode* cur = adjHeads[v];
            bool first = true;
            while (cur) {
                if (!first) cout << ", ";
                cout << cur->neighbor << "(" << cur->dist << " km)";
                first = false;
                cur = cur->next;
            }
            cout << "\n";
        }
        cout << "\n✅ Map displayed successfully.\n";
    }

    bool bfs(const string& src, const string& dest, vector<string>& path) {
        if (adjHeads.find(src) == adjHeads.end() || adjHeads.find(dest) == adjHeads.end())
            return false;

        unordered_map<string, bool> visited;
        unordered_map<string, string> parent;

        for (auto &p : adjHeads) {
            visited[p.first] = false;
            parent[p.first] = "NONE";
        }

        queue<string> q;
        q.push(src);
        visited[src] = true;
        parent[src] = "SOURCE";

        while (!q.empty()) {
            string u = q.front();
            q.pop();
            if (u == dest) break;

            for (AdjNode* cur = adjHeads[u]; cur; cur = cur->next) {
                string v = cur->neighbor;
                if (!visited[v]) {
                    visited[v] = true;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }

        if (!visited[dest]) return false;

        // Reconstruct path safely
        path.clear();
        string at = dest;
        while (at != "SOURCE" && at != "NONE") {
            path.push_back(at);
            at = parent[at];
        }
        reverse(path.begin(), path.end());
        return true;
    }

    bool dijkstra(const string& src, const string& dest, double& totalDist, vector<string>& path) {
        if (adjHeads.find(src) == adjHeads.end() || adjHeads.find(dest) == adjHeads.end())
            return false;

        unordered_map<string, double> dist;
        unordered_map<string, bool> visited;
        unordered_map<string, string> parent;

        for (auto &p : adjHeads) {
            dist[p.first] = 1e9;
            visited[p.first] = false;
            parent[p.first] = "NONE";
        }

        dist[src] = 0.0;
        parent[src] = "SOURCE";

        using P = pair<double, string>;
        priority_queue<P, vector<P>, greater<P>> pq;
        pq.push({0.0, src});

        while (!pq.empty()) {
            auto curPair = pq.top();
            pq.pop();
            double d = curPair.first;
            string u = curPair.second;

            if (visited[u]) continue;
            visited[u] = true;
            if (u == dest) break;

            for (AdjNode* cur = adjHeads[u]; cur; cur = cur->next) {
                string v = cur->neighbor;
                double w = cur->dist;
                if (!visited[v] && dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        if (dist[dest] == 1e9) return false;
        totalDist = dist[dest];

        // Safe path reconstruction
        path.clear();
        string at = dest;
        while (at != "SOURCE" && at != "NONE") {
            path.push_back(at);
            at = parent[at];
        }
        reverse(path.begin(), path.end());
        return true;
    }

    bool nodeExists(const string& name) { return vertices.count(name); }

    void cleanup() {
        for (auto &p : adjHeads) {
            AdjNode* cur = p.second;
            while (cur) {
                AdjNode* nxt = cur->next;
                delete cur;
                cur = nxt;
            }
            p.second = nullptr;
        }
    }
};

// ---------------- BUILD CAMPUS MAP -------------------
CampusGraph buildCampusMap() {
    CampusGraph g;

    // Hostel rows & connections
    g.addEdge("I3", "I2", 0.06);
    g.addEdge("I2", "Old_Mess", 0.05);
    g.addEdge("Old_Mess", "B4", 0.06);
    g.addEdge("B4", "B5", 0.05);
    g.addEdge("B5", "B6", 0.05);
    g.addEdge("B5", "B3", 0.07);
    g.addEdge("B3", "B2", 0.05);
    g.addEdge("B2", "B1", 0.05);
    g.addEdge("B2", "New_Mess", 0.06);
    g.addEdge("New_Mess", "G4", 0.05);
    g.addEdge("G4", "G5", 0.05);
    g.addEdge("G5", "G6", 0.05);
    g.addEdge("G5", "G3", 0.06);
    g.addEdge("G3", "G2", 0.05);
    g.addEdge("G2", "G1", 0.05);
    g.addEdge("G1", "Y4", 0.08);
    g.addEdge("Y4", "Y3", 0.07);
    g.addEdge("Y3", "O4", 0.08);
    g.addEdge("O4", "O3", 0.07);
    g.addEdge("O3", "Akaash_Complex", 0.12);
    g.addEdge("Akaash_Complex", "Football_Field", 0.10);
    g.addEdge("Football_Field", "PHC", 0.08);

    // Landmarks and academic area
    g.addEdge("G6", "Tapri", 0.05);
    g.addEdge("B6", "Tapri", 0.10);
    g.addEdge("Tapri", "LHC1", 0.08);
    g.addEdge("LHC1", "LHC2", 0.06);
    g.addEdge("LHC1", "Chemistry", 0.07);
    g.addEdge("Chemistry", "Physics", 0.06);
    g.addEdge("Chemistry", "Bio", 0.06);
    g.addEdge("Bio", "CSE", 0.08);
    g.addEdge("CSE", "Maths", 0.07);
    g.addEdge("Maths", "SME", 0.08);
    g.addEdge("SME", "SOLA", 0.07);
    g.addEdge("SOLA", "Shamiyana", 0.12);
    g.addEdge("Shamiyana", "Civil", 0.10);
    g.addEdge("Civil", "Mechanical", 0.08);
    g.addEdge("Mechanical", "Electrical", 0.08);
    g.addEdge("Electrical", "Materials", 0.09);
    g.addEdge("Materials", "Physics", 0.10);

    // Some cross connections
    g.addEdge("New_Mess", "LHC1", 0.18);
    g.addEdge("B3", "Tapri", 0.12);
    g.addEdge("G1", "LHC2", 0.20);
    g.addEdge("O4", "Physics", 0.30);
    g.addEdge("Old_Mess", "Shamiyana", 0.45);
    g.addEdge("B1", "CSE", 0.55);

    return g;
}

// ---------------- PATH PRINTER -------------------
void printPath(const vector<string>& path) {
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i + 1 < path.size()) cout << " -> ";
    }
    cout << "\n";
}

// ---------------- MAIN -------------------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "============================================================\n";
    cout << "   IIT JODHPUR CAMPUS NAVIGATOR (Graph + Linked Lists)\n";
    cout << "============================================================\n\n";
    cout << "This program helps you explore routes inside IIT Jodhpur campus.\n";
    cout << "It supports:\n";
    cout << "  • Viewing the map (adjacency lists)\n";
    cout << "  • Finding shortest path by number of turns (BFS)\n";
    cout << "  • Finding shortest route by actual distance (Dijkstra)\n\n";
    cout << "---------------------------------------------\n";
    cout << "AVAILABLE LOCATIONS:\n";
    cout << "  Hostels: I3, I2, B1–B6, G1–G6, Y3, Y4, O3, O4\n";
    cout << "  Messes: Old_Mess, New_Mess\n";
    cout << "  Other: Tapri, Shamiyana, Akaash_Complex, Football_Field, PHC\n";
    cout << "  LHCs: LHC1, LHC2\n";
    cout << "  Departments: CSE, Electrical, Mech (Mechanical), Civil, Physics,\n";
    cout << "               Chemistry, Bio, Materials, Maths, SME, SOLA\n";
    cout << "---------------------------------------------\n";
    cout << "Press ENTER to continue to the main menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.clear();

    CampusGraph g = buildCampusMap();

    while (true) {
        cout << "\n\n================ MAIN MENU ================\n";
        cout << "1. Display Campus Map (Adjacency Lists)\n";
        cout << "2. Find Shortest Path (by Hops - BFS)\n";
        cout << "3. Find Shortest Route (by Distance - Dijkstra)\n";
        cout << "4. List All Valid Node Names Again\n";
        cout << "5. Exit\n";
        cout << "===========================================\n";
        cout << "Enter your choice (1–5): ";

        int opt;
        if (!(cin >> opt)) {
            cin.clear();
            string s;
            getline(cin, s);
            cout << "Invalid input. Please enter a number between 1 and 5.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (opt == 1) {
            g.displayMap();
        }
        else if (opt == 2 || opt == 3) {
            string src, dst;
            cout << "\nEnter source location (e.g., B3, New_Mess, LHC1, CSE): ";
            getline(cin, src);
            cout << "Enter destination location: ";
            getline(cin, dst);

            if (!g.nodeExists(src) || !g.nodeExists(dst)) {
                cout << "\n⚠️  Invalid name(s). Use option 4 to view valid nodes.\n";
                continue;
            }

            vector<string> path;
            if (opt == 2) {
                bool ok = g.bfs(src, dst, path);
                if (!ok) cout << "No route found (BFS).\n";
                else {
                    cout << "\n📍 Shortest path (fewest turns):\n";
                    printPath(path);
                    cout << "Total hops: " << (int)path.size() - 1 << "\n";
                }
            } else {
                double totalDist = 0.0;
                bool ok = g.dijkstra(src, dst, totalDist, path);
                if (!ok) cout << "No route found (Dijkstra).\n";
                else {
                    cout << "\n📍 Shortest route by distance:\n";
                    printPath(path);
                    cout << "Total Distance: " << fixed << setprecision(3) << totalDist << " km\n";
                }
            }
        }
        else if (opt == 4) {
            cout << "\nVALID LOCATIONS:\n";
            cout << "Hostels: I3, I2, B1–B6, G1–G6, Y3, Y4, O3, O4\n";
            cout << "Messes: Old_Mess, New_Mess\n";
            cout << "Landmarks: Tapri, Shamiyana, Akaash_Complex, Football_Field, PHC\n";
            cout << "LHCs: LHC1, LHC2\n";
            cout << "Departments: CSE, Electrical, Mech, Civil, Physics, Chemistry, Bio,\n";
            cout << "             Materials, Maths, SME, SOLA\n";
        }
        else if (opt == 5) {
            cout << "\nThank you for using IIT Jodhpur Campus Navigator!\n";
            g.cleanup();
            break;
        }
        else {
            cout << "Invalid option. Please choose between 1–5.\n";
        }
    }

    return 0;
}
