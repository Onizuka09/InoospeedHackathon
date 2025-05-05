import cv2
import mediapipe as mp
import numpy as np
import threading
from stateMachine import *
from firebaseModule import * 
from secrets_1 import *  
# Right get choices 
# left implement the choce by distance between 2 fingers 


## -- init firebase 
initialize_firebase(jpath, fdatabaseURL)
SM = StateMachine() 

# board = pyfirmata.Arduino('/dev/ttyACM0')
mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_hands = mp.solutions.hands

# For webcam input:
cap = cv2.VideoCapture(0)

# Create a lock to control access to the thread
thread_lock = threading.Lock()

# Create a flag to indicate if the function is currently running
function_running = False

# Function to set servo angle without blocking
def myThreadFunc(fstate,distance):
    print(fstate,distance)
    x=SM.get_state()
    
    update_firabase_data(x.value,fstate,int(distance))
    # print(index) 
    # if (index == 1 ):  # gate
    #     print("gate selected")
    # elif (index == 2 ):  # LED ON 
    #     print("LED selected ")
    # elif (index == 3 ):  # LED ON 
    #     print("Servo selected ")

    # else :
    #     print("nothing to select ")
        

    # set_servo_angle(angle, index)

# Function to run your code on a separate thread
def run_my_function(fstate,distance):
    global function_running
    with thread_lock:
        function_running = True
        thread = threading.Thread(target=myThreadFunc,args=(fstate,distance))
        thread.start()

def calculate_distance(point1, point2):
    return np.sqrt((point1[0] - point2[0])**2 + (point1[1] - point2[1])**2)





"""
Main func stats here 
"""


with mp_hands.Hands(
    model_complexity=0,
    min_detection_confidence=0.5,
    min_tracking_confidence=0.5,
    max_num_hands=2) as hands:  # Set max_num_hands to 1 to track only one hand
    while cap.isOpened():
        distance= -1
        fstatus= "off"
        success, image = cap.read()
        image = cv2.flip(image, 1) 
        fingerCount = 0


        if not success:
            print("Ignoring empty camera image.")
            # If loading a video, use 'break' instead of 'continue'.
            continue

        # To improve performance, optionally mark the image as not writeable to
        # pass by reference.
        image.flags.writeable = False
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        results = hands.process(image)

        # Draw the hand annotations on the image.
        image.flags.writeable = True
        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

        # Initially set finger count to 0
        fingerCount = 0

        if results.multi_hand_landmarks:
            for i, hand_landmarks in enumerate(results.multi_hand_landmarks):
                #hand_landmarks = results.multi_hand_landmarks[0]  # Get the first (and only) tracked hand
                if hand_landmarks and results.multi_handedness[i].classification[0].label == "Right" :
                    # Set variable to keep landmarks positions (x and y)
                    handLandmarks = []

                    # Fill list with x and y positions of each landmark
                    for landmarks in hand_landmarks.landmark:
                        handLandmarks.append([landmarks.x, landmarks.y])

                    # Test conditions for each finger: Count is increased if finger is
                    # considered raised.
                    # Thumb: TIP x position must be lower than IP x position
                    if handLandmarks[4][0] < handLandmarks[3][0]:
                        fingerCount += 1
                        SM.set_state(State.IDLE)

                    # Other fingers: TIP y position must be lower than PIP y position,
                    # as the image origin is in the upper left corner.
                    if handLandmarks[8][1] < handLandmarks[6][1]:  # Index finger
                        fingerCount += 1


                    if handLandmarks[12][1] < handLandmarks[10][1]:  # Middle finger
                        fingerCount += 1


                    if handLandmarks[16][1] < handLandmarks[14][1]:  # Ring finger
                        fingerCount += 1

                    if handLandmarks[20][1] < handLandmarks[18][1]:  # Pinky
                        fingerCount += 1

            

                    # cv2.line(image, fingertip1, fingertip2, (0, 255, 0), 2)
                    

                if hand_landmarks and results.multi_handedness[i].classification[0].label == "Left":
                    # Initialize lists to store the coordinates of the fingertips
                    fingertip_coordinates = []

                    for i, landmark in enumerate(hand_landmarks.landmark):
                        # Convert landmark coordinates to pixel values
                        image_height, image_width, _ = image.shape
                        x, y = int(landmark.x * image_width), int(landmark.y * image_height)

                        if i == 4 or i == 8:  # Fingertips (e.g., landmarks[8] and landmarks[12])
                            fingertip_coordinates.append((x, y))

                        # Draw landmarks on the image
                        #cv2.circle(image, (x, y), 5, (0, 0, 255), -1)  # Draw a red circle at each landmark

                    if len(fingertip_coordinates) >= 2:
                        fingertip1 = fingertip_coordinates[0]
                        fingertip2 = fingertip_coordinates[1]

                        # Calculate and display the distance between the two fingertips
                        distance = calculate_distance(fingertip1, fingertip2)
                        # cv2.putText(image, f"Distance: {distance:.2f}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

                        # Draw a line between the fingertips
                        
        
                # Draw hand landmarks
                mp_drawing.draw_landmarks(
                    image,
                    hand_landmarks,
                    mp_hands.HAND_CONNECTIONS,
                    mp_drawing_styles.get_default_hand_landmarks_style(),
                    mp_drawing_styles.get_default_hand_connections_style())
        if (fingerCount== 1 ):  # gate
            SM.set_state(State.GATE)
            print("gate selected")
        elif (fingerCount == 2 ):  # LED ON 
            SM.set_state(State.LED)
            
        elif (fingerCount == 3 ):  # LED ON 
            SM.set_state(State.SERVO)
            
        else :
            print("nothing to select ")
            SM.set_state(State.IDLE)
        
        if (distance < 20 and distance >= 0 ):
            fstatus = "on"
        elif ( distance > 20 ):
            fstatus = "off"
        # Display finger count
        cv2.putText(image, str(fingerCount), (500, 450), cv2.FONT_HERSHEY_SIMPLEX, 3, (255, 0, 0), 10)
        cv2.putText(image, f"Distance: {distance:.2f}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
        cv2.putText(image, f"{SM.get_state()}", (250, 450), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
        cv2.putText(image, f"{fstatus}", (10, 450), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
        run_my_function(fstatus,distance)

        # Display image
        cv2.imshow('MediaPipe Right Hand Finger Count', image)
        if cv2.waitKey(5) & 0xFF == 27:
            break

cap.release()
cv2.destroyAllWindows()
