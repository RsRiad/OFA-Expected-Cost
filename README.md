# Monte Carlo DP Facility Assignment

## 📌 Overview
This project implements a **Monte Carlo + Dynamic Programming solver** for an **online facility assignment problem on a regular polygon**.  

- Facilities are arranged on the vertices of an `n`‑gon.
- Customers arrive randomly on edges, with a uniformly distributed position along the edge.
- Each customer is assigned to the nearest **free facility**.
- If multiple facilities are equally close, ties are broken randomly.
- The **cost** is the distance traveled by the customer to the assigned facility.
- The program computes the **expected total assignment cost** starting from an empty state.

---

## 🔹 Key Features
- **Random number generation** using `mt19937` for reproducible Monte Carlo sampling.
- **Geometry helpers** to compute shortest circular distances and edgewise distances.
- **Bitmask representation** of facility states (`0 = free`, `1 = occupied`).
- **Backward induction DP**:
  - Groups states by the number of occupied facilities.
  - Computes expected costs recursively, starting from terminal states (all facilities occupied).
- **Monte Carlo simulation**:
  - Samples random arrivals (`NUM_SAMPLES` per state).
  - Estimates expected cost by averaging over samples.
- **Detailed logging**:
  - Prints step‑by‑step analysis for the first state at each DP level.
  - Shows random arrivals, distance calculations, tie‑breaking, and cost decomposition.

---

## 🚀 Usage
1. Clone the repository:
   ```bash
   https://github.com/RsRiad/OFA-Expected-Cost.git
   cd OFA-Expected-Cost

2. Compile the program:
   ```bash
   g++ -std=c++17 -O2 main.cpp -o facility_dp

3. Run the executable:
   ```bash
   ./facility_dp

⚙️ Configuration
Adjust n (number of facilities) in main().
Adjust NUM_SAMPLES (Monte Carlo iterations per state) for accuracy vs. runtime.
   

