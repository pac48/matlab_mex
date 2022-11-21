classdef Light < handle

    properties(Access=public)
        name
        color
        direction
        type
        transform
    end

    methods
        function obj = Light(light_struct)
            obj.name = light_struct.name;
            obj.color = light_struct.color;
            obj.direction = light_struct.direction;
            obj.type = light_struct.type;
            obj.transform = light_struct.transform;

        end

    end

end