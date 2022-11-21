classdef Scene < handle

    properties(Access=public)
        lights = {};
        actors = {};
        meshes = {};
        cameras = {};

    end
    properties(Access=public)

    end

    methods
        function obj = Scene(filename)
            p = which('Scene.m');
            pathparts = strsplit(p, filesep);
            directory = fullfile(filesep, pathparts{1:end-1});

            caching = false;
            if ~caching
                warning('not caching human object');
            end
            if exist(fullfile(directory, 'cachedScenes.mat')) == 2 && caching
                keyboard
                %                 tmp = load([directory name '.mat']);
                %                 obj = tmp.obj;
            else
                [controllers, meshes, cameras, lights] = parseDAEMex(filename);
                obj.meshes = cellfun(@(x) Mesh(x), meshes);
                obj.lights = cellfun(@(x) Light(x), lights);
                obj.cameras = cellfun(@(x) Camera(x), cameras);
                obj.actors = cellfun(@(x) Human(x), controllers, 'UniformOutput', false);
                

            end

        end

    end


end