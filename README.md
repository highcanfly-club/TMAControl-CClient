# TMAControl-CClient

# Installation
  First of all you must have your Raspberry Pi computer up and connected to the Internet. There are plenty of tutorials for doing that. You can connect to the network via ethernet (easy), wifi, gprs/umts/lte or via exotic methods. 
  
  I use Ubuntu Server 20.04.2 LTS but with some minor adjustments it can work with any distro.
  Why this distro ? Because it is a long term supported version, security holes are rapidly treated and… because I use it everywhere !

  I use a Motorola professional VHF
  3.5mm audio output is connected to the accessory port pin #2 (Audio In 80mV RMS) and pin #7 (GND)
  rpi3b+ pin #40 (GPIO.29) is conencted via a Sharp PC817c optoisolator to accessory pin #3 (active low)

  Orcad Capture Lite (free version) project can be found in hardware sub directory

  ![schematic](https://github.com/eltorio/TMAControl-CClient/blob/main/hardware/screenshot.png?raw=true)

  powering the Motorola radio is done via RPi pin #38 (GPIO.28) and a 12v relay drived via a NPN transistor, this is for avoiding to stay on emission in case of a crash (PTT is active low). So a crash on the RPi side can turn on the PTT.
  But with the security relay, if the rPi crash, the watchdog will reboot the Pi and the GPIO will be reset to 0 so the radio will be turn off. If app restarts, the PTT will be rise high and the radio will be turned on.

## getting from github
```bash
git clone --recursive https://github.com/eltorio/TMAControl-CClient.git
#ou via ssh
git clone --recursive git@github.com:eltorio/TMAControl-CClient.git
```
## building on macos
```bash
brew install openssl ao mpg123
autoreconf --install --force && ./configure --with-openssl=/usr/local/opt/openssl --with-sound-files=`pwd`/sounds/ --enable-debug --enable-inverted-output-logic=yes --with-pre-on=1000 --with-post-on=250 && make
./tmaClient  https://tmalille31.highcanfly.club/tmastatesecuredmessage
```
## building on Raspberry pi (Ubuntu Server 20.04.2 LTS)
  Default PTT pin numbered wPI=29 (physical 40 on RPi3b+), use gpio readall command for getting correct numbering (wPi column) : 
```bash
#on a Raspberry Pi 3b+
gpio readall
 +-----+-----+---------+------+---+---Pi 3B+-+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 |     |     |    3.3v |      |   |  1 || 2  |   |      | 5v      |     |     |
 |   2 |   8 |   SDA.1 | ALT0 | 1 |  3 || 4  |   |      | 5v      |     |     |
 |   3 |   9 |   SCL.1 | ALT0 | 1 |  5 || 6  |   |      | 0v      |     |     |
 |   4 |   7 | GPIO. 7 |   IN | 1 |  7 || 8  | 1 | ALT5 | TxD     | 15  | 14  |
 |     |     |      0v |      |   |  9 || 10 | 1 | ALT5 | RxD     | 16  | 15  |
 |  17 |   0 | GPIO. 0 |   IN | 0 | 11 || 12 | 0 | IN   | GPIO. 1 | 1   | 18  |
 |  27 |   2 | GPIO. 2 |   IN | 0 | 13 || 14 |   |      | 0v      |     |     |
 |  22 |   3 | GPIO. 3 |   IN | 0 | 15 || 16 | 0 | IN   | GPIO. 4 | 4   | 23  |
 |     |     |    3.3v |      |   | 17 || 18 | 0 | IN   | GPIO. 5 | 5   | 24  |
 |  10 |  12 |    MOSI | ALT0 | 0 | 19 || 20 |   |      | 0v      |     |     |
 |   9 |  13 |    MISO | ALT0 | 0 | 21 || 22 | 0 | IN   | GPIO. 6 | 6   | 25  |
 |  11 |  14 |    SCLK | ALT0 | 0 | 23 || 24 | 1 | OUT  | CE0     | 10  | 8   |
 |     |     |      0v |      |   | 25 || 26 | 1 | OUT  | CE1     | 11  | 7   |
 |   0 |  30 |   SDA.0 |   IN | 1 | 27 || 28 | 1 | IN   | SCL.0   | 31  | 1   |
 |   5 |  21 | GPIO.21 |   IN | 1 | 29 || 30 |   |      | 0v      |     |     |
 |   6 |  22 | GPIO.22 |   IN | 1 | 31 || 32 | 0 | IN   | GPIO.26 | 26  | 12  |
 |  13 |  23 | GPIO.23 |   IN | 0 | 33 || 34 |   |      | 0v      |     |     |
 |  19 |  24 | GPIO.24 |   IN | 0 | 35 || 36 | 0 | IN   | GPIO.27 | 27  | 16  |
 |  26 |  25 | GPIO.25 |   IN | 0 | 37 || 38 | 0 | IN   | GPIO.28 | 28  | 20  |
 |     |     |      0v |      |   | 39 || 40 | 1 | OUT  | GPIO.29 | 29  | 21  |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+---Pi 3B+-+---+------+---------+-----+-----+
```

 
```bash
git clone --recursive https://github.com/eltorio/TMAControl-CClient.git
mkdir -p /usr/local/share/TMAControl-CCLient/sounds
cp sounds/* /usr/local/share/TMAControl-CCLient/sounds/
sudo apt -y install make gcc autoconf libcurl4-openssl-dev libao-dev libmpg123-dev libssl-dev libwiringpi-dev wiringpi alsa-utils
autoreconf --install --force && ./configure --with-openssl=/usr --with-sound-files=/usr/local/share/TMAControl-CCLient/sounds/ --with-ptt-pin=29 --enable-inverted-output-logic=yes --with-pre-on=1000 -with-post-on=250 && make
./tmaClient  https://tmalille31.highcanfly.club/tmastatesecuredmessage
```
  * Note on --enable-inverted-output-logic=yes/no
  
    GPIO.29 is translated from 3.3V logic to 5V logic via a opto-isolator with NPN transistor output so when you put 0V on GPIO.29 raspberry pi you'll get +5V on PC817c pin 3. Note also that Motorola® Maxtrac radios use 0V for PTT radio ON and +5V for PTT radio OFF. So with --enable-inverted-output-logic=yes wich is default radio is in TX mode with GPIO.29=+3.3V and in rx mode GPIO.29=0V

  * Configure parameters:
  * --with-ptt-pin=29 , wiringpi number of the pin used to control the radio PTT
  * --with-pre-on=1000 -with-post-on=250 time in ms to on the radio beftore and after speaking


## security
  * first : if the certificate is not coming from an authority agreed on the client, libcurl reports the error
  ```bash
./tmaClient https://tmalille31.highcanfly.club/tmastatesecuredmessage
Error 60: SSL peer certificate or SSH remote key was not OK
SSL certificate problem: self signed certificate in certificate chain
````
  * second : if the message signature is not OK, for example the message has been altered, or signed with another key (key must be the same as the certificate)
  the error is reported

  * finally : the timestamp of the message is checked for testing it is useful to use --enable-debug flag in configure, it will show some useful messages. But for production without the flag the binary will be smaller and faster.

  ## server
   * see https://github.com/eltorio/TMAControl