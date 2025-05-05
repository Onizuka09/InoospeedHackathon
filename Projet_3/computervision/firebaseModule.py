# firebase_module.py

import firebase_admin
from firebase_admin import credentials, db
door_ref = None 
led_ref = None 
gate_ref = None 
fstatus_ref = None 
fdistance_ref = None 
menu_ref = None 
# --- Firebase Initialization ---
def initialize_firebase(path, database_url):
    if not firebase_admin._apps:
        cred = credentials.Certificate(path)
        firebase_admin.initialize_app(cred, {'databaseURL': database_url})

# --- References ---
def get_refs():
    door_ref = db.reference("/door")
    led_ref = db.reference("/led")
    gate_ref = db.reference("/gate")
    fstatus_ref = db.reference("/fstatus")
    fdistance_ref = db.reference("/fdistance")
    menu_ref = db.reference("/menu")

    return door_ref, led_ref, gate_ref, fstatus_ref,fdistance_ref, menu_ref 

# --- Set Boolean States ---
# --- References ---
def update_firabase_data(menu,fstatus,fdistance):
    door_ref, led_ref, gate_ref, fstatus_ref,fdistance_ref, menu_ref = get_refs()
    
    # door_ref = db.reference("/door")
    # led_ref = db.reference("/led")
    # gate_ref = db.reference("/gate")
    fstatus_ref.set(fstatus)
    fdistance_ref.set(fdistance)
    menu_ref.set(menu)


def get_states():
    door_ref, led_ref, gate_ref = get_refs()
    return {
        'door': door_ref.get(),
        'led': led_ref.get(),
        'gate': gate_ref.get()
    }

