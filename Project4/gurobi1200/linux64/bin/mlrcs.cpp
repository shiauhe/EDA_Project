#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <cstdlib>
using namespace std;
string temp;
struct node
{
    int type = -1, l = 0, t = 0;
    string id = "";
    vector<node *> depand;
};
vector<node *> nodes;
vector<node *> readynodes;
vector<node *> ongoingnodes;
vector<node *> work_node;
vector<node *> outputnodes;
node *getnode(string id)
{

    for (auto &i : nodes)
    {
        if (i->id == id)
        {
            return i;
        }
    }
    // cout<<"no found\n";
    return nullptr;
}
void propogation(node *t_node)
{
    // cout<<t_node->depand.size();
    for (auto &i : t_node->depand)
    {
        // cout<<i->id<<endl;
        if (i->l < t_node->l + 1)
        {
            i->l = t_node->l + 1;
            propogation(i);
        }
        // cout<<i->l;
    }
}
bool compareByLevelDesc(const node *a, const node *b)
{
    return a->l > b->l;
}
bool cango(const node *a)
{
    for (auto &i : a->depand)
    {
        vector<node *>::iterator it = std::find(readynodes.begin(), readynodes.end(), i);
        if (it == readynodes.end())
            return false;
    }
    return true;
}
int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        cerr << "Usage: " << argv[0] << " INPUT_FILE OUTPUT_FILE" << endl;
        return 1;
    }
    string inputfilename = argv[2];
    bool mode;
    ifstream inputFile(inputfilename, ios::in);
    ofstream outputFile("list.lp", ios::out);
    string line, boundbuffer = "", parameter = argv[1];
    vector<int> unit;
    unit.push_back(stoi(argv[3]));
    unit.push_back(stoi(argv[4]));
    unit.push_back(stoi(argv[5]));

    if (parameter == "-h")
    {
        mode = 0;
    }
    else if (parameter == "-e")
    {
        mode = 1;
    }
    else
    {
        cout << "Parameter1 must be -h/-e\n";
        exit(1);
    }

    outputFile << "Minimize\n\tlatency\n\nSubject To\n";

    int numnodes, numterminals, count = 1;
    while (getline(inputFile, line))
    {
        if (!line.empty())
        {
            stringstream iss(line);
            iss >> temp;
            if (temp == ".inputs")
            {
                while (iss >> temp)
                {
                    if (temp == "\\")
                    {
                        getline(inputFile, line);
                        iss.clear();
                        iss.str(line);
                    }
                    else
                    {
                        node *t_node = new node();
                        t_node->id = temp;
                        nodes.push_back(t_node);
                        readynodes.push_back(t_node);
                    }
                }
            }
            else if (temp == ".outputs")
            {
                // cout << line << endl;
                while (iss >> temp)
                {
                    if (temp == "\\")
                    {
                        getline(inputFile, line);
                        iss.clear();
                        iss.str(line);
                    }
                    else
                    {
                        node *t_node = new node();
                        t_node->id = temp;
                        outputnodes.push_back(t_node);
                    }
                }
            }
            else if (temp == ".names")
            {
                node *t_node = new node();
                while (iss >> temp)
                {
                    node *ptr = getnode(temp);
                    if (ptr != nullptr)
                    {
                        // cout<<ptr->id<<" ";
                        t_node->depand.push_back(ptr);
                    }
                }
                t_node->id = temp;
                int max = 0;

                getline(inputFile, line);
                iss.clear();
                iss.str(line);

                iss >> temp;

                if (temp == "0")
                    t_node->type = 2;
                else if (temp.find('-') != std::string::npos)
                {
                    for (int i = 0; i < temp.size() - 1; i++)
                    {
                        getline(inputFile, line);
                        t_node->type = 1;
                    }
                }
                else
                {
                    t_node->type = 0;
                }
                // cout<<t_node.l<< " type:  " + to_string(t_node.type)<<endl;
                // cout<<t_node->depand.size()<<" 123 ";
                nodes.push_back(t_node);
                ongoingnodes.push_back(t_node);
                work_node.push_back(t_node);
            }
        }
    }
    inputFile.close();
    for (auto &i : outputnodes)
    {
        node *t_node = getnode(i->id);
        // cout<<t_node->id<<" "<<t_node->depand.size()<<endl;
        propogation(t_node);
    }
    for (auto &i : nodes)
    {
        for (auto &n : i->depand)
        {
            vector<node *>::iterator it = std::find(readynodes.begin(), readynodes.end(), n);
            if (it == readynodes.end())
            {
                outputFile << "\tc" << count << ": start_" << i->id << " - start_" << n->id << " >= 1\n";
                count++;
            }
        }
    }
    for (auto &i : outputnodes)
    {
        outputFile << "\tc" << count << ": latency - start_" << i->id << " >= 0\n";
        count++;
    }

    boundbuffer += "Bounds\n";
    for (auto &i : ongoingnodes)
    {
        boundbuffer += "\tstart_" + i->id + " >= 1\n";
    }
    boundbuffer += "\tlatency >= 1\n";
    outputFile << "\n";
    std::sort(ongoingnodes.begin(), ongoingnodes.end(), compareByLevelDesc);
    // for (auto &i : ongoingnodes)
    // {
    //     cout << i->l << " " << i->type << " " << i->id << endl;
    // }
    int latency = 0;
    vector<int> v;
    vector<string> vs;
    vector<vector<string>> outputline;
    if (mode == 0)
        cout << "Heuristic Scheduling Result\n";

    while (!ongoingnodes.empty())
    {
        v.assign(unit.begin(), unit.end());
        outputline.assign(3, vs);
        int index = 0;
        while (!(v[0] == 0 && v[1] == 0 && v[2] == 0) && index < ongoingnodes.size())
        {
            if (cango(ongoingnodes[index]) && v[ongoingnodes[index]->type] > 0)
            {
                outputline[ongoingnodes[index]->type].push_back(ongoingnodes[index]->id);
                v[ongoingnodes[index]->type]--;
                ongoingnodes[index]->t = latency + 1;
            }
            index++;
        }
        latency++;
        node *t_node;
        if (mode == 0)
            cout << latency << ": ";
        for (int i = 0; i < 3; i++)
        {
            if (mode == 0)
                cout << "{";
            for (auto &s : outputline[i])
            {
                if (mode == 0)
                    cout << s;
                if (mode == 0)
                    if (s != outputline[i].back())
                        cout << " ";
                t_node = getnode(s);
                vector<node *>::iterator it = std::find(ongoingnodes.begin(), ongoingnodes.end(), t_node);
                ongoingnodes.erase(ongoingnodes.begin() + distance(ongoingnodes.begin(), it));
                readynodes.push_back(t_node);
            }
            if (mode == 0)
                cout << "} ";
        }
        if (mode == 0)
            cout << endl;
    }
    if (mode == 0)
        cout << "LATENCY: " << latency << "\nEND\n";

    if (mode == 0)
    {
        outputFile.close();
        exit(1);
    }

    count = 1;
    for (int i = 1; i <= latency; i++)
    {
        outputFile << "\tr" << count << ": used_AND_t" << i << " <= " << unit[0] << "\n";
        count++;
        outputFile << "\tr" << count << ": used__OR_t" << i << " <= " << unit[1] << "\n";
        count++;
        outputFile << "\tr" << count << ": used_NOT_t" << i << " <= " << unit[2] << "\n";
        count++;
    }
    outputFile << "\n";

    for (int i = 1; i <= latency; i++)
    {
        outputFile << "\tr" << count << ": used_AND_t" << i;
        for (auto &n : work_node)
        {
            if (n->type == 0)
            {
                outputFile << " - x_" << n->id << "_t" << i;
            }
        }
        count++;
        outputFile << " = 0\n";
        outputFile << "\tr" << count << ": used__OR_t" << i;
        for (auto &n : work_node)
        {
            if (n->type == 1)
            {
                outputFile << " - x_" << n->id << "_t" << i;
            }
        }
        count++;
        outputFile << " = 0\n";
        outputFile << "\tr" << count << ": used_NOT_t" << i;
        for (auto &n : work_node)
        {
            if (n->type == 2)
            {
                outputFile << " - x_" << n->id << "_t" << i;
            }
        }
        outputFile << " = 0\n";
        count++;
    }

    outputFile << "\n";

    for (auto &i : work_node)
    {
        outputFile << "\tstart_" << i->id;
        for (int index = 1; index <= latency; index++)
        {
            outputFile << " - " << index << " x_" << i->id << "_t" << index << " ";
        }
        outputFile << "= 0\n";
    }

    outputFile << "\n";

    for (auto &i : work_node)
    {
        outputFile << "\tx_" << i->id << "_t1";
        for (int index = 2; index <= latency; index++)
        {
            outputFile << " + " << "x_" << i->id << "_t" << index;
        }
        outputFile << " = 1\n";
    }

    outputFile << "\n"
               << boundbuffer;

    outputFile << "\nBinary\n\t";

    for (auto &i : work_node)
    {
        for (int index = 1; index <= latency; index++)
        {
            outputFile << "x_" << i->id << "_t" << index << " ";
        }
    }

    outputFile << "\n\nEnd\n";
    outputFile.close();

    int latency2 = -1;
    string command = "./gurobi_cl ResultFile=result.sol list.lp ";
    system(command.c_str());
    ifstream resultfile("result.sol", ios::in);
    cout << "ILP-based Scheduling Result\n";

    while (getline(resultfile, line))
    {
        stringstream iss(line);
        iss >> temp;
        if (temp == "latency")
        {
            iss >> temp;
            latency2 = stoi(temp);
        }
        else if (temp == "used_NOT_t" + to_string(latency))
        {
            for (int index = 1; index <= latency2; index++)
            {
                outputline.clear();
                outputline.assign(3, vs);
                for (auto &n : work_node)
                {
                    getline(resultfile, line);
                    iss.clear();
                    iss.str(line);
                    iss >> temp;
                    size_t first_underscore = temp.find('_');
                    size_t second_underscore = temp.find('_', first_underscore + 1);
                    string result = temp.substr(first_underscore + 1, second_underscore - first_underscore - 1);
                    iss >> temp;
                    if (temp == "1")
                    {
                        node *t_node = getnode(result);
                        outputline[t_node->type].push_back(t_node->id);
                    }
                }
                cout << to_string(index) << ": ";
                for (int i = 0; i < 3; i++)
                {
                    cout << "{";
                    for (auto &s : outputline[i])
                    {
                        cout << s;
                        if (s != outputline[i].back())
                        {
                            cout << " ";
                        }
                    }
                    cout << "} ";
                }
                cout << "\n";
            }
        }
    }
    cout << "LATENCY: " << latency2 << "\nEND\n";
    for (auto &n : nodes)
    {
        delete n;
    }
    resultfile.close();
    return 0;
}
