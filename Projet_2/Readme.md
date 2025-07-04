# Challenge 2 

This challenge involves creating a smart door system that communicates via Wi-Fi using Firebase. When a user presses a virtual button in the Firebase interface, the door automatically opens. Additionally, a notification is sent to the Firebase user once someone passes through the door. The system also includes a people counter: each time a person enters, the total number of entries is displayed on either a 7-segment display or an LCD screen.

Link for the libraries used are 
- ESP32 Servo [link](https://github.com/jkb-git/ESP32Servo)
- firebase Client [link](https://github.com/mobizt/FirebaseClient)
- I2CLiguidCrytal [link](https://github.com/johnrickman/LiquidCrystal_I2C)
# Template PlatformIO
This repo is a template project, for using ESP32 with platformIO.
I use platform IO CLI. 

```bash
curl -fsSL -o get-platformio.py https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py
python3 get-platformio.py
```
then run the script 

```bash
cd /path-to-dir/where/get-platformio.py/is-located

# run it
python get-platformio.py

# update the envirronement path variable 
# Open .bashrc locate in the $HOME PATH 

export PATH="$PATH:$HOME/.platformio/penv/bin/"

# exit the file and source the .bashrc file 
source $HOME/.bashrc
```
> NOTE <br> 
> for more INFO visit the [plaftromIO](https://docs.platformio.org/en/latest/core/installation/methods/installer-script.html) website

By manipulating `platformio.ini`, you can set it to your needs. 

I have also provided a makefile for ease of use. 

Provided targets are: 

- `build` : to build project 
- `flash` to upload code to esp32 
- `generate_commands_json`: for intelicence in vim 
- `init_project`: initialises the project 
