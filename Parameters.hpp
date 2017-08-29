//
//  Parameters.hpp
//  SAN_EA_project
//
//  Created by Scott S Forer on 8/21/17.
//  Copyright © 2017 SAN_EA_project. All rights reserved.
//

#ifndef Parameters_hpp
#define Parameters_hpp

#include <stdio.h>

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
    int num_object = 2;                 //number of objectives
    int gen_max = 50;                   //number of generations
    double mutation_rate = 0.5;         //mutation rate
    double mutate_range = 0.1;          //mutation range
    int num_x_val = 4;
    double x_val_min = 0;
    double x_val_max = 10;
    
    //target function
    double target_val_1 = 1*1 + cos(2)*cos(2) + sin(3) + exp(4);
    double target_val_2 = 4*4*4 + cos(3) + sin(2) + tan(1);
    vector<double> target_vals;
    
    //Multi-objective selection method
    int linear_combo = 1;               //0=off, 1=on
    
    
private:
};

#endif /* Parameters_hpp */
