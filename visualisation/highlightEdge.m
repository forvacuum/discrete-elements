function highlightEdge(fig, edgeX, edgeY)
    
    tmp = importdata('grid.txt', ' ');
    workspace = tmp(1 : 4);
    gridWidth = tmp(5);
    gridHeight = tmp(6);
    
    ax = fig.CurrentAxes;
    
    scatter(edgeX, edgeY, 'filled');
    
    grid on;
    xticks(workspace(1) : gridWidth : workspace(2));
    yticks(workspace(3) : gridHeight : workspace(4));
    ax.GridColor = 'black';
    ax.Layer = 'top';
    
end