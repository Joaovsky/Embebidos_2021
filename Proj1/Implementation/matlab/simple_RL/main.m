function exec 

%Initialize Q table
N_states = 7;
N_actions = 5;
Q_table = zeros(N_states,N_actions);

%Randomize Q initial values
Q_table = randn(size(Q_table));

%Set initial parameters
Action = 3;                     %All Actuators OFF
Disc_factor = 0.5;              %0.9
Greed = 0.3;%0.8                %Using Greedy for exploration vs exploitation
Learn_rate = 0.3;%0.7           %
Total_episodes = 1000;%100
Episode_reward = zeros(Total_episodes,1);
max_q = 0;
Start_ep_decaying =1;
End_ep_decaying = fix(Total_episodes/2);
Greed_decay_val = Greed / (End_ep_decaying-Start_ep_decaying);

%Loop for each episode
for Episode=1:Total_episodes
    
%Set new initial current and reference temperatures 
init_cur_temp = (27-15)*rand + 15;
ref_temp = round((27-15)*rand + 15);
%ref_temp = 21;

%Initialize S
[cur_temp, cs, r] = environment(Action,init_cur_temp, ref_temp);

%Loop for each step of the episode
while(1)
    
%Choose A from S using policy derived from Q using 
probability = rand;
if rand > Greed
    [Chosen_value,Action] = max((Q_table(cs,:))');
else
    Action = randi(width(Q_table));
end

%Take action A, observe R, S'
[cur_temp, ns, r] = environment(Action,cur_temp, ref_temp);
Episode_reward(Episode) = Episode_reward(Episode)+r;

%Determine max Q value of given action and next state
[max_q] = max((Q_table(ns,:))');

%Update Q value from current state and action
Q_table(cs,Action) = (1-Learn_rate)*Q_table(cs,Action) + Learn_rate*(reward(cs,Action) + Disc_factor*max_q);

%S <- S'
cs = ns;

%If S = 1, temperature converged, break
if cs == 1
    break;
end
end

%Decrease exploration as Episode number increases. Exploit only when half
%of the total episodes are executed
if End_ep_decaying >= Episode >= Start_ep_decaying
    Greed = Greed - Greed_decay_val;
end
end

%Plot Average Reward for the first n episodes, 1<=n<=Total_episodes
%for n=1:Total_episodes
%hold on
%plot(n,sum(Episode_reward(1:n))/n, 'c+');
%Plot(episode, cur_temp, 'o');
%hold off
%end

%Save QTable with otimized values
%save('QTable_S7_A5.mat','Q_table');
end