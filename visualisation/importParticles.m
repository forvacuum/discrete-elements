function [trajectory, border, radius, time] = importParticles(trajectoryFilename, infoFilename)
%{ Return values:
%
%   trajectory: each row defines a coordinate at the related moment of time
%               odd column (2n - 1) defines the abscissa of the n-th particle
%               even column (2n) -- same for the ordinate   
%   border = [x_left, x_right, y_bottom, y_top] 
%   radius: array containing radii of given particles
%   time: time step array
%}

%{Files structure:
%
%       trajectoryFilename:
% t_0   x_0_1   y_0_1   ... ...   x_0_N y_0_N
% ...    ...     ...    ... ...    ...   ...
% t_max x_max_1 y_max_1 ... ... x_max_N y_max_N
%
%       infoFilename:
% x_left x_right y_bottom y_top a_1 ... a_N
%
%       energyFilename:
% t_0   E_0_1   ...    E_0_N
% ...   ...     ...    ...
% t_max E_max_1 ...    E_max_N
%}
    temp = importdata(trajectoryFilename, ' ');
    time = temp(:, 1);
    trajectory = temp(:, 2 : size(temp, 2));
    
    temp = importdata(infoFilename, ' ');
    border = temp(1, 1 : 4);
    radius = temp(1, 5 : size(temp, 2));
end