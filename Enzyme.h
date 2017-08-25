//
//  Enzyme.h
//
//

class Enzyme : public Graph
{
private:
    std::vector<std::string> gEnzyme;
    std::vector<std::string> ucEnzyme;
    
    std::map<int, std::list<std::string>> gList;
    std::map<int, std::list<std::string>> ucList;
    
    std::list<std::string> singleFragments;
    std::map<std::string, int> mapMid;
    std::string start, end;
    
    void createEnzymeList();
    void divideGEnzyme(std::vector<std::string> gEnzyme);
    void divideUCEnzyme(std::vector<std::string> ucEnzyme);
    
    void findStartEndVertex();
    
public:
    
    void findRNAChain(std::string g, std::string uc);
};
