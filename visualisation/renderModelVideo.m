function frame = renderModelVideo(trajectory, border, radius, time, timeDelay, borderWidth)
% Return value is an array of frames needed to make a video or GIF-animation

    N = size(trajectory, 2) / 2; %total amount of particles in the system
    color = [180, 180, 180] ./ 255;
    frameCounter = 1;
    %initialBorder = border; % when the border is moving
    border(2) = border(2) * 2;

     for t = 1 : length(time)
%         border(3) = border(3) + 1e-5; % border speed
        if (mod(t, timeDelay) ~= 0)
            continue;
        end
        
        for j = 1 : N
            r = trajectory(t, (2 * j - 1) : 2 * j);
            drawParticle(r, radius(j), color);
            axis equal;
            axis(border);
            hold on;
        end
        drawBorders(border, borderWidth);
        drawnow;
        
        frame(frameCounter) = getframe;
        frameCounter = frameCounter + 1;
        clf;
    end
end