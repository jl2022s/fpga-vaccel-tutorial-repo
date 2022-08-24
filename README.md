# Vaccel - FPGA lab 3

---------------------------------------
Using the _files in the build example first (non patched versions)

```
cp app/fpga_example
mkdir build
cd build
cmake ../
make
```


> $./sample/fpga_sample1
time = 0.005855 sec
time = 0.006024 sec
time = 0.006050 sec
time = 0.005999 sec
time = 0.006084 sec
time = 0.006012 sec
time = 0.006083 sec
time = 0.006032 sec
time = 0.006097 sec
time = 0.006028 sec
---------------------------------------
Applying patch are the versions with no _ as prefix

```
cd build
make
```
Producing a libsample1.so
```
cd ../../
make wrapper
```
Throws the same error as in lab3 then:
>LD_LIBRARY_PATH=fpga_example/build/sample/ ./wrapper 
>time = 0.006113 sec
time = 0.006091 sec
time = 0.006049 sec
time = 0.006073 sec
time = 0.005968 sec
time = 0.006102 sec
time = 0.006128 sec
time = 0.006092 sec
time = 0.006599 sec
time = 0.006110 sec
---------------------------------------
Again:

```
cd ../vaccelrt
mkdir build
cd build
cmake ../ -DBUILD_PLUGIN_EXEC=ON
make
cd ../../app
```

```
make libwrapper.so
make wrapper-vaccel
```
>LD_LIBRARY_PATH=.:../vaccelrt/build/src/ VACCEL_DEBUG_LEVEL=4 VACCEL_BACKENDS=../vaccelrt/build/plugins/exec/libvaccel-exec.so ./wrapper-vaccel 
2022.03.21-07:22:08.52 - <debug> Initializing vAccel
2022.03.21-07:22:08.52 - <debug> Registered plugin exec
2022.03.21-07:22:08.52 - <debug> Registered function noop from plugin exec
2022.03.21-07:22:08.52 - <debug> Registered function exec from plugin exec
2022.03.21-07:22:08.52 - <debug> Loaded plugin exec from ../vaccelrt/build/plugins/exec/libvaccel-exec.so
2022.03.21-07:22:08.52 - <debug> session:1 New session
Initialized session with id: 1
2022.03.21-07:22:08.52 - <debug> session:1 Looking for plugin implementing exec
2022.03.21-07:22:08.52 - <debug> Found implementation in exec plugin
2022.03.21-07:22:08.52 - <debug> Calling exec for session 1
2022.03.21-07:22:08.52 - <debug> [exec] library: fpga_example/build/sample/libsample1.so
2022.03.21-07:22:08.52 - <debug> [exec] symbol: sample1
time = 0.006046 sec
time = 0.006011 sec
time = 0.006079 sec
time = 0.006134 sec
time = 0.006073 sec
time = 0.006041 sec
time = 0.006021 sec
time = 0.006410 sec
time = 0.006146 sec
time = 0.006036 sec
2022.03.21-07:22:09.32 - <debug> session:1 Free session
2022.03.21-07:22:09.32 - <debug> Shutting down vAccel
2022.03.21-07:22:09.32 - <debug> Cleaning up plugins
2022.03.21-07:22:09.32 - <debug> Unregistered plugin exec
---------------------------------------

