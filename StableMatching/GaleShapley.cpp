//
//  GaleShapley.cpp
//  Stable matching problem
//

#include <iostream>
#include <queue>
#include <map>
#include <ctime>

using namespace std;
#define M 10
#define W 10
typedef struct
{
    queue<int> mPrefs[M];
    int wRank[W][M];
    map<int,int> w_engage;
    queue<int> free_m;
    
}Problem;

template<typename K, typename V>
void PrintMap(std::map<K,V> const &m)
{
    for (auto const& pair: m) {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}

void CreateProblem(Problem& P){
    // init mPrefs and wRank
    for (int i = 0; i < M; i ++){
        for (int j = 0; j < W; j ++){
//            P.mPrefs[i].push(rand()%W);
            P.mPrefs[i].push(j);
//            P.wRank[j][i] = rand()%M;
            P.wRank[j][i] = M-i;
        }
    }
    
    // init free man list
    for (int i = 0; i < M; i ++){
        P.free_m.push(i);
    }
    
}

void GaleShapley(Problem& P){
    
    while(P.free_m.size()> 0){
        
        int cur_m = P.free_m.front();
        
        // man proposed according to preference list
        while(P.mPrefs[cur_m].size()>0){
            int cur_w = P.mPrefs[cur_m].front();
            
            // if the preferred women is not engaged
            if (P.w_engage.find(cur_w) == P.w_engage.end()){
                P.w_engage[cur_w]= cur_m;
                P.free_m.pop();
                break;
            }
            
            // if the preferred woman is engaged, check ranking list
            int cur_matched_m = P.w_engage[cur_w];
            if (P.wRank[cur_w][cur_matched_m] < P.wRank[cur_w][cur_m]){
                P.free_m.push(cur_matched_m);
                P.w_engage[cur_w]= cur_m;
                P.free_m.pop();
                break;
            }
            
            // preferred woman has been enaged, eliminate from the list
            P.mPrefs[cur_m].pop();
        }
        
    }
}

int main(int argc, const char * argv[]) {
    srand(time(0));
    Problem P;
    CreateProblem(P);
    GaleShapley(P);
    PrintMap(P.w_engage);
    std::cout << "\nHappy Ending :)\n";
    return 0;
    
}




