##同步poky源代码
	git clone https://git.yoctoproject.org/git/poky
进入poky目录同步树莓派代码

	git clone https://git.yoctoproject.org/git/meta-raspberrypi

##初始化环境
	source oe-init-build-env raspberrypi

说明：指定目录raspberrypi

##编辑配置文件

	vim ./conf/local.conf 

将

	MACHINE ??= "raspberrypi2"
注释

	#PACKAGECONFIG_append_pn-qemu-native = " sdl"
	#PACKAGECONFIG_append_pn-nativesdk-qemu = " sdl"
	#ASSUME_PROVIDED += "libsdl-native"

文件末尾增加
	GPU_MEN = "16"

说明：16为CPU核数

	vim ./conf/bblayers.conf
修改成以下

	BBLAYERS ?= " \
	  /home/gsta/liang/poky/meta \
	  /home/gsta/liang/poky/meta-yocto \
	  /home/gsta/liang/poky/meta-yocto-bsp \
	  /home/gsta/liang/poky/meta-raspberrypi \
	  "
	##增加树莓派目录
	BBLAYERS_NON_REMOVABLE ?= " \
	  /home/gsta/liang/poky/meta \
	  /home/gsta/liang/poky/meta-yocto \
	  "
	

##生成
	bitbake rpi-basic-image

注意：同步过程很漫长，同步过程中一定要开启git代理和终端代理

整个同步下来大概有18G