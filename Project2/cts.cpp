#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
int max_dist = -1;
    struct point
    {
        int x = 0;
        int y = 0;
        int value = 0;
        int dist = max_dist;
    };
    struct line{
        point a,b;
    };
    vector<line> lines;
	point** table;
    vector<point> sinkpoint,outputpoint,inlinepoint,routepoint,spreadpoint,temppoint,midroutepoint;
    string temp;
    int point_num,dimx,dimy,step,temp_size;
    point t_point,s_point,m_point;
    bool mid_is_sink = false;
bool In_table(int x,int y){
    if(x>=dimx||y>=dimy||x<0||y<0)
        return false;
    return true;

}
bool In_table(point p){
    if(p.x>=dimx||p.y>=dimy||p.x<0||p.y<0)
        return false;
    return true;

}

bool same_pos(point x,point y){
    if(x.x==y.x&&x.y==y.y)
        return true;
    return false;
}
bool Not_sink(point p){
    if(same_pos(m_point,p)){
        mid_is_sink = true;
        return true;
    }
    for(int i=0;i<sinkpoint.size();i++){
        if(same_pos(p,sinkpoint[i]))
            return false;
    }
    return true;
}
void remove_mid_sink(){
    for(int i=0;i<sinkpoint.size();i++){
        if(same_pos(sinkpoint[i],m_point)){
            sinkpoint.erase(sinkpoint.begin()+i);
            return;
        }
    }
}
void set_value(){
    int spread_num = min(dimx,dimy)/6;
    int tempx = 0,tempy = 0;
    for (int i = 0; i < dimx; i++)
	{
		for (int j = 0; j < dimy; j++)
		{   
			table[i][j].value = 0;
		}
	}
    for(int i=0;i<sinkpoint.size();i++){
        t_point = sinkpoint[i];
        for(int j=1;j<spread_num;j++){
            tempx = t_point.x;
            tempy = t_point.y;
            for(int k=0;k<j*2;k++){
                if(In_table(tempx-j+k,tempy+j))
                    table[tempx-j+k][tempy+j].value+=(spread_num-j);
                if(In_table(tempx+j,tempy+j-k))
                    table[tempx+j][tempy+j-k].value+=(spread_num-j);
                if(In_table(tempx+j-k,tempy-j))
                    table[tempx+j-k][tempy-j].value+=(spread_num-j);
                if(In_table(tempx-j,tempy-j+k))
                    table[tempx-j][tempy-j+k].value+=(spread_num-j);
            }
        }
    }
}

