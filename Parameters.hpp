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
    int num_pol = 2;
    int to_kill = num_pol/2;
    int num_object = 3;
    int gen_max = 2;
    
    
private:
};

#endif /* Parameters_hpp */
