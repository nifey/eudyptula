For this task, I have added a sysfs file to *atkbd* driver that handles AT and PS2 keyboards.

The id files is then created under /sys/devices/platform/i8042/serio0 directory.

The path of the sys file may vary based on system configuration.

# Instructions
- Apply the patch, make and install the kernel
  ```bash
  git apply 0001-Add-sysfs-attribute-to-atkbd.patch
  sudo make
  sudo make install
  ```
- Make sure that atkbd module is loaded and you have a device that uses it
- Read from id file. It will return the ID
  ```bash
  cat /sys/devices/platform/i8042/serio0/id
  # 1234567
  ```
- Write to id file. It will return "Invalid argument" error for everything except the correct ID
  ```bash
  echo -n "hello" | sudo tee /sys/devices/platform/i8042/serio0/id
  # -bash: echo: write error: Invalid argument
  echo $?
  # 1
  echo -n "1234567" | sudo tee /sys/devices/platform/i8042/serio0/id
  echo $?
  # 0
  ```
