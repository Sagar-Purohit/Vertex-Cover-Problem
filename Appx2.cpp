//
// Created by User on 11/27/2019.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include "Appx2.h"

std::vector<int> Appx2(std::vector<int> adj[], int V){

    std::vector<int> vc;
    std::vector<int> eliminate;

    for (int i=0; i<V; i++){

        if (adj[i].size() > 0){

            if (!std::count(vc.begin(), vc.end(), i) ){
                
                for(int j=0; j< adj[i].size(); j++){

                    if (!std::count(vc.begin(), vc.end(), adj[i][j]) ){

                        vc.push_back(i);
                        vc.push_back(adj[i][j]);

                        break;

                    }
                }
            }
        }

    }

    return vc;
}

int main() {
    int V=7;
    std::vector<int> adj[7]={{4,6},{2,4,6},{1,4,6},{5,6},{0,1,2},{3},{0,1,2,3}};

    std::vector<int> vc;

    vc= Appx2(adj,V);

    for(int j=0; j< vc.size(); j++){

        std::cout << vc[j] << " ";
    }


    return 0;
}