# Instructions

- Compile the module by running
  ```bash
  make
  ```

- Load the module
  ```bash
  sudo insmod eudyptula.ko
  # Now the character device will be present in /dev/eudyptula
  # And a kthread [eudyptula] will be created
  ```

- Check if the kthread is created
  ```bash
  ps -aux | grep "\[eudyptula\]"
  ```

- Watch the kernel log for eudyptula messages
  ```bash
  watch -n1 "sudo dmesg | rg eudyptula | tail"
  ```

- Test the device by writing to it.
  ```bash
  echo "Alice" > /dev/eudyptula
  echo "Bob" > /dev/eudyptula
  sleep 15
  echo "Dave" > /dev/eudyptula
  echo "Gena" > /dev/eudyptula
  sudo rmmod eudyptula
  ```
  The rmmod command will only succeed when all the tasks on the list have been completed.

- Unload the module
  ```bash
  sudo rmmod eudyptula
  # Now the character device /dev/eudyptula and the kthread
  # [eudyptula] will not be present
  ```
