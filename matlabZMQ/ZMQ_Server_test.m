% clear all;

% while 1
%     server = ZMQ_Server(5555, 100);
%     server = ZMQ_Server(5556, 100);
%     clear all;
% end
global server

server = ZMQ_Server(5555, 100, 'numpy');

server.send(rand(100,1));

clc
mat = reshape([1:12 1:12], 4, 3, 2)
mat.*mat
% matP = permute(mat,flip(1:length(size(mat))) );
server.send(mat);

t = timer(Period=.1, ExecutionMode='fixedRate');
t.TimerFcn = @(x,y) printServer();
start(t)


% while 1
% disp(server.recv());
% end


%% clean up
stop(timerfind)
pause(.1)
clear all

function printServer()
global server
disp(server.recv())
end