//
//  Enzyme.cpp
//  by Risa Toyoshima
//

#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

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

    cout << "start: " << start << " end: " << endl;
    
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
    string::iterator it;
    
    for (unsigned int i = 0; i < gEnzyme.size(); i++) {
        for (it = gEnzyme[i].begin(); it != gEnzyme[i].end(); it++) {
            tmp += *it;
            if (*it == 'U' || *it == 'C'|| it == gEnzyme[i].end() - 1) {
                gList[i].push_back(tmp);
                tmp.clear();
            }
            
            if (it == gEnzyme[i].end() -1 && *it != 'G') {  //Find the ending vertex
                end = gEnzyme[i];
            }
        }
    }


    map<int, std::list<std::string>>::iterator iter;
    for (iter = gList.begin(); iter != gList.end(); iter++)
        cout << iter -> second << endl;

}

void Enzyme::divideUCEnzyme(vector<string> ucEnzyme) {
    string tmp;
    string::iterator it;
    
    for (unsigned int i = 0; i < ucEnzyme.size(); i++) {
        for (it = ucEnzyme[i].begin(); it != ucEnzyme[i].end(); it++) {
            tmp += *it;
            if (*it == 'G' || it == ucEnzyme[i].end() -1) {
                ucList[i].push_back(tmp);
                tmp.clear();
            }
            
            if (it == ucEnzyme[i].end() -1 && *it != 'U') {  //Find the end vertex
                if (*it != 'C') {
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
    
    
    // remove from the list singleFragments if the string found in midMap
    for (it = singleFragments.begin(); it != singleFragments.end(); it++) {
        if (mapMid.count(it -> c_str()) && mapMid[it -> c_str()] > 0) {
            singleFragments.erase(it);
            mapMid[it -> c_str()]--;
        }
    }
    
    
    
    
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
