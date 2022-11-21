% GPU code generation for getting started example (mandelbrot_count.m)
%% Create configuration object of class 'coder.MexCodeConfig'.
cfg = coder.gpuConfig('mex');
cfg.GenerateReport = true;

% cfg.GpuConfig.SelectCudaDevice = 2;
% cfg.ComputeCapability =  '7.0';
% cfg.GpuConfig.CompilerFlags = '--fmad=false';
% cfg.GpuConfig.MallocMode = 'unified';

%% Define argument types for entry-point 'mandelbrot_count'.
% resolution = 256*4;
maxRes = 256*4;
maxVerts = 1000000;
% numVerts = inf;%41328;
% allVerts2d, allVertsCamera, ...
%      allColor, boundingBox, vertOrder, minDist, depthBuffer, numVert

ARGS = cell(1,1);
ARGS{1} = cell(7,1);
if false
    ARGS{1}{1} = coder.newtype('single', [maxRes maxRes 3], [1 1 0], 'gpu' , true); % im
    ARGS{1}{2} = coder.newtype('single', [maxRes maxRes], [1 1], 'gpu' , true); % depthBuffer
    ARGS{1}{3} = coder.newtype('int32', [maxRes maxRes], [1 1], 'gpu' , true); % vertInds
    ARGS{1}{4} = coder.newtype('single', [2 maxVerts], [0 1], 'gpu' , true); %allVerts2d
    ARGS{1}{5} = coder.newtype('single', [3 maxVerts], [0 1], 'gpu' , true); %allVertsCamera
    ARGS{1}{6} = coder.newtype('single', [3 maxVerts], [0 1], 'gpu' , true); %allColor
    ARGS{1}{7} = coder.newtype('single', [4 maxVerts], [0 1], 'gpu' , true); %boundingBox
    ARGS{1}{8} = coder.newtype('int32', [1 maxVerts], [0 1], 'gpu' , true); % vertOrder
    ARGS{1}{9} = coder.newtype('single', [1 maxVerts], [0 1], 'gpu' , true); %minDist
else
    %     resolution = 256*4;
    %     numVerts = 41328*10;
    numVerts = 1E6;
    maxTreeSize = 10000;
    binSize = 50;
    ARGS{1}{1} = coder.newtype('single', [2 numVerts], [0 1], 'gpu' , true); %allVerts2d
    ARGS{1}{2} = coder.newtype('single', [3 numVerts], [0 1], 'gpu' , true); %allVertsCamera
    ARGS{1}{3} = coder.newtype('single', [3 numVerts], [0 1], 'gpu' , true); %allColor
    ARGS{1}{4} = coder.newtype('single', [4 numVerts/3], [0 1], 'gpu' , true); %boundingBox
    ARGS{1}{5} = coder.newtype('single', [binSize+11 maxTreeSize], [0 1], 'gpu' , true); %tree
    ARGS{1}{6} = coder.newtype('single', [binSize maxTreeSize 4], [0 1 0], 'gpu' , true); %idxAll
    ARGS{1}{7} = coder.newtype('single', [4 numVerts/3], [0 1], 'gpu' , true); %distances
end

%% Invoke GPU Coder.
codegen -config cfg renderZBuffer -args ARGS{1}