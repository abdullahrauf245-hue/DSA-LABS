// Lab 9 - Task 3: Weighted M-ary Tree - Galaxy Representation
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

// Edge weight between a parent and child star system
class TravelRoute {
public:
    float travel_cost;
    TravelRoute(float cost = 0.0f) : travel_cost(cost) {}
};

class StarSystem {
public:
    int system_id;
    string name;
    int num_planets;
    double x, y, z;
    bool is_habitable;
    // each child is stored together with the route (edge) that leads to it
    vector<pair<StarSystem*, TravelRoute>> children;

    StarSystem(int id, const string& n, int planets,
               double px, double py, double pz, bool habitable)
        : system_id(id), name(n), num_planets(planets),
          x(px), y(py), z(pz), is_habitable(habitable) {}
};

class Galaxy {
    int M;                 // max children per node
    StarSystem* root;

    int subtree_size(StarSystem* x) const {
        if (!x) return 0;
        int s = 1;
        for (auto& c : x->children) s += subtree_size(c.first);
        return s;
    }
    void destroy(StarSystem* x) {
        if (!x) return;
        for (auto& c : x->children) destroy(c.first);
        delete x;
    }

public:
    // Overloaded constructors: M only, or M with an optional root node
    Galaxy(int m) : M(m), root(nullptr) {}
    Galaxy(int m, StarSystem* r) : M(m), root(r) {}
    ~Galaxy() { destroy(root); }

    StarSystem* get_root() const { return root; }
    void set_root(StarSystem* r) { root = r; }

    // Attach child under parent with a route cost; fails if parent already has M children
    bool add_child(StarSystem* parent, StarSystem* child, float cost) {
        if (!parent || !child || (int)parent->children.size() >= M) return false;
        parent->children.push_back({child, TravelRoute(cost)});
        return true;
    }

    // Balanced: at EVERY node, sizes of all child subtrees differ by at most 1
    bool is_balanced(StarSystem* x) const {
        if (!x || x->children.empty()) return true;
        int mn = subtree_size(x->children[0].first), mx = mn;
        for (auto& c : x->children) {
            int s = subtree_size(c.first);
            mn = min(mn, s);
            mx = max(mx, s);
            if (!is_balanced(c.first)) return false;
        }
        return (mx - mn) <= 1;
    }

    int count_leaves(StarSystem* x) const {
        if (!x) return 0;
        if (x->children.empty()) return 1;
        int total = 0;
        for (auto& c : x->children) total += count_leaves(c.first);
        return total;
    }
};

int main() {
    // ---- Galaxy A: balanced ternary tree ----
    Galaxy A(3, new StarSystem(1, "Sol", 8, 0, 0, 0, true));
    StarSystem* sol = A.get_root();
    auto* alpha = new StarSystem(2, "Alpha Centauri", 3, 4.3, 0, 0, true);
    auto* sirius = new StarSystem(3, "Sirius", 2, 8.6, 1, 0, false);
    auto* vega = new StarSystem(4, "Vega", 4, 25, 3, 2, false);
    A.add_child(sol, alpha, 4.3f);
    A.add_child(sol, sirius, 8.6f);
    A.add_child(sol, vega, 25.0f);
    A.add_child(alpha, new StarSystem(5, "Proxima", 2, 4.2, 0, 0, true), 1.2f);
    A.add_child(sirius, new StarSystem(6, "Sirius B", 0, 8.6, 1, 0, false), 0.5f);
    A.add_child(vega, new StarSystem(7, "Vega B", 1, 25, 3, 2, false), 2.0f);

    cout << "Galaxy A\n";
    cout << "  Leaf count : " << A.count_leaves(A.get_root()) << "\n";
    cout << "  Balanced   : " << (A.is_balanced(A.get_root()) ? "yes" : "no") << "\n";

    // ---- Galaxy B: unbalanced (one deep branch) ----
    Galaxy B(2);
    B.set_root(new StarSystem(10, "Root", 1, 0, 0, 0, false));
    auto* l = new StarSystem(11, "Left", 2, 1, 0, 0, true);
    auto* r = new StarSystem(12, "Right", 1, 0, 1, 0, false);
    B.add_child(B.get_root(), l, 3.0f);
    B.add_child(B.get_root(), r, 4.0f);
    auto* l1 = new StarSystem(13, "L1", 1, 2, 0, 0, false);
    B.add_child(l, l1, 1.5f);
    B.add_child(l1, new StarSystem(14, "L2", 1, 3, 0, 0, false), 1.0f);

    cout << "\nGalaxy B\n";
    cout << "  Leaf count : " << B.count_leaves(B.get_root()) << "\n";
    cout << "  Balanced   : " << (B.is_balanced(B.get_root()) ? "yes" : "no") << "\n";

    // ---- Edge cases ----
    Galaxy empty(3);
    cout << "\nEmpty galaxy -> leaves: " << empty.count_leaves(empty.get_root())
         << ", balanced: " << (empty.is_balanced(empty.get_root()) ? "yes" : "no") << "\n";
    return 0;
}