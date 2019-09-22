// Hash table for 15-puzzle
// Universidad Simon Bolivar, 2008.
// Author: Blai Bonet
// Last Revision: 06/01/11
// Modified by:

#include <iostream>
#include <iomanip>
#include <ext/hash_map>

using namespace std;

struct state15_t {
    unsigned p1_, p2_;
    state15_t() : p1_(0), p2_(0) {
        for( int i = 7; i >= 0; --i ) {
            p1_ = p1_<<4;
            p1_ += i;
            p2_ = p2_<<4;
            p2_ += i+8;
        }
    }
    bool operator==(const state15_t &s) const { return (p1_==s.p1_) && (p2_==s.p2_); }
    unsigned bpos() const {
        unsigned p = p1_;
        for( int i = 0; i < 8; ++i, p = p>>4 ) if( (p&0xF) == 0 ) return i;
        p = p2_;
        for( int i = 0; i < 8; ++i, p = p>>4 ) if( (p&0xF) == 0 ) return 8+i;
        return (unsigned)-1;
    }
    unsigned cont(unsigned p) const {
        return (p<8 ? (p1_>>(p<<2)) : (p2_>>((p-8)<<2))) & 0xF;
    }
    void set(unsigned p, unsigned t) {
        if( p < 8 )
            p1_ = (p1_ & ~(0xF<<(p<<2))) | (t<<(p<<2));
        else
            p2_ = (p2_ & ~(0xF<<((p-8)<<2))) | (t<<((p-8)<<2));
    }
    void right() {
        unsigned bp = bpos(), t = cont(1+bp);
        set(1+bp, 0);
        set(bp, t);
    }
    void print(ostream &os) const {
        unsigned p = p1_;
        for( int i = 0; i < 16; ++i ) {
            os << setw(2) << (p&0xF) << ' ';
            p = p>>4;
            if( i%4 == 3 ) os << endl;
            if( i == 7 ) p = p2_;
        }
    }
};

inline ostream& operator<<(ostream &os, const state15_t &s) {
    s.print(os);
    return os;
}

struct value_t;

class node_t {
  public:
    char g_, h_;
    bool open_;
    value_t *prev_, *next_;  // to be used in the priority queue

    node_t(unsigned g = 0, unsigned h = 0, bool open = false) : g_(g), h_(h), open_(open), prev_(0), next_(0) { }
    unsigned g() const { return g_; }
    unsigned h() const { return h_; }
    unsigned f() const { return g_ + h_; }
    bool open() const { return open_; }
    bool closed() const { return !open(); }
    value_t* prev() const { return prev_; }
    value_t* next() const { return next_; }
    void set_g(unsigned g) { g_ = g; }
    void set_h(unsigned h) { h_ = h; }
    void set_open(bool open = false) { open_ = open; }
    void set_prev(value_t *prev) { prev_ = prev; }
    void set_next(value_t *next) { next_ = next; }
    void print(ostream &os) const {
        os << "g=" << g() << ", h=" << h() << ", open=" << (open() ? "true" : "false");
    }

  protected:
};

inline ostream& operator<<(ostream &os, const node_t &n) {
    n.print(os);
    return os;
}

struct value_t : public pair<const state15_t, node_t> {
    void link(value_t *n) {
        n->second.next_ = second.next_;
        if( second.next_ )
            second.next_->second.prev_ = n;
        second.next_ = n;
        n->second.prev_ = this;
    }
    void unlink() {
        if( second.next_ )
            second.next_->second.prev_ = second.prev_;
        if( second.prev_ )
            second.prev_->second.next_ = second.next_;
    }
};

namespace __gnu_cxx {
  template<> class hash<state15_t> {
    public:
      size_t operator()(const state15_t &s) const { return s.p1_ ^ s.p2_; }
  };
};

class hash_t : public __gnu_cxx::hash_map<state15_t,node_t> { };  // class
hash_t hash; // hash instance

class priority_queue_t {
  protected:
    size_t maxsz_;
    value_t *array_;
  public:
    priority_queue_t(size_t maxsz) : maxsz_(maxsz) { array_ = new value_t[maxsz_]; }
    virtual ~priority_queue_t() { delete[] array_; }
    value_t *first();
    void insert(value_t *n) { array_[n->second.f()].link(n); }
    void reorder(value_t *n, int newg);
    void print(ostream &os) const {
        for( size_t i = 0; i < maxsz_; ++i ) {
            for( value_t *v = array_[i].second.next_; v != 0; v = v->second.next_ ) {
                os << v->first << v->second << endl;
            }
        }
    }
};

int main(int argc, char **argv) {
    state15_t state;
    node_t node(10,5,true);
    priority_queue_t pq(100);

    cout << "sizeof(state15_t)=" << sizeof(state15_t)
         << ", sizeof(node_t)=" << sizeof(node_t)
         << endl << endl;

    hash.insert(make_pair(state, node));
    hash_t::iterator it = hash.find(state);
    cout << (*it).first << (*it).second << endl << endl;
    pq.insert((value_t*)&*it);

    state.right();
    state.right();
    node.set_g(node.g()+2);
    node.set_open(false);
    hash.insert(make_pair(state, node));
    it = hash.find(state);
    cout << (*it).first << (*it).second << endl << endl;
    pq.insert((value_t*)&*it);

    cout << "hash size = " << hash.size() << endl << endl;;

    cout << "priority queue:" << endl;
    pq.print(cout);

    return 0;
}


