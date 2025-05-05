# Challenge 5 
This project involves an Al-powered emotion detection system using a camera to recognize facial expressions in real time. Based on the detected emotion, the system reacts accordingly:

- If a happy face is detected, a green LED lights up, a fan is activated, and a happy emoji appears on an LED matrix.

- If a sad face is detected, a blue LED lights up and a sad emoji is displayed on the LED
matrix.

If an angry face is detected, a red LED lights up and an angry emoji appears on the LED matrix.


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
