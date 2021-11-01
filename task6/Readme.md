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

- Read from character device. It will return "1234567"
  ```bash
  cat /dev/eudyptula
  # 1234567
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
  # Now the character device /dev/eudyptula will not be present
  ```
