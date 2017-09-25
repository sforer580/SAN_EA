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
    friend class PaCcET;
    friend class Simulator;
    friend class Policy;
    
protected:
    
    
public:
    Parameters* pP;
    
    vector<Policy> pol;
    vector<double> best_fitness;
    vector<double> ave_fitness;
    vector<double> worst_fitness;

    void Build_Population();
    void Build_Target_Vals();
    void Run_Simulation();
    void Get_Linear_Combo_Fitness();
    void PaCcET_Fitness(PaCcET* pT, int i);
    void Get_PaCcET_Fitness(PaCcET *pT);
    void Evaluate(PaCcET *pT);
    int Binary_Select();
    void Downselect();
    void Mutation(Policy &M);
    void Repopulate();
    struct Less_Than_Policy_Fitness;
    void Sort_Policies_By_Fitness();
    void EA_Process(PaCcET *pT);
    void Output_Best_Policy_Info(int gen);
    void Store_Fitness_Data();
    void Write_Final_Pop_To_File();
    void Write_Data_To_File();
    void Delete_Text_Files();
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
        pol.at(i).PaCcET_fitness = 5485933;
        pol.at(i).fitness = 5485933;
        pol.at(i).linear_combo_fitness = 5485933;
        for (int o=0; o<pP->num_object; o++)
        {
            pol.at(i).object_fitness.push_back(5485933);
            pol.at(i).output.push_back(5485933);
        }
        for (int j=0; j<pP->num_x_val; j++)
        {
            pol.at(i).x_val.push_back(pP->x_val_min + (rand() / double(RAND_MAX))*(pP->x_val_max - pP->x_val_min));
        }
        assert(pol.at(i).x_val.size() == pP->num_x_val);
    }
    assert(pol.size() == pP->num_pol);
}


