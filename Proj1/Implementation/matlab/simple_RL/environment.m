function [cur_temp, state, rewards] = environment(action, last_temp_state, ref_temp)

%Calculate current temperature as function of last temperature state,
%temperature variation given as consequence of action and stochastic noise
%representing outside perturbations
temp_variation_per_action = [-1 -0.5 0 0.5 1];
%temp_variation_per_action = [-0.005 -0.0025 0 0.0025 0.005];
%temp_variation_per_action = [-0.005 -0.0025 0 0.0025 0.005];

cur_temp = last_temp_state + temp_variation_per_action(action);

%Determine state and reward
state = getState(ref_temp,cur_temp);        
rewards = reward(state,action);

end

