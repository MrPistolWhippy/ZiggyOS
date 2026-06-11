
import math
nodes = {
    "G-01": (-35.4021, 174.3644, "Helena Bay"),
    "G-02": (-35.2443, 174.1989, "Manawaora Peninsula"),
    "G-04": (-36.1612, 174.6511, "Te Arai / Tara Iti"),
    "G-05": (-36.7925, 174.9856, "Church Bay")
}
print("=== 🇳🇿 NZ GEOMETRIC DISTANCE MATRIX ===")
keys = list(nodes.keys())
for i in range(len(keys)):
    for j in range(i+1, len(keys)):
        n1, n2 = keys[i], keys[j]
        lat1, lon1, name1 = nodes[n1]
        lat2, lon2, name2 = nodes[n2]
        d = math.sqrt((lat1-lat2)**2 + (lon1-lon2)**2) * 111.1
        print(f"  [ROUTE] {n1} ({name1}) to {n2} ({name2}) -> {d:.2f} km")
