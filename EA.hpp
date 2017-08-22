//
//  EA.hpp
//  SAN_EA_project
//
//  Created by Scott S Forer on 8/21/17.
//  Copyright © 2017 SAN_EA_project. All rights reserved.
//

#ifndef EA_hpp
#define EA_hpp

#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <time.h>
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <cassert>
#include <ctime>
#include <random>

using namespace std;


class EA
{
    friend class Parameters;
    friend class Simulator;
    friend class Policy;
    
protected:
    
    
public:
    Parameters* pP;
    
    vector<Policy> pol;

    void Build_Population();
    void Run_Simulation();
    void Evaluate();
    int Binary_Select();
    void Downselect();
    void Mutation(Policy &M);
    void Repopulate();
    struct Less_Than_Policy_PaCcET_Fitness;
    void Sort_Policies_By_PaCcET_Fitness();
    void EA_Process();
    void Output_Best_Policy_Info(int gen);
    void Run_Program();
    
private:
};


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------
//Builds population of policies
void EA::Build_Population()
{
    for (int i=0; i<pP->num_pol; i++)
    {
        Policy P;
        pol.push_back(P);
        pol.at(i).age = 0;
        for (int o=0; o<pP->num_object; o++)
        {
            pol.at(i).object_fitness.push_back(0);
            pol.at(i).output = 10000000;
            for (int j=0; j<pP->num_x_val; j++)
            {
                pol.at(i).x_val.push_back(pP->x_val_min + (rand() / double(RAND_MAX))*(pP->x_val_max - pP->x_val_min));
            }
            assert(pol.at(i).x_val.size() == pP->num_x_val);
        }
    }
    assert(pol.size() == pP->num_pol);
}


//-------------------------------------------------------------------------
//Puts each policy into the simulation
void EA::Run_Simulation()
{
    for (int i=0; i<pP->num_pol; i++)
    {
        //First we insert a policy into the simulator then we can take the objective data for that policy and store it in our data architecture
        Simulator S;
        Parameters P;
        S.pP = &P;
        Policy* pPo;
        pPo = & pol.at(i);
        S.Simulate(pPo);
    }
}

//-------------------------------------------------------------------------
//Evaluates each policies fitness scores for each objective
void EA::Evaluate()
{
    for (int i=0; i<pP->num_pol; i++)
    {
        double diff;
        diff = abs(pP->target_val - pol.at(i).output);
        pol.at(i).object_fitness.at(0) = diff;
        //This is where additional evaluation steps can occur like PaCcET
        //For now the PaCcET fitness is equal to the objective fitness
        pol.at(i).PaCcET_fitness = pol.at(i).object_fitness.at(0);
    }
}


//-------------------------------------------------------------------------
//Randomly selects two individuals and decides which one will die based on their fitness
int EA::Binary_Select()
{
    int loser;
    int index_1 = rand() % pol.size();
    int index_2 = rand() % pol.size();
    while (index_1 == index_2)
    {
        index_2 = rand() % pol.size();
    }
    //winner is one with lower fitness
    //Can be switched for a maximization problem
    if(pol.at(index_1).PaCcET_fitness < pol.at(index_2).PaCcET_fitness)
    {
        loser = index_2;
        //cout << "loser" << "\t" <<  "agent" << "\t" << index_2 << endl;
    }
    else
    {
        loser = index_1;
        //cout << "loser" << "\t" <<  "agent" << "\t" << index_1 << endl;
    }
    return loser;
}


//-------------------------------------------------------------------------
//Policies are compared to determine the optimal policies for a given generation
void EA::Downselect()
{
    int kill;
    for(int k=0; k<pP->to_kill; k++)
    {
        kill = Binary_Select();
        pol.erase(pol.begin() + kill);
    }
    assert(pol.size() == pP->to_kill);
    for(int i=0; i<pP->to_kill; i++)
    {
        pol.at(i).age += 1;
    }
}


