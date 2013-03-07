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

Table::Table(const int myID)
{
    id = myID;
    dist[myID][myID] = 0.0;
    nextHop[myID] = myID;
    //starts out with no links, don't worry about initializing table
    //see Topology.cc (AddLink and ChangeLink).
    // node created, then links added one by one.
}
Table::Table(const Table &rhs)
{
    //copy constructor
    dist = rhs.dist;
    nextHop = rhs.nextHop;
    id = rhs.id;
}

ostream & Table::Print(ostream &os) const
{
    return os;
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
    SelfUpdate();
}

void Table::ChangeLink(const Link *l)
{
    dist[id][(*l).GetDest()] = (*l).GetLatency();
    SelfUpdate();
}
void Table::SelfUpdate()
{
    //recalculate nextHop
    //probably a better way to do this.
    unsigned tempNext;
    double dist;
    //iterate through destinations
    //compare dist from this node to distance through intermediate node
}

#endif
