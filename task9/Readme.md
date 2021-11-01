# Instructions

- Compile the module by running
  ```bash
  make
  ```

- Load the module
  ```bash
  sudo insmod eudyptula.ko
  # Now the eudyptula directory will be present under /sys/kernel/
  ```

- The /sys/kernel/eudyptula would contain 3 files:
  1. id file
    - Read from id file. It will return the ID
      ```bash
      cat /sys/kernel/eudyptula/id
      # 1234567
      ```

    - Write to character device. It will return "Invalid argument" error for everything except the correct ID
      ```bash
      sudo su
      echo -n "hello" > /sys/kernel/eudyptula/id
      # -bash: echo: write error: Invalid argument
      echo $?
      # 1
      echo -n "1234567" > /sys/kernel/eudyptula/id
      echo $?
      # 0
      ```

  2. jiffies file
    - Read from jiffies file. It will return the current value of the jiffies kernel variable
      ```bash
      cat /sys/kernel/eudyptula/jiffies
      # 4295638952
      ```

  3. foo file
    - Write to foo file as root user. We can write upto 4096 Bytes (page size)
      ```bash
      sudo su
      echo "Vanakkam" > /sys/kernel/eudyptula/foo
      ```

    - Read from foo file. It will return any string that was previously written by root user
      ```bash
      cat /sys/kernel/eudyptula/foo
      # Vanakkam
      ```

- Unload the module
  ```bash
  sudo rmmod eudyptula
  # Now the sysfs directory /sys/kernel/eudyptula will not be present
  ```
