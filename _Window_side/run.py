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
left_stepper_moved=False
right_stepper_moved=False
ball_catched=False
threw=False
hand1=False
hand2=False

wheel_dir=[0,0]

rotate_left_was_pressed=False
rotate_right_was_pressed=False

rotate_speed=200
#================================================================
try:
    x_axis=0.0
    y_axis=0.0
    x_axis_now=0.0
    y_axis_now=0.0
    speed=0.0
    speed_now=0.0
    #make robot stop when start
    client_socket.sendall("\nDC 1 Forward 0\n".encode())
    time.sleep(0.2)
    client_socket.sendall("\nDC 2 Forward 0\n".encode())
    time.sleep(0.2)

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
            if(i==2):
                xx=axis_value
            if(i==3):
                yy=-axis_value
            if(i==5):
                speed_now=(axis_value+1.0)*255/2
    
        if(x_axis_now!=x_axis or y_axis_now!=y_axis):
            x_axis=x_axis_now
            y_axis=y_axis_now
            amplitude=math.sqrt(x_axis*x_axis+y_axis*y_axis)

            # print(f"x-axis = {x_axis}")
            # print(f"y-axis = {y_axis}")
            # print(f"amplitude = {amplitude}")

            angle=0

            if(x_axis==0 and y_axis==0):
                angle=0
            elif(x_axis==0 and y_axis!=0):
                angle=90
            else:
                angle=(math.degrees(math.atan(y_axis/x_axis)))%360
                if(x_axis<0):
                    angle=(180+angle)%360

            # if(amplitude>=0.5):
            #     if (angle>337.5 and angle<=360) or (angle<=22.5 and angle>=0):
            #         wheel_dir=(1,-1,-1,1)
            #     elif (angle>22.5 and angle<=67.5):
            #         wheel_dir=(1,0,0,1)
            #     elif (angle>67.5 and angle<=112.5):
            #         wheel_dir=(1,1,1,1)
            #     elif (angle>112.5 and angle<=157.5):
            #         wheel_dir=(0,1,1,0)
            #     elif (angle>157.5 and angle<=202.5):
            #         wheel_dir=(-1,1,1,-1)
            #     elif (angle>202.5 and angle<=247.5):
            #         wheel_dir=(-1,0,0,-1)
            #     elif (angle>247.5 and angle<=292.5):
            #         wheel_dir=(-1,-1,-1,-1)
            #     else:
            #         wheel_dir=(0,-1,-1,0)

            # else:
            #     wheel_dir=(0,0,0,0)



            if amplitude>=0.5:
                # print("Start direction")
                if(angle>0 and angle<=180):
                    # print("Forward")
                    wheel_dir=(min(1.0,((180.0-angle)/180.0)),min(1.0,angle/180.0));
                else :
                    # print("Backward")
                    wheel_dir=(-min(1.0,(angle-180.0)/180.0),-min(1.0,(360.0-angle)/180.0))
            else :
                wheel_dir=(0,0)

        

        # Print button values
        for i in range(joystick.get_numbuttons()):
            button_value = joystick.get_button(i)
            if(i==4 and button_value==1):
                print("Rotate_left begin")
                wheel_dir=(-1,1)
                rotate_left_was_pressed=True
                time.sleep(0.01)
            if(i==4 and button_value==0 and rotate_left_was_pressed):
                print("Rotate_left stop")
                wheel_dir=(0,0)
                rotate_left_was_pressed=False
                time.sleep(0.01)
            if(i==5 and button_value==1):
                print("Rotate_right begin")
                wheel_dir=(1,-1)
                rotate_right_was_pressed=True
                time.sleep(0.01)
            if(i==5 and button_value==0 and rotate_right_was_pressed):
                print("Rotate_right stop")
                wheel_dir=(0,0)
                rotate_right_was_pressed=False
                time.sleep(0.01)
            if(i==1 and button_value==1):
                command=f"\nSwitch 2\n"
                client_socket.sendall(command.encode())
                time.sleep(0.5)
            if(i==2 and button_value==1):
                command=f"\nSwitch 1\n"
                client_socket.sendall(command.encode())
                time.sleep(0.5)
            if(i==6 and button_value==1):
                if(left_stepper_moved):
                    print("Hand move back")
                    command=f"\nStepper 2 500 Forward\n"
                    client_socket.sendall(command.encode())
                else :
                    print("Hand move forward")
                    command=f"\nStepper 2 500 Backward\n"
                    client_socket.sendall(command.encode())
                left_stepper_moved=left_stepper_moved^True
                time.sleep(1.2)
            if(i==7 and button_value==1):
                if(ball_catched==False):
                    print("catch_ball")
                    command=f"\nServo 1 180\n"
                    client_socket.sendall(command.encode())
                    time.sleep(0.1)
                    command=f"\nServo 2 0\n"
                    client_socket.sendall(command.encode())
                    time.sleep(0.1)
                    ball_catched=True
                else :
                    print("release ball")
                    command=f"\nServo 1 0\n"
                    client_socket.sendall(command.encode())
                    time.sleep(0.1)
                    command=f"\nServo 2 180\n"
                    client_socket.sendall(command.encode())
                    time.sleep(0.1)
                    ball_catched=False
                    time.sleep(0.5)

            if(i==3 and button_value==1):
                command=f"\nDC 5 Backward 255\n"
                client_socket.sendall(command.encode())
                time.sleep(0.3)
            if(i==0 and button_value==1):
                if(threw):
                    command=f"\nDC 5 Forward 0\n"
                    client_socket.sendall(command.encode())
                    time.sleep(0.3)
                    threw=False
                else:
                    command=f"\nDC 5 Forward 30\n"
                    client_socket.sendall(command.encode())
                    time.sleep(0.3)
                    threw=True
            

        # Print hat switch values
        for i in range(joystick.get_numhats()):
            hat_value = joystick.get_hat(i)
                
            if(hat_value==(0,-1)):
                print("Hand move forward")
                command=f"\nStepper 2 600 Backward\n"
                client_socket.sendall(command.encode())
                time.sleep(0.5);
            if(hat_value==(0,1)):
                print("Hand move back")
                command=f"\nStepper 2 600 Forward\n"
                client_socket.sendall(command.encode())
                time.sleep(0.5)
            if(hat_value==(1,0)):
                if(hand1):
                    print("hand1_grip")
                    command=f"\nServo 3 0\n";
                    client_socket.sendall(command.encode())
                    time.sleep(0.5)
                    hand1=False
                else:
                    print("hand1_release")
                    command=f"\nServo 3 60\n";
                    client_socket.sendall(command.encode())
                    time.sleep(0.5)
                    hand1=True
            if(hat_value==(-1,0)):
                if(hand2):
                    print("hand2_grip")
                    command=f"\nServo 4 10\n";
                    client_socket.sendall(command.encode())
                    time.sleep(0.5)
                    hand2=False
                else:
                    print("hand2_release")
                    command=f"\nServo 4 60\n";
                    client_socket.sendall(command.encode())
                    time.sleep(0.5)
                    hand2=True
        if(True):
            speed=speed_now
            for i in range(1,3):
                print(f"Wheel {i} = {max(min(int(speed),255),0)*wheel_dir[i-1]}")
                command=f"\nDC {i} Forward {max(min(int(speed),255),0)*wheel_dir[i-1]}\n"
                print(command)
                # client_socket.sendall(command.encode())
                time.sleep(0.01)

            # print(f"Hat {i}: {hat_value}")
        time.sleep(0.01)

# If something wrong Close the connection

except Exception as ex:
    print(ex)
    print("Exiting.")
    pygame.quit()
    # client_socket.close()
