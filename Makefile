CC:=xc8
CFLAGS:=-I. --chip=16F88 -Q -Wall

SRC:=decoder.c hd44780/hd44780.c morse.c timer.c
DEP:=hd44780_settings.h hd44780/hd44780.h morse.h morse_settings.h timer.h
PRS:=$(subst .c,.p1,$(SRC))
OBJ:=decoder.hex

all: $(OBJ)

$(OBJ): $(PRS)
	$(CC) $(CFLAGS) $^

$(PRS): %.p1: %.c $(DEP)
	$(CC) $(CFLAGS) -o$@ --pass1 $<

clean:
	$(RM) $(addprefix startup.,as lst obj rlf)\
		$(foreach s,$(SRC),$(addprefix $(subst .c,.,$(s)), as cmf cof d hex hxl lst obj p1 pre rlf sdb sym))
