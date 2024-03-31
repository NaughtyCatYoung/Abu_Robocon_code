from flask import Flask, render_template, request, redirect, url_for

app = Flask(__name__, template_folder='/home/youngrasp/Desktop/Abu/Testflask/')

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/handle_button', methods=['POST'])
def handle_button():
    button_action = request.json['action']
    button_name = request.json['button']
    print(f"Button {button_name} {button_action}")
    # Process the button action here (e.g., perform some task)
    # Redirect back to the homepage
    return redirect(url_for('index'))
if __name__ == '__main__':
    app.run(debug=True,port=8000)
