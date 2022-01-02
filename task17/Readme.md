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

- Write to character device. It will return "Invalid argument" error for everything except "1234567"
  ```bash
  echo -n "hello" > /dev/eudyptula
  # -bash: echo: write error: Invalid argument
  echo $?
  # 1
  echo -n "1234567" > /dev/eudyptula
  echo $?
  # 0
  ```

- Unload the module
  ```bash
  sudo rmmod eudyptula
  # Now the character device /dev/eudyptula and the kthread
  # [eudyptula] will not be present
  ```

Screenshot of module's working is shown below:
![Screenshot of working](screenshot.jpg)
