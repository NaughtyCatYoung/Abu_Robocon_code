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
    while True:
        for event in pygame.event.get():
            #update value
            axes = [joystick.get_axis(i) for i in range(joystick.get_numaxes())]
            buttons = [joystick.get_button(i) for i in range(joystick.get_numbuttons())]
            hat = joystick.get_hat(0)
            if event.type == pygame.JOYAXISMOTION:
                # print("Axis", event.axis, "Value:", int(event.value*4096))
                if(event.axis==0 or event.axis==1):
                    #calculate angle
                    x_axis=axes[0]
                    y_axis=-axes[1]
                    angle=(math.degrees(math.atan(y_axis/x_axis)))%360
                    if(x_axis<0):
                        angle=(180+angle)%360
                    print(f"Angle = {angle}")
                    command=f"\nDC_encoder 1 {str(int(int(angle)*1200/360))}\n"
                    client_socket.sendall(command.encode())
                    time.sleep(0.02)
                    command=f"\nDC_encoder 2 {str(int(int(angle)*1200/360))}\n"
                    client_socket.sendall(command.encode())
                    time.sleep(0.01)
                    
                    
                elif(event.axis==5):
                    speed=min(255,(axes[5]+1)/2*255)
                    print(f"Speed = {speed}")
                    command=f"\nDC 1 Forward {max(min(int(speed),255),0)}\n"
                    client_socket.sendall(command.encode())
                    time.sleep(0.01)
                    command=f"\nDC 2 Forward {max(min(int(speed),255),0)}\n"
                    client_socket.sendall(command.encode())
                    time.sleep(0.01)
            elif event.type == pygame.JOYBUTTONDOWN:
                print("Button", event.button, "down.")
            elif event.type == pygame.JOYBUTTONUP:
                print("Button", event.button, "up.")
            elif event.type == pygame.JOYHATMOTION:
                print("Hat", event.hat, "Value:", event.value)
        time.sleep(0.1)
    
    
    
    
    
    

# Close the connection

except Exception as ex:
    print(ex)
    print("Exiting.")
    pygame.quit()
    client_socket.close()
