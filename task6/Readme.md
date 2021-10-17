# Instructions

- Compile the module by running
  ```bash
  make
  ```

- Load the module
  ```bash
  sudo insmod eudyptula.ko
  # Now the character device will be present in /dev/eudyptula
  ```

- Read from character device. It will return "nihaal"
  ```bash
  cat /dev/eudyptula
  # nihaal
  ```

- Write to character device. It will return "Invalid argument" error for everything except "nihaal"
  ```bash
  echo -n "hello" > /dev/eudyptula
  # -bash: echo: write error: Invalid argument
  echo $?
  # 1
  echo -n "nihaal" > /dev/eudyptula
  echo $?
  # 0
  ```

- Unload the module
  ```bash
  sudo rmmod eudyptula
  # Now the character device /dev/eudyptula will not be present
  ```
