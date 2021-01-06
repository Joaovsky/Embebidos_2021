%function exec 

%Initialize Q table
N_states = 7;
N_actions = 5;
Q_table = zeros(N_states,N_actions);
%Q_table = [
%	-0.35936, -1.14788,  1.67857, -0.76064, -1.85203;
%	1.39516,  1.21616,  0.44050,  0.11162, -1.48797;
%	-1.24634, -1.19067,  1.18173, -1.24220, -1.41761;
%	-1.82511, 0.03337, -1.77000, -0.28798, -1.42736;
%	-0.3667, -1.00917, -0.79664, -1.73877, -1.73039;
%	-1.73912, -1.84652,  0.48050, 0.83011,  0.10665;
%   1.23423, -0.85150, -0.24404,  0.29522,  0.47182];


%Randomize Q initial values
Q_table = randn(size(Q_table));

%Q_table=load('QTable_S7_A5_w_greedy.mat').Q_table; 

%Set initial parameters
Action = 3;                     %All Actuators OFF
Disc_factor = 0.5;              %0.9
Greed = 0.5;%0.8                %Using Greedy for exploration vs exploitation
Learn_rate = 0.3;%0.7           %
Total_episodes = 100;%100
Episode_reward = zeros(Total_episodes,1);
max_q = 0;
Start_ep_decaying =1;
End_ep_decaying = fix(Total_episodes/2);
Greed_decay_val = Greed / (End_ep_decaying-Start_ep_decaying);
iteration = 0;

%Loop for each episode
for Episode=1:Total_episodes
    
%Set new initial current and reference temperatures 
init_cur_temp = (27-15)*rand + 15;
cur_temp = init_cur_temp;
ref_temp = round((27-15)*rand + 15);
%ref_temp = 21;

%Initialize S
[cur_temp, cs, r] = environment(Action,init_cur_temp, ref_temp);
ns = cs;
%Loop for each step of the episode
while(1)
    
%Choose A from S using policy derived from Q using 
probability = rand;
if rand > Greed
    [Chosen_value,Action] = max((Q_table(cs,:))');
else
    Action = randi(N_actions);
end

%Take action A, observe R, S'
[cur_temp, ns, r] = environment(Action,cur_temp, ref_temp);
Episode_reward(Episode) = Episode_reward(Episode)+r;

%Determine max Q value of given action and next state
[max_q] = max((Q_table(ns,:))');

%Update Q value from current state and action
Q_table(cs,Action) = (1-Learn_rate)*Q_table(cs,Action) + Learn_rate*(r + Disc_factor*max_q);
%As vezes a tabela nao atualiza

%S <- S'
cs = ns;
iteration = iteration+1;

%If S = 1, temperature converged, break
if cs == 1
    if Action == 3
        iteration = 0;
        break;
         
    end
end
end

%Decrease exploration as Episode number increases. Exploit only when half
%of the total episodes are executed
if End_ep_decaying >= Episode >= Start_ep_decaying
    Greed = Greed - Greed_decay_val;
end
end

%Plot Average Reward for the first n episodes, 1<=n<=Total_episodes
for n=1:Total_episodes
hold on
%plot(n,sum(Episode_reward(1:n))/n, 'c+');
plot(Episode, Episode_reward/Episode, 'c+', Episode, cur_temp, 'o');
%plot(Episode, cur_temp, 'o');
hold off
end

%Save QTable with otimized values
%save('QTable_S7_A5.mat','Q_table');
save('QTable_S7_A5_w_greedy.mat','Q_table');

%end