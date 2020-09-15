#include "ShortestPath.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include<bits/stdc++.h>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include "pthread.h"
#include <algorithm>
#include "Appx2.h"
#include <time.h>
#include <cmath>

std::vector<int> adj[100],final_approx_1,cover_final_array,vc;
int V;
std::vector<float> cnfsat_time;
std::vector<float> appx1_time;
std::vector<float> appx2_time;
std::vector<float> cnfsat_ratio;
std::vector<float> appx1_ratio;
std::vector<float> appx2_ratio;



//adding numbers to adj to connect two point and form line
void add_edge(std::vector<int> adj[], int src, int dest)
{
    adj[src].push_back(dest);
    adj[dest].push_back(src);
}



//minisat solver for adding clauses and generating array of vertex cover
void *  setting_clauses(void *)
{
	cover_final_array.clear();
		
	std::vector<int> adj1[V];
	clockid_t cnfsat_id;
	struct timespec tspec_cnfsat;

	

	for(int i=0;i< V; i++){
		for(int j=0;j<adj[i].size();j++){
			adj1[i].push_back(adj[i][j]);
		
		}
	}
	
    std::unique_ptr<Minisat::Solver> minisat_solver(new Minisat::Solver());
    int vertex_size = 0;
    bool satisfiable;
    while (vertex_size <= V - 1)
    {
		//generating matrix for the solver and adding clauses
		std::vector<std::vector<Minisat::Lit>> matrix_minisat(V);
        for (int i = 0; i < V; i++)
            for (int j = 0; j < vertex_size; j++) {
                Minisat::Lit literal = Minisat::mkLit(minisat_solver->newVar());
                matrix_minisat[i].push_back(literal);
            }
        // adding first condition clause
        for (int i = 0; i < vertex_size; i++) {
            Minisat::vec<Minisat::Lit> initial_literals;
            for (int j = 0; j < V; j++) {
                initial_literals.push(matrix_minisat[j][i]);
            }
            minisat_solver->addClause(initial_literals);
        }

		//checking and adding second condition clause
        for (int m = 0; m < V; m++){
            for (int p = 0; p < vertex_size; p++){
                for (int q = p + 1; q < vertex_size; q++) {
                    minisat_solver->addClause(~matrix_minisat[m][p], ~matrix_minisat[m][q]);
                }
			}
		}
		//checking and adding third condition clause
        for (int m = 0; m < vertex_size; m++){
            for (int p = 0; p < V; p++){
                for (int q = p + 1; q < V; q++) {
                    minisat_solver->addClause(~matrix_minisat[p][m], ~matrix_minisat[q][m]);
                }
			}
		}
		//checking and adding forth condition clause
        for(int vertex1 = 0 ; vertex1 < V; ++vertex1) {
            for (auto vertex2 : adj1[vertex1]) {
                if(vertex2 < vertex1) continue;
                Minisat::vec<Minisat::Lit> set_edge;
                for (int i = 0; i < vertex_size; i++) {
                    set_edge.push(matrix_minisat[vertex1][i]);
                    set_edge.push(matrix_minisat[vertex2][i]);
                }

                minisat_solver->addClause(set_edge);
            }
        }
        satisfiable = minisat_solver->solve();
        if (satisfiable )
        {
            for (int i = 0; i < V; i++)
                for (int j =0; j < vertex_size; j++)
                    if ( Minisat::toInt(minisat_solver->modelValue(matrix_minisat[i][j])) == 0)
                    {
                        cover_final_array.push_back(i);
                    }

            break;
        }
        else {
            minisat_solver.reset(new Minisat::Solver());
        }
        vertex_size++;
    }
	    
	std::sort( cover_final_array.begin(), cover_final_array.end(), std::less<int>());
	minisat_solver.reset(new Minisat::Solver());
	cnfsat_ratio.push_back(cover_final_array.size());
	
    if(pthread_getcpuclockid(pthread_self(),&cnfsat_id)==0){
	    if(clock_gettime(cnfsat_id,&tspec_cnfsat)!=0)
		    std::cout<<"error occured"<<std::endl;
		else{
//		    std::cout<<"time of cnfsat in nano sec is "<<tspec_cnfsat.tv_nsec<<std::endl;
//		    std::cout<<"time of cnfsat in sec is "<<tspec_cnfsat.tv_sec<<std::endl;
		    cnfsat_time.push_back(tspec_cnfsat.tv_sec*1000000+tspec_cnfsat.tv_nsec/1000);
		}
									        }




	return NULL;
}




