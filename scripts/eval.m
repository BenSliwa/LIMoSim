clc
clear all
close all

path = '~/inet/simulte/simulations/test_handover/V0.txt';

A = load(path);
n = 5;
subplot(n,1,1); plot(A(:,1),A(:,4)); grid on; box on;
subplot(n,1,2); plot(A(:,1),A(:,5)); grid on; box on;
subplot(n,1,3); plot(A(:,1),A(:,6)); grid on; box on;
subplot(n,1,4); plot(A(:,1),A(:,7)); grid on; box on;
subplot(n,1,5); plot(A(:,1),A(:,8)); grid on; box on;
