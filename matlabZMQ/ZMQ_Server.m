classdef ZMQ_Server < handle
    %UNTITLED Summary of this class goes here
    %   Detailed explanation goes here

    properties(Constant, Access=private)
        INIT = 0;
        CLOSE = 1;
        RECV = 2;
        SEND = 3;
        HAS_NEW_MSG = 4;
    end

    properties
        ptr
    end

    methods
        function obj = ZMQ_Server(port, period, topic)
            obj.ptr = mexZMQ(obj.INIT, port, period, topic);
            pause(1)
        end

        function send(obj,arr)
            mexZMQ(obj.SEND, obj.ptr, arr);
        end

        function arr = recv(obj)
            arr = [];
            if (obj.hasNewMsg())
                arr = mexZMQ(obj.RECV, obj.ptr);
                arr = permute(arr, flip(1:length(size(arr ))) );
            end
        end
        function bool = hasNewMsg(obj)
            bool = mexZMQ(obj.HAS_NEW_MSG, obj.ptr) ~= 0;
        end

        function delete(obj)
            mexZMQ(obj.CLOSE, obj.ptr);
        end
    end
end