void set_dist(){
    for (int i = 0; i < dimx; i++)
	{
		for (int j = 0; j < dimy; j++)
		{   
			table[i][j].dist = max_dist;
		}
	}
    for(int i = 1;i<midroutepoint.size();i++){
        table[midroutepoint[i].x][midroutepoint[i].y].dist = -2;
    }
    for(int i=0;i<inlinepoint.size();i++){
        //cout<<inlinepoint[i].x<<" "<<inlinepoint[i].y;
        table[inlinepoint[i].x][inlinepoint[i].y].dist = 0;
    }
}
point spread(){
    while(true){
    int erasepoint = -1;
    for(int i=0;i<spreadpoint.size();i++){
        for(int j=0;j<sinkpoint.size();j++){
            if(same_pos(spreadpoint[i],sinkpoint[j])){
                erasepoint = j;
                t_point = sinkpoint[j];
            }
        }
        //cout<<spreadpoint[i].x<<" "<<spreadpoint[i].y<<endl;
    }
    if(erasepoint!=-1){
        sinkpoint.erase(sinkpoint.begin()+erasepoint);
        return t_point;
    }
    temp_size = spreadpoint.size();
    //cout<<step<<" "<<temp_size<<endl;
    step++;
    temppoint.clear();
    for(int i=0;i<temp_size;i++){
            t_point.x = spreadpoint[i].x+1;
            t_point.y = spreadpoint[i].y;
        if(In_table(t_point)&&table[t_point.x][t_point.y].dist==max_dist){
            temppoint.push_back(t_point);
            table[t_point.x][t_point.y].dist = step;
        }

            t_point.x = spreadpoint[i].x-1;
            t_point.y = spreadpoint[i].y;
        if(In_table(t_point)&&table[t_point.x][t_point.y].dist==max_dist){
            temppoint.push_back(t_point);
            table[t_point.x][t_point.y].dist = step;

        }

            t_point.x = spreadpoint[i].x;
            t_point.y = spreadpoint[i].y+1;
        if(In_table(t_point)&&table[t_point.x][t_point.y].dist==max_dist){
            temppoint.push_back(t_point);
            table[t_point.x][t_point.y].dist = step;
        }

            t_point.x = spreadpoint[i].x;
            t_point.y = spreadpoint[i].y-1;
        if(In_table(t_point)&&table[t_point.x][t_point.y].dist==max_dist){
            temppoint.push_back(t_point);            
            table[t_point.x][t_point.y].dist = step;
        }        
    }
    spreadpoint.assign(temppoint.begin(), temppoint.end());

    // if(step>12)
    // return t_point;
    //return spread(step);
    }
}
point spread_init(){
    set_dist();
    spreadpoint.clear();
    routepoint.clear();

    step = 1;
    for(int i=0;i<inlinepoint.size();i++){
            t_point.x = inlinepoint[i].x+1;
            t_point.y = inlinepoint[i].y;
        if(In_table(t_point)&&(table[t_point.x][t_point.y].dist==max_dist||table[t_point.x][t_point.y].dist>step)){
            spreadpoint.push_back(t_point);
            table[t_point.x][t_point.y].dist = step;
        }

            t_point.x = inlinepoint[i].x-1;
            t_point.y = inlinepoint[i].y;
        if(In_table(t_point)&&(table[t_point.x][t_point.y].dist==max_dist||table[t_point.x][t_point.y].dist>step)){
            spreadpoint.push_back(t_point);
            table[t_point.x][t_point.y].dist = step;

        }

            t_point.x = inlinepoint[i].x;
            t_point.y = inlinepoint[i].y+1;
        if(In_table(t_point)&&(table[t_point.x][t_point.y].dist==max_dist||table[t_point.x][t_point.y].dist>step)){
            spreadpoint.push_back(t_point);
            table[t_point.x][t_point.y].dist = step;
        }

            t_point.x = inlinepoint[i].x;
            t_point.y = inlinepoint[i].y-1;
        if(In_table(t_point)&&(table[t_point.x][t_point.y].dist==max_dist||table[t_point.x][t_point.y].dist>step)){
            spreadpoint.push_back(t_point);            
            table[t_point.x][t_point.y].dist = step;
        }
    }
    return spread();

}
void connectmid(){
    for (int i = 0; i < dimx; i++)
	{
		for (int j = 0; j < dimy; j++)
		{   
			table[i][j].dist = max_dist;
		}
	}
    table[s_point.x][s_point.y].dist = 0;
    spreadpoint.clear();
    routepoint.clear();
    step = 0;
    spreadpoint.push_back(s_point);
    bool found = false;
    while(!found){

    temp_size = spreadpoint.size();
    //cout<<step<<" "<<temp_size<<endl;
    step++;
    temppoint.clear();
    for(int i=0;i<temp_size;i++){
        if(same_pos(m_point,spreadpoint[i])){
            found = true;
            break;
        }

            t_point.x = spreadpoint[i].x+1;
            t_point.y = spreadpoint[i].y;
        if(Not_sink(t_point)&&In_table(t_point)&&table[t_point.x][t_point.y].dist==max_dist){
            temppoint.push_back(t_point);
            table[t_point.x][t_point.y].dist = step;
        }

            t_point.x = spreadpoint[i].x-1;
            t_point.y = spreadpoint[i].y;
        if(Not_sink(t_point)&&In_table(t_point)&&table[t_point.x][t_point.y].dist==max_dist){
            temppoint.push_back(t_point);
            table[t_point.x][t_point.y].dist = step;

        }

            t_point.x = spreadpoint[i].x;
            t_point.y = spreadpoint[i].y+1;
        if(Not_sink(t_point)&&In_table(t_point)&&table[t_point.x][t_point.y].dist==max_dist){
            temppoint.push_back(t_point);
            table[t_point.x][t_point.y].dist = step;
        }

            t_point.x = spreadpoint[i].x;
            t_point.y = spreadpoint[i].y-1;
        if(Not_sink(t_point)&&In_table(t_point)&&table[t_point.x][t_point.y].dist==max_dist){
            temppoint.push_back(t_point);            
            table[t_point.x][t_point.y].dist = step;
        }        
    }
    spreadpoint.assign(temppoint.begin(), temppoint.end());

    }
    //cout<<m_point.x<<" mid "<<m_point.y<<endl;
    step-=2;
    found = false;
    point inputpoint = m_point,p=m_point;
    midroutepoint.push_back(inputpoint);
    //cout<<inputpoint.x<<" "<<inputpoint.y<<" "<<step<<endl;
    while(step>=0){

        if(table[p.x+1][p.y].dist==step){
            inputpoint.x = p.x+1;
            inputpoint.y = p.y;
        }
        else  if(table[p.x-1][p.y].dist==step){
            inputpoint.x = p.x-1;
            inputpoint.y = p.y;
        }
        else  if(table[p.x][p.y+1].dist==step){
            inputpoint.x = p.x;
            inputpoint.y = p.y+1;
        }
        else  if(table[p.x][p.y-1].dist==step){
            inputpoint.x = p.x;
            inputpoint.y = p.y-1;
        }
        p = inputpoint;
        //cout<<inputpoint.x<<" "<<inputpoint.y<<" "<<step<<endl;
        midroutepoint.push_back(inputpoint);

        step--;
    }

}
void connectpoint(point p){
    //cout<<"yy "<<p.x<<" "<<p.y<<" "<<step<<endl;
    point inputpoint;
    inputpoint.value = -1;
    t_point.x = p.x+1;
    t_point.y = p.y;
    step--;
    if(In_table(t_point)){
        if((table[t_point.x][t_point.y].dist == step)&&(table[t_point.x][t_point.y].value>inputpoint.value)){
            inputpoint.x = t_point.x;
            inputpoint.y = t_point.y;
            inputpoint.value = table[t_point.x][t_point.y].value;
        }
    //cout<<"aa "<<t_point.x<<" "<<t_point.y<<" "<<table[t_point.x][t_point.y].dist<<" "<<table[t_point.x][t_point.y].value<<endl;
        
    }
    t_point.x = p.x-1;
    t_point.y = p.y;
    if(In_table(t_point)){
        if((table[t_point.x][t_point.y].dist == step)&&(table[t_point.x][t_point.y].value>inputpoint.value)){
            inputpoint.x = t_point.x;
            inputpoint.y = t_point.y;
            inputpoint.value = table[t_point.x][t_point.y].value;
        }
    //cout<<"aa "<<t_point.x<<" "<<t_point.y<<" "<<table[t_point.x][t_point.y].dist<<" "<<table[t_point.x][t_point.y].value<<endl;

    }
    t_point.x = p.x;
    t_point.y = p.y+1;
    if(In_table(t_point)){
        if((table[t_point.x][t_point.y].dist == step)&&(table[t_point.x][t_point.y].value>inputpoint.value)){
            inputpoint.x = t_point.x;
            inputpoint.y = t_point.y;
            inputpoint.value = table[t_point.x][t_point.y].value;
        }
    //cout<<"aa "<<t_point.x<<" "<<t_point.y<<" "<<table[t_point.x][t_point.y].dist<<" "<<table[t_point.x][t_point.y].value<<endl;

    }
    t_point.x = p.x;
    t_point.y = p.y-1;
    if(In_table(t_point)){
        if((table[t_point.x][t_point.y].dist == step)&&(table[t_point.x][t_point.y].value>inputpoint.value)){
            inputpoint.x = t_point.x;
            inputpoint.y = t_point.y;
            inputpoint.value = table[t_point.x][t_point.y].value;
        }
    //cout<<"aa "<<t_point.x<<" "<<t_point.y<<" "<<table[t_point.x][t_point.y].dist<<" "<<table[t_point.x][t_point.y].value<<endl;

    }
    //cout<<"xx "<<inputpoint.x<<" "<<inputpoint.y<<" "<<step<<endl;
    if(inputpoint.value==-1)
        return;
    routepoint.push_back(inputpoint);
    if(step != 0){
        connectpoint(inputpoint);
    }
    else{
        for(int i =0;i<routepoint.size()-1;i++){
            inlinepoint.push_back(routepoint[i]);
        }
    }

}
void addline(){
    line l;
    point a,b,t_a,t_b;
    int end;
    while(!routepoint.empty()){
        end = routepoint.size()-1;
        a=routepoint[end];
        routepoint.pop_back();
        end--;
        b=routepoint[end];
        if(a.x==b.x){
            end--;
            t_a = a;
            t_b = b;
            while(end>=0){
                if(t_a.x==routepoint[end].x){
                    t_b = routepoint[end];
                    end--;
                    routepoint.pop_back();
                }
                else{
                    l.a = t_a;
                    l.b = t_b;
                    lines.push_back(l);
                    //cout<<"add "<<l.a.x<<" "<<l.a.y<<" "<<l.b.x<<" "<<l.b.y<<" "<<routepoint.size()<<" "<<end<<endl;
                    break;
                }        
            }
            if(end<0){
                l.a = t_a;
                l.b = t_b;
                lines.push_back(l);
                //cout<<"add "<<l.a.x<<" "<<l.a.y<<" "<<l.b.x<<" "<<l.b.y<<" "<<routepoint.size()<<" "<<end<<endl;
                return;        
            }    
        }
        else if(a.y==b.y){
            end--;
            t_a = a;
            t_b = b;
            while(end>=0){
                if(t_a.y==routepoint[end].y){
                    t_b = routepoint[end];
                    end--;
                    routepoint.pop_back();
                }
                else{
                    l.a = t_a;
                    l.b = t_b;
                    lines.push_back(l);
                    //cout<<"add "<<l.a.x<<" "<<l.a.y<<" "<<l.b.x<<" "<<l.b.y<<" "<<end<<endl;

                    break;
                }
            }
            if(end<0){
                l.a = t_a;
                l.b = t_b;
                lines.push_back(l);
                //cout<<"add "<<l.a.x<<" "<<l.a.y<<" "<<l.b.x<<" "<<l.b.y<<" "<<routepoint.size()<<" "<<end<<endl;
                return;        
            }
        }

    }
}
int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " INPUT_FILE OUTPUT_FILE" << endl;
        return 1;
    }

    ifstream inputFile(argv[1], ios::in);
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open input file " << argv[1] << endl;
        return 1;
    }

    ofstream outputFile(argv[2], ios::out);
    if (!outputFile.is_open()) {
        cerr << "Error: Could not open output file " << argv[2] << endl;
        inputFile.close(); 
        return 1;
    }
    ofstream pltoutputFile("plt.txt",ios::out);






    inputFile>>temp>>point_num;
    inputFile>>temp>>dimx>>temp>>dimy;
    inputFile>>s_point.x>>s_point.y;

    //inlinepoint.push_back(s_point);
	dimx += 1;
	dimy += 1;

	table = new point* [dimx];
	for (int i = 0; i < dimx; i++)
		table[i] = new point[dimy];
	
	for (int i = 0; i < dimx; i++)
	{
		for (int j = 0; j < dimy; j++)
		{   
            point t_point;
			table[i][j] = t_point;
		}
	}
    for(int i = 0;i<point_num-1;i++){
        inputFile>>t_point.x>>t_point.y;
        sinkpoint.push_back(t_point);
        outputpoint.push_back(t_point);
        m_point.x+=t_point.x;
        m_point.y+=t_point.y;
    }
    inputFile>>temp;
    int found = 0;
    m_point.x/=(point_num-1);
    m_point.y/=(point_num-1);
    //cout<<m_point.x<<" "<<m_point.y<<endl;
    inlinepoint.push_back(m_point);
    connectmid();
    if(mid_is_sink)
        remove_mid_sink();
    routepoint.assign(midroutepoint.begin(),midroutepoint.end());
    addline();
    while(!sinkpoint.empty()){
        for(int i=0;i<sinkpoint.size();i++){
            //cout<<"z "<<sinkpoint[i].x<<" "<<sinkpoint[i].y<< endl;
         }
        set_value();
        point findpoint;
        findpoint = spread_init();

        routepoint.push_back(findpoint);
        table[findpoint.x][findpoint.y].dist=999;
        connectpoint(findpoint);
        for(int i=0;i<routepoint.size();i++){
            //cout<<routepoint[i].x<<" "<<routepoint[i].y<<endl;
        }
        addline();
        found++;
        cout<<"Found:"<<found<<endl;          
    }

    outputFile<<".l "<<lines.size()<<"\n.dimx "<<dimx-1<<"\n.dimy "<<dimy-1<<endl;
    for(int i=0;i<lines.size();i++){
        outputFile<<lines[i].a.x<<" "<<lines[i].a.y<<" "<<lines[i].b.x<<" "<<lines[i].b.y<<endl;        
    }
    outputFile<<".e"<<endl;


    pltoutputFile<<"set xrange [0:"<<dimx-1<<"]\n";
    pltoutputFile<<"set yrange [0:"<<dimy-1<<"]\n\n";
    for(int i = 0;i<lines.size();i++){        
        pltoutputFile<<"set arrow from "<<lines[i].a.x<<","<<lines[i].a.y<<" to "<<lines[i].b.x<<","<<lines[i].b.y<<" nohead\n";

    }
    pltoutputFile<<"\nplot NaN\n\n";
    pltoutputFile<<"plot '-' with points pt 7 ps 0.5 notitle, '-' with points pt 5 ps 0.5 notitle\n";
    for(int i =0;i<outputpoint.size();i++){
        pltoutputFile<<outputpoint[i].x<<" "<<outputpoint[i].y<<endl;
    }
    pltoutputFile<<"e\n";
    pltoutputFile<<s_point.x<<" "<<s_point.y<<"\ne\n";
    //pltoutputFile<<lines.size();

    inputFile.close();
    
    outputFile.close();

    pltoutputFile.close();

	for (int i = 0; i < dimx; i++)
		delete[] table[i];
	delete[] table;
    cout << "File created successfully from " << argv[1] << " to " << argv[2] << endl;
    return 0;
}
