#include "table.h"

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

Table::Table(const Node &myNode) :
{
    node = myNode;
    deque<*Link> outLinks = myNode.GetOutgoingLinks();
    map<unsigned, map<unsigned, double>> dist;
    map<unsigned, unsigned> nextHop;
    deque<*Link>::iterator = outLinks.begin();
    while (iterator!=outLinks.end()) {
        dist[node.number][(*iterator).dest] = lat;
        nextHop[(*iterator).dest] = (*iterator).dest;
        iterator++;
    }
}

ostream & Print(ostream &os) const
{
    
}
unsigned GetNext(unsigned end)
{
    return nextHop[end];
}
map<unsigned,double> GetRow()
{
    return dist[node.number];
}
bool RowUpdate(const unsigned src, const map<unsigned,double> toUpdate)
{
    dist[src] = toUpdate;
    this.selfUpdate();
}
bool SelfUpdate(const Link *l)
{
    dist[node.number][l.dest] = l.lat;
    this.selfUpdate;
}

Table *get_routing_table() const
{
    return this;
}
#endif
