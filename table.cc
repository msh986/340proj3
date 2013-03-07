#include "table.h"
#include "context.h"
#include "error.h"

#if defined(GENERIC)
ostream & Table::Print(ostream &os) const
{
  // WRITE THIS
  os << "Table()";
  return os;
}
#endif

#if defined(LINKSTATE)
ostream & Print(ostream &os) const
{
    
}
unsigned GetNext(const unsigned goal)
{
    
}
bool LinkUpdate(const unsigned src, const unsigned dest, double l, double ts)
{
    
}
Table *get_routing_table() const
{
    return this;
}
#endif

#if defined(DISTANCEVECTOR)
Table::Table()
{ throw GeneralException(); }

Table::Table(const int myID) :
{
    id = myID;
    //starts out with no links, don't worry about initializing table
    //see Topology.cc (AddLink and ChangeLink).
    // node created, then links added one by one.
}

ostream & Print(ostream &os) const
{
    
}
unsigned Table::GetNext(unsigned end)
{
    //nextHop should be complete
    return nextHop[end];
}
map<unsigned,double> Table::GetRow()
{
    //return distance vector for this node
    // to be passed to messages
    return dist[id];
}
void Table::RowUpdate(const unsigned src, const map<unsigned,double> toUpdate)
{
    //given distance vector from other node
    //add to table
    //update nextHop
    dist[src] = toUpdate;
    this.SelfUpdate();
}

void Table::ChangeLink(const Link *l)
{
    dist[id][l.dest] = l.lat;
    this.SelfUpdate();
}
void Table::SelfUpdate()
{
    //recalculate nextHop
    //probably a better way to do this.
}

Table *Table::get_routing_table() const
{
    return this;
}
#endif
