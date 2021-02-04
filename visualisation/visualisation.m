%% Parameters initialization

borderWidth = 1;
actionType = 4;
    % 0 - generate
    % 1 - pack
    % 2 - remove wall
    % 3 - shift wall
    % 4 - angle of repose
    
% % Video parameters
% timeDelay = 50;         % amount of frames to be skipped without rendering
% filename = 'video';     % name of file containing rendered video
% frameRate = 25;
% format = 'MPEG-4';    

%% Computation

initialize(actionType);
!C:\Users\Veronika\discrete-elements\model\cmake-build-debug\discrete-elements.exe

%% Rendering

if(actionType ~= 0)
    [trajectory, border, radius, time] = importParticles('output.txt', 'info.txt');
    N = size(trajectory, 2) / 2; %total amount of particles in the system
    fig = renderModel(trajectory, border, radius, borderWidth);
    
    % If video is needed
    % frame = renderModel(trajectory, border, radius, time, timeDelay, borderWidth);
    % recordVideo(filename, frame, format, frameRate);
else
    disp('Done');
end

if (actionType == 4) 
    [edgeX, edgeY] = importEdge();
    
    [angle, newX, newY] = aor(edgeX, edgeY, max(radius));
    highlightEdge(fig, newX, newY);
end