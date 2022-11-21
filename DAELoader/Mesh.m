classdef Mesh < handle

    properties(Access=public)
        name
        transform % 4x4 tranform for Mesh
        verts
        normals
        textureCoords
        RGB
        vertOrder

        texture
    end

    methods
        function obj = Mesh(mesh_struct)
            obj.name = mesh_struct.name;
            obj.transform = mesh_struct.transform;
            obj.verts = mesh_struct.verts';
            obj.normals = mesh_struct.normals';
            obj.textureCoords = mesh_struct.textureCoords';
            obj.vertOrder = mesh_struct.vertOrder;
            obj.RGB = .5*ones(size(obj.verts));

            if TextureMap.texture_map.isKey(obj.name)
                p = which('Mesh.m');
                pathparts = strsplit(p, filesep);
                directory = fullfile(filesep, pathparts{1:end-1});
                obj.texture = imread(fullfile(directory, 'data', TextureMap.texture_map(obj.name)) );
            end


        end

        function [verts, normals, RGB] = forwardRender(obj)
            verts = obj.transform(1:3,1:3)*obj.verts + obj.transform(1:3,end);
            n  = norm(obj.transform(1:3,1:3));
            normals = obj.transform(1:3,1:3)*obj.normals./n;

            if isempty(obj.texture)
                RGB = obj.RGB;
            else
%                 textureCoords = [obj.textureCoords(2,:); obj.textureCoords(1,:)];
                textureCoords = obj.textureCoords;

                textureCoords(2,:) = 1 - textureCoords(2,:);
%                 textureCoords(textureCoords < 0) = 0;
%                 textureCoords(textureCoords > 1) = 1; 

%                 inds = floor(textureCoords.*[size(obj.texture,2); size(obj.texture,1)]);
%                 indsLinear = inds(1,:) + (inds(2,:))*size(obj.texture,1)+1;


%                 indsLinear = floor(textureCoords(1,:)*(size(obj.texture, 1)-1)) + floor(textureCoords(2,:)*size(obj.texture, 1)*(size(obj.texture, 2)-1)  ) + 1;

                indx = floor(textureCoords(1,:)*(size(obj.texture,2)-1));
                indy = floor(textureCoords(2,:)*(size(obj.texture,1)-1));
                indsLinear = indy + size(obj.texture,1)*indx+1;

%                 subTexture = obj.texture(indy, indx,:);
                textureR = obj.texture(:,:,1);
                textureG = obj.texture(:,:,2);
                textureB = obj.texture(:,:,3);
                RGB = cat(1, textureR(indsLinear) , textureG(indsLinear) , textureB(indsLinear));
                RGB = single(RGB)./255;
            end

        end

    end


end