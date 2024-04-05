from flask import Flask, render_template, request, redirect, url_for
import serial

ser = serial.Serial(
    port = '/dev/ttyUSB1',
    baudrate = 115200,
    timeout = 10,
    xonxoff = False
)

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
        sp = int(request.json['speed']*4095/100*0.5)
        ang = int(((request.json['angle']-90)%360)*140/360)
        print(f"Speed: {sp} \nAngle: {ang}\n=================")
        val = ser.write(f"DC 1 Forward {sp}\n".encode(encoding = 'ascii', errors = 'strict'))
        print("Bytes written: ", val)
        val = ser.write(f"DC 2 Forward {sp}\n".encode(encoding = 'ascii', errors = 'strict'))
        print("Bytes written: ", val)
        val = ser.write(f"DC_encoder 1 {ang}\n".encode(encoding = 'ascii', errors = 'strict'))
        print("Bytes written: ", val)
        val = ser.write(f"DC_encoder 2 {ang}\n".encode(encoding = 'ascii', errors = 'strict'))
        print("Bytes written: ", val)
    elif action=='Button':
        command=request.json['button']
        print(f"Button {command}")
    else:print(f"Unknown Action: {action}") 
    # Process the button action here (e.g., perform some task)
    # Redirect back to the homepage
    
    return redirect(url_for('index'))
if __name__ == '__main__':
    app.run( port=8000)
