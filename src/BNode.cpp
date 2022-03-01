#include <vector>
#include <cmath>
#include <boost/dynamic_bitset.hpp>
using std::vector;
using std::pow;
using boost::dynamic_bitset;

// * Note - the bitsets are arranged opposite to in the paper -
// * i.e. push_back() adds a MOST significant digit to the string
// * So the successor of 00 (in 3 digits) is 100 (or 4 in decimal)
// TODO smart pointers

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
    BNode* firstChild();
    BNode* nextSibling();
};

BNode::BNode(vector<dynamic_bitset<>> description, int strings, int bits, int height)
{
    this->description = description;
    this->strings = strings;
    this->bits = bits;
    this->height = height;
}

BNode::~BNode()
{
    // ?
}

/**
 * @brief Calculates the number of children directly 
 * 
 * Uses sum i=1 to n (2^i) = 2(2^n -1)
 * 
 */
int BNode::numChildren(){
    if (this->strings <= 0)
    {
        return 0;
    } else
    {
        return (2*(pow(2, this->bits) - 1));
    }
    
    
}

BNode* BNode::firstChild()
{
    vector<dynamic_bitset<>> newDesc = this->description;
    if (newDesc.size() >= this->height)
    {
        return nullptr;
    }
    else if (strings <= 0)
    {
        auto childLabel = dynamic_bitset<>(); // child must be an empty
        newDesc.push_back(childLabel);
        BNode* newNode = new BNode(newDesc, strings, 0, this->height);
        return newNode;
    }
    else
    {
        auto childLabel = dynamic_bitset<>(this->bits, 0); // all 0 bits will be the furthest left
        newDesc.push_back(childLabel);
        BNode* newNode = new BNode(newDesc, strings - 1, 0, this->height);
        return newNode;
    }
    
}

BNode* BNode::nextSibling()
{
    // ? may have to deepcopy
    vector<dynamic_bitset<>> newDesc = this->description;
    int newStrings = this->strings;
    int newBits = this->bits;
    
    auto label = newDesc.back();
    if (newBits == 0) // No extra bits can be used
    {
        if (label.all()) // All bits are 1 => this is the final sibling
        {
            return nullptr;
        }
        else if (!label[label.size()-1]) // If the last bit is 0,
        {
            label.pop_back();// Remove the trailing bit
            newBits++;
        }
        else // The last bit must be a 1
        {
            // remove trailing 1s and one more digit
            do
            {
                label.pop_back();
                newBits++;
            } while (label[label.size()-1]);
            
        }
        
    }
    else // There is 'room' for more bits in the string,
    {
        // so to get to the next sibling, add 1 and as many more 0s
        label.push_back(1);
        newBits--;
        while (newBits>0)
        {
            label.push_back(0);
            newBits--;
        }
        
    }
    
    if (label.empty()) 
    {
        newStrings++; 
        newBits--; // A leading bit has been removed, so should not have been added back
    }
    
    newDesc.pop_back();
    newDesc.push_back(label);

    BNode* newNode = new BNode(newDesc, newStrings, newBits, this->height);
    return newNode;
}

int main(int argc, char const *argv[])
{
    // Testing code
    vector<dynamic_bitset<>> desc = vector<dynamic_bitset<>>();
    dynamic_bitset<> A (5, 22);
    dynamic_bitset<> B (5, 7);
    dynamic_bitset<> C (2, 0);
    desc.push_back(A);
    desc.push_back(B);
    desc.push_back(C);
    BNode node = BNode(desc, 2, 1, 4);
    
    BNode newNode = *node.nextSibling();
    newNode = *newNode.nextSibling();
    newNode = *newNode.nextSibling();
    newNode = *newNode.firstChild();
    // newNode = *newNode.firstChild(); throws exception, as another child would exceed height


    return 0;
}
