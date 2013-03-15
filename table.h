#ifndef _table
#define _table

#include <iostream>

using namespace std;

#if defined(GENERIC)
class Table {
  // Students should write this class
 public:
  Table();
  Table(const unsigned myID);
  ostream & Print(ostream &os) const;
};
#endif


#if defined(LINKSTATE)
#define NOLINK -1;
#include <deque>
#include <map>
#include <vector>
#include "node.h"
struct neighbordata{
  unsigned idVal; double length;
  neighbordata(unsigned first, double second):idVal(first),length(second){}
  neighbordata(){idVal=0;length=0;}
};
// struct queueNode{
//   unsigned idVal; double currentDist;
// };
class greater_neighbor_length
{ public:
    bool operator() ( const neighbordata& a, const  neighbordata& b) const
    { return a.length>b.length ; }
};
class Table {
	private:
  map<unsigned,unsigned> predecessor;
	map<unsigned,double> costTable;
  map<unsigned,bool> visited;
  vector<neighbordata> nodeQueue;
	map<unsigned,unsigned> nextHop;
  map<unsigned,vector<neighbordata> > neighborhoods;
	unsigned id;
	void dijkstra();
  // Students should write this class
 public:
  Table();
  Table(const unsigned myID);
  Table(const Table &rhs);
  ostream & Print(ostream &os) const;
  unsigned GetNext(const unsigned goal);
  vector<neighbordata> GetRow();
  void ChangeLink(const Link *l);
  void NeighborUpdate(const unsigned src, const vector<neighbordata> toupdate);
  void Refresh();
  Table *get_routing_table() const;
};
#endif

#if defined(DISTANCEVECTOR)

#include <deque>
#include <map>
#include "node.h"
class Table {
 private:
 	map<unsigned, map<unsigned, double> > dist;
 	map<unsigned, unsigned> nextHop;
 	unsigned id;
 public:
   Table();
   Table(const unsigned myID);
   Table(const Table &rhs);
   ostream & Print(ostream &os) const;
   unsigned GetNext(unsigned end);
   map<unsigned,double> GetRow();
   void RowUpdate(const unsigned src, const map<unsigned,double> toUpdate);
   void ChangeLink(const Link *l);
   void SelfUpdate();
};
#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
