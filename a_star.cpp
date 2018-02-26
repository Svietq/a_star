#include "a_star.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>
#include "windows.h"

namespace
{
    std::map<Cell::Type, char> to_char{
        {Cell::Type::UNVISITED, ' '},
        {Cell::Type::OPENED,    'O'},
        {Cell::Type::CLOSED,    'C'},
        {Cell::Type::START,     'S'},
        {Cell::Type::END,       'E'},
        {Cell::Type::WALL,      '.'},
        {Cell::Type::TRACK,     'x'}
    };

    void gotoxy(short ix, short iy)
    {
        static HANDLE h = NULL;
          if(!h)
            h = GetStdHandle(STD_OUTPUT_HANDLE);
          COORD c = { ix, iy };
          SetConsoleCursorPosition(h,c);
    }
}

void A_star::init_cells()
{
    for(auto &row : maze)
    {
        for(auto &cell : row)
        {
            cell = std::make_shared<Cell>();
        }
    }
}

void A_star::read_board()
{
    char ch = ' ';
    int x = 0;
    int y = 0;
    std::fstream file("maze.txt", std::fstream::in);
    while (file >> std::noskipws >> ch)
    {
        if(ch == '\n')
        {
            ++y;
            x=0;
        }
        else if(ch == '.')
        {
            maze[y][x]->set_type(Cell::Type::WALL);
            ++x;
        }
        else
            ++x;
    }
}

void A_star::print_board()
{
    for(const auto &row : maze)
    {
        for(const auto &cell : row)
        {
            std::cout.width(2);
            std::cout << to_char[cell->get_type()];
        }
        std::cout << '\n';
    }
}

void A_star::print_open_list()
{
    std::cout << "Open list: ";
    for(const auto &cell : open_list)
        std:: cout << to_char[cell->get_type()] << "(" << cell->coords.x << "," << cell->coords.y << ")="
                   << "(" << cell->G << "," << cell->H << ")" << ' ';
    std::cout << '\n';
}

void A_star::print_closed_list()
{
    std::cout << "Closed list: ";
    for(const auto &cell : closed_list)
        std:: cout << to_char[cell->get_type()] << "(" << cell->coords.x << "," << cell->coords.y << ")="
              << "(" << cell->G << "," << cell->H << ")" << ' ';
    std::cout << '\n';
}

void A_star::print_curr_cell()
{
    std::cout << "Current cell: ";
    std::cout << to_char[curr_cell->get_type()] << "(" << curr_cell->coords.x << "," << curr_cell->coords.y << ")" << '\n';
}

void A_star::print_path()
{
    for(const auto &cell : path)
    {
        std::cout << to_char[cell->get_type()]  << "(" << cell->coords.x << "," << cell->coords.y << ")" << " <- ";
    }
    std::cout << '\n';
}

void A_star::set_coords()
{
    std::size_t i=0;
    std::size_t j=0;
    for(auto &row : maze)
    {
        for(auto &cell : row)
        {
            cell->coords = Cell::Coords{j,i};
            ++j;
        }
        ++i;
        j=0;
    }
}

void A_star::calc_G(Cell &cell)
{
    cell.G = cell.parent->G;
    if( (cell.coords.x == cell.parent->coords.x) || (cell.coords.y == cell.parent->coords.y) )
        cell.G += 10;
    else
        cell.G += 14;
}

void A_star::calc_H(Cell &cell)
{
    cell.H = std::abs(DIM - cell.coords.x - cell.coords.y);
}

void A_star::calc_F(Cell &cell)
{
    cell.F = cell.G + cell.H;
}

void A_star::calc_costs()
{
    for(auto &x : open_list)
    {
        calc_G(*x);
        calc_H(*x);
        calc_F(*x);
    }
}

void A_star::choose_next_cell()
{
    //find cell with the smallest F value:
    auto min_it = std::min_element(open_list.begin(), open_list.end(),
                  [](const CellPtr &c1, const CellPtr &c2)
                  {
                    return c1->F < c2->F;
                  });
    //refer to the found cell as the current cell:
    curr_cell = *min_it;
    //switch the found cell to the closed list:
    open_list.erase(min_it);
    (*min_it)->set_type(Cell::Type::CLOSED);
    closed_list.push_back(*min_it);
}

void A_star::update_open_list()
{
    std::size_t x = curr_cell->coords.x;
    std::size_t y = curr_cell->coords.y;

    for(int i=-1; i<2; ++i)
    {
        for(int j=-1; j<2; ++j)
        {
            if(!(i==0 && j==0))
            {
                if(maze[y+j][x+i]->get_type() == Cell::Type::UNVISITED ||
                   maze[y+j][x+i]->get_type() == Cell::Type::END)
                {
                    maze[y+j][x+i]->parent = curr_cell;
                    maze[y+j][x+i]->set_type(Cell::Type::OPENED);
                    open_list.push_back( maze[y+j][x+i] );
                }
            }
        }
    }
}

void A_star::make_path(CellPtr &icell)
{
    if(icell->get_type() == Cell::Type::START)
        return;
    path.push_back(icell);
    make_path(icell->parent);
}

void A_star::clear_maze()
{
    for(auto &row : maze)
    {
        for(auto &cell : row)
        {
            switch(cell->get_type())
            {
            case Cell::Type::CLOSED:
            case Cell::Type::OPENED:
                cell->clear();
                break;
            default:
                break;
            }
        }
    }
}

void A_star::set_start(CellPtr &istart)
{
    istart->set_type(Cell::Type::START);
}

void A_star::set_end(CellPtr &iend)
{
    iend->set_type(Cell::Type::END);
}

A_star::A_star()
{
    init_cells();
    set_coords();
    read_board();

    CellPtr start = maze[1][1];
    set_start(start);
    set_end(maze[DIM/2+1][DIM/2+1]);
    curr_cell = start;

    open_list.push_back(start);
}

void A_star::run_algorithm()
{
    while(true)
    {
        if(curr_cell->get_type() == Cell::Type::END) break; // destination point is found
        if(open_list.empty()) break;                        // whole maze was searched
        choose_next_cell();
        update_open_list();
        calc_costs();
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
        gotoxy(0,0);
        print_board();
        //print_curr_cell();
    }
    make_path(curr_cell);
    for(auto &cell : path)
    {
        cell->set_track();
    }
    clear_maze();
    gotoxy(0,0);
    print_board();

//    print_path();
//    print_closed_list();
//    print_open_list();

}




