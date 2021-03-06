#ifndef _messages
#define _messages

#include <iostream>

#include "node.h"
#include "link.h"
#include "table.h"

#if defined(GENERIC)
struct RoutingMessage {
 public:
  ostream & Print(ostream &os) const;
};
#endif

#if defined(LINKSTATE)
struct RoutingMessage {
  unsigned nodeID;
  unsigned origNodeID;
  vector<neighbordata> neighborhood;
  RoutingMessage();
  RoutingMessage(unsigned last,unsigned source,vector<neighbordata> &neighbors);
  RoutingMessage(const RoutingMessage &rhs);
  RoutingMessage &operator=(const RoutingMessage &rhs);

  ostream & Print(ostream &os) const;
};
#endif

#if defined(DISTANCEVECTOR)
struct RoutingMessage {

  unsigned nodeID;
  map<unsigned,double> distances;
  RoutingMessage();
  RoutingMessage(unsigned number, map<unsigned,double> &table);
  RoutingMessage(const RoutingMessage &rhs);
  RoutingMessage &operator=(const RoutingMessage &rhs);

  ostream & Print(ostream &os) const;
};
#endif


inline ostream & operator<<(ostream &os, const RoutingMessage &m) { return m.Print(os);}

#endif
