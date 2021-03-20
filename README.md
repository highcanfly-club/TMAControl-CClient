TMAControl-CClient
  git clone --recursive https://github.com/eltorio/TMAControl-CClient.git
  ou via ssh
  git clone --recursive git@github.com:eltorio/TMAControl-CClient.git

  building on macos
  brew install openssl ao mpg123
  autoconf && ./configure --with-openssl=/usr/local/opt/openssl && make
  ./tmaClient  https://tmalille31.highcanfly.club/tmastatesecuredmessage
