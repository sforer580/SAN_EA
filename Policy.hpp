//
//  Policy.hpp
//  SAN_EA_project
//
//  Created by Scott S Forer on 8/21/17.
//  Copyright © 2017 SAN_EA_project. All rights reserved.
//

#ifndef Policy_hpp
#define Policy_hpp

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
