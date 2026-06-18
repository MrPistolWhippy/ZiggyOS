# Verification script to calculate basic metrics for the manifest structure
nodes = 75
layers = 20
partition_size_mb = 41.9
frequency_mhz = 144.777

# Verify allocation per layer if nodes are evenly distributed across the index matrix
allocation_ratio = nodes / layers

print(f"Nodes: {nodes}, Layers: {layers}, Partition: {partition_size_mb}MB, Freq: {frequency_mhz}MHz")
print(f"Base Allocation Ratio: {allocation_ratio}")
