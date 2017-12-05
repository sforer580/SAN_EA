//
//  main.cpp
//  T1
//
//  Created by Scott S Forer on 9/5/17.
//  Copyright © 2017 T1. All rights reserved.
//

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



#ifndef Parameters_hpp
#define Parameters_hpp

using namespace std;


class Parameters
{
    friend class EA;
    friend class Simulator;
    friend class Policy;
    
protected:
    
    
public:
    int num_pol = 100;                  //number of policies
    int to_kill = num_pol/2;
    int num_object = 4;                 //number of objectives
    int gen_max = 100;                   //number of generations
    double mutation_rate = 0.5;         //mutation rate
    double mutate_range = 0.05;          //mutation range
    int num_x_val = 4;
    double x_val_min = 0;
    double x_val_max = 10;
    
    //target function
    double target_val_0 = 1*1 + cos(2) + sin(3) + tan(4);
    double target_val_1 = 2*2 + cos(3) + sin(4) + tan(1);
    double target_val_2 = 3*3 + cos(4) + sin(1) + tan(2);
    double target_val_3 = 4*4 + cos(1) + sin(2) + tan(3);
    vector<double> target_vals;
    
    //Multi-objective selection method
    bool linear_combo = false;               //falase=off, true=on
    bool PaCcET = true;                     //false=off, true=on
    
    
private:
};

#endif /* Parameters_hpp */



#ifndef Policy_hpp
#define Policy_hpp

using namespace std;


class Policy
{
    friend class EA;
    friend class Parameters;
    friend class Simulator;
    
protected:
    
    
public:
    
    //Include all needed information about any policy here
    
    
    vector<double> object_fitness;
    double PaCcET_fitness;
    double linear_combo_fitness;
    double fitness;
    int age;
    
    vector<double> x_val;
    vector<double> output;
    
    
private:
};

#endif /* Policy_hpp */


#ifndef Simulator_hpp
#define Simulator_hpp

using namespace std;


class Simulator
{
    friend class EA;
    friend class Parameters;
    friend class Policy;
    
protected:
    
    
public:
    Parameters* pP;
    
    void Simulate(Policy* pPo);
    
    
private:
};


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------
//Runs the entire simulation process
void Simulator::Simulate(Policy* pPo)
{
    double sum_0;
    sum_0 = (pPo->x_val.at(0)*pPo->x_val.at(0)) + cos(pPo->x_val.at(1)) + sin(pPo->x_val.at(2)) + tan(pPo->x_val.at(3));
    pPo->output.at(0) = sum_0;
    
    
    double sum_1;
    sum_1 = (pPo->x_val.at(1)*pPo->x_val.at(1)) + cos(pPo->x_val.at(2)) + sin(pPo->x_val.at(3)) + tan(pPo->x_val.at(0));
    pPo->output.at(1) = sum_1;


    double sum_2;
    sum_2 = (pPo->x_val.at(2)*pPo->x_val.at(2)) + cos(pPo->x_val.at(3)) + sin(pPo->x_val.at(0)) + tan(pPo->x_val.at(1));
    pPo->output.at(2) = sum_2;

    double sum_3;
    sum_3 = (pPo->x_val.at(3)*pPo->x_val.at(3)) + cos(pPo->x_val.at(0)) + sin(pPo->x_val.at(1)) + tan(pPo->x_val.at(2));
    pPo->output.at(3) = sum_3;
}


#endif /* Simulator_hpp */





#ifndef PaCcET_h
#define PaCcET_h

#define PaCcET_VERBOSE 0

#define PFRONT_THRESHOLD 50
#define PFRONT_BUFFER 10

#define OBJECTIVES 4
#define PI 3.1415

#ifndef VECTOR_INCLUDE
#define VECTOR_INCLUDE
#include <vector>
#include <list>
#include <numeric>



#include <iostream>
#include <cstdlib>
#include <cmath>
#include <time.h>
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <cassert>
#include <ctime>
#include <random>
#endif

class PaCcET{
private:
    void N_Pro_transform();
    void N_Dummy_transform();
    
    /// Scaling, Nadir, Utopia calculations
    void scale();
    void calculate_scaled_pareto();
    void nad_ut();
    vector<double> utopia;
    vector<double> nadir;
    
    vector< vector<double> > PFront; /// P_I^*
    vector< vector<double> > scPFront; /// P_I^{*,norm}
    
