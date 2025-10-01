FROM ubuntu:noble

LABEL org.opencontainers.image.authors="wongy@hawaii.edu,henric@hawaii.edu"

# add repositories
RUN apt-get update

# set timezone
RUN echo "America/Los_Angeles" > /etc/timezone && export DEBIAN_FRONTEND=noninteractive && apt-get install -y tzdata

# build environment and utilitie
RUN apt-get -y install pkg-config git cmake cmake-data wget sudo curl vim

# install python
RUN apt-get install -y python3.12
RUN apt-get install -y python3-pip

# install compiler and boost
RUN apt-get -y install gcc g++
RUN apt-get -y install libboost-all-dev

# set root's environment variable
ENV CXX="g++" CC="gcc"
WORKDIR /tmp

# install SimGrid
RUN wget --no-check-certificate https://framagit.org/simgrid/simgrid/-/archive/v4.0/simgrid-v4.0.tar.gz && tar -xf simgrid-v4.0.tar.gz && cd simgrid-v4.0 && mkdir build && cd build && cmake .. && make -j1  && make install && ldconfig && cd ../.. && rm -rf simgrid-v4.0*

# install SimGrid's FSMod
RUN wget https://github.com/simgrid/file-system-module/archive/refs/tags/v0.3.tar.gz && tar -xf v0.3.tar.gz && cd file-system-module-0.3 && mkdir build && cd build && cmake .. && make  && make install && ldconfig && cd ../.. && rm -rf v0.3.tar.gz file-system-module-v*

# install json for modern c++
RUN wget https://github.com/nlohmann/json/archive/refs/tags/v3.11.3.tar.gz && tar -xf v3.11.3.tar.gz && cd json-3.11.3 && mkdir build && cd build && cmake .. && make -j2 && make install && ldconfig && cd ../.. && rm -rf v3.11.3* json-3.11.3

# install WRENCH 2.6
RUN wget https://github.com/wrench-project/wrench/archive/refs/tags/v2.6.tar.gz && tar -xzf v2.6.tar.gz && cd wrench-2.6 && mkdir build && cd build && cmake .. && make -j2 && make install && ldconfig && cd ../..


#################################################
# User
#################################################
RUN useradd -ms /bin/bash dockeruser
RUN adduser dockeruser sudo
RUN echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

USER dockeruser
WORKDIR /home/dockeruser

# set user's environment variable
ENV CXX="g++" CC="gcc"

# install Simcal
RUN git clone https://github.com/wrench-project/simcal.git && cd simcal && git pull && git checkout 86445d59177922fa3711473bbf4e5e207005fcc2 && sudo pip install -r requirements.txt --break-system-packages && sudo pip install . --break-system-packages &&  cd ..

# install Simulator
RUN git clone  https://github.com/wrench-project/pmbs2025_calibration_casestudy2_reproducibility && cd pmbs2025_calibration_casestudy2_reproducibility/simulator && make  -j2 && sudo make install && sudo ldconfig && cd ..

# install additional python dependencies
RUN pip install --break-system-packages pytimeparse pandas
