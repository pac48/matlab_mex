%% init
tc = TinyCUDA();

%% num_params
params = tc.getParams()

tc.setParams(params);


%% forward
input = gpuArray(zeros(2, 128,'single'));
output = tc.forward(input)

input = gpuArray(zeros(2, 128*50,'single'));
while 1
    tic
    output = tc.forward(input);
    toc
%     pause(.1)
end
% 0.000614 seconds (963.3911 hz) for batch of 6400 with 83968 parameters
% 0.000155 seconds (6410.3 hz) for batch of 6400 with 83968 parameters on
% desktop

%% backward
input = gpuArray(zeros(2, 128,'single'));
output = tc.forward(input)
dL_doutput = gpuArray(rand(1, 128,'single'));

input = gpuArray(zeros(2, 128*50,'single'));
output = tc.forward(input);
[dL_dinput, dL_dparams]  = tc.backward(input, output, dL_doutput) 

while 1
    tic
    for i =1:5
    [dL_dinput, dL_dparams]  = tc.backward(input, output, dL_doutput); 
    end
    toc
%     pause(.1)
end
% 1355.7 hz for batch of 6400 with 83968 parameters

%% destroy
clear tc


