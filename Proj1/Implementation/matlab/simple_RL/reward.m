function [rewards] = reward(state,action)

%Give reward according to the respective state
reward_per_state = [100 -1 -2 -5 -5 -2 -1];
reward_per_action = [2 1 0 1 2];

%Reward is a function of state and action
rewards =  reward_per_state(state) - 0.5*reward_per_action(action);

end