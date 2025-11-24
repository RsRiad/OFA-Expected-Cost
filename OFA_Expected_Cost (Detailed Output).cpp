#include <bits/stdc++.h>
#include <random>
#include <iomanip> // Used for formatting output nicely
using namespace std;

// --- Random Number Generator Setup ---
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

double random_double() {
    return uniform_real_distribution<double>(0.0, 1.0)(rng);
}

int random_int(int n) {
    return uniform_int_distribution<int>(0, n-1)(rng);
}

// --- Geometry Helpers ---
double steps_between(int a, int b, int n) {
    int forward = (b - a + n) % n;
    int backward = (a - b + n) % n;
    return min(forward, backward);
}

double edgewise_distance(int n, int e, double t, int v) {
    int v1 = e;
    int v2 = (e + 1) % n;
    double dist1 = t + steps_between(v1, v, n);
    double dist2 = (1.0 - t) + steps_between(v2, v, n);
    return min(dist1, dist2);
}

string mask_to_string(int mask, int n) {
    string s;
    for (int i = n-1; i >= 0; i--) s += (mask & (1 << i)) ? '1' : '0';
    return s;
}

int main() {
    int n = 9;
    int NUM_SAMPLES = 5000; 

    cout << fixed << setprecision(3); // Format numbers to 3 decimal places
    cout << "========== DETAILED MONTE CARLO LOG ==========\n";

    int N = 1 << n;
    vector<double> V(N, 0.0);

    vector<vector<int>> masks_by_pc(n+1);
    for (int m=0; m<N; m++) masks_by_pc[__builtin_popcount(m)].push_back(m);

    // === DP BACKWARD INDUCTION ===
    for (int pc = n-1; pc >= 0; pc--) {
        
        cout << "\n" << string(60, '=') << "\n";
        cout << "LEVEL " << pc << ": (States with " << pc << " facilities occupied)\n";
        cout << string(60, '=') << "\n";

        bool printed_example_for_this_level = false;

        for (int mask : masks_by_pc[pc]) {
            
            // We will only print detailed logs for the FIRST state of each level
            // to keep the output readable.
            bool verbose = !printed_example_for_this_level;
            if (verbose) {
                cout << "\nAnalyzing State [" << mask_to_string(mask, n) << "] (Detailed logs for first 3 samples)...\n";
                printed_example_for_this_level = true;
            }

            vector<int> freev;
            for (int v=0; v<n; v++) 
                if (!(mask & (1<<v))) freev.push_back(v);

            double total_cost_accumulator = 0.0;

            for (int k = 0; k < NUM_SAMPLES; k++) {
                
                // 1. Random Generation
                int e = random_int(n);     
                double t = random_double(); 

                // 2. Distance Calculation
                double min_d = 1e18;
                vector<int> ties;

                if (verbose && k < 3) {
                    cout << "  [Sample " << k+1 << "] Cust at Edge " << e << ", t=" << t << "\n";
                    cout << "    Dists: ";
                }

                for (int fv : freev) {
                    double d = edgewise_distance(n, e, t, fv);
                    if (verbose && k < 3) cout << "F" << fv << "=" << d << " "; // Print distance

                    if (d < min_d - 1e-9) {
                        min_d = d;
                        ties.clear();
                        ties.push_back(fv);
                    } else if (abs(d - min_d) < 1e-9) {
                        ties.push_back(fv);
                    }
                }
                if (verbose && k < 3) cout << "\n";

                // 3. Assignment & Tie Breaking
                int chosen_facility = -1;
                
                if (ties.size() == 1) {
                    chosen_facility = ties[0];
                    if (verbose && k < 3) cout << "    -> Nearest: F" << chosen_facility << " (No tie)\n";
                } else {
                    // COIN TOSS
                    int random_index = random_int(ties.size());
                    chosen_facility = ties[random_index];
                    
                    if (verbose && k < 3) {
                        cout << "    -> TIE DETECTED between: { ";
                        for(int t : ties) cout << "F" << t << " ";
                        cout << "}. Coin Toss Winner: F" << chosen_facility << "\n";
                    }
                }

                // 4. Cost Calculation
                int next_mask = mask | (1 << chosen_facility);
                double future_val = V[next_mask]; // Look up DP table
                double trial_cost = min_d + future_val;

                if (verbose && k < 3) {
                    cout << "    -> Cost = " << min_d << " (current dist) + " << future_val << " (future cost)\n";
                    cout << "    -> Total Sample Cost: " << trial_cost << "\n\n";
                }

                total_cost_accumulator += trial_cost;
            }

            V[mask] = total_cost_accumulator / NUM_SAMPLES;
            
            if (verbose) {
                cout << "  ... (" <<NUM_SAMPLES-3 <<" more samples hidden) ...\n";
                cout << "  ==> Average Value for State [" << mask_to_string(mask, n) << "] = " << V[mask] << "\n";
            }
        }
    }

    cout << "\n" << string(50, '=') << "\n";
    cout << "FINAL RESULT (Expected Total Dist): " << V[0] << "\n";
    cout << string(50, '=') << "\n";

    return 0;
}
