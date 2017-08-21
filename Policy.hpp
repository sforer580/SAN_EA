//
//  Policy.hpp
//  SAN_EA_project
//
//  Created by Scott S Forer on 8/21/17.
//  Copyright © 2017 SAN_EA_project. All rights reserved.
//

#ifndef Policy_hpp
#define Policy_hpp

#include <stdio.h>

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
    int age;
    
    
private:
};

#endif /* Policy_hpp */