void * cnf_vertex_1_algo(void *){

	final_approx_1.clear();
	bool continue_loop = true;
	std::vector<int> adj2[V];
    clockid_t appx1_id;
    struct timespec tspec_appx1;
    

	for (int i=0;i<V;i++){
		for(int j=0;j<adj[i].size();j++){

			adj2[i].push_back(adj[i][j]);
		}		
	}	

	do{

		int num=0,max_number=0,index_max=0;

        for(int i=0;i< V; i++){

			if (!std::count(final_approx_1.begin(), final_approx_1.end(), i) ){

				num = adj2[i].size();

				for (int j=0; j<adj2[i].size(); j++){

					if (std::count(final_approx_1.begin(), final_approx_1.end(), adj2[i][j]) ){

						num = num-1;
					}
				}

				if(max_number<num) {
					max_number=num;
					index_max=i;
				}
			}
		}

		if (max_number>0){
			final_approx_1.push_back(index_max);
			continue_loop = true;
		}
		else{
			
			continue_loop = false;
		}
	
	
/*	
			int max_number=0,index_max=0;
//		std::cout<<"algo1 starts with V:"<<V<<"\n";
		for(int i=0;i< V; i++){
			if(max_number<adj2[i].size())
			{
				max_number=adj2[i].size();
				index_max=i;
			}
		}
		
//		std::cout<<"nodes in vector["<<index_max<<": are ["<<adj[index_max].size()<<"]";


		int array_to_remove[adj2[index_max].size()];
		int max_size_to_remove = adj2[index_max].size();
		for(int j=0;j<adj2[index_max].size();j++){
			array_to_remove[j]=adj2[index_max][j];
//			std::cout<<"deletinng array no :"<<array_to_remove[j]<<"\n";
		}	
		for(int k=0;k<max_size_to_remove;k++)
		{
			adj2[array_to_remove[k]].clear();
		}
		final_approx_1.push_back(index_max);
		adj2[index_max].clear();
//		std::cout << "adj2 cleared";
		for(int i=0 ; i<V; i++){
//			std::cout<<i<<"  :";
			if(adj2[i].size()>0){
				continue_loop = true;
			}
			else {
				continue_loop = false;
			}
			for(int j=0;j<adj2[i].size();j++){
//				std::cout<<adj[i][j]<<"  ";
			}
//			std::cout<<"\n";
		}
*/
		
	}
	while(continue_loop == true);

	std::sort( final_approx_1.begin(), final_approx_1.end(), std::less<int>());
	appx1_ratio.push_back(final_approx_1.size());

	if(pthread_getcpuclockid(pthread_self(),&appx1_id)==0){
		if(clock_gettime(appx1_id,&tspec_appx1)!=0)
			std::cout<<"error occured"<<std::endl;
		else{
//		    std::cout<<"time of Appx1 in nano sec is "<<tspec_appx1.tv_nsec<<std::endl;
//		    std::cout<<"time of Appx1 in sec is "<<tspec_appx1.tv_sec<<std::endl;
		    appx1_time.push_back(tspec_appx1.tv_sec*1000000+tspec_appx1.tv_nsec/1000);
		}
   }




	return NULL;
}




