#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

# include "cellularautomata.h"
#include <vector>

#include <QObject>
#include <QDebug>

//assuming grid dimensions will be same i.e. 2x2,5x5,10x10...
class GameOfLife : public QObject, public CellularAutomata
{
    Q_OBJECT
signals:
public:

    //using cellular_automata::cellular_automata;
    GameOfLife(QObject *ptr = nullptr) : QObject(ptr)
    {
        qDebug() << "constructing game of life";
    }

    ~GameOfLife() { qDebug() << "destructing game of life"; }

    Q_INVOKABLE void recalculate()
    {
        qDebug() << "calculating grid";
        const auto [kRowCount, kColumnCount] = G.dimension();
        auto alive_count = 0;
        for (auto i = size_t{ 0 }; i < kRowCount ;++i)
        {
            for (auto j = size_t{ 0 }; j < kColumnCount; ++j)
            {
                alive_count = 0;
                for(const auto &[x, y] : dirs)
                {
                    const auto kRowIndex    = ((x + kRowCount) + i) % kRowCount;
                    const auto kColumnIndex = ((y + kColumnCount) + j) % kColumnCount;
                    alive_count += get_state(G(kRowIndex, kColumnIndex));
                }
                apply_rules(G(i,j), alive_count);
            }
        }

        for (auto i = size_t{ 0 }; i < kRowCount ;++i)
        {
            for (auto j = size_t{ 0 }; j < kColumnCount ;++j)
            {
                auto& state = G(i,j);
                G(i,j) = (1 == state) || (2 == state);
            }
        }
    }

    Q_INVOKABLE bool state_of(int i)
    {
        return G(i);
    }

    Q_INVOKABLE void set_state_of_cell(int idx, bool b)
    {
        G(idx) = b;
    }
    Q_INVOKABLE void set_grid_dimension(int row,int kColumnCount)
    {
        qDebug() << "setting grid dimention" << row << kColumnCount;
        G = Grid(row, kColumnCount, 0);
    }
    Q_INVOKABLE void randomize_cells()
    {
        qDebug() << "randomizing cells";
        randomized_states();
    }
    Q_INVOKABLE void setAll()
    {
        qDebug() << "setting all. Calling cellular_automata::setAll()";
        CellularAutomata::setAll();
    }
    Q_INVOKABLE void clearAll()
    {
        qDebug() << "setting all. Calling cellular_automata::reset()";
        CellularAutomata::reset();
    }
private:
    int get_state(const int &c)//returns 0 or 1
    {
        return (1 == c) || (-1 == c);
    }
    void apply_rules(int &cell,int alive_count)
    {
        if (1 == cell && alive_count < 2) { cell = -1 ; }
        if (1 == cell && (alive_count == 2 || alive_count == 3)) { cell = 1; }
        if (1 == cell && alive_count > 3) { cell = -1; }
        if (0 == cell && alive_count == 3) { cell = 2; }
    }
    const std::vector<pair<int,int>> dirs{
        {-1,-1},{-1,0} ,{-1,1},
        {0 ,-1}       ,{ 0,1},
        {1 ,-1},{1, 0} ,{ 1,1}
    };
};

#endif
