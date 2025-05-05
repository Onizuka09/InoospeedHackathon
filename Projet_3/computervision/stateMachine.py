
from enum import Enum

class State(Enum):
    IDLE = 0
    GATE = 1
    LED = 2
    SERVO = 3
    ERROR = 4


class StateMachine:
    def __init__(self):
        self.st = State.IDLE 

    def set_state(self, State):
        self.st = State

         

    def get_state(self): 
        return self.st 