//AVL
#include <cstdio>
#include <iostream>
using namespace std;
int n, c;
long long r;

int abs(int x)
{
    if (x > 0) return x; else return -x;
}

int min(int a, int b)
{
    if (a < b) return a; else return b;
}

class T
{
    public:
        int v, h, s, c;
        T *l, *r;
        T(int v)
        {
            this->v = v;
            this->h = 1;
            this->s = 1;
            this->c = 1;
            this->l = nullptr;
            this->r = nullptr;
        }
        void u()
    {
            s = 0;
            if (l) s += l->s;
            if (r) s += r->s;
            s += c;
        }
};

class AVL {
    public:
        AVL()
        {
            rt = nullptr;
            s = 0;
        }
        void a(int k)
        {
            rt = a(rt, k);
        }
    
        void r(int k)
        {
            rt = r(rt, k);
        }
    
        int p(int k)
        {
            return p(rt, k);
        }
    
        int next(int k)
        {
            return next(rt, k);
        }
    
        bool e() {
        return s == 0;
        }
    
    private:
        T *rt;
        int s;
        int h(T *n)
        {
            if (n) return n->h;
            else return 0;
        }
    
        int bf(T *n) {
            if (n) return h(n->l) - h(n->r);
            else return 0;
        }
    
        int sz(T *n) {
            if (n) return n->s;
            else return 0;
        }
    
        T* m(T* n) {
            if (n->l) return m(n->l);
            else return n;
        }
    
        int mx(int a, int b) {
            if (a > b) return a;
            else return b;
        }
    
        int p(T* n, int k) {
            if(n == nullptr) return -1;
            if(n->v == k) {
                if(n->l == nullptr) return -1;
                T* tmp = n->l;
                while(tmp->r) tmp = tmp->r;
                return tmp->v;
            }
            if(n->v > k) return p(n->l, k);
            int res = p(n->r, k);
            if(res == -1) return n->v;
            return res;
        }
    
        int next(T* n, int k)
        {
            if(n == nullptr) return -1;
            if(n->v == k) {
                if(n->r == nullptr) return -1;
                T* tmp = n->r;
                while(tmp->l) tmp = tmp->l;
                return tmp->v;
            }
            if(n->v < k) return next(n->r, k);
            int res = next(n->l, k);
            if(res == -1) return n->v;
            return res;
        }

        T* rr(T* b)
        {
            T* a = b->l;
            T* tmp = a->r;
            a->r = b;
            b->l = tmp;
            b->h = mx(h(b->l), h(b->r)) + 1;
            a->h = mx(h(a->l), h(a->r)) + 1;
            b->u();
            a->u();
            return a;
        }
    
        T* lr(T* b)
        {
            T* a = b->r;
            T* tmp = a->l;
            a->l = b;
            b->r = tmp;
            b->h = mx(h(b->l), h(b->r)) + 1;
            a->h = mx(h(a->l), h(a->r)) + 1;
            b->u();
            a->u();
            return a;
        }
    
        T* a(T* n, int k) {
            
            if(n == nullptr)
            {
                s++;
                return new T(k);
            }
            if(n->v == k)
            {
                n->c++;
                n->u();
                return n;
            }
            if(n->v > k) n->l = a(n->l, k);
            else if(n->v < k) n->r = a(n->r, k);
            n->h = mx(h(n->l), h(n->r)) + 1;
            n->u();
            int baf = bf(n);
            if (baf > 1 and bf(n->l) > 0) return rr(n);
            if (baf < -1 and bf(n->r) < 0) return lr(n);
            if (baf > 1 and bf(n->l) < 0) {
                n->l = lr(n->l);
                return rr(n);
            }
            if (baf < -1 && bf(n->r) > 0) {
                n->r = rr(n->r);
                return lr(n);
            }
            return n;
        }

        T* r(T* n, int k){
            if (n == nullptr){
                return nullptr;
            }
            T* new_root = nullptr;
            if(n->v > k) {
                n->l = r(n->l, k);
                new_root = n;
            } else if (n->v < k) {
                n->r = r(n->r, k);
                new_root = n;
            } else {
                if(n->c > 1) {
                    n->c--;
                    n->u();
                    return n;
                }
                if  (n->l == nullptr) {
                    T* right = n->r;
                    n->r = nullptr;
                    s--;
                    new_root = right;
                } else if (n->r == nullptr) {
                    T* left = n->l;
                    n->l = nullptr;
                    s--;
                    new_root = left;
                } else {
                    T* mn = m(n->r);
                    mn->r = r(n->r, mn->v);
                    mn->l = n->l;
                    n->l = n->r = nullptr;
                    new_root = mn;
                }
            }
            if(new_root == nullptr) return nullptr;
            new_root->h = mx(h(new_root->l), h(new_root->r)) + 1;
            new_root->u();
            int baf = bf(new_root);
            if(baf > 1 and bf(new_root->l) >= 0) return rr(new_root);
            if(baf < -1 and bf(new_root->r) <= 0) return lr(new_root);
            if(baf > 1 and bf(new_root->l) < 0) {
                new_root->l = lr(new_root->l);
                return rr(new_root);
            }
            if(baf < -1 and bf(new_root->r) > 0) {
                new_root->r = rr(new_root->r);
                return lr(new_root);
            }
            return new_root;
        }
};

int main()
{
    cin>>n;
    AVL t;
    for(int i = 0; i < n; i++)
    {
        int a, b;
        cin>>a>>b;
        if(t.e())
        {
            c = a;
            t.a(b);
            continue;
        }

        if(a == c) t.a(b);
        else {
            int p = t.p(b);
            int n = t.next(b);
            if(p == -1) {
                r += abs(n - b);
                t.r(n);
            }
            else if(n == -1) {
                r += abs(p - b);
                t.r(p);
            }
            else{
                if(abs(p - b) <= abs(n - b)) {
                    r += abs(p - b);
                    t.r(p);
                }
                else{
                    r += abs(n - b);
                    t.r(n);
                }
            }
        }
    }
    cout<<r<<endl;
    return 0;
}
