classdef ZED_Camera < handle
    %UNTITLED Summary of this class goes here
    %   Detailed explanation goes here

    properties
        camera_open

        camInfo
        image_size
        nbFrame

        InitParameters
        RuntimeParameters
        requested_depth_size
    end

    methods
        function obj = ZED_Camera()

            obj.InitParameters.camera_resolution = 2; %HD720
            obj.InitParameters.camera_fps = 60;
            obj.InitParameters.coordinate_units = 2; %METER
            obj.InitParameters.depth_mode = 1; %PERFORMANCE

            %InitParameters.svo_input_filename = '../mySVOfile.svo'; % Enable SVO playback
            obj.InitParameters.depth_maximum_distance = 7;% Define maximum depth (in METER)
            obj.requested_depth_size = [720 404];

            if ~mexZED('isOpened')
                result = mexZED('open', obj.InitParameters);
            else
                result = 'FAILURE';
            end
            obj.camera_open = strcmp(result, 'SUCCESS');

            if  ~obj.camera_open
                warning('ZED camera failed to open')
            else
                obj.camInfo = mexZED('getCameraInformation');
                obj.image_size = [obj.camInfo.left_cam.width obj.camInfo.left_cam.height];
                obj.nbFrame = mexZED('getSVONumberOfFrames');
                obj.RuntimeParameters.sensing_mode = 0; % STANDARD sensing mode
            end
        end

        function [image_left, image_right] = read_stereo(obj)
            if ~obj.camera_open
                image_left = [];
                image_right = [];
                return
            end

            result = mexZED('grab', obj.RuntimeParameters);
            if(strcmp(result,'SUCCESS'))
                % retrieve letf image
                image_left = mexZED('retrieveImage', 0); %left
                % retrieve right image
                image_right = mexZED('retrieveImage', 1); %right
            end
        end

        function [image_depth, depth] = read_depth(obj)
            if ~obj.camera_open
                image_depth = [];
                depth = [];
                return
            end

            result = mexZED('grab', obj.RuntimeParameters);
            if(strcmp(result,'SUCCESS'))
                % retrieve depth as a normalized image
                image_depth = mexZED('retrieveImage', 9); %depth
                % retrieve the real depth, resized
                depth = mexZED('retrieveMeasure', 1, obj.requested_depth_size(1), obj.requested_depth_size(2)); %depth
            end
        end

        function delete(obj)
            if mexZED('isOpened')
                mexZED('close')
            end
        end
    end
end