# Instructions

- Compile the module by running
  ```bash
  make
  ```

- Copy the module to your modules folder
  ```bash
  sudo cp hello.ko /lib/modules/`uname -r`/
  ```

- Run Depmod for the kernel to detect the module
  ```bash
  sudo depmod -A
  ```

- Now, plug in a USB keyboard and you will see that the hello module has be loaded automatically.
