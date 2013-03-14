#include "messages.h"


#if defined(GENERIC)
ostream &RoutingMessage::Print(ostream &os) const
{
  os << "RoutingMessage()";
  return os;
}
#endif


#if defined(LINKSTATE)

ostream &RoutingMessage::Print(ostream &os) const
{
  return os;
}

RoutingMessage::RoutingMessage()
{}


RoutingMessage::RoutingMessage(const RoutingMessage &rhs)
{neighborhood=rhs.neighborhood;
nodeID=rhs.nodeID;
origNodeID=rhs.nodeID;
}
RoutingMessage::RoutingMessage(unsigned last,unsigned source,vector<neighbordata> &neighbors)
{
	nodeID=last;
	origNodeID=source;
	neighborhood=neighbors;
}

#endif


#if defined(DISTANCEVECTOR)

ostream &RoutingMessage::Print(ostream &os) const
{
  return os;
}

RoutingMessage::RoutingMessage()
{}

RoutingMessage::RoutingMessage(unsigned number, map<unsigned,double> &table)
{
  distances = table;
  nodeID = number;
}


RoutingMessage::RoutingMessage(const RoutingMessage &rhs)
{}

#endif

