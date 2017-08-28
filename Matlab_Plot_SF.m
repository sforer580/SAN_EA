close all; clear all; clc

data_1 = load('Best_Fitness.txt');
size(data_1)
data_2 = load('Ave_Fitness.txt');
data_3 = load('Worst_Fitness.txt');

num_gen = 50;
Generation = 1:1:num_gen;

figure(1)
hold on;
subplot(3,1,1)
plot(Generation, data_1, 'LineWidth', 1.5)
axis([0 num_gen 0 0.5])
title('Best Fitness')
xlabel('Generation', 'FontSize', 20, 'FontWeight', 'bold')
ylabel('Fitness', 'FontSize', 20, 'FontWeight', 'bold')
set(gca,'FontSize',15)

subplot(3,1,2)
plot(Generation, data_2, 'LineWidth', 1.5)
axis([0 num_gen 0 1600])
title('Average Fitness')
xlabel('Generation', 'FontSize', 20, 'FontWeight', 'bold')
ylabel('Fitness', 'FontSize', 20, 'FontWeight', 'bold')
set(gca,'FontSize',15)

subplot(3,1,3)
plot(Generation, data_3, 'LineWidth', 1.5)
axis([0 num_gen 0 25000])
title('Worst Fitness')
xlabel('Generation', 'FontSize', 20, 'FontWeight', 'bold')
ylabel('Fitness', 'FontSize', 20, 'FontWeight', 'bold')
set(gca,'FontSize',15)