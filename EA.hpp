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
    friend class Individual;
    friend class Policy;
    
protected:
    
    
public:
    Parameters* pP;
    
    vector<Individual> agent;

    
private:
};




#endif /* EA_hpp */
