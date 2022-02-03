#include "zlks.hpp"

namespace pg
{

    ZLKSSolver::ZLKSSolver(Oink *oink, Game *game) : Solver(oink, game)
    {
    }

    ZLKSSolver::~ZLKSSolver()
    {
        // TODO
    }

    int ZLKSSolver::maxPr(bitset subgame)
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

    bitset ZLKSSolver::prSubgame(int pr, bitset supergame)
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

    bitset ZLKSSolver::attract(bool player, bitset subgame)
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

    pair<bitset, bitset> ZLKSSolver::solve(bitset game)
    {
        iterations++;
        bitset W0 (nodecount());
        bitset W1 (nodecount());
        pair<bitset, bitset> WPair (W0, W1);

        // std::cout << "*******" << game.count() << std::endl;
        if (game.count()==0)
        {
            return WPair;
        }

        int n = maxPr(game);
        int player = n % 2;

        bitset maxAttr = attract(player, prSubgame(n, game));
        WPair = solve(game & ~maxAttr);
        W0 = WPair.first;
        W1 = WPair.second;

        bitset* WP = (player) ? &W1 : &W0;
        bitset* WPc = (player) ? &W0 : &W1;
        bitset WpPc = attract(1-player, *WPc);

        if (WpPc == *WPc)
        {
            *WP = *WP | maxAttr;
        } else
        {
            WPair = solve(game & ~WpPc);
            W0 = WPair.first;
            W1 = WPair.second;
            WP = (player) ? &W1 : &W0;
            WPc = (player) ? &W0 : &W1;
            *WPc = *WPc | WpPc;
        }
        WPair = pair<bitset, bitset>(W0, W1);

        // std::cout << "****0*" << W0.count() << std::endl;
        // std::cout << "****1*" << W1.count() << std::endl;
        // std::cout << "****p*" << (*WP).count() << std::endl;
        // std::cout << "****c*" << (*WPc).count() << std::endl;
        // std::cout << "****c*" << (*WPc).count() << std::endl;

        return WPair;
        
    }

    void ZLKSSolver::run(){
        iterations = 0;

        // std::cout << "*********1" << std::endl;
        str = new int[nodecount()];

        bitset G(nodecount());
        G = disabled;
        G.flip();

        pair<bitset, bitset> WPair = solve(G);
        bitset W0 = WPair.first;
        bitset W1 = WPair.second;

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