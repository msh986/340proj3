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
Table::Table(const unsigned myID){}
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

Table::Table(const unsigned myID)
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
    map<unsigned,double>::iterator iter = dist[src].begin();
    //check for new nodes
    while(iter!=dist[src].end())
    {
        //if a node is not present in this node's links
        // and also not in the nextHop table
        if(dist[id].count(iter->first)==0&&nextHop.count(iter->first)==0)
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
    //node that will be stored as nextHop for current
    unsigned tempNextHop;
    //distance to test
    double tempNextDist;
    //shortest distance found so far
    double nextDist;
    //iterate through nextHop
    //compare dist from this node to distance through intermediate node
    map<unsigned,unsigned>::iterator outside = nextHop.begin();
    map<unsigned, map<unsigned,  double> >::iterator inside;
    while (outside!=nextHop.end()) {
        //store current shortest path
        nextDist = dist[id][outside->second] + dist[outside->second][outside->first];
        tempNextHop = outside->second;
        //for all neighbors
        inside = dist.begin();
        while(inside!=dist.end())
        {
        if (inside->second.count(outside->first)==1) {
            //if there is a distance for this link (current -> intermediate + intermediate->endpoint
            tempNextDist = dist[id][inside->first] + dist[inside->first][outside->first];
            // if it's shorter, keep track of it and which node it was
            if(tempNextDist < nextDist)
            {
                nextDist = tempNextDist;
                tempNextHop = inside->first;
            }
        }
        inside++;
    	}
        //set nextHop to the shortest path
        outside->second = tempNextHop;
        outside++;
    }
}

#endif
