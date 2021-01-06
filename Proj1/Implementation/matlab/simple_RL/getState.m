function [s] = getState(ref,cur)
    
   %If temperature difference surpasses 15: 
    if (ref-cur) >= 15
        s=4;
        return
    end
    if (cur-ref) >= 15
        s=5;
        return
    end
    
    %Get State
    lookup_table = [5 6 7 0 0;5 6 7 0 0;5 6 7 0 0;5 6 7 0 0;5 6 1 3 4;0 0 2 3 4;0 0 2 3 4;0 0 2 3 4;0 0 2 3 4];
    s = lookup_table(5+fix(rem((ref-cur),5)),3+fix((ref-cur)/5));

end