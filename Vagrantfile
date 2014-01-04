API_VERSION="2"

Vagrant.configure(API_VERSION) do |config|

  vmname = "java8-box-%s" % Time.now.strftime("%Y%b%d-%Hh%Mm")

  # base vm configuration
  config.vm.hostname  = vmname
  config.vm.box       = "precise64"
  config.vm.box_url   = "http://files.vagrantup.com/precise64.box"
  config.vm.provider "virtualbox" do |vb|
    vb.name = vmname
    vb.customize ["modifyvm", :id, "--cpus", "2"]
    vb.customize ["modifyvm", :id, "--memory", "2048"]
  end

  # host folder sharing
  config.vm.synced_folder ".",      "/home/vagrant/java8_sandbox"
  config.vm.synced_folder "~/.m2",  "/home/vagrant/.m2"

  config.vm.provision :shell do |s| s.path = 'vagrant_init.sh' end

  config.ssh.forward_agent = true

end

