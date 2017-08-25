//
//  Graph.h
//
//

class Graph
{
    int pathMax = 0;
    std::map<std::string, std::list<std::pair<std::string, std::string>>> adj;
    
    void printAllPathsUtil(std::string , std::string ,std::string , std::map<std::string, int>, std::map<int, std::string>, int &);
    
public:
    void addEdge(std::string u, std::string v, std::string e);
    void printAllPaths(std::string s, std::string d);
};
