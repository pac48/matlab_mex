classdef Camera < handle

    properties(Access=public)
        name
        xfov
        aspect_ratio
        znear
        zfar
        transform
    end

    methods
        function obj = Camera(camera_struct)
            obj.name = camera_struct.name;
            obj.xfov = camera_struct.xfov;
            obj.aspect_ratio = camera_struct.aspect_ratio;
            obj.znear = camera_struct.znear;
            obj.zfar = camera_struct.zfar;
            obj.transform = camera_struct.transform;

        end

    end

end