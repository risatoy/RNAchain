//
//  main.cpp
//  by Risa Toyoshima
//

#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "Graph.h"
#include "Enzyme.h"

using namespace std;

int main(int argc, char** argv)
{
    Enzyme e;
    
    string gInput = argv[1];
    string ucInput = argv[2];
    
    /*
    gInput = "AUCG,AUG,G,CU,ACUAUACG";
    ucInput = "GGAC,U,AU,GAU,C,U,AC,GC,AU";
    */
    
    e.findRNAChain(gInput, ucInput);    // Find all possible original RNA chains
    
    return 0;
}

