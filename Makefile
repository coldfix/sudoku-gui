# Global variables:
#

BIN = wxSudokuSolver
OBJ = description_txt.o html_templates.o sudApp.o sudEditor.o sudFrame.o sudGen.o sudgendata.o sudgrid.o sudokucreationdialog.o SudokuSave.o sudSolve.o waitdialog.o wxprec.o

CC = g++
OUT_DIR = release

CFLAGS = -Wall -O3
LFLAGS = -Wall -O3
#debug : CFLAGS = -Wall -g
#debug : LFLAGS = -Wall -g

WXCFLAGS = `wx-config --cxxflags`
WXLFLAGS = `wx-config --libs`


# Build targets:
#

default: all
all:     prepare $(OUT_DIR)/$(BIN)

$(OUT_DIR)/$(BIN): $(addprefix $(OUT_DIR)/, $(OBJ))
	$(CC) $(LFLAGS) -o $(OUT_DIR)/$(BIN) $^ $(WXLFLAGS)

clean:
	rm -f $(OUT_DIR)/*.o *~
	`test !(-d $(OUT_DIR))` || `rmdir $(OUT_DIR)`

prepare:
	`test -d $(OUT_DIR)` || `mkdir $(OUT_DIR)`


# Build rules:
#

$(OUT_DIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $< $(WXCFLAGS)


