#include "zlks.hpp"

namespace pg
{

    ZLKSSolver::ZLKSSolver(Oink *oink, Game *game) : Solver(oink, game), Q(game->nodecount())
    {
    }

    ZLKSSolver::~ZLKSSolver()
    {
        // TODO
    }

    int ZLKSSolver::maxPr(bitset Subgame)
    {
        // TODO
    }

    bitset prSubgame(int pr)
    {
        // TODO
    }

    bitset ZLKSSolver::attract(bool player, bitset Subgame)
    {
        // Based on the inductive definition in Zielonka (1998)
        bitset attr(Subgame);
        bitset compAttr(Subgame.size());

        while (attr != compAttr) // Repeat until the attractor no longer changes
        {
            compAttr = bitset(attr);
            for (int v = 0; v < Subgame.size(); v++) 
            {
                if (attr[v]) // If v is currently in the attractor
                {
                    for (auto *in = ins(v); *in != -1; in++) // Consider each in edge
                    {
                        if (owner(*in) == player) // The player can force from 'in' to 'v'
                        {
                            attr.set(*in);
                        }
                        else // Check if the opponent has no choice but to enter the attractor 
                        {
                            bool attracts = true;
                            for (auto *out = outs(*in); *out != -1; out++)
                            {
                                // If there is an edge from 'out' that doesnt lead to the attractor
                                if (!attr[*out]) 
                                {
                                    attracts = false;
                                    break;
                                }
                            }
                            if (attracts)
                            {
                                attr.set(*in);
                            }
                        }
                    }
                }
            }
        }
    }

    void ZLKSSolver::solve(bitset Subgame)
    {
    }

} // namespace pg