//-------------------------------------------------------------------------
//Mutates the copies of the winning individuals
void EA::Mutation(Policy &M)
{
   //This is where the policy is slightly mutated
    
    for (int j=0; j<pP->num_x_val; j++)
    {
        double r = ((double)rand()/RAND_MAX);
        if (r <= pP->mutation_rate)
        {
            double R1 = ((double)rand()/RAND_MAX) * pP->mutate_range;
            double R2 = ((double)rand()/RAND_MAX) * pP->mutate_range;
            M.x_val.at(j) = M.x_val.at(j) + (R1-R2);
            if (M.x_val.at(j) < pP->x_val_min)
            {
                M.x_val.at(j) = pP->x_val_min;
            }
            if (M.x_val.at(j) > pP->x_val_max)
            {
                M.x_val.at(j) = pP->x_val_max;
            }
        }
        assert(M.x_val.at(j) >= pP->x_val_min && M.x_val.at(j) <= pP->x_val_max);
    }
}


//-------------------------------------------------------------------------
//The population is repopulated based on small mutations of the remaining policies
void EA::Repopulate()
{
    int to_replicate = pP->to_kill;
    for (int rep=0; rep<to_replicate; rep++)
    {
        Policy M;
        int spot = rand() % pol.size();
        M = pol.at(spot);
        //cout << "cp" << endl;
        Mutation(M);
        pol.push_back(M);
        pol.at(pol.size()-1).age = 0;
    }
    assert(pol.size() == pP->num_pol);
}


//-------------------------------------------------------------------------
//Runs the entire EA loop process
void EA::EA_Process()
{
    Run_Simulation();
    Evaluate();
    Downselect();
    Repopulate();
}


//-------------------------------------------------------------------------
//Sorts the population based on their fitness from lowest to highest
struct EA::Less_Than_Policy_PaCcET_Fitness
{
    inline bool operator() (const Policy& struct1, const Policy& struct2)
    {
        return (struct1.PaCcET_fitness < struct2.PaCcET_fitness);
    }
};


//-------------------------------------------------------------------------
//Sorts population
void EA::Sort_Policies_By_PaCcET_Fitness()
{
    for (int i=0; i<pP->num_pol; i++)
    {
        sort(pol.begin(), pol.end(), Less_Than_Policy_PaCcET_Fitness());
    }
}


//-------------------------------------------------------------------------
//Outputs the best policies information
void EA::Output_Best_Policy_Info(int gen)
{
    if (gen < pP->gen_max-1)
    {
        cout << "GENERATION" << "\t" << gen << endl;
        cout << "BEST POLICY" << endl;
        cout << "TARGET" << "\t" << pP->target_val << endl;
        cout << "OUTPUT" << "\t" << pol.at(0).output << endl;
        cout << "X_VALS" << "\t";
        for (int i=0; i<pP->num_x_val; i++)
        {
            cout << pol.at(0).x_val.at(i) << "\t";
        }
        cout << endl;
        cout << "PaCcET FITNESS" << "\t" << pol.at(0).PaCcET_fitness << endl;
        cout << "OBJECTIVE FITNESS" << "\t";
        for (int o=0; o<pP->num_object; o++)
        {
            cout << pol.at(0).object_fitness.at(0) << "\t";
        }
        cout << endl;
        cout << endl;
    }
    
    else
    {
        cout << "FINAL GENERATION" << endl;
        cout << "BEST POLICY" << endl;
        cout << "TARGET" << "\t" << pP->target_val << endl;
        cout << "OUTPUT" << "\t" << pol.at(0).output << endl;
        cout << "X_VALS" << "\t";
        for (int i=0; i<pP->num_x_val; i++)
        {
            cout << pol.at(0).x_val.at(i) << "\t";
        }
        cout << endl;
        cout << "PaCcET FITNESS" << "\t" << pol.at(0).PaCcET_fitness << endl;
        cout << "OBJECTIVE FITNESS" << "\t";
        for (int o=0; o<pP->num_object; o++)
        {
            cout << pol.at(0).object_fitness.at(0) << "\t";
        }
        cout << endl;
        cout << endl;
    }
}


//-------------------------------------------------------------------------
//Runs the entire program
void EA::Run_Program()
{
    Build_Population();
    for (int gen=0; gen<pP->gen_max; gen++)
    {
        if (gen < pP->gen_max-1)
        {
            EA_Process();
            Sort_Policies_By_PaCcET_Fitness();
            Output_Best_Policy_Info(gen);
        }
        else
        {
            Run_Simulation();
            Evaluate();
            Sort_Policies_By_PaCcET_Fitness();
            Output_Best_Policy_Info(gen);
        }
    }
}


#endif /* EA_hpp */
