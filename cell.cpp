#include "cell.h"

void Cell::set_type(const Cell::Type &itype)
{
    if(type==Type::UNVISITED || type==Type::OPENED)
        type = itype;
}

const Cell::Type &Cell::get_type() const
{
    return type;
}

void Cell::set_track()
{
    if(type != Type::END)
        type = Type::TRACK;
}

void Cell::clear()
{
    type = Type::UNVISITED;
}
