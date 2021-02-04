function recordVideo(filename, frame, format, frameRate)
    writer = VideoWriter(filename, format);
    writer.FrameRate = frameRate;
    open(writer);
    writeVideo(writer, frame);
    close(writer);
end