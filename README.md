# edgeDetector

## build:
#### install opencv
guide: [opencv 3.1](http://embedonix.com/articles/image-processing/installing-opencv-3-1-0-on-ubuntu/)
```bash
sudo apt-get install build-essential
sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev

cd /opt
git clone https://github.com/opencv/opencv.git
cd opencv
sudo git checkout 3.2.0
sudo mkdir release
cd release
sudo cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..
sudo make -j4
sudo make install
pkg-config --modversion opencv
```
### install tbb
```bash
sudo apt-get install libtbb-dev
```

## run
```bash
qmake edgeDetectors.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug && make -j4 
```
``
./edgeDetectors -i <input_dir> -o <output_dir> --threh 35 --threh_max 255 
``
or
``
./edgeDetectors -i <image> -o <output_dir> -g <YES/NO> 
``
