classdef Robot < handle
    % export obj as +z up and +y forward

    properties(Constant, Access=private)
        INIT = 0
        DESTROY = 1
        SETJOINTS = 2
        GETJOINTS = 3
        GETJACOB = 4
        GETBODYTRANS = 5
        GETJOINTTRANS = 6
        GETOPPOS = 7 % operational position
        GETJOINTTREE = 8
        REVOLUTE = 9
        PRISMATIC = 10
        FIXED = 11;

    end

    properties(Access=private)
        robotPtr
        verts
        vertsNormals
        vertsNames
        body2VertMap
        directory
        timerObj
        hSpinner
        hSlider
        fig
    end

    properties
        numJoints
        numBodies
        jointNames
        bodyNames
        numEndEffector
        jointMinimums
        jointMaximums
        home 
        bodies
        armInds
        armVerts2Body
        controlVertInds
        controlBodyInds
        intervals
        controlPoints
        name
    end

    methods
        function obj = Robot(name, homeDict, controlPoints)
            obj.name = name;
            obj.controlPoints = controlPoints;

            p = which('Robot.m');
            pathparts = strsplit(p, filesep);
            directory = fullfile(filesep, pathparts{1:end-1});
            urdfName = fullfile(directory, [name '.urdf']);
            objName = fullfile(directory, [name '.obj']);

            [obj.robotPtr, obj.numJoints, obj.numBodies, obj.jointNames,...
                obj.bodyNames,obj.numEndEffector, obj.jointMinimums, obj.jointMaximums] ...
                = robot_mex(obj.INIT, urdfName);
            [points, verts, vertsNormals, verticesTextureCoordinates, vertsNames] = object_loader(objName);
            vertsNames = cellfun(@(x) x(find(x =='_',1)+1:end), vertsNames,'UniformOutput',false);

            %             save([directory 'obj'], 'verts', 'vertsNames','vertsNormals');
            obj.verts = verts;
            obj.vertsNormals = vertsNormals;
            obj.vertsNames = vertsNames;

            obj.body2VertMap = containers.Map();
            for b = 1:length(obj.bodyNames)
                tmp = cellfun(@(x) strcmp(obj.bodyNames{b}, x), obj.vertsNames);
                obj.body2VertMap(obj.bodyNames{b}) = find(tmp,1);
                ind = find(tmp,1);
                if ~isempty(ind)
                    obj.bodies{b} = obj.verts{ind};
                end
            end

            obj.home = arrayfun(@(x) homeDict(x), obj.jointNames);
            obj.setJoints(obj.home)
            obj.intervals = cumsum(cellfun(@(x) size(x,1), obj.verts));
            obj.intervals = cat(1,1, obj.intervals);

            [verts, ~, ~] = obj.forwardRender();
            for i = 1:size(obj.controlPoints,2)
                tmp = sum((verts-obj.controlPoints(:,i)).^2,1);
                obj.controlVertInds(i) = find(tmp==min(tmp),1);
                obj.controlBodyInds(i) = find(obj.controlVertInds(i) > obj.intervals, 1, 'last' );
            end




            %                        [~, ~, ~, robotArmInds]  = obj.getRobotVerts();
            %             obj.armInds = robotArmInds(indsSawyer);
            %             obj.armVerts2Body = zeros(size(obj.armInds));


            %             return
            %
            %             tmp = load([pwd '\correspondenceNet\inds_sawyer.mat']);
            %             indsSawyer = tmp.inds; % inds specifying alligned arms verts
            %
            %
            %
            %             for i = 1:numel(obj.armInds)
            %                 b = find(obj.armInds(i) < intervals, 1);
            %                 if isempty(b)
            %                     b = length(intervals);
            %                 end
            %                 obj.armVerts2Body(i) = b;
            %             end

        end

        %         function [verts, TBase, TEnd, vertsInds]  = getRobotVerts(obj)
        %             obj.setJoints(obj.home)
        %
        %             [verts, ~, ~] = obj.forwardRender();
        %
        %             baseBodyInd = 7;
        %             endBodyInd = 22;
        %
        %             vertsInds = [];
        %             count = 0;
        %             for b = 1:length(obj.bodies)
        %                 if b >= baseBodyInd && b <= endBodyInd
        %                     vertsInds = cat(1, vertsInds,(1:size(obj.bodies{b}, 1))'+count);
        %                 end
        %                 count = count + size(obj.bodies{b}, 1);
        %             end
        %             verts = verts(:, vertsInds)';
        %             TBase = obj.getBodyTransform(baseBodyInd);
        %             TEnd = obj.getBodyTransform(endBodyInd);
        %         end
        function delete(obj)
            robot_mex(obj.DESTROY, obj.robotPtr);
        end
        function setJoints(obj, jointAngles)
            jointAngles = reshape(jointAngles, [], 1);
            jointAngles = min(jointAngles, obj.jointMaximums);
            jointAngles = max(jointAngles, obj.jointMinimums);
            robot_mex(obj.SETJOINTS, obj.robotPtr, jointAngles);
        end
        function jointAgnles = getJoints(obj)
            jointAgnles = robot_mex(obj.GETJOINTS, obj.robotPtr);
        end
        function J = getJacobian(obj)
            J = robot_mex(obj.GETJACOB, obj.robotPtr);
        end
        function T = getBodyTransform(obj, index)
            T = robot_mex(obj.GETBODYTRANS, obj.robotPtr, index);
        end
        function T = getJointTransform(obj, index)
            T = robot_mex(obj.GETJOINTTRANS, obj.robotPtr, index);
        end
        function operationalPosition = getOperationalPosition(obj, index)
            operationalPosition = robot_mex(obj.GETOPPOS, obj.robotPtr, index);
        end
        function [jointInds, jointTypes] = getJointTree(obj, index)
            [jointInds, jointTypes] = robot_mex(obj.GETJOINTTREE, obj.robotPtr, index);
        end
        %         function [JeR, JwR, JeL, JwL] = getElbowWristJacobians(obj)
        %             R = cell(14, 1);
        %             P = cell(14, 1);
        %             for i = 1:7
        %                 % right
        %                 T = obj.getBodyTransform(i+1);
        %                 R{i} = T(1:3, 1:3);
        %                 P{i} = T(1:3, end);
        %                 % left
        %                 T = obj.getBodyTransform(i+1+10);
        %                 R{i+7} = T(1:3, 1:3);
        %                 P{i+7} = T(1:3, end);
        %             end
        %
        %             JeR = zeros(3, obj.numJoints);
        %             for j = 1:2
        %                 Rj = R{j};
        %                 JeR(:,j) = cross(Rj(:,3),P{3}-P{j});
        %             end
        %             JwR = zeros(3, obj.numJoints);
        %             for j = 1:4
        %                 Rj = R{j};
        %                 JwR(:,j) = cross(Rj(:,3),P{5}-P{j});
        %             end
        %
        %             JeL = zeros(3, obj.numJoints);
        %             for j = 1:2
        %                 Rj = R{j+7};
        %                 JeL(:,j + obj.numJoints/2) = cross(Rj(:,3), P{3+7}-P{j+7});
        %             end
        %             JwL = zeros(3, obj.numJoints);
        %             for j = 1:4
        %                 Rj = R{j+7};
        %                 JwL(:,j + obj.numJoints/2) = cross(Rj(:,3),P{5+7}-P{j+7});
        %             end
        %         end
        %         function Jall = getAllJacobians(obj)
        %             % right side
        %             jointOffset = 1;
        %             Jall = cell(length(obj.bodyNames), 1);
        %             for j = 1:(length(obj.bodyNames)+1)/2
        %                 J = zeros(3, obj.numJoints);
        %                 T = obj.getBodyTransform(j);
        %                 Ptip = T(1:3, end);
        %                 for i = 1:(j-1-jointOffset)
        %                     % right
        %                     if contains(obj.bodyNames{i+jointOffset}, 'gripper')
        %                         continue
        %                     end
        %                     T = obj.getBodyTransform(i+jointOffset);
        %                     R = T(1:3, 1:3);
        %                     P = T(1:3, end);
        %                     J(:, i) = cross(R(:, end) , Ptip - P);
        %                 end
        %                 Jall{j} = J;
        %             end
        %             % left side
        %             jointOffset = 10;
        %             for j = jointOffset+1:length(obj.bodyNames)
        %                 J = zeros(3, obj.numJoints);
        %                 T = obj.getBodyTransform(j);
        %                 Ptip = T(1:3, end);
        %                 for i = 1:(j-1-jointOffset)
        %                     % right
        %                     if contains(obj.bodyNames{i+jointOffset}, 'gripper')
        %                         continue
        %                     end
        %                     T = obj.getBodyTransform(i+jointOffset);
        %                     R = T(1:3, 1:3);
        %                     P = T(1:3, end);
        %                     J(:, i + length(obj.jointNames)/2) = cross(R(:, end) , Ptip - P);
        %                 end
        %                 Jall{j} = J;
        %             end
        %         end
        function JControl = getControlPointsJacobians(obj)
            JControlPoints = obj.getPointJacobians(obj.controlPoints, obj.controlBodyInds);
            JControl = JControlPoints{1};
        end

        function JVerts = getPointJacobians(obj, controlPoints, bodyInds)
            % verts should be 3 x n, e.g. x1,y1,z1,x2,y2,z2...
            JVerts = cell(size(controlPoints,2), 1);

            jointInds = cell(length(obj.bodyNames), 1);
            jointTypes = cell(length(obj.bodyNames), 1);
            for b = 1:length(obj.bodyNames)
                [jointIndsb, jointTypesb] = obj.getJointTree(obj.bodyNames{b});
                jointInds{b} = jointIndsb;
                jointTypes{b} = jointTypesb;
            end

            for j = 1:length(JVerts)
                b = bodyInds(j); %obj.armVerts2Body(j);
%                 b = obj.body2VertMap(obj.bodyNames{bodyInds(j)});
                jointIndsb = jointInds{b};
                jointTypesb = jointTypes{b};

                J = zeros(6, obj.numJoints);
                Ptip = controlPoints(:, j);

                for i = 1:length(jointIndsb)
                    % right
                    if jointTypesb(i) == obj.FIXED
                        continue
                    end
                    if jointTypesb(i) == obj.REVOLUTE
                        T = obj.getJointTransform(jointIndsb(i));
                        R = T(1:3, 1:3);
                        P = T(1:3, end);
                        J(1:3, jointIndsb(i)) = cross(R(:, end) , Ptip - P);
                        J(4:6, jointIndsb(i)) = T(1:3,3);
                    end
                    if jointTypesb(i) == obj.PRISMATIC
                        T = obj.getJointTransform(jointIndsb(i));
                        J(1:3, jointIndsb(i)) = -T(1:3,1);
                        J(4:6, jointIndsb(i)) = 0;
                    end
                end
                JVerts{j} = J;
            end

        end

        function [verts, normals, RGB] = forwardRender(obj)
            numVerts = sum(cellfun(@(x) length(x), obj.verts));
            verts = zeros(numVerts, 4);
            normals = zeros(numVerts, 3);

            count = 0;
            for i = 1:length(obj.bodyNames)
                ind = obj.body2VertMap(obj.bodyNames{i});
                if isempty(ind)
                    continue
                end
                tmp = obj.verts{ind};
                tmp2 = obj.vertsNormals{ind};
                T = obj.getBodyTransform(i);
                inds = count+(1:size(tmp,1));
                verts(inds,:) = cat(2, tmp, ones(size(tmp,1), 1))*T';
                normals(inds,:)  = tmp2*T(1:3,1:3)';
                count = count + length(inds);
            end
            verts = verts(:,1:3)';
            normals = normals';
            RGB = .8*ones(size(normals));


        end

        function plotObject(obj)
            % mainly used for debugging
            vNew = [];
            color = [];
            lighDir = [-1; -.5; -1];
            lighDir = lighDir./norm(lighDir);
            for i = 1:length(obj.bodyNames)
                ind = obj.body2VertMap(obj.bodyNames{i});
                if isempty(ind)
                    continue
                end
                tmp = obj.verts{ind};
                tmp2 = obj.vertsNormals{ind};
                T = obj.getBodyTransform(i);
                vNew = cat(1, vNew, cat(2, tmp, ones(size(tmp,1), 1))*T');
                vNormNew = tmp2*T(1:3,1:3)';
                color = cat(1, color, max(-vNormNew*lighDir, .3));
            end
            vNew = vNew(:,1:3);
            T = reshape(1:size(vNew, 1), 3, [])';

            trisurf(T, vNew(:, 1), vNew(:, 2),  vNew(:, 3), color)
            axis equal
            ylim([-.7 .7])
            xlim([-.5 1.3])
            zlim([-.8 1.2])
            view(120, 15)
            colormap gray
            shading interp
        end
    end
end