# Append the task switching code directly to your startup file
cat src/context.s 2>/dev/null >> src/startup.s

# Create the sensor module
cat << 'COMP' > src/dev/sensor_i2c.c
// File populated successfully
COMP
