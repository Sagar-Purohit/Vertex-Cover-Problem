#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"

std::vector<int> setting_clauses(int V,std::vector<int> adj[])
{
	std::vector<int> cover_final_array;
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
            for (auto vertex2 : adj[vertex1]) {
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
	return cover_final_array;
}
