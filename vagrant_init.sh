#! /bin/bash

set -x

java8_dl="http://download.java.net/jdk8/archive/b120/binaries/jdk-8-ea-bin-b120-linux-x64-12_dec_2013.tar.gz?q=download"

sudo apt-get update
sudo apt-get install -y git vim tree screen curl

curl $java8_dl | tar -xz

echo "export PATH=\$PATH:\$HOME/jdk1.8.0/bin" >> /home/vagrant/.bashrc
