#ifndef A_STAR_H
#define A_STAR_H

#include "cell.h"
#include <array>
#include <vector>

class A_star
{
    constexpr static std::size_t DIM = 33;
    using CellPtr = std::shared_ptr<Cell>;
    using Maze = std::array< std::array<CellPtr, DIM>, DIM >;
    Maze maze;

    void init_cells();
    void read_board();
    void set_coords();
    void print_board();
    void print_open_list();
    void print_closed_list();
    void print_curr_cell();
    void print_path();
    void calc_G(Cell &cell);
    void calc_H(Cell &cell);
    void calc_F(Cell &cell);
    void calc_costs();
    void choose_next_cell();
    void update_open_list();
    void make_path(CellPtr &icell);
    void clear_maze();

    std::vector<CellPtr> open_list;
    std::vector<CellPtr> closed_list;
    std::vector<CellPtr> path;
    CellPtr curr_cell;
    void set_start(CellPtr &istart);
    void set_end(CellPtr &iend);

public:
    A_star();
    void run_algorithm();

};

#endif // A_STAR_H
