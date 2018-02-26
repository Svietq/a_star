#ifndef CELL_H
#define CELL_H

#include <memory>
#include <map>

class Cell
{
public:
    Cell() = default;
    Cell(const Cell&) = default;
    Cell(Cell&&) = default;
    Cell& operator=(const Cell&) = default;
    Cell& operator=(Cell&&) = default;

    struct Coords { std::size_t x; std::size_t y;} coords;
    unsigned int G = 0;
    unsigned int H = 31;
    unsigned int F = 31;
    using CellPtr = std::shared_ptr<Cell>;
    CellPtr parent = std::make_shared<Cell>(*this);

    enum class Type {UNVISITED, OPENED, CLOSED, START, END, WALL, TRACK};
    void set_type(const Type& itype);
    const Type& get_type() const;
    void set_track();
    void clear();

private:
    Type type = Type::UNVISITED;

};

#endif // CELL_H
