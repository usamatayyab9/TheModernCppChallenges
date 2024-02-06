/**
 * @file cellularautomata.h
 * @author Usama Tayyab (usamatayyab9@gmail.com)
 * @brief 
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef CELLULARAUTOMATA_H
#define CELLULARAUTOMATA_H

#include <ctime>
#include "grid.h"

#define ON  true
#define OFF false
#define ALIVE  true
#define DEAD false

class CellularAutomata
{
    public:
        CellularAutomata() = default;
        CellularAutomata(size_t r, size_t c) : G(r, c)
        {
        }

        void setAll()
        {
            G.setAll(ALIVE);
        }

        void reset()
        {
            G.setAll(DEAD);
        }

        void set(size_t i, size_t j, int val=ALIVE)
        {
            G(i,j) = (ALIVE == val);
        }

        void randomized_states()
        {
            srand(time(nullptr));
            const auto [kRowCount, kColumnCount] = G.dimension();
            for (auto i = size_t{ 0 }; i < kRowCount ;++i)
            {
                for (auto j = size_t{ 0 }; j < kColumnCount ;++j)
                {
                    G.at(i,j) = rand() % 2;
                }
            }
        }

        friend void print(const CellularAutomata &CA)
        {
            print(CA.G);
        }
        virtual void recalculate()=0;
    protected:
        Grid G;
};

#endif
