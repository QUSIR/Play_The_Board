##配置编译器
	mkdir ~/arm64-tc
	
###输入如下命令下载编译需要用到的组件压缩包 
	wget https://releases.linaro.org/14.09/components/toolchain/binaries/
	gcc-linaro-aarch64-linux-gnu-4.9-2014.09_linux.tar.xz
	
###然后输入如下命令解压上面下载的压缩包 
	
	tar --strip-components=1 -C ~/arm64-tc -xf gcc-linaro-aarch64-linux-gnu-4.9-2014.09_linux.tar.xz
	
###然后设置环境变量 
	
	export PATH=~/arm64-tc/bin:$PATH

##目前有两种版本选择，一种为Landing Team版本，另外一种为Reference Platform Build版本 

##下载Landing Team版本内核源码 
	git clone https://github.com/96boards/linux.git
	
	然后输入如下命令检出最新内核版本15.11 
	
	cd linux
	git checkout -b working-hikey 96boards-hikey-15.11

##如果使用Reference Platform Build版本内核源码，输入如下命令下载 

	git clone https://github.com/rsalveti/linux.git
	
	然后输入如下命令检出最新内核 
	
	git checkout reference-hikey-rebase

##最后输入如下命令开始编译内核 

	export ARCH=arm64
	export CROSS_COMPILE=aarch64-linux-gnu-
	export LOCALVERSION="-linaro-hikey"
	make distclean 
	make defconfig 
	make -j8 Image modules hi6220-hikey.dtb 2>&1 | tee build-log.txt

##使用以下命令编译内核模块 

	export PWD=`pwd`
	export INSTALL_MOD_PATH="$PWD/installed-modules"
	mkdir $INSTALL_MOD_PATH
	make -j8 modules_install