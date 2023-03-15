%% init
tc = TinyCUDA();

%% num_params
tc.params


%% forward
input = gpuArray(zeros(2, 128,'single'));
output = tc.forward(input)

input = gpuArray(zeros(2, 128*200,'single'));
while 1
    tic
    output = tc.forward(input);
    toc
%     pause(.1)
end
% 0.000614 seconds (963.3911 hz) for batch of 6400 with 83968 parameters


%% destroy
clear tc