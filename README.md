# TMAControl-CClient

# Installation
## getting from github
```bash
git clone --recursive https://github.com/eltorio/TMAControl-CClient.git
#ou via ssh
git clone --recursive git@github.com:eltorio/TMAControl-CClient.git
```
## building on macos
```bash
brew install openssl ao mpg123
autoreconf --install --force && ./configure --with-openssl=/usr/local/opt/openssl --with-sound-files=`pwd`/sounds/ && make
./tmaClient  https://tmalille31.highcanfly.club/tmastatesecuredmessage
```
## building on Raspberry pi
```bash
sudo apt -y install autoconf automake gcc git libao-dev openssl libmpg123-dev
autoreconf --install --force && ./configure --with-sound-files=`pwd`/sounds/  && make
./tmaClient  https://tmalille31.highcanfly.club/tmastatesecuredmessage
```