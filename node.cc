#include "node.h"
#include "context.h"
#include "error.h"


Node::Node(const unsigned n, SimulationContext *c, double b, double l) : 
    number(n), context(c), bw(b), lat(l) 
{
  table=new Table(n);
}

Node::Node() 
{ throw GeneralException(); }

Node::Node(const Node &rhs) : 
  number(rhs.number), context(rhs.context), bw(rhs.bw), lat(rhs.lat), table(rhs.table){}

Node & Node::operator=(const Node &rhs) 
{
  return *(new(this)Node(rhs));
}

void Node::SetNumber(const unsigned n) 
{ number=n;}

unsigned Node::GetNumber() const 
{ return number;}

void Node::SetLatency(const double l)
{ lat=l;}

double Node::GetLatency() const 
{ return lat;}

void Node::SetBW(const double b)
{ bw=b;}

double Node::GetBW() const 
{ return bw;}

Node::~Node()
{}

// Implement these functions  to post an event to the event queue in the event simulator
// so that the corresponding node can recieve the ROUTING_MESSAGE_ARRIVAL event at the proper time
void Node::SendToNeighbors(const RoutingMessage *m)
{
  context->SendToNeighbors(this,m);
  // deque<Node*> *neighborNodes = GetNeighbors();
  // deque<Node*>::iterator nodeIter;
  
  // for (nodeIter = neighborNodes->begin(); nodeIter != neighborNodes->end(); nodeIter++) {
  //   SendToNeighbor(*nodeIter, m);
  // }
}

void Node::SendToNeighbor(const Node *n, const RoutingMessage *m)
{
  deque<Link*> *outLinks = context->GetOutgoingLinks(this);
  deque<Link*>::iterator linkIter = outLinks->begin();
  Link tempLink;
  
  while(linkIter != outLinks->end())
  {
    if((*linkIter)->GetDest() == n->GetNumber()){
      tempLink = *(*linkIter);
      break;
    }
    linkIter++;
  }
  Event *out = new Event(context->GetTime()+tempLink.GetLatency(),
                        ROUTING_MESSAGE_ARRIVAL,
                        (void*) n,
                        (void*) m);
  context->PostEvent(out);
}

deque<Node*> *Node::GetNeighbors()
{
  return context->GetNeighbors(this);
}

deque<Link*> *Node::GetOutgoingLinks()
{
    return context->GetOutgoingLinks(this);
}

void Node::SetTimeOut(const double timefromnow)
{
  context->TimeOut(this,timefromnow);
}


bool Node::Matches(const Node &rhs) const
{
  return number==rhs.number;
}


#if defined(GENERIC)
void Node::LinkHasBeenUpdated(const Link *l)
{
  cerr << *this << " got a link update: "<<*l<<endl;
  //Do Something generic:
  SendToNeighbors(new RoutingMessage);
}


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
  cerr << *this << " got a routing messagee: "<<*m<<" Ignored "<<endl;
}


void Node::TimeOut()
{
  cerr << *this << " got a timeout: ignored"<<endl;
}

Node *Node::GetNextHop(const Node *destination) const
{
  return 0;
}

Table *Node::GetRoutingTable() const
{
  return new Table;
}


ostream & Node::Print(ostream &os) const
{
  os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw<<")";
  return os;
}

#endif

#if defined(LINKSTATE)


void Node::LinkHasBeenUpdated(const Link *l)
{ //if neighbor link, update appropriate row in adjacency matrix, update djikstra's, send out msg on all links but this.
  cerr << *this<<": Link Update: "<<*l<<endl;
  return;
}


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
  cerr << *this << " Routing Message: "<<*m;
  return;
}

void Node::TimeOut()
{
  cerr << *this << " got a timeout: ignored"<<endl;
}

Node *Node::GetNextHop(const Node *destination) const
{
  // WRITE
  return 0;
}

Table *Node::GetRoutingTable() const
{
  // WRITE
  return 0;
}


ostream & Node::Print(ostream &os) const
{
  os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw<<")";
  return os;
}
#endif


#if defined(DISTANCEVECTOR)

void Node::LinkHasBeenUpdated(const Link *l)
{
   map<unsigned, double> oldDist = table->GetRow();
  // // update our table
  table->ChangeLink(l);
   map<unsigned, double> newDist = table->GetRow();
  if(oldDist != newDist){
    SendToNeighbors(new RoutingMessage(number, newDist));
    cerr << "table has changed" << endl;
  }else{
    cerr << "table has not changed" << endl;
  }

  // send out routing mesages
  cerr << *this<<": Link Update: "<<*l<<endl;
}


void Node::ProcessIncomingRoutingMessage(const RoutingMessage *m)
{
  cerr <<"routing message received at node "<< number <<" from node "<< m->nodeID<<endl;
  map<unsigned, double> oldDist = table->GetRow();
  table->RowUpdate(m->nodeID,m->distances);
  map<unsigned, double> newDist = table->GetRow();
  //comparison isn't working. Otherwise great. 
  if(oldDist != newDist){
    SendToNeighbors(new RoutingMessage(number, newDist));
    cerr << "table has changed" << endl;
  }else{
    cerr << "table has not changed" << endl;
  }
}

void Node::TimeOut()
{
  cerr << *this << " got a timeout: ignored"<<endl;
}


Node *Node::GetNextHop(const Node *destination) const
{ Node *this2=new Node(*this);
  unsigned next=table->GetNext(destination->GetNumber());
  deque<Node*> *neighborsOfNode = (*this2).GetNeighbors();
  deque<Node*>::iterator nodeIter=neighborsOfNode->begin();
  while(nodeIter!=neighborsOfNode->end())
  {
    if(next==(*nodeIter)->GetNumber())
      {
        //needed the double dereference here. 
        Node *result = new Node(**nodeIter);
        return result;
      }
    nodeIter++;
  }
  //return 0;
}

Table *Node::GetRoutingTable() const
{
    Table *table2 = table;
    return table2;
}


ostream & Node::Print(ostream &os) const
{
  os << "Node(number="<<number<<", lat="<<lat<<", bw="<<bw<<"table: "<<*table;
  return os;
}
#endif
