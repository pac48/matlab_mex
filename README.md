# matlab_mex
### build
```git clone https://github.com/pac48/matlab_mex.git
cd matlab_mex/cpp
mkdir build
cd build
```
### default build
```
cmake .. -DMATLAB_ROOT=/media/paul/MATLAB2022b/MATLAB/R2022b -DBUILD_ROBOT=1 -DBUILD_OBJ_LOADER=1 -DBUILD_ZMQ=1
make install
```

### Fix path
Add all folders other than cpp to the matlab path: matlab_mex/mex, matlab_mex/yumiMatlab, matlab_mex/matlabZMQ