//-------------------------------------------------------------------------
//Builds the vector of target values
void EA::Build_Target_Vals()
{
    pP->target_vals.push_back(pP->target_val_1);
    pP->target_vals.push_back(pP->target_val_2);
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
//Gets the linear combination fitness for each policy
void EA::Get_Linear_Combo_Fitness()
{
    for (int i=0; i< pP->num_pol; i++)
    {
        double sum = 0;
        for (int o=0; o<pP->num_object; o++)
        {
            sum += pol.at(i).object_fitness.at(o);
        }
        pol.at(i).linear_combo_fitness = sum;
        pol.at(i).fitness = pol.at(i).linear_combo_fitness;
    }
}


//-------------------------------------------------------------------------
//Gets PaCcET fitness a policy
void EA::PaCcET_Fitness(PaCcET* pT, int i)
{
    vector<double> MO;
    vector<double>* pMO = &MO;
    MO = pol.at(i).object_fitness;
    vector<double> OMO = MO;
    pT->execute_N_transform(pMO);
    pol.at(i).fitness = MO.at(0) + MO.at(1);
    pol.at(i).PaCcET_fitness = pol.at(i).fitness;
}


//-------------------------------------------------------------------------
//Gets the PaCcET fitness for each policy
void EA::Get_PaCcET_Fitness(PaCcET *pT)
{
    //cout << "XXXX"  << endl;
    for (int i=0; i<pP->num_pol; i++)
    {
        pol.at(i).fitness = 0;
        pT->Pareto_Check(pol.at(i).object_fitness);
    }
    
    //cout << "XXyX"  << endl;
    
    for (int i=0; i<pP->num_pol; i++)
    {
            PaCcET_Fitness(pT, i);
    }
    //cout << "XyyX"  << endl;
}


//-------------------------------------------------------------------------
//Evaluates each policies fitness scores for each objective
void EA::Evaluate(PaCcET *pT)
{
    for (int i=0; i<pP->num_pol; i++)
    {
        double diff_1;
        diff_1 = abs(pP->target_val_1 - pol.at(i).output.at(0));
        pol.at(i).object_fitness.at(0) = diff_1;
        
        double diff_2;
        diff_2 = abs(pP->target_val_2 - pol.at(i).output.at(1));
        pol.at(i).object_fitness.at(1) = diff_2;
        
    }
        
    //using linear combination
    if (pP->linear_combo == 1)
    {
        Get_Linear_Combo_Fitness();
    }
    
    //This is where additional evaluation steps can occur like PaCcET
    //using PaCcET
    else if (pP->PaCcET == 1)
    {
        Get_PaCcET_Fitness(pT);
    }
    
    //using first objective fitness
    else
    {
        //For now the PaCcET fitness is equal to the objective fitness
        for (int i=0; i< pP->num_pol; i++)
        {
            pol.at(i).fitness = pol.at(i).object_fitness.at(0);
        }
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
    if(pol.at(index_1).fitness < pol.at(index_2).fitness)
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
void EA::EA_Process(PaCcET *pT)
{
    Run_Simulation();
    Evaluate(pT);
    Downselect();
    Repopulate();
}


//-------------------------------------------------------------------------
//Sorts the population based on their fitness from lowest to highest
struct EA::Less_Than_Policy_Fitness
{
    inline bool operator() (const Policy& struct1, const Policy& struct2)
    {
        return (struct1.fitness < struct2.fitness);
    }
};


//-------------------------------------------------------------------------
//Sorts population
void EA::Sort_Policies_By_Fitness()
{
    for (int i=0; i<pP->num_pol; i++)
    {
        sort(pol.begin(), pol.end(), Less_Than_Policy_Fitness());
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
        cout << "FITNESS" << "\t" << pol.at(0).fitness << endl;
        cout << "AGE" << "\t" << pol.at(0).age << endl;
        cout << "TARGETS" << "\t";
        for (int i=0; i<pP->num_object; i++)
        {
            cout << pP->target_vals.at(i) << "\t";
        }
        cout << endl;
        cout << "OUTPUTS" << "\t";
        for (int i=0; i<pP->num_object; i++)
        {
            cout << pol.at(0).output.at(i) << "\t";
        }
        cout << endl;
        cout << "X_VALS" << "\t";
        for (int i=0; i<pP->num_x_val; i++)
        {
            cout << pol.at(0).x_val.at(i) << "\t";
        }
        cout << endl;
        cout << "PaCcET FITNESS" << "\t" << pol.at(0).PaCcET_fitness << endl;
        cout << "LINEAR COMBINATION FITNESS" << "\t" << pol.at(0).linear_combo_fitness << endl;
        cout << "OBJECTIVE FITNESS" << "\t";
        for (int o=0; o<pP->num_object; o++)
        {
            cout << pol.at(0).object_fitness.at(o) << "\t";
        }
        cout << endl;
        cout << endl;
    }
    
    else
    {
        cout << "FINAL GENERATION" << endl;
        cout << "BEST POLICY" << endl;
        cout << "FITNESS" << "\t" << pol.at(0).fitness << endl;
        cout << "AGE" << "\t" << pol.at(0).age << endl;
        cout << "TARGETS" << "\t";
        for (int i=0; i<pP->num_object; i++)
        {
            cout << pP->target_vals.at(i) << "\t";
        }
        cout << endl;
        cout << "OUTPUTS" << "\t";
        for (int i=0; i<pP->num_object; i++)
        {
            cout << pol.at(0).output.at(i) << "\t";
        }
        cout << endl;        cout << "X_VALS" << "\t";
        for (int i=0; i<pP->num_x_val; i++)
        {
            cout << pol.at(0).x_val.at(i) << "\t";
        }
        cout << endl;
        cout << "PaCcET FITNESS" << "\t" << pol.at(0).PaCcET_fitness << endl;
        cout << "LINEAR COMBINATION FITNESS" << "\t" << pol.at(0).linear_combo_fitness << endl;
        cout << "OBJECTIVE FITNESS" << "\t";
        for (int o=0; o<pP->num_object; o++)
        {
            cout << pol.at(0).object_fitness.at(o) << "\t";
        }
        cout << endl;
        cout << endl;
    }
}


//-------------------------------------------------------------------------
//Stores the statistics of the population for each generation
void EA::Store_Fitness_Data()
{
    //Best Fitness
    best_fitness.push_back(pol.at(0).fitness);
    
    
    //Average Fitness
    double sum = 0;
    for (int p=0; p<pP->num_pol; p++)
    {
        sum += pol.at(p).fitness;
    }
    double ave = 0;
    ave  = sum/pP->num_pol;
    ave_fitness.push_back(ave);
    
    
    //Worst Fitness
    worst_fitness.push_back(pol.at(pol.size()-1).fitness);
    
    
}


//-------------------------------------------------------------------------
//Writes final population to a txt file
void EA::Write_Final_Pop_To_File()
{
    ofstream File6;
    File6.open("Final_Population_Fitness.txt");
    ofstream File7;
    File7.open("Final_Population_Function_Values.txt");
    for (int i=0; i<pP->num_pol; i++)
    {
        File6 << pol.at(i).fitness << endl;
        for (int o=0; o<pP->num_object; o++)
        {
            File7 << pol.at(i).object_fitness.at(o) << "\t";
        }
        File7 << endl;
    }
    File6.close();
    File7.close();
}


//-------------------------------------------------------------------------
//Writes the data to a txt file
void EA::Write_Data_To_File()
{
    assert(best_fitness.size() == pP->gen_max);
    assert(ave_fitness.size() == pP->gen_max);
    assert(worst_fitness.size() == pP->gen_max);
    
    
    //
    ofstream File1;
    File1.open("Best_Fitness.txt", ios_base::app);
    for (int i=0; i<best_fitness.size(); i++)
    {
        File1 << best_fitness.at(i) << "\t";
    }
    File1 << endl;
    File1.close();
    
    
    //
    ofstream File2;
    File2.open("Ave_Fitness.txt", ios_base::app);
    for (int i=0; i<ave_fitness.size(); i++)
    {
        File2 << ave_fitness.at(i) << "\t";
    }
    File2 << endl;
    File2.close();
    
    
    //
    ofstream File3;
    File3.open("Worst_Fitness.txt", ios_base::app);
    for (int i=0; i<worst_fitness.size(); i++)
    {
        File3 << worst_fitness.at(i) << "\t";
    }
    File3 << endl;
    File3.close();
    
    
    //
    ofstream File4;
    File4.open("Best_Policy_Info.txt", ios_base::app);
    File4 << "Policy Fitness" << "\t" << pol.at(0).fitness << endl;
    File4 << "Policy Age" << "\t" << pol.at(0).age << endl;
    File4 << "Target Values" << "\t";
    for (int i=0; i<pP->num_object; i++)
    {
        File4 << pP->target_vals.at(i) << "\t";
    }
    File4 << endl;
    File4 << "Output Values" << "\t";
    for (int i=0; i<pP->num_object; i++)
    {
        File4 << pol.at(0).output.at(i) << "\t";
    }
    File4 << endl;
    File4 << "Policy X Vals" << "\t";
    for (int i=0; i<pP->num_x_val; i++)
    {
        File4 << pol.at(0).x_val.at(i) << "\t";
    }
    File4 << endl;
    File4 << "Policy PaCcET Fitness" << "\t" << pol.at(0).PaCcET_fitness << endl;
    File4 << "Policy linear combination Fitness" << "\t" << pol.at(0).linear_combo_fitness << endl;
    File4 << "Objective fitness" << "\t";
    for (int o=0; o<pP->num_object; o++)
    {
        File4 << pol.at(0).object_fitness.at(o) << "\t";
    }
    File4 << endl;
    File4.close();
    
    
    //
    ofstream File5;
    File5.open("Parameters.txt", ios_base::app);
    File5 << "----------EA Parameters----------" << endl;
    File5 << "Number of Policies" << "\t" << pP->num_pol << endl;
    File5 << "Number of Objectives" << "\t" << pP->num_object << endl;
    File5 << "Number of Generations" << "\t" << pP->gen_max << endl;
    File5 << "Mutation Rate" << "\t" << pP->mutation_rate*100 << endl;
    File5 << "Mutation Range" << "\t" << pP->mutate_range << endl;
    File5 << "Number of X Values" << "\t" << pP->num_x_val << endl;
    File5 << "X Value Min/Max" << "\t" << "\t" << pP->x_val_min << "\t" << pP->x_val_max << endl;
    File5 << "Target Values" << "\t";
    for (int i=0; i<pP->num_object; i++)
    {
        File5 << pP->target_vals.at(i) << "\t";
    }
    File5 << endl;
    File5 << "Selection method" << "\t";
    if (pP->linear_combo == 1)
    {
        File5 << "Linear combination" << endl;
    }
    else if (pP->PaCcET == 1)
    {
        File5 << "PaCcET" << endl;
    }
    else
    {
        File5 << "First objective" << endl;
    }
    File5.close();
    
    Write_Final_Pop_To_File();
}


//-------------------------------------------------------------------------
//Deltes the text files
void EA::Delete_Text_Files()
{
    //
    if( remove( "Best_Fitness.txt" ) != 0 )
        perror( "ERROR DELETING FILE Best_Fitness" );
    else
        puts( "Best_Fitness FILE SUCCESSFULLY DELETED" );
    cout << endl;
    
    //
    if( remove( "Ave_Fitness.txt" ) != 0 )
        perror( "ERROR DELETING FILE Ave_Fitness" );
    else
        puts( "Ave_Fitness FILE SUCCESSFULLY DELETED" );
    cout << endl;
    
    //
    if( remove( "Worst_Fitness.txt" ) != 0 )
        perror( "ERROR DELETING FILE Worst_Fitness" );
    else
        puts( "Worst_Fitness FILE SUCCESSFULLY DELETED" );
    cout << endl;
    
    //
    if( remove( "Best_Policy_Info.txt" ) != 0 )
        perror( "ERROR DELETING FILE Best_Policy_Info" );
    else
        puts( "Best_Policy_Info FILE SUCCESSFULLY DELETED" );
    cout << endl;
    
    //
    if( remove( "Parameters.txt" ) != 0 )
        perror( "ERROR DELETING FILE Parameters" );
    else
        puts( "Parameters FILE SUCCESSFULLY DELETED" );
    cout << endl;
}


//-------------------------------------------------------------------------
//Runs the entire program
void EA::Run_Program()
{
    //For PaCcET use
    PaCcET* pT;
    if (pP->PaCcET == 1)
    {
        PaCcET T;
        pT = &T;
    }
    
    
    Delete_Text_Files();
    Build_Target_Vals();
    Build_Population();
    for (int gen=0; gen<pP->gen_max; gen++)
    {
        if (gen < pP->gen_max-1)
        {
            EA_Process(pT);
            Sort_Policies_By_Fitness();
            Output_Best_Policy_Info(gen);
            Store_Fitness_Data();
        }
        else
        {
            Run_Simulation();
            Evaluate(pT);
            Sort_Policies_By_Fitness();
            Output_Best_Policy_Info(gen);
            Store_Fitness_Data();
        }
    }
    Write_Data_To_File();
}


#endif /* EA_hpp */
