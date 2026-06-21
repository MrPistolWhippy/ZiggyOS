#!/bin/bash
# ==============================================================================
#          ZIGGY-OS: LOCAL AUTOMATED BUILD LOOP RUNNER
# ==============================================================================

echo "--------------------------------------------------------"
echo "    ZIGGY-OS: RUNNING LOCAL MAKEFILE LOOP TRACKER"
echo "    Monitoring workspace... Press [CTRL+C] to terminate."
echo "--------------------------------------------------------"

# Baseline snapshot tracker
LAST_STATE=$(ls -l Makefile fast_core.c riscv_driver.c 2>/dev/null)

while true; do
    CURRENT_STATE=$(ls -l Makefile fast_core.c riscv_driver.c 2>/dev/null)
    
    if [ "$CURRENT_STATE" != "$LAST_STATE" ]; then
        clear
        echo "========================================================"
        echo " [⚡ LIVE BUILD] Source code shift detected! Executing custom loop..."
        echo "========================================================"
        echo ""
        
        # Fire off our efficient custom make targets
        make all
        
        echo ""
        echo "========================================================"
        echo " [✓] Custom incremental build complete."
        echo "========================================================"
        
        LAST_STATE="$CURRENT_STATE"
    fi
    sleep 2
done
