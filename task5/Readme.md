# Instructions to make this work

- Compile the module by running
  $ make

- Copy the module to your modules folder
  $ sudo cp hello.ko /lib/modules/`uname -r`/

- Run Depmod for the kernel to detect the module
  $ sudo depmod -A

- Now, plug in a USB keyboard and you will see that the hello module has be loaded automatically.
