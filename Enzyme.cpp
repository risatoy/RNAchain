//
//  Enzyme.cpp
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

void Enzyme::findRNAChain(string g, string uc) {
    
    string tempInput;
    
    // Put G-Enzyme fragments in vector 'gEnzyme'
    for (unsigned int i = 0; i < g.size(); i++) {
        tempInput += g[i];
        if (g[i + 1] == ',' || i + 1 == g.size()) {
            gEnzyme.push_back(tempInput);
            i++;
            tempInput.clear();
        }
    }
    
    // Put U.C-Enzyme fragments in vector 'ucEnzyme'
    for (unsigned int i = 0; i < uc.size(); i++) {
        tempInput += uc[i];
        if (uc[i + 1] == ',' || i + 1 == uc.size()) {
            ucEnzyme.push_back(tempInput);
            i++;
            tempInput.clear();
        }
    }
    
    createEnzymeList();
    findStartEndVertex();
    
    printAllPaths(start, end);
    
}

/*
 Create vector of lists
 
 [gList]                        [ucList]
 AU| -> C -> G                  G | -> G -> AC
 AU| -> G                       U |
 G |                            AU|
 C | -> U                       G | -> AU
 AC| -> U -> AU -> AC -> G      C |
                                U |
                                AC|
                                G | -> C
                                AU|
*/

void Enzyme::createEnzymeList() {
    divideGEnzyme(gEnzyme);    // fragments of vector 'gEnzyme' into vector of list deviding by U or C
    divideUCEnzyme(ucEnzyme);  // fragments of vector 'ucEnzyme' into vector of list deviding by G
}

void Enzyme::divideGEnzyme(vector<string> gEnzyme) {
    string tmp;

    for (unsigned int i = 0; i < gEnzyme.size(); i++) {
        for (unsigned int j = 0; j < gEnzyme[i].size(); j++) {
            tmp += gEnzyme[i][j];
            if (gEnzyme[i][j] == 'U' || gEnzyme[i][j] == 'C'|| j == gEnzyme[i].size()-1) {
                gList[i].push_back(tmp);
                tmp.clear();
            }
            
            if (j == gEnzyme[i].size()-1 && gEnzyme[i][j] != 'G') { //Find the ending vertex
              end = gEnzyme[i];
            }
        }
    }
}

void Enzyme::divideUCEnzyme(vector<string> ucEnzyme) {
    string tmp;

    for (unsigned int i = 0; i < ucEnzyme.size(); i++) {
        for (unsigned int j = 0; j < ucEnzyme[i].size(); j++) {
            tmp += ucEnzyme[i][j];
            if (ucEnzyme[i][j] == 'G' || j == ucEnzyme[i].size()-1) {
                ucList[i].push_back(tmp);
                tmp.clear();
            }
            
            if (j == ucEnzyme[i].size()-1 && ucEnzyme[i][j] != 'U') { //Find the ending vertex
                if (ucEnzyme[i][j] != 'C') {
                    end = ucEnzyme[i];
                }
            }
        }
    }
}


/*
 Put all sigle fragments in list 'sigleFragments'
 G -> U -> AU -> C -> U -> AC -> AU
 
 Remove all interior extended bases (the ones in the middle)
 U -> AU
 
 The remaining ones are either start or end vertex!
 */

/*
 Alongside, create verties and add relations by addEdge()
 
 since single fragments are all repeted ones, create vertices and their relations with all non-single fragments
 */

void Enzyme::findStartEndVertex() {
    list<string>::iterator it;
    string temp;
    
    // find all the single fragments in gList and ucList and put them in list singleFragments
    for (unsigned int i = 0; i < gList.size(); i++) {
        if (gList[i].size() == 1) {  // single fragments
            it = gList[i].begin();
            singleFragments.push_back(*it);
        }
        else {   // Create edges   non-single fragments add to addEdge
            it = gList[i].begin();
            string tempBegin = *it;
            
            it = prev(gList[i].end(), 1);
            string tempEnd = *it;
            
            if (gList[i].size() == 2) {
                addEdge(tempBegin, tempEnd, "");
            }
            else {
                for (it = next(gList[i].begin(), 1); it != prev(gList[i].end(), 1); it++) {
                    temp += *it;
                }
                addEdge(tempBegin, tempEnd, temp);
                temp.clear();
            }
        }
    }
    
    for (unsigned int j = 0; j < ucList.size(); j++) {
        if (ucList[j].size() == 1) {
            it = ucList[j].begin();
            singleFragments.push_back(*it);
        }
        else {   // Create edges   non-single fragments add to addEdge
            it = ucList[j].begin();
            string tempBegin = *it;
            
            it = prev(ucList[j].end(), 1);
            string tempEnd = *it;
            
            if (ucList[j].size() == 2) {
                addEdge(tempBegin, tempEnd, "");
            }
            else {
                for (it = next(ucList[j].begin(), 1); it != prev(ucList[j].end(), 1); it++) {
                    temp += *it;
                }
                addEdge(tempBegin, tempEnd, temp);
                temp.clear();
            }
        }
    }
    
    // find the ending and the beginning vertex by removing the interior extended bases from list singleFragments 
    
    //create midMap that hold all the interior extended bases, and count the numbers of the string that appeared
    for (unsigned int i = 0, j = 0; i < gList.size() || j < ucList.size(); i++, j++) {
        if (gList[i].size() > 2) {
            for (it = next(gList[i].begin(), 1); it != prev(gList[i].end(), 1); it++) {
                if (!mapMid.count(it -> c_str()))    //if the string is found in map mapMid, set int to 1
                    mapMid.emplace(it -> c_str(), 1);
                else   //if the string is already in mapMid, incrase the number by 1
                    mapMid[it -> c_str()]++;
            }
        }
        if(ucList[j].size() > 2) {
            for (it = next(ucList[i].begin(), 1); it != prev(ucList[i].end(), 1); it++) {
                if (!mapMid.count(it -> c_str()))    //if the string is found in map mapMid, set int to 1
                    mapMid.emplace(it -> c_str(), 1);
                else   //if the string is already in mapMid, incrase the number by 1
                    mapMid[it -> c_str()]++;
            }
        }
    }

    /*** print out mapMid ***/
    map<string, int>::iterator itmapMid;
    for (itmapMid = mapMid.begin(); itmapMid != mapMid.end(); itmapMid++) {
        cout << itmapMid->first << "| " << itmapMid->second;
    }
    cout << endl;
    
    
    // remove from the list singleFragments if the string found in midMap
    for (it = singleFragments.begin(); it != singleFragments.end(); it++) {
        if (mapMid.count(it -> c_str()) && mapMid[it -> c_str()] > 0) {
            singleFragments.erase(it);
            //cout << "just erased " << it -> c_str() << "from singleFragments" << endl;
            mapMid[it -> c_str()]--;
        }
    }
    
    /*** print out mapMid ***/
    map<string, int>::iterator itmapMid;
    for (itmapMid = mapMid.begin(); itmapMid != mapMid.end(); itmapMid++) {
        cout << itmapMid->first << "| " << itmapMid->second << endl;
    }
    cout << endl;


    /*** print out singleFragments ***/
    for (it = singleFragments.begin(); it != singleFragments.end(); it++) {
        cout << it->data() << " -> ";
    }
    cout << "null" << endl;
    
    
    
    while (!singleFragments.empty()) {    //find the start node
        string temp = singleFragments.front();
        
        if(end.find(temp) == string::npos) {
            start = temp;
        }
        else
            end = temp;
        
        singleFragments.pop_front();
    }
}
