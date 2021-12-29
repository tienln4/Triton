# Triton
## Run
  docker run --shm-size=1g --ulimit memlock=-1 -p 8000:8000 -p 8001:8001 -p 8002:8002 --gpus all --ulimit stack=67108864 -ti nvcr.io/nvidia/tritonserver:21.07-py3

## install opencv
  pip3 install opencv-python
  
  apt-get update
  
  apt install -y libgl1-mesa-glx
## Daly
  https://github.com/triton-inference-server/dali_backend/tree/main/docs/examples/resnet50_trt
