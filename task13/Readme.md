# Instructions
The patch required to add kmem\_cache to task 12 code is given as 0001-Add-kmem\_cache-for-Task-13.patch

- Apply the patch
  ```bash
  cd ..;
  git apply 0001-Add-kmem_cache-for-Task-13.patch
  cd task12;
  make clean;
  ```

- Compile the module by running
  ```bash
  make
  ```

- Load the module
  ```bash
  sudo insmod list.ko
  ```

- Check dmesg log for output
  ```bash
  sudo dmesg
  ```

- Check /proc/slabinfo for kmem\_cache entry
  ```bash
  sudo cat /proc/slabinfo | grep identity
  ```

- Unload the module
  ```bash
  sudo rmmod list
  ```
