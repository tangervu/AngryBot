#Makefile for AngryBot!
#tuomas.angervuori@gmail.com
# $Id$

angrybot: angrybot.cc irc.cc irc.hh network.cc network.hh tools.hh tools.cc
	g++ -Wall -pedantic -std=c++11 -o angrybot angrybot.cc irc.cc network.cc tools.cc

