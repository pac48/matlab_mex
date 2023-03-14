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

    properties(Access=private)
        messageOut
    end

    properties
        ptr
        port 
        period 
        topic
    end

    methods(Static)
        function lobj = loadobj(obj)
            obj.ptr = mexZMQ(obj.INIT, obj.port, obj.period, obj.topic);
            obj.messageOut = false;
            pause(1)
            lobj = obj;
        end
    end

    methods
        function obj = ZMQ_Server(port, period, topic)
            obj.port = port;
            obj.period = period;
            obj.topic = topic;
            obj.ptr = mexZMQ(obj.INIT, port, period, topic);
            obj.messageOut = false;
            pause(1)
        end

        function sobj = saveobj(obj)
            % Call superclass saveobj method
%             sobj = saveobj@handle(obj);
%             obj.ptr = [];
%             obj.messageOut = [];
            sobj = obj;
        end

        function send(obj, arr)
            if(obj.messageOut)
                warning('only one message can be out at a time');
                out = [];
                ind = 0;
                while isempty(out) && ind < 10
                    out = recv(obj);
                    ind = ind + 1;
                    pause(.1)
                end
            end
            mexZMQ(obj.SEND, obj.ptr, arr);
            obj.messageOut = true;
        end

        function arr = recv(obj)
            arr = [];
            if (obj.hasNewMsg())
                obj.messageOut = false;
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