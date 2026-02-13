#!/bin/bash

# Navigate to script directory
cd "$(dirname "$0")"

# Activate Virtual Environment
if [ -d "venv" ]; then
    source venv/bin/activate
else
    echo "Virtual environment 'venv' not found. Creating..."
    python3 -m venv venv
    source venv/bin/activate
fi

# Ensure dependencies are installed (quietly)
pip install -q -r trainer_app/requirements.txt

# Kill anything running on port 5002 (Webview Port)
echo "Cleaning up port 5002..."
PIDS=$(lsof -ti:5002)
if [ -n "$PIDS" ]; then
    echo "Killing processes: $PIDS"
    kill -9 $PIDS 2>/dev/null || true
    sleep 1
fi

# Run the Trainer App (Native Wrapper)
echo "Starting Neural Calibration Trainer (Native Mode)..."
python native_app.py