void * Appx2(void *){
	vc.clear();
	std::vector<int> adj3[V];
	clockid_t appx2_id;
	struct timespec tspec_appx2;
	 
	
	for(int i=0;i< V; i++){
    	for(int j=0;j<adj[i].size();j++){
	
			adj3[i].push_back(adj[i][j]);
		}
    }

    for (int i=0; i<V; i++){

        if (adj3[i].size() > 0){

            if (!std::count(vc.begin(), vc.end(), i) ){
                
                for(int j=0; j< adj3[i].size(); j++){

                    if (!std::count(vc.begin(), vc.end(), adj3[i][j]) ){
							//std::cout<<i<<j<<"\n";

                        vc.push_back(i);
                        vc.push_back(adj3[i][j]);

                        break;

                    }
                }
            }
        }

    }

	std::sort( vc.begin(), vc.end(), std::less<int>());
	appx2_ratio.push_back(vc.size());
	if(pthread_getcpuclockid(pthread_self(),&appx2_id)==0){
		if(clock_gettime(appx2_id,&tspec_appx2)!=0)
			std::cout<<"error occured"<<std::endl;
		else{
//		    std::cout<<"time of appx2 in nano sec is "<<tspec_appx2.tv_nsec<<std::endl;
//			std::cout<<"time of appx2 in sec is "<<tspec_appx2.tv_sec<<std::endl;
			appx2_time.push_back(tspec_appx2.tv_sec*1000000+tspec_appx2.tv_nsec/1000);
		}
	}





    return NULL;
}


