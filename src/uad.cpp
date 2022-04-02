#include "uad.hpp"

namespace pg
{

    UADSolver::UADSolver(Oink *oink, Game *game) : Solver(oink, game)
    {
    }

    UADSolver::~UADSolver()
    {
        // TODO
    }

    int UADSolver::maxPr(bitset subgame)
    {
        int max = -1;
        for (int v = 0; v < nodecount(); v++)
        {
            if (subgame[v] && priority(v) > max)
            {
                max = priority(v);
            }
        }

        return max;
    }

    bitset UADSolver::prSubgame(int pr, bitset supergame)
    {
        bitset subgame(nodecount());
        for (int v = 0; v < nodecount(); v++)
        {
            if (supergame[v] && priority(v) == pr)
            {
                subgame.set(v);
            }
        }

        return subgame;
    }

    bitset UADSolver::attract(bool player, bitset subgame)
    {
        // Based on the inductive definition in Zielonka (1998)
        bitset attr(subgame);
        bitset compAttr(nodecount());

        while (attr != compAttr) // Repeat until the attractor no longer changes
        {
            compAttr = bitset(attr);
            for (int v = 0; v < nodecount(); v++)
            {
                if (attr[v]) // If v is currently in the attractor
                {
                    for (auto *in = ins(v); *in != -1; in++) // Consider each in edge to v
                    {
                        if (owner(*in) == player) // The player can force from 'in' to 'v'
                        {
                            str[*in] = v;
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
        return attr;
    }

    bitset UADSolver::solveEven(bitset Subgame, int d, BNode evenTree, BNode oddTree){
        // int k = oddTree.numChildren();
        // Bitsets corresponding to the subgames in the algorithm from JM2020
        // Since we no longer need X_i after we have X_i+1, we store in 1 variable each
        bitset G = Subgame;
        bitset D;
        bitset U;
        // Gp = G' in the paper
        bitset Gp;

        if (!oddTree.canFirstChild())
        {
            return G;
        }
        

        // Corresponds to T_i ^Odd in the paper
        BNode treeChild = *oddTree.firstChild();

        while (treeChild.canNextSibling())
        {
            D = prSubgame(d, G);
            Gp = G & ~attract(0, D);
            U = solveOdd(Gp, d-1, evenTree, treeChild);
            G = G & ~attract(1, U);
            treeChild = *treeChild.nextSibling();
        }
        D = prSubgame(d, G);
        Gp = G & ~attract(0, D);
        U = solveOdd(Gp, d-1, evenTree, treeChild);
        G = G & ~attract(1, U);
        return G;
    };

    bitset UADSolver::solveOdd(bitset Subgame, int d, BNode evenTree, BNode oddTree){
        // int l = evenTree.numChildren();
        // Bitsets corresponding to the subgames in the algorithm from JM2020
        // Since we no longer need X_i after we have X_i+1, we store in 1 variable each
        bitset G = Subgame;
        bitset D;
        bitset U;
        // Gp = G' in the paper
        bitset Gp;

        if (!evenTree.canFirstChild())
        {
            return G;
        }
        // Corresponds to T_i ^Even in the paper
        BNode treeChild = *evenTree.firstChild();

        while (treeChild.canNextSibling())
        {
            D = prSubgame(d, G);
            Gp = G & ~attract(0, D);
            U = solveEven(Gp, d-1, treeChild, oddTree);
            G = G & ~attract(1, U);
            treeChild = *treeChild.nextSibling();
        }
        D = prSubgame(d, G);
        Gp = G & ~attract(0, D);
        U = solveEven(Gp, d-1, treeChild, oddTree);
        G = G & ~attract(1, U);
        return G;
    };

    void UADSolver::run(){
        iterations = 0;

        // std::cout << "*********1" << std::endl;
        str = new int[nodecount()];

        bitset G(nodecount());
        G = disabled;
        G.flip();
        int d = maxPr(G);

        bitset W0;
        bitset W1;

        // This is quite a large number of repeats, but most examples are solved in a few (< 3 or 4)
        // iterations of the outer loop
        for (int k = 0; k <= ceil(log2(nodecount())); k++)
        {
            for (int t = 0; t <= ceil(log2(nodecount())); t++)
            {
                iterations++;
                BNode evenTree (vector<dynamic_bitset<>>(), k-1, (k-1)+t, (double) ceil(d/2.0));
                BNode oddTree (vector<dynamic_bitset<>>(), k-1, (k-1)+t, (double) ceil(d/2.0));

                // TODO test with different ways of calling even and odd
                if (d % 2 == 0)
                {
                    W0 = solveEven(G, d, evenTree, oddTree);
                    W1 = ~W0;
                } else
                {
                    W0 = solveEven(G, d+1, evenTree, oddTree);
                    W1 = ~W0;
                }

                // TODO if solved then break

            }
            
        }
        
#ifndef NDEBUG
    if (trace) {
        for (int v=0; v<nodecount(); v++) {
            if (disabled[v]) continue;
            logger << "vertex " << label_vertex(v) << " is solved by";
            if (W0[v]) {
                logger << " even";
                if (owner(v) == 0) {
                    logger << " (";
                    if (str[v] == -1) logger << "-1";
                    else logger << label_vertex(str[v]);
                    logger << ")";
                }
            }
            if (W1[v]) {
                logger << " odd";
                if (owner(v) == 1) {
                    logger << " (";
                    if (str[v] == -1) logger << "-1";
                    else logger << label_vertex(str[v]);
                    logger << ")";
                }
            }
            logger << std::endl;
        }
    }
#endif

        // std::cout << "*********2" << std::endl;
        for (int v=0; v<nodecount(); v++) {
            if (disabled[v]) continue;
            if (W0[v]) oink->solve(v, 0, str[v]);
            if (W1[v]) oink->solve(v, 1, str[v]);
        }

        logger << "solved with " << iterations << " iterations." << std::endl;

        // check if actually all solved
#ifndef NDEBUG
    for (int i=0; i<nodecount(); i++) {
        if (!disabled[i]) { logger << "search was incomplete!" << std::endl; exit(-1); }
    }
#endif

        delete[] str;
    }

} // namespace pg