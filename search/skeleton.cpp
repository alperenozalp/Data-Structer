#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <sstream>
#include <map>
using namespace std;

struct Node
{
    string MID;
    vector<Node *> adj = {};
    vector<string> relation = {};
};

map<string, Node *> graph_map = {};

map<string, string> name_map = {};
map<int, Node *, greater<int>> adj_size_map = {}; // adjacency size for finding degree centrality
map<Node *, Node *> parentnode = {};
map<Node *, int> relationindex = {}; // relation index of node's parent
map<Node *, bool> visited = {};      // bool flag for bfs algorithm

// BFS algorithm for find shortest path
vector<Node *> PathBFS(Node *current, Node *target)
{
    vector<Node *> path;
    queue<Node *> q;
    q.push(current);
    parentnode[current] = NULL;
    visited[current] = true;
    while (!q.empty())
    {
        Node *temp = q.front();
        q.pop();
        visited[temp] = true;
        if (temp == target)
        {
            while (temp != nullptr)
            {
                path.push_back(temp);
                temp = parentnode[temp];
            }
            return path;
        }
        for (unsigned int i = 0; i < temp->adj.size(); i++)
        {
            if (!visited[temp->adj[i]])
            {
                visited[temp->adj[i]] = true;
                
                if (parentnode.find(temp->adj[i]) == parentnode.end())
                { // if already have a parent don't change the parent because if this MID will be use for path that means there is shorter way to reach this MID
                    parentnode[temp->adj[i]] = temp;
                    relationindex[temp->adj[i]] = i;
                }
                q.push(temp->adj[i]);
            }
        }
    }
    return path; // Path couldn't find
}
void FindShortestPath(string start, string target)
{
    Node *node_start = graph_map[start];
    Node *node_target = graph_map[target];
    if (start == target)
    {
        cout << node_start->MID << "\n";
        return;
    }
    vector<Node *> path = PathBFS(node_start, node_target);
    if (path.empty())
    {
        cout << "There is no path between these two MID" << "\n";
        return;
    }
    int a = 1;
    cout << "Distance: " << path.size() - 1 << "\n";
    for (int i = path.size() - 1; i > 0; i--)
    {
        Node *parent = path[i];
        Node *child = path[i - 1];
        cout << "Edge " << a << ": " << parent->MID << " " << parent->relation[relationindex[child]] << " " << child->MID << " (" << name_map[parent->MID] << " TO " << name_map[child->MID] << ")" << "\n";
        a++;
    }
}
int main(int argc, char **argv)
{

    ifstream infile("freebase.tsv");

    string line;
    while (getline(infile, line))
    {
        istringstream ss(line);
        string ent1, relationship, ent2;
        getline(ss, ent1, '\t');
        getline(ss, relationship, '\t');
        getline(ss, ent2, '\r');

        Node *ent1_node, *ent2_node;

        if (graph_map.find(ent1) == graph_map.end())
        {
            ent1_node = new Node;
            ent1_node->MID = ent1;
            graph_map[ent1] = ent1_node;
        }
        else
        {
            ent1_node = graph_map[ent1];
        }

        if (graph_map.find(ent2) == graph_map.end())
        {
            ent2_node = new Node;
            ent2_node->MID = ent2;
            graph_map[ent2] = ent2_node;
        }
        else
        {
            ent2_node = graph_map[ent2];
        }

        ent1_node->adj.push_back(ent2_node);
        ent1_node->relation.push_back(relationship);

        ent2_node->adj.push_back(ent1_node);
        ent2_node->relation.push_back(relationship);
    }
    infile.close();

    ifstream infile2("mid2name.tsv");

    while (getline(infile2, line))
    {
        string MID = line.substr(0, line.find("\t"));
        string remain = line.substr(line.find("\t") + 1, line.length() - MID.length());
        string name = remain.substr(0, remain.find("\r"));

        if (name_map.find(MID) == name_map.end() && graph_map[MID] != NULL)
        {
            name_map[MID] = name; // pairing MIDs and their names
            Node *n = graph_map[MID];
            adj_size_map[n->adj.size()] = n; // the number of MID's adjacency is kept from largest to smallest for degree centrality
        }
    }

    string choice = argv[1];
    string givenmid1;
    string givenmid2;
    // printing adjs of given MID

    if (choice == "part1")
    {
        givenmid1 = argv[2];
        if (graph_map[givenmid1] == nullptr)
        {
            cout << "There is no MID as this name\n";
            return 0;
        }
        cout << givenmid1 << " " << name_map[givenmid1] << "\n";
        cout << graph_map[givenmid1]->adj.size() << " Neighbour\n";
        for (unsigned int i = 0; i < graph_map[givenmid1]->adj.size(); i++)
        {
            cout << graph_map[givenmid1]->adj[i]->MID << " ";
            cout << name_map[graph_map[givenmid1]->adj[i]->MID] << endl;
        }
    }
    // printing most central 10 MID
    if (choice == "part2")
    {
        int a = 1;
        for (auto it : adj_size_map)
        {
            if (a <= 10)
            {
                Node *temp = it.second;

                cout << it.first << " " << " " << temp->MID << " " << name_map[temp->MID] << "\n";
            }
            a++;
        }
    }
    // printing shortest path
    if (choice == "part3")
    {
        givenmid1 = argv[2];
        givenmid2 = argv[3];
        if (graph_map[givenmid1] == nullptr)
        {
            cout << "There is no MID 1 as this name\n";
            return 0;
        }
        if (graph_map[givenmid2] == nullptr)
        {
            cout << "There is no MID  2 as this name\n";
            return 0;
        }
        FindShortestPath(givenmid1, givenmid2);
    }

    return 0;
}