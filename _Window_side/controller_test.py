import pygame
import time

def main():
    # Initialize Pygame
    pygame.init()

    # Initialize the joystick module
    pygame.joystick.init()

    # Check for available joysticks
    joystick_count = pygame.joystick.get_count()
    if joystick_count == 0:
        print("No joysticks found.")
        return

    # Initialize the first joystick
    joystick = pygame.joystick.Joystick(0)
    joystick.init()

    print("Joystick Name:", joystick.get_name())

    try:
        # Main loop
        while True:
            # Check for events
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    return

            # Poll for joystick events
            pygame.event.pump()

            # Print joystick axes values
            for i in range(joystick.get_numaxes()):
                axis_value = joystick.get_axis(i)
                print(f"Axis {i}: {axis_value:.2f}")

            # Print button values
            for i in range(joystick.get_numbuttons()):
                button_value = joystick.get_button(i)
                print(f"Button {i}: {button_value}")

            # Print hat switch values
            for i in range(joystick.get_numhats()):
                hat_value = joystick.get_hat(i)
                print(f"Hat {i}: {hat_value}")
            time.sleep(1)

    finally:
        # Clean up Pygame
        pygame.quit()

if __name__ == "__main__":
    main()
