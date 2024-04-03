from flask import Flask, render_template, request, redirect, url_for

import logging

logging.basicConfig(filename='app.log', level=logging.INFO)
app = Flask(__name__, 
            template_folder='/home/youngrasp/Desktop/Abu/TestJoy/')

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/handle_button', methods=['POST'])
def handle_button():
    action=request.json['Action']
    if action=='Motor':
        sp = request.json['speed']
        ang = request.json['angle']
        print(f"Speed: {sp} \nAngle: {ang}\n=================")
    elif action=='Button':
        command=request.json['button']
        print(f"Button {command}")
    else:print(f"Unknown Action: {action}") 
    # Process the button action here (e.g., perform some task)
    # Redirect back to the homepage
    
    return redirect(url_for('index'))
if __name__ == '__main__':
    app.run( port=8000)
