#!bin/bash

CXX := g++
CXXFLAGS := -c
LD := g++
LDFLAGS := -lncurses -lform -std=c++14

NAME := WConfig
SRCDIR := src
OBJDIR := obj
DESTDIR = /usr/bin
HOMEDIR = $(HOME)/$(NAME)

SRC := $(wildcard $(SRCDIR)/*.cpp)
OBJ := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
EXE := $(NAME)

.PHONY: all clean

all: $(EXE)

clean:
	rm -f $(OBJ)
	rm -f $(EXE)
install: all
	sh -c "if [ ! -d $(HOMEDIR) ] ; then mkdir $(HOMEDIR) ; fi"
	cp $(EXE) $(DESTDIR)
	echo "Uzytkownicy;/etc/passwd" > $(HOMEDIR)/menu.conf
	echo "Aplikacja zainstalowana!"

$(EXE): $(OBJDIR) $(OBJ)
	$(LD) $(OBJ) $(LDFLAGS) -o $@

$(OBJDIR):
	mkdir $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.h
	$(CXX) $(CXXFLAGS) $< -o $@

