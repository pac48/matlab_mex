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

        positionTrackingEnabled
    end

    methods
        function obj = ZED_Camera(varargin)
            if mod(length(varargin),2) ~= 0
                error('each input name must be paired with a value')
            end

            obj.InitParameters.camera_resolution = 2; %HD720
            obj.InitParameters.camera_fps = 60;
            obj.InitParameters.coordinate_units = 2; %METER
            obj.InitParameters.depth_mode = 1; %PERFORMANCE
            %InitParameters.svo_input_filename = '../mySVOfile.svo'; % Enable SVO playback
            obj.InitParameters.depth_maximum_distance = 7;% Define maximum depth (in METER)
                        obj.requested_depth_size = [720 404];
            obj.InitParameters.coordinate_system = 3; %COORDINATE_SYSTEM_RIGHT_HANDED_Z_UP

            if ~mexZED('isOpened')
                result = mexZED('open', obj.InitParameters);
            else
                result = 'FAILURE';
            end
            obj.camera_open = strcmp(result, 'SUCCESS');

            if  ~obj.camera_open
                warning('ZED camera failed to open')
                return
            end

            obj.camInfo = mexZED('getCameraInformation');
            obj.image_size = [obj.camInfo.left_cam.width obj.camInfo.left_cam.height];
            obj.nbFrame = mexZED('getSVONumberOfFrames');
            obj.RuntimeParameters.sensing_mode = 0; % STANDARD sensing mode

            s = struct(varargin{:});
            obj.positionTrackingEnabled = isfield(s, 'enablePositionTracking') && s.enablePositionTracking;
            if obj.positionTrackingEnabled
                obj.enablePositionTracking();
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

        function enablePositionTracking(obj)
            %enable Tracking
            PositionalTrackingParameters.enable_spatial_memory = 1;
            %TrackingParameters.initial_world_transform = [1,0,0,0;0,1,0,0;0,0,1,0;0,0,0,1];
            mexZED('enablePositionalTracking', PositionalTrackingParameters);
            obj.positionTrackingEnabled = true;
        end

        function position = getPosition(obj)
            position = [];
            if obj.positionTrackingEnabled
                % retrieve camera Path
                %                 result = mexZED('grab', obj.RuntimeParameters); this
                %                 could cases problems
                %                 if(strcmp(result,'SUCCESS'))
                [position, state] = mexZED('getPosition');
                %                 end
            end
        end

        function resetPositionTracking(obj)
            if obj.positionTrackingEnabled
                mexZED('resetTracking'); % prob bad
            end
        end

        function delete(obj)
            if mexZED('isOpened')
                mexZED('close')
            end
        end
    end
end