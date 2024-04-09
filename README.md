Build
```
  mkdir -p catkin_ws/src
  git clone https://github.com/chobitsfan/my_udp_ros.git
  cd ../
  catkin_make
```

Config
```
  nano catkin_ws/src/udp_ros/src/udp_ros.launch
  #change IP address to IP address of your raspberry pi
```


Run
```
  source catkin_ws/devel/setup.bash
  roslaunch catkin_ws/src/udp_ros/src/udp_ros.launch
```
