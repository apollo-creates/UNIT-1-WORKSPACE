import webview
import sys
import threading
import time
import os

# Add the project root to sys.path if not present
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

from trainer_app.app import app

def start_flask():
    # Run Flask in a thread
    # We use a slightly different port or just 5001 as established
    app.run(port=5002, use_reloader=False)

if __name__ == '__main__':
    # Start Flask in a daemon thread
    t = threading.Thread(target=start_flask)
    t.daemon = True
    t.start()

    # Give Flask a second to start
    time.sleep(1)

    # Create a native window
    webview.create_window(
        'Neural DSP Trainer', 
        'http://127.0.0.1:5002',
        width=1200,
        height=800,
        resizable=True
    )

    # Start the UI loop
    webview.start()
