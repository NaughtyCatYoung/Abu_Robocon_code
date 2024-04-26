import socket
import pygame
import math
import time
# Raspberry Pi's IP address and port
HOST = 'YoungRasp.local'  # Replace with your Raspberry Pi's IP address
PORT = 12345            # Replace with the port number you chose

client_socket =socket.socket(socket.AF_INET,socket.SOCK_STREAM)
client_socket.connect((HOST, PORT))
print('Connected to Raspberry Pi')

# Initialize Pygame
pygame.init()

# Set up the display
pygame.display.set_caption("Joystick Test")

# Initialize the joystick
pygame.joystick.init()

# Check if any joystick is connected
joystick_count = pygame.joystick.get_count()
if joystick_count == 0:
    print("No joystick detected")
else:
    # Connect to the first joystick
    joystick = pygame.joystick.Joystick(0)
    joystick.init()
    print("Joystick detected:", joystick.get_name())
    
#================================================================
try:
    x_axis=0.0
    y_axis=0.0
    x_axis_now=0.0
    y_axis_now=0.0
    speed=0.0
    speed_now=0.0
    while True:

        # Poll for joystick events
        pygame.event.pump()

        # Print joystick axes values
        for i in range(joystick.get_numaxes()):
            axis_value = joystick.get_axis(i)
            if(i==0):
                x_axis_now=axis_value
            if(i==1):
                y_axis_now=-axis_value
            if(i==5):
                speed_now=(axis_value+1.0)*255/2
            # print(f"Axis {i}: {axis_value:.2f}")
        if(x_axis_now!=x_axis or y_axis_now!=y_axis):
            x_axis=x_axis_now
            y_axis=y_axis_now
            angle=0
            if(x_axis==0 and y_axis==0):
                angle=0
            elif(x_axis==0 and y_axis!=0):
                angle=90
            else:
                angle=(math.degrees(math.atan(y_axis/x_axis)))%360
                if(x_axis<0):
                    angle=(180+angle)%360
            print(f"Angle = {angle}")
            command=f"\nDC_encoder 1 {str(int(int(angle)*1200/360))}\n"
            client_socket.sendall(command.encode())
            time.sleep(0.02)
            command=f"\nDC_encoder 2 {str(int(int(angle)*900/360))}\n"
            client_socket.sendall(command.encode())
            time.sleep(0.02)
        if(speed_now!=speed):
            speed=speed_now
            print(f"Speed = {speed}")
            command=f"\nDC 1 Forward {max(min(int(speed),255),0)}\n"
            client_socket.sendall(command.encode())
            time.sleep(0.01)
            command=f"\nDC 2 Forward {max(min(int(speed),255),0)}\n"
            client_socket.sendall(command.encode())
            time.sleep(0.01)
        # Print button values
        for i in range(joystick.get_numbuttons()):
            button_value = joystick.get_button(i)
            if(i==10 and button_value==1):
                command=f"\nDC_encoder 1 {0}\n"
                client_socket.sendall(command.encode())
                time.sleep(0.01)
                command=f"\nDC_encoder 2 {0}\n"
                client_socket.sendall(command.encode())
                time.sleep(0.01)
                
            # print(f"Button {i}: {button_value}")

        # Print hat switch values
        for i in range(joystick.get_numhats()):
            hat_value = joystick.get_hat(i)
            if(hat_value==(0,1)):
                print("up")
                command=f"\nStepper 1 200 Forward\n"
                client_socket.sendall(command.encode())
                time.sleep(0.3)
            if(hat_value==(0,-1)):
                print("down")
                command=f"\nStepper 1 200 Backward\n"
                client_socket.sendall(command.encode())
                time.sleep(0.3)
            # time.sleep(0.01)
            
            # print(f"Hat {i}: {hat_value}")
        time.sleep(0.02)
    
    
    
    
    
    

# Close the connection

except Exception as ex:
    print(ex)
    print("Exiting.")
    pygame.quit()
    client_socket.close()
