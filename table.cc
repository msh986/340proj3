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
    map<unsigned,double> result;
    map<unsigned,unsigned>::iterator iter = nextHop.begin();
    while(iter!=nextHop.end())
    {
        result[iter->first] = dist[id][iter->second]+dist[iter->second][iter->first];
        iter++;
    }
    return result;
}
void Table::RowUpdate(const unsigned src, const map<unsigned,double> toUpdate)
{
    //given distance vector from other node
    //add to table
    //update nextHop
    dist[src] = toUpdate;
    map<unsigned,double>::iterator iter = toUpdate.begin();
    //check for new nodes
    while(iter!=toUpdate.end())
    {
        //if it's not present in this node's links
        // and not in the nextHop table
        if(dist[id].count(iter->first)==0&&nextHop.count()==0)
        {
            //add to nextHop table
            nextHop[iter->first] = src;
        }
        iter++;
    }
    SelfUpdate();
}

void Table::ChangeLink(const Link *l)
{
    dist[id][(*l).GetDest()] = (*l).GetLatency();
    //if it's to a new node add it to the nextHop table
    if (nextHop.count((*l).GetDest())==0) {
        nextHop[(*l).GetDest()] = (*l).GetDest();
    }else{
        SelfUpdate();
    }
}
void Table::SelfUpdate()
{
    //recalculate nextHop
    //probably a better way to do this.
    unsigned tempNextHop;
    double tempDist;
    double dist;
    //iterate through nextHop
    //compare dist from this node to distance through intermediate node
    map<unsigned,double>::iterator outside = nextHop.begin();
    map<unsigned, map<unsigned,  double> >::iterator inside;
    while (outside!=nextHop.end()) {
        //store current shortest path
        dist = dist[id][outside->second] + dist[outside->second][outside->first];
        tempNextHop = outside->second;
        //for all neighbors
        inside = dist.begin();
        if (inside->second.count(outside->first)==1) {
            //if there is a distance for this link (current -> intermediate + intermediate->endpoint
            tempDist = dist[id][inside->first] + dist[inside->first][inside->second[outside->first]];
            // if it's shorter, keep track of it and which node it was
            if(tempDist < dist)
            {
                dist = tempDist;
                tempNextHop = inside->first;
            }
        }
        //set nextHop to the shortest path
        outside->second = tempNextHop;
    }
}

#endif
