
#ifndef ZLKS_HPP
#define ZLKS_HPP

#include <queue>

#include "solver.hpp"

namespace pg
{

class ZLKSSolver : public Solver
{
public:
    ZLKSSolver(Oink *oink, Game *game);
    virtual ~ZLKSSolver();

    virtual void run();

    int iterations;

    uintqueue Q;

    /**
     * @brief returns the highest priority in the subgame.
     */
    int maxPr(bitset Subgame);
    
    /**
     * @brief returns the subgame of all nodes with priority pr.
     */
    bitset prSubgame(int pr);
    
    /**
     * @brief returns the attractor of the subgame for the specified player 
     */
    bitset attract(bool player, bitset Subgame);
    
    int* subgameInEdges(bitset Subgame);

    void solve(bitset Subgame);
};

}

#endif 