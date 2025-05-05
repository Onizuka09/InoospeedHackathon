# Challenge 1 

In this challenge, the objective is to establish communication between an ESP32 and an Arduino Uno using a protocol of your choice (such as 12C, SPI, or serial communication). The ESP32 is connected to a push button, while the Arduino controls three LEDs: red, green, and orange. When the button is pressed for 5 seconds, the red LED lights up. If the button remains pressed for 10 seconds, the red LED turns off and the green LED lights up instead. Continuing to hold the button for 15 seconds will turn off the green LED and activate the orange LED.


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
