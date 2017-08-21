//
//  Individual.hpp
//  SAN_EA_project
//
//  Created by Scott S Forer on 8/21/17.
//  Copyright © 2017 SAN_EA_project. All rights reserved.
//

#ifndef Individual_hpp
#define Individual_hpp

#include <stdio.h>

using namespace std;


class Individual
{
    friend class EA;
    friend class Parameters;
    friend class Simulator;
    friend class Policy;
    
protected:
    
    
public:
    vector<Policy> pol;
    
    
private:
};

#endif /* Individual_hpp */
