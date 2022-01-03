obj-m := eudyptula.o
CFLAGS_eudyptula.o := -DDEBUG

all:
	make -C /usr/lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /usr/lib/modules/$(shell uname -r)/build M=$(PWD) clean
