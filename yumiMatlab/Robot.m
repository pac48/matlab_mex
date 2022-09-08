classdef Robot < handle

    properties(Constant, Access=private)
        INIT = 0
        DESTROY = 1
        SETJOINTS = 2
        GETJOINTS = 3
        GETJACOB = 4;
        GETTRANS = 5
        GETOPPOS = 6;
    end

    properties(Access=private)
        robotPtr
        verts
        vertsNormals
        vertsNames
        body2VertMap
        startInd
        directory
        timerObj
        hSpinner
        hSlider
        fig
    end

    properties
        name
        numBodies
        bodyNames
        bodyNamesMap
        numJoints
        jointNames
        jointNamesMap
        numEndEffector
        jointMinimums
        jointMaximums
        home = [(46.0 / 180.0) * 3.14;
            (-121.0 / 180.0) * 3.14;
            pi/4+(-87.0 / 180.0) * 3.14;
            (4.0 / 180.0) * 3.14;
            (44.0 / 180.0) * 3.14*0;
            (54.0 / 180.0) * 3.14;
            (-3.0 / 180.0) * 3.14*0;
            0;
            0;
            -(46.0 / 180.0) * 3.14;
            (-121.0 / 180.0) * 3.14;
            -(-87.0 / 180.0) * 3.14;
            (4.0 / 180.0) * 3.14;
            -(44.0 / 180.0) * 3.14*0;
            (54.0 / 180.0) * 3.14*0;
            (-3.0 / 180.0) * 3.14*0;
            0;
            0;
            ];
        miminJointMap
    end

    methods
        function obj = Robot(name)
            % Constructor for robot object. Creates a robot object pointed
            % to by robotPtr. This method also caches values for efficiency.

            assert(strcmp(name, 'yumi'), 'name must be a char array of a supported robot. Supported robots: yumi')
            obj.name = name;
            p = which('Robot.m');
            directory = p(1:max(find(p==filesep,9999)));
            [obj.robotPtr, obj.numJoints, obj.numBodies, obj.jointNames,...
                obj.bodyNames,obj.numEndEffector, obj.jointMinimums, obj.jointMaximums] ...
                = robot_mex(obj.INIT, fullfile(directory, name));

            obj.bodyNamesMap = containers.Map(obj.bodyNames, num2cell(1:obj.numBodies));
            obj.jointNamesMap = containers.Map(obj.jointNames, num2cell(1:obj.numJoints));
            obj.miminJointMap = containers.Map();
            for i = 1:length(obj.jointNames)
                jointName = obj.jointNames{i};
                if length(jointName) > 2 && strcmp(jointName(end-1:end), '_m')
                    tmp = cellfun(@(x) strcmp(x, jointName(1:end-2)), obj.jointNames);
                    is_mimic = any(tmp);
                    if is_mimic
                        ind = find(tmp,1);
                        mimicedJointName = obj.jointNames{ind};
                        obj.miminJointMap(mimicedJointName) = i;
                    end
                end
            end

            if exist([directory 'obj.mat']) == 2
                tmp = load([directory 'obj.mat']);
                obj.verts = tmp.verts;
                obj.vertsNames = tmp.vertsNames;
                obj.vertsNormals = tmp.vertsNormals;
            else
                [points, verts, vertsNormals, verticesTextureCoordinates, vertsNames] = object_loader([directory name '.obj']);
                vertsNames = cellfun(@(x) x(find(x =='_',1)+1:end), vertsNames,'UniformOutput',false);
                save([directory 'obj'], 'verts', 'vertsNames','vertsNormals');
                obj.verts = verts;
                obj.vertsNormals = vertsNormals;
                obj.vertsNames = vertsNames;
            end

            obj.body2VertMap = containers.Map();
            for b = 1:length(obj.bodyNames)
                tmp = cellfun(@(x) strcmp(obj.bodyNames{b}, x), obj.vertsNames);
                obj.body2VertMap(obj.bodyNames{b}) = find(tmp,1);
            end

            obj.setJoints(obj.home)
            obj.startInd = 1;

        end
        function delete(obj)
            % Deletes the allocated robot object. This is called
            % automatically when obj is deleted.

            robot_mex(obj.DESTROY, obj.robotPtr);
        end
        function setJoints(obj, varargin)
            % Sets the joint angles for the robot.
            if length(varargin)==1
                jointAngles = varargin{1};
            elseif length(varargin)==2
                jointAnglesIn = varargin{2};
                jointAngles = zeros(size(obj.home));
                jointNamesIn = varargin{1};
                for i = 1:length(jointNamesIn)
                    ind = obj.jointNamesMap(jointNamesIn{i});
                    jointAngles(ind) = jointAnglesIn(i);
                    if obj.miminJointMap.isKey(jointNamesIn{i})
                        jointAngles(obj.miminJointMap(jointNamesIn{i})) = jointAngles(ind);
                    end
                end
            else
                error('wrong nubmer of inputs')
            end

            jointAngles = reshape(jointAngles, [], 1);
            jointAngles = min(jointAngles, obj.jointMaximums);
            jointAngles = max(jointAngles, obj.jointMinimums);
            robot_mex(obj.SETJOINTS, obj.robotPtr, jointAngles);
        end
        function jointAgnles = getJoints(obj)
            % Gets the current joint angles of the robot

            jointAgnles = robot_mex(obj.GETJOINTS, obj.robotPtr);
        end
        function J = getJacobian(obj)
            % Gets the jacobian for the end effectors. J has 6 rows for
            % each end effector and one column for each joint.

            J = robot_mex(obj.GETJACOB, obj.robotPtr);
        end
        function T = getTransform(obj, arg)
            % Gets the tranform from the ith joint to global cooordinates.
            % The ordeer of the indexing order is the same as the order of
            % obj.jointNames
            if isa(arg, "double")
                i = arg;
            elseif isa(arg, "char")
                i = obj.bodyNamesMap(arg);
            else
                error("wrong input type")
            end
            T = robot_mex(obj.GETTRANS, obj.robotPtr, i);
        end
        function operationalPosition = getOperationalPosition(obj, i)
            % Gets the tranform from the ith end effector to global coordinates

            operationalPosition = robot_mex(obj.GETOPPOS, obj.robotPtr, i);
        end
        function [JeR, JwR, JeL, JwL] = getElbowWristJacobians(obj)
            % Gets the jacobians for the left and right elbor and wrist
            % joints. JeR, JwR, JeL, JwL all have 3 rows one column for
            % each joint.

            assert(strcmp(obj.name, 'yumi'), 'nMethod for yumi robot only');

            R = cell(14, 1);
            P = cell(14, 1);
            for i = 1:7
                % right
                T = obj.getTransform(i+1);
                R{i} = T(1:3, 1:3);
                P{i} = T(1:3, end);
                % left
                T = obj.getTransform(i+1+10);
                R{i+7} = T(1:3, 1:3);
                P{i+7} = T(1:3, end);
            end

            JeR = zeros(3, obj.numJoints);
            for j = 1:2
                Rj = R{j};
                JeR(:,j) = cross(Rj(:,3),P{3}-P{j});
            end
            JwR = zeros(3, obj.numJoints);
            for j = 1:4
                Rj = R{j};
                JwR(:,j) = cross(Rj(:,3),P{5}-P{j});
            end

            JeL = zeros(3, obj.numJoints);
            for j = 1:2
                Rj = R{j+7};
                JeL(:,j + obj.numJoints/2) = cross(Rj(:,3), P{3+7}-P{j+7});
            end
            JwL = zeros(3, obj.numJoints);
            for j = 1:4
                Rj = R{j+7};
                JwL(:,j + obj.numJoints/2) = cross(Rj(:,3),P{5+7}-P{j+7});
            end
        end
        function Jall = getAllJacobians(obj)
            % Gets the jacobians for all joints. Jall is a cell array of
            % jacobians for each joint. The sizes are 3 rows and one column
            % each joint.

            % right side
            jointOffset = 1;
            Jall = cell(length(obj.bodyNames), 1);
            for j = 1:(length(obj.bodyNames)+1)/2
                J = zeros(3, obj.numJoints);
                T = obj.getTransform(j);
                Ptip = T(1:3, end);
                for i = 1:(j-1-jointOffset)
                    % right
                    if contains(obj.bodyNames{i+jointOffset}, 'gripper')
                        continue
                    end
                    T = obj.getTransform(i+jointOffset);
                    R = T(1:3, 1:3);
                    P = T(1:3, end);
                    J(:, i) = cross(R(:, end) , Ptip - P);
                end
                Jall{j} = J;
            end
            % left side
            jointOffset = 10;
            for j = jointOffset+1:length(obj.bodyNames)
                J = zeros(3, obj.numJoints);
                T = obj.getTransform(j);
                Ptip = T(1:3, end);
                for i = 1:(j-1-jointOffset)
                    % right
                    if contains(obj.bodyNames{i+jointOffset}, 'gripper')
                        continue
                    end
                    T = obj.getTransform(i+jointOffset);
                    R = T(1:3, 1:3);
                    P = T(1:3, end);
                    J(:, i + length(obj.jointNames)/2) = cross(R(:, end) , Ptip - P);
                end
                Jall{j} = J;
            end

        end
        function plotObject(obj, varargin)
            % Display the 3D robot configuration. (mainly used for debugging)

            vNew = [];
            color = [];
            lighDir = [-1; -.5; -1];
            lighDir = lighDir./norm(lighDir);
            for i = 1:length(obj.bodyNames)
                ind = obj.body2VertMap(obj.bodyNames{i});
                tmp = obj.verts{ind};
                tmp2 = obj.vertsNormals{ind};
                T = obj.getTransform(i);
                vNew = cat(1, vNew, cat(2, tmp, ones(size(tmp,1), 1))*T');
                vNormNew = tmp2*T(1:3,1:3)';
                color = cat(1, color, max(-vNormNew*lighDir, .3));
            end
            vNew = vNew(:,1:3);
            T = reshape(1:size(vNew, 1), 3, [])';

            trisurf(T, vNew(:, 1), vNew(:, 2),  vNew(:, 3), color)
            axis equal
            ylim([-.7 .7])
            xlim([-.5 .9])
            zlim([-.1 1.2])
            if isempty(varargin)
                view(120, 15)
            else
                viewDir = varargin{1}; 
                view(viewDir)
            end
            colormap gray
            shading interp
        end
    end
end