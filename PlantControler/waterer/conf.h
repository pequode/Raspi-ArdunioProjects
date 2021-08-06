#ifndef CONF_H
#define CONF_H
#include <ctime>
#include <iostream>
#include <fstream>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <bits/stdc++.h>

#include <pthread.h>
#include <semaphore.h>

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


#define GPIO_PIN1 1
#define GPIO_PIN2 4
#define GPIO_PIN3 5
#define GPIO_PIN4 6
#define LogPath "logs/log.txt"
#define ConfPath "conf/conf.txt"
#define DEBUG 0

int waterTimeInd = 0;
int waterRestartTimerInd = 1;
int waterRestartIntervalSec = 2;

int fanTimeInd = 3;
int fanRestartTimerInd = 4;
int fanRestartIntervalSec = 5;

int lightTimeInd = 6;
int lightRestartTimerInd = 7;
int lightRestartIntervalSec = 8;

int Water = GPIO_PIN1;
int Fans = GPIO_PIN2;
int Lights = GPIO_PIN3;

#endif

