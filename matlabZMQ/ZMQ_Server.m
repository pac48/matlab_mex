classdef ZMQ_Server < handle
    %UNTITLED Summary of this class goes here
    %   Detailed explanation goes here

    properties(Constant, Access=private)
        INIT = 0;
        CLOSE = 1;
        RECV = 2;
        SEND = 3;
    end

    properties
        ptr
    end

    methods
        function obj = ZMQ_Server(port, period, topic)
            obj.ptr = mexZMQ(obj.INIT, port, period, topic);

        end

        function send(obj,arr)
            mexZMQ(obj.SEND, obj.ptr, arr);
        end

        function arr = recv(obj)
            arr = mexZMQ(obj.RECV, obj.ptr);
        end

        function delete(obj)
            mexZMQ(obj.CLOSE, obj.ptr);
        end
    end
end