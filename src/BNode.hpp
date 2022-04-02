#include <vector>
#include <cmath>
#include <boost/dynamic_bitset.hpp>
using std::vector;
using std::pow;
using boost::dynamic_bitset;

class BNode
{
private:
    vector<dynamic_bitset<>> description;
    int strings;
    int bits;
    int height;
public:
    BNode(vector<dynamic_bitset<>> desc, int strings, int bits, int height);
    ~BNode();
    int numChildren();
    bool canFirstChild();
    BNode* firstChild();
    bool canNextSibling();
    BNode* nextSibling();
};