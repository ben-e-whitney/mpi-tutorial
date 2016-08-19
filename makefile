CC=mpicc
CCFLAGS=-std=c99 -Wall -Werror
LDFLAGS=
DIR_INC=include
DIR_SRC=src
DIR_OBJ=obj
DIR_BIN=bin

DIR_SCR=scripts
DIR_DAT=data

ARPABET_MAKER=$(DIR_SCR)/schema_to_c
ARPABET_SCHEMA=$(DIR_DAT)/telephone/arpabet_schema.txt
ARPABET_H=$(DIR_INC)/telephone/arpabet.h
ARPABET_C=$(DIR_SRC)/telephone/arpabet.c

ARPABET_DICT_LATIN1_URL=http://svn.code.sf.net/p/cmusphinx/code/trunk/cmudict/cmudict-0.7b
ARPABET_DICT_LATIN1=$(DIR_DAT)/telephone/arpabet_latin1.txt
ARPABET_DICT_UTF8=$(DIR_DAT)/telephone/arpabet_utf8.txt

NORVIG_WORDS_URL=http://norvig.com/ngrams/count_1w100k.txt
NORVIG_WORDS=$(DIR_DAT)/telephone/norvig_words.txt
NORVIG_WORDS_TRIMMED=$(DIR_DAT)/telephone/norvig_words_trimmed.txt

ARPABET_FILTERER=$(DIR_SCR)/winnow_arpabet
ARPABET_DICT_COMMON=$(DIR_DAT)/telephone/arpabet_common.txt

.PHONY: hello hello-solution telephone telephone-solution regenerate-dictionary homework homework-solution clean

$(ARPABET_DICT_LATIN1):
	wget $(ARPABET_DICT_LATIN1_URL) --output-document=$@

$(ARPABET_DICT_UTF8): $(ARPABET_DICT_LATIN1)
	iconv --from-code latin1 --to-code utf8 < $< > $@

$(NORVIG_WORDS):
	wget $(NORVIG_WORDS_URL) --output-document=$@

$(NORVIG_WORDS_TRIMMED): $(NORVIG_WORDS)
	sed --expression 's/[^A-Z]//g' $< | head --lines 16K > $@

regenerate-dictionary: $(ARPABET_DICT_UTF8) $(NORVIG_WORDS_TRIMMED)
	echo ';;; THIS FILE HAS BEEN MODIFIED. SEE `README`.' > $(ARPABET_DICT_COMMON)
	$(ARPABET_FILTERER) $(NORVIG_WORDS_TRIMMED) $(ARPABET_DICT_UTF8) >> $(ARPABET_DICT_COMMON)

#This will also generate the header.
$(DIR_SRC)/telephone/arpabet.c: $(ARPABET_MAKER) $(ARPABET_SCHEMA) $(ARPABET_DICT_COMMON)
	$(ARPABET_MAKER) $(ARPABET_SCHEMA) $(ARPABET_DICT_COMMON) $(DIR_INC)/telephone/arpabet.h $@

hello: $(DIR_BIN)/hello/main
hello-solution: $(DIR_BIN)/hello/solution
telephone: $(DIR_BIN)/telephone/main
telephone-solution: $(DIR_BIN)/telephone/solution
homework: $(DIR_BIN)/homework/main
homework-solution: $(DIR_BIN)/homework/solution

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	mkdir --parents $(dir $@)
	$(CC) $(CCFLAGS) -I $(DIR_INC)/$(dir $*) -o $@ -c $<

$(DIR_BIN)/hello/main: $(DIR_OBJ)/hello/main.o
$(DIR_BIN)/hello/solution: $(DIR_OBJ)/hello/solution.o
$(DIR_BIN)/telephone/main: $(addprefix $(DIR_OBJ)/telephone/,arpabet.o main.o mutate.o parse.o word_similarity.o)
$(DIR_BIN)/telephone/solution: $(addprefix $(DIR_OBJ)/telephone/,arpabet.o solution.o mutate.o parse.o word_similarity.o)
$(DIR_BIN)/homework/main: $(addprefix $(DIR_OBJ)/homework/,main.o times_table.o)
$(DIR_BIN)/homework/solution: $(addprefix $(DIR_OBJ)/homework/,solution.o times_table.o)

$(DIR_BIN)/hello/main $(DIR_BIN)/hello/solution $(DIR_BIN)/telephone/main $(DIR_BIN)/telephone/solution $(DIR_BIN)/homework/main $(DIR_BIN)/homework/solution:
	mkdir --parents $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	$(RM) $(DIR_OBJ)/*/*.o $(DIR_BIN)/*/* $(ARPABET_H) $(ARPABET_C)
