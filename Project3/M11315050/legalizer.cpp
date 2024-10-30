#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>
using namespace std;
const float INF = numeric_limits<float>::max();
string temp;
float total_displacement=0,maximum_displacement=0;
struct node{
    float w,h,x,y;
    string name;
};
struct segment
{
    float y,w,subrowOrigin;
    int numsites;
    bool used = false;
};
float calculate_dist(const node& t_node,float x,float y){
    float result = abs(t_node.x-x)+abs(t_node.y-y);
    return result;
}
vector<vector<segment>> rows;
vector<node> nodes;
void split_segment(const node& t_node,const float& near_x,const int& near_row){
    vector<segment> t_segments = rows[near_row];
    float w = t_segments[0].w;
    int pos = (near_x-t_segments[0].subrowOrigin)/w,count=0,need_sites = t_node.w/w;
    while (pos>=t_segments[count].numsites)
    {
        count++;
        pos = (near_x-t_segments[count].subrowOrigin)/w;
    }
    // cout<<"Pre:";
    // for(auto& a:rows[near_row]){
    //     string used;
    //     used = (a.used)?" XX ":" OO ";
    //     cout<<used<<near_row<<" "<<a.subrowOrigin<<" "<<a.subrowOrigin+a.numsites*a.w<<used<<" | ";
    // }
    // cout<<endl;

    segment r_segment = t_segments[count];
    r_segment.subrowOrigin = near_x;
    r_segment.numsites = need_sites;
    r_segment.used = true;
    if(t_segments[count].numsites==need_sites){
        t_segments.erase(t_segments.begin()+count);
        t_segments.insert(t_segments.begin()+count,r_segment);
    }
    else if(pos>0){
        segment t_segment1 = t_segments[count];
        //cout<<t_segment1.used<<count<<"dada\n";
        t_segment1.numsites = pos;
        
        if(pos+need_sites==t_segments[count].numsites){
            t_segments.erase(t_segments.begin()+count);
            t_segments.insert(t_segments.begin()+count,r_segment);
            t_segments.insert(t_segments.begin()+count,t_segment1);

            //cout<<"split1:"<<t_segment1.subrowOrigin<<" "<<t_segment1.numsites<<endl;
        }
        else{
            segment t_segment2 = t_segments[count];
            t_segment2.subrowOrigin = t_segments[count].subrowOrigin+(pos+need_sites)*w;
            t_segment2.numsites = t_segments[count].numsites-(pos+need_sites);
            t_segments.erase(t_segments.begin()+count);
            t_segments.insert(t_segments.begin()+count,t_segment2);
            t_segments.insert(t_segments.begin()+count,r_segment);
            t_segments.insert(t_segments.begin()+count,t_segment1);
            //cout<<"split2:"<<t_segment1.subrowOrigin<<" "<<t_segment1.numsites<<" "<<t_segment2.subrowOrigin<<" "<<t_segment2.numsites<<endl;
        }
    }
    else{
        segment t_segment1 = t_segments[count];
        t_segment1.subrowOrigin = t_segments[count].subrowOrigin+need_sites*w;
        t_segment1.numsites = t_segments[count].numsites-need_sites;
        t_segments.erase(t_segments.begin()+count);
        t_segments.insert(t_segments.begin()+count,t_segment1);
        t_segments.insert(t_segments.begin()+count,r_segment);
        //cout<<"split3:"<<t_segment1.subrowOrigin<<" "<<t_segment1.numsites<<endl;
    }
    rows[near_row] = t_segments;
    // cout<<"Row:";
    // for(auto& a:rows[near_row]){
    //     string used;
    //     used = (a.used)?" XX ":" OO ";
    //     cout<<used<<near_row<<" "<<a.subrowOrigin<<" "<<a.subrowOrigin+a.numsites*a.w<<used<<" | ";
    // }
    //cout<<endl;
    
}
float find_min(int row_num,const node& t_node,float& near_x){
    float cost = INF;
    vector<segment> t_segments = rows[row_num];
    //cout<<"start finding\n\n";
    for(int i=0;i<t_segments.size();i++){
        if((t_segments[i].numsites>=(t_node.w/t_segments[i].w))&&!t_segments[i].used){
            int pos = (t_node.x-t_segments[i].subrowOrigin)/t_segments[i].w,n_sites = t_node.w/t_segments[i].w;
            if(pos<0){
                float t_cost = calculate_dist(t_node,t_segments[i].subrowOrigin,t_segments[i].y);
                if(cost>t_cost){
                    cost = t_cost;
                    near_x = t_segments[i].subrowOrigin;
                }           
            }
            else if(pos+n_sites<t_segments[i].numsites){
                float t_cost = calculate_dist(t_node,t_segments[i].subrowOrigin + pos*t_segments[i].w,t_segments[i].y);
                if(cost>t_cost){
                    cost = t_cost;
                    near_x = t_segments[i].subrowOrigin + pos*t_segments[i].w;  
                }
            
            }
            else{
                float t_cost = calculate_dist(t_node,t_segments[i].subrowOrigin + (t_segments[i].numsites-n_sites)*t_segments[i].w,t_segments[i].y);
                if(cost>t_cost){
                    cost = t_cost;
                    near_x = t_segments[i].subrowOrigin + (t_segments[i].numsites-n_sites)*t_segments[i].w;  
                }                
            }
            //cout<<"segment:"<<i<<" pos "<<pos<<" near_x "<<near_x<<endl;
            //cout<<"cost:"<<cost<<" "<< near_x<<" "<<t_segments[i].y<<endl;

        }
    }
    //cout<<"end finding\n\n";
    return cost;
}
node inputnode(node t_node){
    float cost = INF,t_cost,near_x,t_x;
    int min_row,min_site;
    for(int i=0;i<nodes.size();i++){
        if(t_node.name == nodes[i].name){
            t_node.w = nodes[i].w;
            t_node.h = nodes[i].h;
            break;
        }
    }
    for(int i=0;i<rows.size();i++){
        t_cost = find_min(i,t_node,t_x);
        if(cost>t_cost){
            min_row = i;
            cost = t_cost;
            near_x =t_x;
            //cout<<"found near row "<<i<<" x "<<near_x<<" cost "<<cost<<endl;            
        }        
    }
    total_displacement+=cost;
    if(cost>maximum_displacement){
        maximum_displacement = cost;
    }
    //cout<<total_displacement<<" "<<maximum_displacement<<endl;
    split_segment(t_node,near_x,min_row);
    t_node.x = near_x;
    t_node.y = rows[min_row][0].y;
    //cout<<min_row<<" "<<near_x<<" "<<near_x+t_node.w<<" "<<rows[min_row][0].y<<" "<<cost<<endl<<endl;
    return t_node;
}
void create_aux(string outputname){
    ofstream outputFile(outputname+".aux", ios::out);
    outputFile<<"RowBasedPlacement : "<<outputname<<".nodes "<<outputname<<".nets "<<outputname<<".wts "<<outputname<<".pl "<<outputname<<".scl\n";
    outputFile.close();
}
void create_scl(string inputname,string outputname){
    ifstream inputFile(inputname, ios::in);
    ofstream outputFile(outputname, ios::out);
    string line;
    while (getline(inputFile, line)) {
        outputFile << line << endl;
        size_t found = line.find("CoreRow");
        if(found!=string::npos){
            segment temprow;
            getline(inputFile, line);
            outputFile << line << endl;
            istringstream iss(line);
            iss>>temp;
            iss>>temp;
            iss>>temprow.y;
            getline(inputFile, line);
            outputFile << line << endl;
            getline(inputFile, line);
            outputFile << line << endl;
            iss.clear();
            iss.str(line);
            iss>>temp;
            iss>>temp;
            iss>>temprow.w;        
            getline(inputFile, line);
            outputFile << line << endl;
            getline(inputFile, line);
            outputFile << line << endl;
            getline(inputFile, line);
            outputFile << line << endl;
            getline(inputFile, line);
            outputFile << line << endl;
            iss.clear();
            iss.str(line);
            iss>>temp;
            iss>>temp;
            iss>>temprow.subrowOrigin;
            iss>>temp;
            iss>>temp;
            iss>>temprow.numsites;
            getline(inputFile, line);
            outputFile << line << endl;
            vector<segment> t_segments;
            t_segments.push_back(temprow);
            rows.push_back(t_segments);
        }
        else{
            outputFile << line << endl;

        }
    }
    inputFile.close();
    outputFile.close();
}
void create_nodes(string inputname,string outputname){
    ifstream inputFile(inputname, ios::in);
    ofstream outputFile(outputname, ios::out);
    string line;
    int numnodes,numterminals,count = 0;
    while (getline(inputFile, line)) {
        outputFile << line << endl;
        if(!line.empty()){
            istringstream iss(line);
            iss>>temp;
            if(temp[0]=='a'){
                node t_node;
                t_node.name = temp;
                iss>>t_node.w;
                iss>>t_node.h;
                nodes.push_back(t_node);
            }
        }
    }
    inputFile.close();
    outputFile.close();
}
void create_pl(string inputname,string outputname){
    ifstream inputFile(inputname, ios::in);
    ofstream outputFile(outputname, ios::out);
    string line;
    int numnodes,numterminals,count = 0;
    while (getline(inputFile, line)) {
        if(!line.empty()){
            istringstream iss(line);
            iss>>temp;
            if(temp[0]=='a'){
                outputFile << temp << "\t";
                node t_node;
                t_node.name = temp;
                iss>>t_node.x;
                iss>>t_node.y;
                t_node = inputnode(t_node);
                outputFile << t_node.x<<"  "<<t_node.y<<" : N\n";
                //cout<<"input count:"<<++count<<endl;
            }
            else{
                outputFile << line << endl;
            }
        }
        else{
            outputFile << line << endl;
        }
    }
    inputFile.close();
    outputFile.close();
}
void copyfile(string inputname,string outputname){
    ifstream inputFile(inputname, ios::in);
    ofstream outputFile(outputname, ios::out);
    string line;
    while (std::getline(inputFile, line)) {
        outputFile << line << std::endl;
    }
    inputFile.close();
    outputFile.close();
}
void show_rows(){
    for (const auto& g : rows) {
        for (const auto& t : g) {
            cout << t.y << endl;
            cout << t.w << endl;
            cout << t.subrowOrigin << endl;
            cout << t.numsites << endl;
        }        
    }
    cout<<"rows:"<<rows.size()<<endl;
}
void show_nodes(){
    for (const auto& t : nodes) {
        cout << t.name << endl;
        cout << t.w << endl;
        cout << t.h << endl;
    }
    cout<<"nodes:"<<nodes.size()<<endl;
}
void show_result(){
    ofstream outputFile("output_graph.txt", ios::out);
    for(auto& g:rows){
        for(auto& a:g){
        string used;
        used = (a.used)?" XX ":" OO ";
        outputFile<<used<<" "<<a.subrowOrigin<<" "<<a.subrowOrigin+a.numsites*a.w<<used<<" | ";
    }
    outputFile<<"\n\n";
    }
}
int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " INPUT_FILE OUTPUT_FILE" << endl;
        return 1;
    }
    string inputfilename = argv[1],outputfilename = argv[2];
    create_aux(outputfilename);
    cout<<"finish aux\n";
    create_scl(inputfilename+".scl",outputfilename+".scl");
    cout<<"finish scl\n";
    //show_rows();
    create_nodes(inputfilename+".nodes",outputfilename+".nodes");
    cout<<"finish nodes\n";
    //show_nodes();
    create_pl(inputfilename+".pl",outputfilename+".pl");
    //copyfile(inputfilename+".nets",outputfilename+".nets");
    //copyfile(inputfilename+".wts",outputfilename+".wts");
    //show_result();
    cout << "File created successfully from " << argv[1] << " to " << argv[2] << endl;
    cout.precision(1);
    cout<<"Total displacement: "<<std::fixed<<total_displacement<<endl;
    cout<<"Maximum displacement: "<<maximum_displacement<<endl;
    return 0;
}
