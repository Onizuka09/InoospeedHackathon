# Projt 4 
In this challenge, participants are invited to design and build a game of their choice such as Snake, Super Mario, or Guess the Word that runs primarily on an LCD screen and uses analog input components (like potentiometers, buttons, or joysticks).

Participants are free to add any additional components (e.g., LEDs, buzzers, sensors), and will earn extra points for each functional component creatively integrated into the game.

At the start of the game, the word "START" should appear on the LCD, and a Python program running on a laptop will play an audio message saying "Start", followed by back- ground music during gameplay. When the player loses, the LCD will display "GAME OVER", and the Python program will play a voice message saying "Game Over". 

Additionally, the player's score or play duration should be automatically sent and dis- played on a Firebase real-time databas


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
