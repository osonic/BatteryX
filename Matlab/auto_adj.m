function [bar, init_counter, success] = auto_adj(buffer)
    
    max_value = max(buffer);
    bar = max_value * 0.5;

    % to-do  auto adj
    init_counter = 2000;
end