void* IOthreadFuntion(void*){
//	int V=0;
    char sep = ' ';
    std::vector<int> vect;
    //std::vector<int> adj;
    std::string line;
    std::string E_data;
	bool E_set=false;
	bool vertex_cover_flag=false;

    while (!std::cin.eof()) {
        try {
            std::getline(std::cin, line);
            if (line.length() > 0) {
                if (line[0] == 'V' && line[1] == ' ') {
                    std::stringstream(line) >> sep >> V;
                    //adj[V];
					for(int i=0;i<V;i++){
					adj[i].clear();
					}
					adj[V];
					//adj.clear();
//					std::cout<<"before clear final_approx 1:"<<final_approx_1.size();
//					std::cout<<"before cover_final_array:"<<cover_final_array.size();
//					std::cout<<"before clear vc"<<vc.size();
					final_approx_1.clear();
					cover_final_array.clear();
					vc.clear();
//					std::cout<<"after clear final_approx 1:"<<final_approx_1.size();
//					                      std::cout<<"after cover_final_array:"<<cover_final_array.size();
//										                        std::cout<<"after clear vc"<<vc.size();

				}
                else if (line[0] == 'E' && line[1] == ' ') {
                    if (V != 0) {
                        std::string E_input_string = line;
                        std::stringstream(line) >> sep >> E_data;
                        E_data = line;
                        //removing last { and 'E {'
                        E_data = E_data.erase(0, 3);
                        E_data = E_data.erase(E_data.length() - 1);
                        //removing the < and last >
                        E_data = E_data.erase(0, 1);
                        E_data = E_data.erase(E_data.length() - 1);
                        replace(E_data.begin(), E_data.end(), ',', ' ');
                        replace(E_data.begin(), E_data.end(), '<', ' ');
                        replace(E_data.begin(), E_data.end(), '>', ' ');
                        std::stringstream ss(E_data);
                        std::string token;
                        //removing white space
                        while (getline(ss, token, ' ')) {
                            if (token.length() > 0) {
                                vect.push_back(std::stoi(token));
                            }

                        }
                        //pushing source to destination from string
                        while (!vect.empty()) {
                            int start = vect.front();
                            vect.erase(vect.begin());
                            int end = vect.front();
                            vect.erase(vect.begin());
                            //add error for more edge number than vertices
                            if (start < V && end < V) {
                                add_edge(adj, start, end);
                            } else {
                                throw std::runtime_error("Error: Edge number must be less than V");
                            }
                        }
						E_set= true;
						
						if(E_set == true)
						{
							pthread_t cnf_thread;
							pthread_t apprx2_thread;
							pthread_t apprx1_thread;
							pthread_create(&cnf_thread,NULL,setting_clauses,NULL);
							pthread_create(&apprx1_thread,NULL,cnf_vertex_1_algo,NULL);
							pthread_create(&apprx2_thread,NULL,Appx2,NULL);
							pthread_join(apprx2_thread,NULL);	
							pthread_join(apprx1_thread,NULL);
							pthread_join(cnf_thread,NULL);
//							std::vector<int> vertex_cover_array=cover_final_array;
							vertex_cover_flag = true;
							//printing cnf ans
							std::cout<<"CNF-SAT-VC: ";
							for(unsigned i=0;i<cover_final_array.size();i++)
							{
//									std::cout<<i;
								if(i == cover_final_array.size() - 1)
								std::cout<<cover_final_array[i];
								else
								std::cout<<cover_final_array[i]<<",";
							}
							if(vertex_cover_flag == true)
							std::cout<<std::endl;
							//printingg approx 1 ans
							std::vector<int> vertex_cover_array_approx_1 = final_approx_1;
							std::cout<<"APPROX-VC-1: ";
							for(unsigned i=0;i<vertex_cover_array_approx_1.size();i++)
							{
								if(i == vertex_cover_array_approx_1.size() - 1)
								std::cout<<vertex_cover_array_approx_1[i];
								else
								std::cout<<vertex_cover_array_approx_1[i]<<",";
							}
							if(vertex_cover_flag == true)
							std::cout<<std::endl;
						//printing approx 2
							std::vector<int> vertex_cover_array_approx_2 = vc;
							std::cout<<"APPROX-VC-2: ";
							for(unsigned i=0;i<vertex_cover_array_approx_2.size();i++)
							{
								if(i == vertex_cover_array_approx_2.size() - 1)
								std::cout<<vertex_cover_array_approx_2[i];
								else
								std::cout<<vertex_cover_array_approx_2[i]<<",";
							}
							if(vertex_cover_flag == true)
							std::cout<<std::endl;
						}
						
                    }
                    else {
                        throw std::runtime_error("Error: Enter number of Vertices first");
                    }

                } /*else if (line[0] == 's' && line[1] == ' ') {
                    //separate s and d from string
                    std::stringstream(line) >> sep >> src >> dst;
                    if(src>V){
                        throw std::runtime_error("Error: Source is greater than number of vertices");
                    }
                    else if(dst>V){
                        throw std::runtime_error("Error: Destination is greater than number of vertices");
                    }
                    else{
                        printShortestDistance(adj, src, dst, V);
                    }

                }*/ else {
                    std::cout << "Error: Wrong command\n";
                }
            }
            else{
                //throw std::runtime_error("Error: Wrong Command");
            }
        }

        catch (std::runtime_error &e) {
            std::cout << e.what() << "\n";
        }

    }


/*			
	float mean_cnfsat=0,mean_appx1=0,mean_appx2=0,tot_cnfsat=0,tot_appx1=0,tot_appx2=0;
	for(int i=0; i<cnfsat_time.size(); i++){
		
	//		std::cout <<"Entry "<< i << "of cnfsat is " << cnfsat_time[i]<<"\n";	
		tot_cnfsat= tot_cnfsat+cnfsat_time[i];
	}	
	mean_cnfsat=tot_cnfsat/cnfsat_time.size();
	//std::cout <<"cnfsat mean time= " << mean_cnfsat << "\n";
	std::cout << mean_cnfsat << "\n";

	for(int i=0; i<appx1_time.size(); i++){

	    tot_appx1= tot_appx1+appx1_time[i];
	}
    mean_appx1=tot_appx1/appx1_time.size();
	//std::cout <<"appx1 mean time= " << mean_appx1 << "\n";
	std::cout << mean_appx1 << "\n";
	
	for(int i=0; i<appx2_time.size(); i++){

	    tot_appx2= tot_appx2+appx2_time[i];
				        }
    mean_appx2=tot_appx2/appx2_time.size();
	//std::cout <<"appx2 mean time= " << mean_appx2 << "\n";
	std::cout << mean_appx2 << "\n";
	float var_cnfsat=0,var_appx1=0,var_appx2=0,sd_cnfsat=0,sd_appx1=0,sd_appx2=0;

	for(int i=0; i<cnfsat_time.size(); i++){
		
			var_cnfsat=var_cnfsat+((cnfsat_time[i]-mean_cnfsat)*(cnfsat_time[i]-mean_cnfsat));
	
	}
	
	sd_cnfsat=std::sqrt(var_cnfsat/(cnfsat_time.size()-1));
//	std::cout <<"cnfsat sd time= " << sd_cnfsat << "\n";
	std::cout <<sd_cnfsat << "\n";
//	std::cout <<"cnfsat var time= " << var_cnfsat << "\n";
	for(int i=0; i<appx1_time.size(); i++){

		var_appx1=var_appx1+(appx1_time[i]-mean_appx1)*(appx1_time[i]-mean_appx1);

    }
    sd_appx1=sqrt(var_appx1/(appx1_time.size()-1));
	//std::cout <<"appx1 sd time= " << sd_appx1 << "\n";
	std::cout <<sd_appx1 << "\n";
	for(int i=0; i<appx2_time.size(); i++){

		var_appx2=var_appx2+(appx2_time[i]-mean_appx2)*(appx2_time[i]-mean_appx2);

	}
    sd_appx2=sqrt(var_appx2/(appx2_time.size()-1));
	//std::cout <<"appx2 sd time= " << sd_appx2 << "\n";
	std::cout <<sd_appx2 << "\n";
	
	float ratio1=0,ratio2=0,mean_ratio1=0,mean_ratio2=0,tot_ratio1=0,tot_ratio2=0;
	std::vector<float> appx1_ratio_set;
	std::vector<float> appx2_ratio_set;
	for(int i=0; i<cnfsat_ratio.size(); i++){

		ratio1= appx1_ratio[i]/cnfsat_ratio[i];
		ratio2= appx2_ratio[i]/cnfsat_ratio[i];
		tot_ratio1=tot_ratio1+ratio1;
		tot_ratio2=tot_ratio2+ratio2;
		appx1_ratio_set.push_back(ratio1);
		appx2_ratio_set.push_back(ratio2);

	}
	mean_ratio1=tot_ratio1/cnfsat_ratio.size();
	mean_ratio2=tot_ratio2/cnfsat_ratio.size();

//	std::cout << "Tot_ratio 2: "<<tot_ratio2 << "\n";
//	std::cout << "Number of entries: "<<cnfsat_ratio.size() << "\n";
	std::cout << mean_ratio1 << "\n";
	std::cout << mean_ratio2 << "\n";

	float var_ratio1=0,var_ratio2=0,sd_ratio1=0,sd_ratio2=0;

	for(int i=0; i<appx1_ratio_set.size(); i++){

              var_ratio1=var_ratio1+((appx1_ratio_set[i]-mean_ratio1)*(appx1_ratio_set[i]-mean_ratio1));

      }

    sd_ratio1=std::sqrt(var_ratio1/(appx1_ratio_set.size()-1));
	
	for(int i=0; i<appx2_ratio_set.size(); i++){

                var_ratio2=var_ratio2+((appx2_ratio_set[i]-mean_ratio2)*(appx2_ratio_set[i]-mean_ratio2));

        }

    sd_ratio2=std::sqrt(var_ratio2/(appx2_ratio_set.size()-1));

	std::cout << sd_ratio1 << "\n";
    std::cout << sd_ratio2 << "\n";
*/
	
}


int main()
{
	pthread_t io_thread;
	pthread_create(&io_thread,NULL,IOthreadFuntion,NULL);
	pthread_join(io_thread,NULL);
	   	
    return 0;
}
