##安装软件包

	sudo apt-get install bison build-essential curl flex git gnupg gperf libesd0-dev
	liblz4-tool libncurses5-dev libsdl1.2-dev libwxgtk2.8-dev libxml2 libxml2-utils lzop maven openjdk-7-jdk
	pngcrush schedtool squashfs-tools xsltproc zip zlib1g-dev
64位

	sudo apt-get install g++-multilib gcc-multilib lib32ncurses5-dev lib32readline-gplv2-dev
	lib32z1-dev
##同步源码

	curl https://storage.googleapis.com/git-repo-downloads/repo > ~/bin/repo
###添加权限

	chmod a+x ~/bin/repo

###添加到环境变量
	vim .profile

	PATH="$HOME/bin:$PATH"
###同步cm11分支
	repo init -u https://github.com/CyanogenMod/android.git -b cm-11.0

###开始同步
	repo sync

##获取默认应用
在vendor/cm目录下执行

	./get-prebuilts

##初始化环境变量

	source build/envsetup.sh

##获取相应驱动和内核
	breakfast aries
##提取专有文件
到/device/xiaomi/aries目录

将手机插上电脑adb执行以下脚本

	./extract-files.sh

##编译内核
进入到/kernel/xiaomi/aries目录
	make
##初始化编译选项

	lunch aosp_aries-userdebug

##编译recovery

	make recovery

##直接生成刷机包

	make otapackage -j8 