//
//  Simulator.hpp
//  SAN_EA_project
//
//  Created by Scott S Forer on 8/21/17.
//  Copyright © 2017 SAN_EA_project. All rights reserved.
//

#ifndef Simulator_hpp
#define Simulator_hpp

#include <stdio.h>

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
    double sum;
    sum = (pPo->x_val.at(0)*pPo->x_val.at(0)) + cos(pPo->x_val.at(1)) + sin(pPo->x_val.at(2)) + exp(pPo->x_val.at(3));
    pPo->output = sum;
}


#endif /* Simulator_hpp */
