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

Table::Table()
{ throw GeneralException(); }
Table::Table(const unsigned myID)
{
    id = myID;
    dist[myID][myID] = 0;
    nextHop[myID] = myID;
    //starts out with no links, don't worry about initializing table
    //see Topology.cc (AddLink and ChangeLink).
    // node created, then links added one by one.
}
void Table::dijkstra(){
    unsigned nodeID;
    neighbordata tempNeighbor;
    neighbordata tempNodeInQueue;
    double cost;
    map<unsigned, vector<nieghbordata>>::iterator outside = nieghborhoods.begin();
    while (outside!=neighborhoods.end()) {
    nodeID=outside->first;
    if(nodeID!=id){
    costTable[nodeID]=INT_MAX;
    visited[nodeID]=false;
    predecessor[nodeID]=nodeID;
    nodeQueue.pushBack(neighbordata(nodeID,INT_MAX))
    }
    }
    nodeID=id;
    costTable[nodeID]=0;
    visited[nodeID]=false;
    nodeQueue.pushBack(neighbordata(nodeID,0));
    make_heap(nodeQueue.begin(),nodeQueue.end(),greater_neighbor_length);
    cost=0;
    while(!nodeQueue.empty()){
        tempNodeInQueue=nodeQueue.front();
        pop_heap(nodeQueue.begin(),nodeQueue.end()); 
        nodeQueue.pop_back();
        nodeID=tempNodeInQueue.idVal;
        visited[nodeID]=true;
        costTable[nodeID]=tempNodeInQueue.length;
        cost=costTable[nodeID];
    for(x=0;x<neighborhoods[nodeID].size())
    {   tempNeighbor=neighborhoods[nodeID][x];
        if(!visited[tempNeighbor.idVal])
        {
        if(costTable[tempNeighbor.idVal>(cost+tempNeighbor.length))
        {   for(x=0;x<nodeQueue.size();x++)
            {
                if(nodeQueue[x].idVal==tempNeighbor.idVal)
                {
                    nodeQueue[x].length=cost+tempNeighbor.length;
                }
            }
            make_heap(nodeQueue.begin(),nodeQueue.end(),greater_neighbor_length);
            costTable[tempNeighbor.idVal]=cost+tempNeighbor.length;
            predecessor[tempNeighbor.idVal]=nodeID;
        }
    }
}
}
}
Table::Table(const Table &rhs)
{
    //copy constructor
    costTable=rhs.CostTable;
    nodeQueue=rhs.nodeQueue;
    neighborhoods=rhs.neighborhoods;
    nextHop = rhs.nextHop;
    id = rhs.id;
}

ostream & Print(ostream &os) const
{
    
}
unsigned Table::GetNext(unsigned end)
{
    //nextHop should be complete
    return nextHop[end];
}
bool LinkUpdate(const unsigned src, const unsigned dest, double l, double ts)
{
    
}
vector<neighbordata> Table::GetRow()
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
    dist[myID][myID] = 0;
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
    Table *table2 = new Table(*this);
    os<<"Dist Table for node " << id << ": ";
    map<unsigned,double>::iterator inside;
    map<unsigned, map<unsigned,  double> >::iterator outside = table2->dist.begin();
    while (outside!=table2->dist.end()) {
        os << "Row for " << outside->first << ": ";
        inside = outside->second.begin();
        while(inside!=outside->second.end())
        {
            os<< "(" << inside->first << "," << inside->second << ") ";
            inside++;
        }
        outside++;
        os << endl;
    }
    map<unsigned,unsigned>::iterator nextIter = table2->nextHop.begin();
    os <<"NextHop Table: ";
    while(nextIter != table2->nextHop.end())
    {
        os<< "(" << nextIter->first << "," << nextIter->second << ") ";
        nextIter++;
    }
    os<<endl;
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
    map<unsigned,double> copy = toUpdate;
    map<unsigned,double>::iterator iter = copy.begin();
    while(iter!=copy.end())
    {
        dist[src][iter->first]=iter->second;
        iter++;
    }
    iter = dist[src].begin();
    //check for new nodes
    while(iter!=dist[src].end())
    {
        //if a node is not present in this node's links
        // and also not in the nextHop table
        if(dist[id].count(iter->first)==0&&nextHop.count(iter->first)==0)
        //if(nextHop.count(iter->first)==0)
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
            if(tempNextDist <= nextDist)
            {
                nextDist = tempNextDist;
                if(inside->first != id)
                {
                    tempNextHop = inside->first;
                }else{
                    tempNextHop = outside->first;
                }
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
