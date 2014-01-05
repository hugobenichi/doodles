#! /bin/bash

set -x


# config variables
java8_dl="http://download.java.net/jdk8/archive/b120/binaries/jdk-8-ea-bin-b120-linux-x64-12_dec_2013.tar.gz?q=download"
jdk8_path="/home/vagrant/jdk1.8.0/bin"
java_home="/home/vagrant/jdk1.8.0/"

ant_dl="http://ftp.yz.yamagata-u.ac.jp/pub/network/apache//ant/binaries/apache-ant-1.9.3-bin.tar.gz"
ant_path="/home/vagrant/apache-ant-1.9.3/bin"

jruby_dl="http://jruby.org.s3.amazonaws.com/downloads/1.7.9/jruby-bin-1.7.9.tar.gz"
jruby_path="/home/vagrant/jruby-1.7.9/bin"


# package installation
sudo apt-get update
sudo apt-get install -y git vim tree screen curl make
curl $java8_dl | tar -xz
curl $ant_dl | tar -xz
curl $jruby_dl | tar -xz


# env configuration
echo "export PATH=\$PATH:$jdk8_path:$jruby_path:$ant_path" >> /home/vagrant/.bashrc
echo "export JAVA_HOME=$java_home" >> /home/vagrant/.bashrc
export PATH=$PATH:$jdk8_path:$jruby_path:$ant_path
export JAVA_HOME=$java_home


# jruby configuration and gem installation
cd /home/vagrant/jruby-1.7.9/tool/nailgun/ && ./configure && make
(jruby --ng-server &)

jruby -S jgem install rspec shoulda bundler jbundler
