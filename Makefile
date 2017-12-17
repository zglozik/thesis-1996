
# Makefile for Language Tutor

compopt = -I/usr/X11R6/include
linkopt = -L/usr/X11R6/lib -lXm -lXt -lX11

ltobjects = unjumble.o dialoghalves.o simpleselect.o wordguess.o nougthsandcrosses.o\
	storyboard.o memory.o guesstitle.o dotest.o testedit.o exercises.o\
	testcollection.o exercisec.o valuation.o lt.o indexedfile.o\
	functionoftext.o testfile.o

lt: $(ltobjects)
	cc -o $@ $(ltobjects) $(linkopt)

clean:
	rm *.o

.c.o:
	cc -c $(compopt) $*.c

# Header files are coming

functionoftext.h: types.h

dialoghalves.h: types.h exercise.h
	touch $@

dotest.h: types.h exercise.h testcollection.h valuation.h
	touch $@

exercise.h: types.h
	touch $@

exercisec.h: types.h exercise.h unjumble.h dialoghalves.h simpleselect.h wordguess.h\
	storyboard.h memory.h guesstitle.h nougthsandcrosses.h indexedfile.h
	touch $@

exercises.h: types.h exercise.h exercisec.h valuation.h
	touch $@

guesstitle.h: types.h exercise.h functionoftext.h
	touch $@

lt.h: types.h unjumble.h dialoghalves.h simpleselect.h wordguess.h\
	storyboard.h memory.h guesstitle.h nougthsandcrosses.h indexedfile.h\
	exercises.h testedit.h dotest.h
	touch $@

memory.h: types.h exercise.h
	touch $@

nougthsandcrosses.h: types.h exercise.h
	touch $@

simpleselect.h: types.h exercise.h
	touch $@

storyboard.h: types.h exercise.h
	touch $@

testcollection.h: types.h exercise.h unjumble.h simpleselect.h dialoghalves.h wordguess.h\
	indexedfile.h testfile.h
	touch $@

testedit.h: types.h exercise.h testfile.h indexedfile.h
	touch $@

unjumble.h: types.h exercise.h
	touch $@

valuation.h: types.h
	touch $@

wordguess.h: types.h exercise.h
	touch $@

# Object files are coming

dialoghalves.o: dialoghalves.c dialoghalves.h

dotest.o: dotest.c dotest.h

exercisec.o: exercisec.c exercisec.h

exercises.o: exercises.c exercises.h

guesstitle.o: guesstitle.c guesstitle.h

indexedfile.o: indexedfile.c indexedfile.h

lt.o: lt.c lt.h

memory.o: memory.c memory.h

nougthsandcrosses.o: nougthsandcrosses.c nougthsandcrosses.h

simpleselect.o: simpleselect.c simpleselect.h

storyboard.o: storyboard.c storyboard.h

testcollection.o: testcollection.c testcollection.h

testedit.o: testedit.c testedit.h

unjumble.o: unjumble.c unjumble.h

valuation.o: valuation.c valuation.h

wordguess.o: wordguess.c wordguess.h

functionoftext.o: functionoftext.c functionoftext.h

testfile.o: testfile.c testfile.h
