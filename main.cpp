//
//  main.cpp
//  SAN_EA_project
//
//  Created by Scott S Forer on 8/21/17.
//  Copyright © 2017 SAN_EA_project. All rights reserved.
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



#include "Parameters.hpp"
#include "Policy.hpp"
#include "Individual.hpp"
#include "Simulator.hpp"
#include "EA.hpp"



int main()
{
    Parameters P;
    EA E;
    E.pP = &P;
    cout << "END" << endl;
}
