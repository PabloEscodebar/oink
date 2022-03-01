
#ifndef UAD_HPP
#define UAD_HPP

#include <utility>
#include <cmath>

#include "oink.hpp"
#include "solver.hpp"
#include "BNode.cpp"

using std::log2;
using std::ceil;

namespace pg
{

class UADSolver : public Solver
{
public:
    UADSolver(Oink *oink, Game *game);
    virtual ~UADSolver();

    virtual void run();

    /**
     * @brief returns the highest priority in the subgame.
     */
    int maxPr(bitset Subgame);
    
    /**
     * @brief returns the subgame of all nodes with priority pr.
     */
    bitset prSubgame(int pr, bitset supergame);
    
    /**
     * @brief returns the attractor of the subgame for the specified player 
     */
    bitset attract(bool player, bitset Subgame);
    
    bitset solveEven(bitset Subgame, int d, BNode evenTree, BNode oddTree);
    bitset solveOdd(bitset Subgame, int d, BNode evenTree, BNode oddTree);

protected:
    int iterations;
    int *str;
    // bitset W0, W1; // current approximation of winning areas
};

}

#endif 