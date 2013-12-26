#include <cstdio>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
using namespace std;
#define rep(i,n) for (int i = 0; i < (int)(n); i++)
#define iter(v) __typeof((v).end())
#define foreach(it,v) for (iter(v) it = (v).begin(); it != (v).end(); it++)
typedef long long ll;
typedef pair <int, int> PII;
typedef map <int, int> mii;
typedef mii *miip;
const int N = 100005;
int n, m;
int c[N], ans[N];
vector <PII> Q[N];
vector <int> E[N];

struct Node {
    Node *ch[2];
    int fix, key, size;
    void up();
    int query(int);
}pool[N * 20], *C = pool + 1, *null = pool;

void Node::up() {
    if (this == null) return;
    size = ch[0]->size + ch[1]->size + 1;
}

int Node::query(int k) {
    if (this == null) return 0;
    if (key >= k)
        return ch[0]->query(k) + 1 + ch[1]->size;
    else
        return ch[1]->query(k);
}

Node *newNode(int key) {
    C->ch[0] = C->ch[1] = null;
    C->size = 1;
    C->key = key;
    C->fix = (rand() << 16) ^ rand();
    return C++;
}

// o = 0 -- left child up
void rot(Node *&u, int o) {
    Node *v = u->ch[o];
    u->ch[o] = v->ch[!o];
    v->ch[!o] = u;
    u->up();
    v->up();
    u = v;
}

void insert(Node *&u, int key) {
    if (u == null) {
        u = newNode(key);
    } else {
        u->size++;
        if (key < u->key) {
            insert(u->ch[0], key);
            if (u->ch[0]->fix < u->fix) rot(u, 0);
        } else {
            insert(u->ch[1], key);
            if (u->ch[1]->fix < u->fix) rot(u, 1);
        }
    }
}

//if each element in u less or equal than each element in v
Node *merge(Node *u, Node *v) {
    if (u == null) return v;
    if (v == null) return u;
    if (u->fix < v->fix) {
        u->ch[1] = merge(u->ch[1], v);
        u->up();
        return u;
    } else {
        v->ch[0] = merge(u, v->ch[0]);
        v->up();
        return v;
    }
}

void erase(Node *&u, int key) {
    if (u == null) return;
    if (u->key == key) {
        u = merge(u->ch[0], u->ch[1]);
    } else {
        key < u->key ? erase(u->ch[0], key) : erase(u->ch[1], key);
        u->up();
    }
}