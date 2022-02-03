
#ifndef ZLKS_HPP
#define ZLKS_HPP

#include <utility>

#include "oink.hpp"
#include "solver.hpp"

using std::pair, std::get;

namespace pg
{

class ZLKSSolver : public Solver
{
public:
    ZLKSSolver(Oink *oink, Game *game);
    virtual ~ZLKSSolver();

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
    
    pair<bitset, bitset> solve(bitset Subgame);

protected:
    int iterations;
    int *str;
    // bitset W0, W1; // current approximation of winning areas
};

}

#endif 