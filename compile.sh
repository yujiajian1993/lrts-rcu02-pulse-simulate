#!/bin/bash

cd ./build
cmake .. && make
cd ..
cd ./bin
sudo chmod 777 pulse_simulate
sudo ./pulse_simulate