    /// Components
    double calc_v_1(vector<double>);            // eq. 6
    double calc_v_B(vector<double>);            // eq. 7
    double calc_v_hp(vector<double>);           // eq. 8
    double calc_dtau(double, double, double);   // eq. 9
    
    /// Domination utilities
    void eliminate_not_dominating(list<vector<double> >& scPFront_temp, vector<double> td);
    
    
    vector< vector<double> > exhaustive_PFront; /// all points that have ever been part of PFront
    
    vector<double> input;
    vector<double> output;
    void take_input(vector<double>* coords);
    void give_output(vector<double>* coords);
    
    void thresh_PFront();
    void rand_thresh();
    
public:
    vector<vector<double> > get_PFront();
    
    void exhaustive_to_file();
    void PFront_to_file();
    
    /// PARETO UTILITIES
    bool Pareto_Check(vector<double>);
    
    
    /// PaCcET FUNCTIONALITY
    void Pareto_Reset();
    void execute_N_transform(vector<double>* pinputs);
    
    /// I/O
    void cout_pareto();
    void cout_scaled_pareto();
    
    /// QUADRET FUNCTIONS
    int get_PFront_size();
    vector<double> get_ut();
    bool does_v1_dominate_v2(vector<double> v1, vector<double> v2);
};

vector<vector<double> > PaCcET::get_PFront()
{
    return PFront;
}




vector<double> PaCcET::get_ut(){
    return utopia;
}

int PaCcET::get_PFront_size(){
    return PFront.size();
}

void report(FILE* pFILE, double value) { /// report to text file
    fprintf(pFILE, "%.5f\t", value);
}

void newline(FILE* pFILE) { /// report to text file
    fprintf(pFILE, "\b \b\n");
}

void PaCcET::exhaustive_to_file(){
    /// All points that have ever been in P_I^*
    FILE* PFILE;
    cout << "exhaustive in" << endl;
    PFILE=fopen("exhaustive_pareto.txt","w");
    for(int i=0; i<exhaustive_PFront.size(); i++){
        for(int j=0; j<exhaustive_PFront.at(i).size(); j++){
            report(PFILE,exhaustive_PFront.at(i).at(j));
        }
        newline(PFILE);
    }
    fclose(PFILE);
    cout << "exhaustive out" << endl;
}

void PaCcET::PFront_to_file(){
    /// Only current P_I^*
    FILE* PFILE;
    cout << "Pfront to file in" << endl;
    PFILE=fopen("T_final_front.txt","w");
    for(int i=0; i<PFront.size(); i++){
        for(int j=0; j<PFront.at(i).size(); j++){
            report(PFILE,PFront.at(i).at(j));
        }
        newline(PFILE);
    }
    cout << "Pfront to file out" << endl;
}

void PaCcET::Pareto_Reset(){
    PFront.clear();
    scPFront.clear();
    utopia.clear();
    nadir.clear();
    input.clear();
    output.clear();
}

