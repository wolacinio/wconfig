#!bin/bash

CXX := g++
CXXFLAGS := -c
LD := g++
LDFLAGS := -lncurses -std=c++14

NAME := wconfig
SRCDIR := src
OBJDIR := obj
DESTDIR = /usr/bin
HOMEDIR = $(HOME)/.config
MANDIR = /usr/local/man/man1

SRC := $(wildcard $(SRCDIR)/*.cpp)
OBJ := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
EXE := $(NAME)

.PHONY: all clean

all: $(EXE)

clean:
	rm -f $(OBJ)
	rm -f $(EXE)
install: all
	@sh -c "if [ ! -d $(HOMEDIR) ] ; then mkdir $(HOMEDIR) ; fi"
	@sh -c "if [ ! -d $(MANDIR) ] ; then mkdir $(MANDIR) ; fi"
	@install -T $(EXE) $(DESTDIR)/$(NAME)
	@cp ./manual $(MANDIR)/$(NAME).1
	@echo "Uzytkownicy;/etc/passwd" > $(HOMEDIR)/.wconfig
	echo "Aplikacja zainstalowana!"

$(EXE): $(OBJDIR) $(OBJ)
	$(LD) $(OBJ) $(LDFLAGS) -o $@

$(OBJDIR):
	mkdir $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.h
	$(CXX) $(CXXFLAGS) $< -o $@

