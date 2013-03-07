#ifndef _table
#define _table


#include <iostream>

using namespace std;

#if defined(GENERIC)
class Table {
  // Students should write this class
 public:
  ostream & Print(ostream &os) const;
};
#endif


#if defined(LINKSTATE)
#include <deque>

class Table {
	private:
	map<int,double> costTable;
	map<int, int> nextHop;
	unsigned id;
	vector<unsigned> nodes;
	vector<Link*> links;
	void dijkstra();
  // Students should write this class
 public:
  ostream & Print(ostream &os) const;
  unsigned GetNext(const unsigned goal);
  bool LinkUpdate(const unsigned src, const unsigned dest, double l, double ts);
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
 	int id;
 public:
   Table();
   Table(const int myID); 
   ostream & Print(ostream &os) const;
   unsigned GetNext(unsigned end);
   map<unsigned,double> GetRow();
   bool RowUpdate(const unsigned src, const map<unsigned,double> toUpdate);
   bool SelfUpdate(const Link *l);
   Table *get_routing_table() const;
};
#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