void PaCcET::cout_pareto(){
    cout << "Current Non-Dominated Set:" << endl;
    int P = PFront.size();
    for(int p=0; p<P; p++){
        for(int q=0; q<PFront.at(p).size(); q++){
            cout << PFront.at(p).at(q) << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

void PaCcET::cout_scaled_pareto(){
    cout << endl;
    cout << "Current Non-Dominated Set (NORM):" << endl;
    int P = scPFront.size();
    for(int p=0; p<P; p++){
        for(int q=0; q<PFront.at(p).size(); q++){
            cout << scPFront.at(p).at(q) << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

bool PaCcET::does_v1_dominate_v2(vector<double> v1, vector<double> v2){
    int counter=0;
    for(int obj=0; obj<OBJECTIVES; obj++){
        /// If v1 scores better on a criteria, increment counter
        if(v1.at(obj) <= v2.at(obj)){
            counter++;
        }
    }
    if(counter==(OBJECTIVES)){
        /// If v1 scored higher or equal on all criteria...
        /// v2 is dominated.
        /// Return True.
        return true;
    }
    /// It is not dominated. Return False.
    return false;
}


bool PaCcET::Pareto_Check(vector<double> unscaled_coords){
    /// <Display the point in question>
    //cout << "Pareto Checking Point: ";
    //for(int i=0; i<coords.size(); i++){
    //    cout << coords.at(i) << "\t";
    //}
    //cout << endl;
    
    /// <Is it dominated by any point in the Pareto front??>
    /// For each Pareto point
    for(int pt=0; pt<PFront.size(); pt++){
        // determine if coords is dominated:
        if(does_v1_dominate_v2(PFront.at(pt),unscaled_coords)){
            return false;
        }
    }
    
    /// <Does it dominate any points on the Pareto front?>
    vector<int> eliminate;
    for(int pt=0; pt<PFront.size(); pt++){
        if(does_v1_dominate_v2(unscaled_coords,PFront.at(pt))){
            /// If the new point scored higher or equal on all criteria
            /// The "Pareto" point is dominated, and should be eliminated.
            eliminate.push_back(pt);
        }
    }
    
    /// <Eliminate dominated points on the Pareto Front>
    for(int e=eliminate.size()-1; e>=0; e--){
        /// We eliminate from end -> beginning so that the indices we calculated remain valid.
        int spot = eliminate.at(e);
        PFront.erase(PFront.begin()+spot);
    }
    
    /// <Add new point in correct spot of Pareto Front>
    PFront.push_back(unscaled_coords);
    // also add it to master list.
    exhaustive_PFront.push_back(unscaled_coords);
    
    thresh_PFront();
    
    /// Since Pareto Front has changed, we recalculate the dominated hyperspace.
    nad_ut();
    calculate_scaled_pareto();
    return true;
}

void PaCcET::thresh_PFront(){
    /// Function: Makes sure that the PFront is maintained at below a threshold size.
    /// If PFront is over the threshold
    if(PFront.size()>=PFRONT_THRESHOLD+PFRONT_BUFFER){
        rand_thresh();
    }
}

void PaCcET::rand_thresh(){
    while(PFront.size()>=PFRONT_THRESHOLD){
        PFront.erase(PFront.begin()+rand()%PFront.size());
    }
}

void PaCcET::calculate_scaled_pareto(){
    /// update vector< vector<double> > scPFront;
    //cout << "Calculating Scaled Pareto\t";
    scPFront.clear();
    for(int i=0; i < PFront.size(); i++){
        vector<double> dual;
        for(int j=0; j < PFront.at(i).size(); j++){
            //cout << "Pfront at i size: " << PFront.at(i).size() << endl;
            //cout << "Nadir size: " << nadir.size() << endl;
            double val = PFront.at(i).at(j);
            double min = utopia.at(j);
            double range = nadir.at(j)-utopia.at(j);
            double scval = (val - min) / range;
            dual.push_back(scval);
        }
        scPFront.push_back(dual);
    }
    //cout_scaled_pareto();
}

void PaCcET::nad_ut(){
    /// 1) calculate utopia from PFront
    /// 2) calculate nadir from PFront
    
    // 1)
    utopia.clear();
    double min;
    double mindex;
    
    for(int o=0; o<OBJECTIVES; o++){
        min=99999999999;
        mindex=-1;
        for(int p=0; p<PFront.size(); p++){
            if(PFront.at(p).at(o) < min){
                min=PFront.at(p).at(o);
                mindex=p;
            }
        }
        utopia.push_back(PFront.at(mindex).at(o)-0.001);
    }
    /// Added constant avoids atan2 problems at the edges of the PFront.
    
    // 2)
    nadir.clear();
    double max;
    double maxdex;
    for(int o=0; o<OBJECTIVES; o++){
        max=-999999999999;
        maxdex=-1;
        for(int p=0; p<PFront.size(); p++){
            if(PFront.at(p).at(o)>max){
                max=PFront.at(p).at(o);
                maxdex=p;
            }
        }
        nadir.push_back(PFront.at(maxdex).at(o));
    }
    //cout << "Nadir: " << nadir.at(0) << "\t" << nadir.at(1) << endl;
    //cout << "Utopia: " << utopia.at(0) << "\t" << utopia.at(1) << endl;
}

void PaCcET::N_Dummy_transform() {
    output.clear();
    for(int i=0; i<input.size(); i++){
        output.push_back(input.at(i));
    }
}

void PaCcET::N_Pro_transform() {
    /// For notation, See Yliniemi and Tumer, SEAL 2014.
    
    vector<double> deltas;
    int I=input.size();
    
    for(int i=0; i<I; i++){
        deltas.push_back(input.at(i));
        //cout << "DELTAS: " << i << " " << deltas.at(i) << endl;
    }
    
    vector<double> directional_ratios;
    
    double sumdeltassq=0.0;
    for(int i=0; i<I; i++){
        sumdeltassq+=deltas.at(i)*deltas.at(i);
    }
    sumdeltassq=sqrt(sumdeltassq);
    for(int i=0; i<I; i++){
        /// More truly: directional cosines.
        directional_ratios.push_back(deltas.at(i)/sumdeltassq);
    }
    
    /// <FIND v_1>
    double v_1;
    v_1 = calc_v_1(deltas);
    if(PaCcET_VERBOSE > 0){
        cout << "v_1: " << v_1 << endl;
    }
    
    /// <FIND v_B>
    double v_B;
    v_B = calc_v_B(directional_ratios);
    if(PaCcET_VERBOSE > 0){
        cout << "v_B: " << v_B << endl;
    }
    
    /// <FIND v_hp>
    double v_hp;
    v_hp = calc_v_hp(directional_ratios);
    if(PaCcET_VERBOSE > 0){
        cout << "v_hp: " << v_hp << endl;
    }
    
    /// <CALCULATE dtau>
    double dtau = v_1*v_hp/v_B;
    if(PaCcET_VERBOSE > 0){
        cout << "dtau: " << dtau << endl;
    }
    
    output.clear();
    for(int i=0; i<I; i++){
        output.push_back(dtau*directional_ratios.at(i));
    }
    
    
}

double PaCcET::calc_v_1(vector<double> deltas){
    int I=input.size();
    
    double v_1=0;
    for(int i=0; i<I; i++){
        v_1+=deltas.at(i)*deltas.at(i);
    }
    v_1=sqrt(v_1);
    return v_1;
}

void PaCcET::eliminate_not_dominating(list<vector<double> >& scPFront_temp, vector<double> td){
    
    if(scPFront_temp.size() ==1)
    {
        //cout << "SHORTCUT!" << endl;
        /// only one is dominating, no need to reduce any more.
        return;
    }
    
    bool dominated;
    
    //cout << "ELIMINATE IN: " << scPFront_temp.size() << endl;
    //for(int i=0; i< td.size(); i++){
    //cout << td.at(i) << "\t";
    //}
    //cout << endl;
    //for (std::list< vector<double> >::iterator it=scPFront_temp.begin(); it != scPFront_temp.end(); ++it){
    //vector<double> aaa = *it;
    //for(int j=0; j<aaa.size(); j++){
    //cout << "XXX " << j << " " << aaa.at(j) << endl;
    //}
    //}
    
    for (std::list< vector<double> >::iterator it=scPFront_temp.begin(); it != scPFront_temp.end(); ++it){
        dominated = does_v1_dominate_v2(*it,td);
        if(dominated==true){
            // nothing happens
            //cout << "STILL ALIVE" << endl;
        }
        if(dominated==false){
            // we don't need to use this pfront point for further calc_v_B calculations on this iteration:
            scPFront_temp.erase(it);
            //cout << "ELIMINATED!" << endl;
        }
    }
    
    //cout << "ELIMINATE OUT: " <<scPFront_temp.size() << endl;
    //for(int p=0; p<scPFront_temp.size(); p++){
    //    dominated = does_v1_dominate_v2(scPFront_temp.at(p),td);
    //    if(dominated==true){
    // nothing happens
    //   }
    //   if(dominated==false){
    // we don't need to use this pfront point for further calc_v_B calculations on this iteration:
    //        scPFront_temp.erase(scPFront_temp.begin()+p);
    //   }
    //}
}

double PaCcET::calc_v_B(vector<double> directional_ratios){
    static int iii;
    iii++;
    
    /// normalize deltas:
    /*
     double sumdirr=0.0;
     for( int i=0; i< directional_ratios.size(); i++){
     sumdirr += directional_ratios.at(i);
     }
     for( int i=0; i< directional_ratios.size(); i++){
     directional_ratios.at(i)/=sumdirr;
     }
     */
    
    //return 0.1; 6-> 18
    int I=input.size();
    /// <FIND v_B>
    double v_B;
    /// find distance along utopia->input vector which is first dominated.
    double lowerbound = 0;
    double upperbound = 0;
    for(int i=0; i<I; i++){
        upperbound+=1;
    }
    upperbound = 2*sqrt(upperbound)+0.1;
    double candidate;
    double margin=upperbound-lowerbound;
    bool dominated;
    vector<double> td;
    td.resize(I);
    //vector< vector<double> > scPFront_temp = scPFront;
    list< vector<double> > scPFront_temp;
    std::copy( scPFront.begin(), scPFront.end(), std::back_inserter( scPFront_temp ) );
    
    static int jjj;
    
    //return 0.1; 0->6
    while(margin>0.01){
        jjj++;
        dominated=false;
        candidate=(upperbound+lowerbound)/2;
        for(int i=0; i<I; i++){
            td.at(i) = candidate*directional_ratios.at(i);
        }
        
        for (std::list< vector<double> >::iterator it=scPFront_temp.begin(); it != scPFront_temp.end(); ++it){
            dominated = does_v1_dominate_v2(*it,td);
            if(dominated==true){break;}
        }
        
        //for(int p=0; p<scPFront_temp.size(); p++){
        //dominated = does_v1_dominate_v2(scPFront_temp.at(p),td);
        //if(dominated==true){break;} // once we know it is dominated, we don't need to continue calculating.
        //}
        
        if(dominated==true){upperbound = candidate;}
        //if(dominated==true){upperbound = candidate; eliminate_not_dominating(scPFront_temp,td);}
        if(dominated==false){lowerbound = candidate;}
        margin=upperbound-lowerbound;
        //cout << "SCPFRONT TEMP SIZE: " << scPFront_temp.size() << endl;
    }
    v_B=(upperbound+lowerbound)/2;
    
    //cout << "INSIDE CALC_D: " << iii << " , " << jjj << endl;
    return v_B;
    
}

double PaCcET::calc_v_hp(vector<double> directional_ratios){
    int I=input.size();
    /// <FIND v_hp>
    double v_hp;
    
    vector<double> td;
    td.resize(I);
    
    double lowerbound = 0;
    double upperbound = 0;
    for(int i=0; i<I; i++){
        upperbound+=1;
    }
    upperbound = 2*sqrt(upperbound)+0.1;
    double margin=upperbound-lowerbound;
    while(margin>0.01){
        bool dominated=false;
        double candidate=(upperbound+lowerbound)/2;
        for(int i=0; i<I; i++){
            td.at(i) = candidate*directional_ratios.at(i);
        }
        if(accumulate(td.begin(),td.end(),0.0) <= 1){dominated = true;}
        if(dominated==true){lowerbound = candidate;}
        if(dominated==false){upperbound = candidate;}
        margin=upperbound-lowerbound;
    }
    v_hp = (upperbound+lowerbound)/2;
    return v_hp;
}

double PaCcET::calc_dtau(double v_1, double v_B, double v_hp){
    double dtau = v_1*v_hp/v_B;
    return dtau;
}

void PaCcET::take_input(vector<double>* pcoords){
    /// Assign input to class variable.
    
    if(pcoords->size() != OBJECTIVES){cout << "Are we doing a " << OBJECTIVES << " objective problem or not?" << endl;}
    input.clear();
    for(int obj=0; obj<OBJECTIVES; obj++){
        input.push_back(pcoords->at(obj));
    }
}

void PaCcET::give_output(vector<double>* pcoords){
    /// Assign output to external variable.
    for(int obj=0; obj<OBJECTIVES; obj++){
        pcoords->at(obj) = output.at(obj);
    }
}

void PaCcET::scale(){
    /// Scale values of objectives to be less than one, with the nadir point taking on (1,1).
    if(PaCcET_VERBOSE > 0){
        cout << "SCALING BEFORE!\t";
        cout << input.at(0) << "," << input.at(1) << endl;
    }
    for(int obj=0; obj<OBJECTIVES; obj++){
        double val = input.at(obj);
        double range = nadir.at(obj)-utopia.at(obj);
        double scval = (val - utopia.at(obj)) / range;
        input.at(obj) = scval;
    }
    if(PaCcET_VERBOSE > 0){
        cout << "SCALING AFTER!\t";
        cout << input.at(0) << "," << input.at(1) << endl;
    }
}

void PaCcET::execute_N_transform(vector<double>* pinputs){
    take_input(pinputs);
    scale();
    N_Pro_transform();
    give_output(pinputs);
}

void Pro_Pareto_Filter_Testing(){
    PaCcET T;
    vector<double> coords;
    for(int i=0; i<1000; i++){
        cout << "Trial " << i << endl;
        coords.push_back(rand()%100000);
        coords.push_back(rand()%100000);
        cout << "coords 0\t" << coords.at(0) << endl;
        cout << "coords 1\t" << coords.at(1) << endl;
        T.Pareto_Check(coords);
        coords.clear();
        T.cout_pareto();
    }
    cout << "Placement" << endl;
    coords.push_back(-100000);
    coords.push_back(-100000);
    T.Pareto_Check(coords);
    T.cout_pareto();
    coords.clear();
}

void Procedural_Testing(){
    PaCcET T;
    vector<double> coords;
    vector<double>* pcoords = &coords;
    for(int i=0; i<100; i++){
        cout << "Trial " << i << endl;
        coords.push_back(rand()%1000);
        coords.push_back(rand()%1000);
        cout << "coords 0:\t" << coords.at(0) << endl;
        cout << "coords 1:\t" << coords.at(1) << endl;
        T.Pareto_Check(coords);
        T.execute_N_transform(pcoords);
        cout << "coords after 0\t" << coords.at(0) << endl;
        cout << "coords after 1\t" << coords.at(1) << endl;
        coords.clear();
        T.cout_pareto();
        T.cout_scaled_pareto();
    }
    cout << "Placement" << endl;
    coords.push_back(-1);
    coords.push_back(-1);
    T.Pareto_Check(coords);
    T.cout_pareto();
    T.cout_scaled_pareto();
    coords.clear();
}

#endif /* PaCcET_h */



#ifndef EA_hpp
#define EA_hpp

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
    void Write_Data_To_File(float seconds);
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
    pP->target_vals.push_back(pP->target_val_0);
    pP->target_vals.push_back(pP->target_val_1);
    pP->target_vals.push_back(pP->target_val_2);
    pP->target_vals.push_back(pP->target_val_3);
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
    	pol.at(i).object_fitness.at(0) = 0;
    	pol.at(i).object_fitness.at(1) = 0;
    	pol.at(i).object_fitness.at(2) = 0;
    	pol.at(i).object_fitness.at(3) = 0;

        double diff_0;
        diff_0 = abs(pP->target_val_0 - pol.at(i).output.at(0));
        pol.at(i).object_fitness.at(0) = diff_0;
        
        double diff_1;
        diff_1 = abs(pP->target_val_1 - pol.at(i).output.at(1));
        pol.at(i).object_fitness.at(1) = diff_1;

        double diff_2;
        diff_2 = abs(pP->target_val_2 - pol.at(i).output.at(2));
        pol.at(i).object_fitness.at(2) = diff_2;

        double diff_3;
        diff_3 = abs(pP->target_val_3 - pol.at(i).output.at(3));
        pol.at(i).object_fitness.at(3) = diff_3;
        
    }
    
    //using linear combination
    if (pP->linear_combo == true)
    {
        Get_Linear_Combo_Fitness();
    }
    
    //This is where additional evaluation steps can occur like PaCcET
    //using PaCcET
    else if (pP->PaCcET == true)
    {
        Get_PaCcET_Fitness(pT);
    }
    
    //using first objective fitness
    else
    {
        //For now the PaCcET fitness is equal to the objective fitness
        for (int i=0; i< pP->num_pol; i++)
        {
            //pol.at(i).fitness = pol.at(i).object_fitness.at(0);
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
void EA::Write_Data_To_File(float seconds)
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
    if (pP->linear_combo == true)
    {
        File5 << "Linear combination" << endl;
    }
    else if (pP->PaCcET == true)
    {
        File5 << "PaCcET" << endl;
    }
    else
    {
        File5 << "First objective" << endl;
    }
    File5 << "Run time" << "\t" << seconds << "\t" << "seconds" << endl;
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
    clock_t t1, t2;
    t1 = clock();
    
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
    cout << endl;
    t2 = clock();
    float diff ((float)t2-(float)t1);
    //cout<<diff<<endl;
    //system ("pause");
    float seconds = diff / CLOCKS_PER_SEC;
    cout << "run time" << "\t" << seconds << endl;
    Write_Data_To_File(seconds);
}


#endif /* EA_hpp */


double run_time;

int main()
{
    srand(time(NULL));
    Parameters P;
    EA E;
    E.pP = &P;
    E.Run_Program();
    cout << "END PROGRAM" << endl;
}
