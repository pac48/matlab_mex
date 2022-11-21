classdef Human < handle

    properties(Access=public)
        name
        skin
        transform
        bindShapeMatrix
        bones
        influences
        bindMatrixInvsT
        boneConnections
        boneTransformsT
        boneRotations

    end

    methods
        function obj = Human(controller_struct)
            if isempty(controller_struct.skin.name)
                controller_struct.skin.name = [controller_struct.name '-mesh'];
            end

            obj.name = controller_struct.name;
            obj.skin = Mesh(controller_struct.skin);
            obj.transform = controller_struct.transform;
            obj.bindShapeMatrix= controller_struct.bindShapeMatrix;
            obj.bones = controller_struct.bones;
            obj.influences = controller_struct.influences;
            obj.bindMatrixInvsT = controller_struct.bindMatrixInvsT;
            obj.boneConnections = controller_struct.boneConnections;
            obj.boneTransformsT = controller_struct.boneTransformsT;

            boneRots = obj.boneTransformsT(1:3,1:3,:);
            obj.boneRotations = permute(boneRots,[2 1 3]);


            % load texture!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1

            %             assert(strcmp(name, 'female'), 'name must be a char array of a supported human. Supported robots: female')
            %             p = which('Human.m');
            %             pathparts = strsplit(p, filesep);
            %             directory = fullfile(filesep, pathparts{1:end-1});
            %
            %             caching = false;
            %             if ~caching
            %                 warning('not caching human object');
            %             end
            %             if exist([directory name '.mat']) == 2 && caching
            %                 tmp = load([directory name '.mat']);
            %                 obj = tmp.obj;
            %             else
            %             filename = fullfile(directory , 'data', [name '.dae']);
            %             [controllers, ~, ~, ~] = parseDAEMex(filename);
            %             assert(length(controllers)==1, 'ensure scene only has one armature in it');
            %             obj.controller = controllers{1};
            %             obj.texture = imread([directory 'data' '\texture.png']);
            %             obj.boneNames = obj.controller.bones;
            %             boneRots = obj.controller.boneTransformsT(1:3,1:3,:);
            %             obj.boneRotations = permute(boneRots,[2 1 3]);
            %             obj.transform = obj.controller.transform; % maybe broken
            %             save([directory name], 'obj');
            %             end

        end

        function [verts, normals, RGB, boneTransforms] = forwardRender(obj)

            %             controllerStruct = obj.controller;
            %             baseTransform = obj.transform;
            boneTransformsT = obj.boneTransformsT;
            boneTransformsT(1:3, 1:3,:) = permute(obj.boneRotations, [2 1 3]);

            %  T = controller.boneTransformsT(:,:,1)';
            %     T(1:3, 1:3) = T(1:3, 1:3)*eul2rotm([0 0 2*(rand(1)) ]); % eul2rotm([0 x 0]) is twist
            %     controller.boneTransformsT(:,:,1) = T';

            % % boneTransformsT(1:3, 1:3,2)'
            bones = [0 1]; % parent cur
            while ~isempty(bones)
                parent = bones(1);
                bone = bones(2);
                bones = bones(3:end);
                row = obj.boneConnections(bone, :);
                children = find(row ~=0 , 9999);
                newBones = cat(1, bone*ones(size(children)), children);
                bones = cat(2, bones, reshape(newBones,1,[]));

                if bone <= 1
                    continue
                end
                if parent == 1
                    T1 = eye(4);
                else
                    T1 = boneTransformsT(:,:, parent-1)';
                end
                boneTransformsT(:,:,bone-1) = ...
                    (T1*boneTransformsT(:,:,bone-1)')';
            end


            boneTransforms = permute(boneTransformsT,[2 1 3]);


            [verts, normals, RGB] = obj.skin.forwardRender();
            verts = verts';  
            normals = normals';

            tmp = cat(2, verts, ones(size(verts,1), 1))*obj.bindShapeMatrix';
            tmp2 = reshape(obj.influences(obj.skin.vertOrder, :), [], 1, size(boneTransformsT,3));
            allTranforms = pagemtimes(obj.bindMatrixInvsT, boneTransformsT);
            verts = pagemtimes(tmp, allTranforms);
            verts = verts.*tmp2;
            verts = sum(verts, 3);
            verts = obj.transform*verts';
            verts = verts(1:3, :);


            meanNorms = zeros(3, max(obj.skin.vertOrder));
            meanNorms = getMeanNormals_mex(normals, obj.skin.vertOrder, meanNorms);
            meanNorms = meanNorms(:, obj.skin.vertOrder)';


            tmp = cat(2, meanNorms, ones(size(meanNorms,1), 1))*obj.bindShapeMatrix';
            tmp2 = reshape(obj.influences(obj.skin.vertOrder, :), [], 1, size(boneTransformsT,3));
            normals = pagemtimes(tmp, allTranforms);
            normals = normals.*tmp2;
            normals = sum(normals, 3);
            normals = obj.transform*normals';
            normals = normals(1:3, :);
            normals = normals./sqrt(sum(normals.^2,1));





        end

    end


end