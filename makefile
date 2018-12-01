OOPTS = -g -std=c99 -Wall -Wextra -c
LOPTS = -g -std=c99 -Wall -Wextra
DAOBJS = da.o da-0-0.o integer.o
CDAOBJS = cda.o test-cda4.o integer.o
QOBJS = queue.o cda.o test-queue.o integer.o
SETOBJS = set.o da.o set-0-2.o integer.o real.o string.o
BSTOBJS = tnode.o bst.o queue.o cda.o bst-0-4.o integer.o real.o
GSTOBJS = tnode.o bst.o queue.o cda.o gst.o gst-0-0.o integer.o real.o
RBTOBJS = tnode.o bst.o queue.o cda.o gst.o rbt-0-0.o integer.o real.o rbt.o
TREESOBJS = tnode.o bst.o gst.o rbt.o cda.o queue.o string.o real.o integer.o scanner.o interpreter.o trees.o
KRUSK = kruskal.o set.o da.o string.o integer.o scanner.o queue.o cda.o tnode.o bst.o gst.o rbt.o

TESTDIR = .testing/
TESTS = $(sort $(basename $(notdir $(wildcard $(TESTDIR)*.c))))
KRUSKAL-TESTS = ga-4-0 ga-4-1 ga-4-2 ga-4-3 ga-4-4 g-4-0 g-4-1 g-4-2 g-4-3 g-4-4 g-4-5 g-4-6 g-4-7 g-4-8 g-4-9 g-4-10

all : da cda queue bst gst rbt SET kruskal

copy: all
	cp -f *.o $(TESTDIR)
	cp -f *.h $(TESTDIR)
	cp -f kruskal $(TESTDIR)

test-kruskal: all copy
	for x in $(KRUSKAL-TESTS); do \
          echo; echo -------; echo $$x.expected; echo -------; cat $(TESTDIR)$$x.expected; \
          ./kruskal $(TESTDIR)$$x > $(TESTDIR)$$x.yours; \
          echo -------; echo $$x.yours; echo -------; cat $(TESTDIR)$$x.yours; echo -------; \
          cmp --silent $(TESTDIR)$$x.expected $(TESTDIR)$$x.yours && echo "PASSED" || echo "FAILED"; echo -------; \
  done

valgrind-kruskal: all copy
	for x in $(KRUSKAL-TESTS); do \
          valgrind --log-file=$(TESTDIR)$$x.valgrind kruskal $(TESTDIR)$$x; \
          echo; echo -------; echo $$x.valgrind; echo -------;  cat $(TESTDIR)$$x.valgrind; echo; \
  done

tester : all $(TESTS)
	for x in $(TESTS); do \
          echo; echo -------; echo $$x.expected; echo -------; cat $(TESTDIR)$$x.expected; \
          ./$(TESTDIR)$$x > $(TESTDIR)$$x.yours; \
          echo -------; echo $$x.yours; echo -------; cat $(TESTDIR)$$x.yours; echo -------; \
          cmp --silent $(TESTDIR)$$x.expected $(TESTDIR)$$x.yours && echo "PASSED" || echo "FAILED"; echo -------; \
  done

valgrind : all $(TESTS)
	for x in $(TESTS); do \
          valgrind --log-file=$(TESTDIR)$$x.valgrind $(TESTDIR)$$x; \
          echo; echo -------; echo $$x.valgrind; echo -------;  cat $(TESTDIR)$$x.valgrind; echo; \
  done

$(TESTS): %: $(TESTDIR)%.c copy
	gcc $(LOPTS) -o $(TESTDIR)$@ $< $(OBJS)

SET : $(SETOBJS)
	gcc $(LOPTS) $(SETOBJS) -o SET
da : $(DAOBJS)
	gcc $(LOPTS) $(DAOBJS) -o da
cda : $(CDAOBJS)
	gcc $(LOPTS) $(CDAOBJS) -o cda
queue : $(QOBJS)
	gcc $(LOPTS) $(QOBJS) -o queue
bst : $(BSTOBJS)
	gcc $(LOPTS) $(BSTOBJS) -o bst
gst : $(GSTOBJS)
	gcc $(LOPTS) $(GSTOBJS) -o gst
rbt : $(RBTOBJS)
	gcc $(LOPTS) $(RBTOBJS) -o rbt
trees : $(TREESOBJS)
	gcc $(LOPTS) $(TREESOBJS) -o trees
kruskal : $(KRUSK)
	gcc $(LOPTS) $(KRUSK) -o kruskal

set.o: set.c set.h
	gcc $(OOPTS) set.c
integer.o : integer.c integer.h
	gcc $(OOPTS) integer.c
real.o : real.c real.h
	gcc $(OOPTS) real.c
string.o : string.c string.h
	gcc $(OOPTS) string.c
scanner.o:	scanner.c scanner.h
	gcc $(OOPTS) scanner.c
da.o: da.c
	gcc $(OOPTS) da.c
cda.o : cda.c cda.h
	gcc $(OOPTS) cda.c
queue.o : queue.c queue.h cda.h
	gcc $(OOPTS) queue.c
tnode.o : tnode.c tnode.h
	gcc $(OOPTS) tnode.c
bst.o : bst.c bst.h tnode.h queue.h
	gcc $(OOPTS) bst.c
gst.o : gst.c gst.h bst.h tnode.h queue.h
	gcc $(OOPTS) gst.c
rbt.o : rbt.c rbt.h tnode.h gst.h
	gcc $(OOPTS) rbt.c
trees.o : trees.c gst.h rbt.h string.h interpreter.h
	gcc $(OOPTS) trees.c
kruskal.o: kruskal.c set.h da.h rbt.h string.h integer.h
	gcc $(OOPTS) kruskal.c


set-0-2.o : set-0-2.c set.h da.h integer.h real.h string.h
	gcc $(OOPTS) set-0-2.c
da-0-0.o : da-0-0.c da.h
	gcc $(OOPTS) da-0-0.c
test-cda4.o : test-cda4.c cda.h
	gcc $(OOPTS) test-cda4.c
test-queue.o : test-queue.c queue.h cda.h
	gcc $(OOPTS) test-queue.c
bst-0-4.o : bst-0-4.c integer.h string.h queue.h bst.h real.h
	gcc $(OOPTS) bst-0-4.c
gst-0-0.o : gst-0-0.c integer.h string.h queue.h bst.h real.h
	gcc $(OOPTS) gst-0-0.c
rbt-0-0.o : rbt-0-0.c gst.h rbt.h integer.h real.h string.h
	gcc $(OOPTS) rbt-0-0.c


test : kruskal #SET #oldkruskal newkruskal #bst  rbt  cda queue
	#./da
	#./cda
	#./queue
	#./bst
	#./rbt
	#./SET
	./kruskal emptest


#valgrind : rbt #kruskal SET  bst rbt #trees cda queue
	#valgrind --leak-check=full ./da
	#valgrind --leak-check=full ./cda
	#valgrind --leak-check=full ./queue
	#valgrind --leak-check=full bst
	#valgrind --leak-check=full gst
	#valgrind --leak-check=full rbt
	#valgrind --leak-check=full kruskal
	#valgrind --leak-check=full SET

clean :
	rm -f $(BSTOBJS) $(GSTOBJS) $(RBTOBJS) $(QOBJS) $(DAOBJS) $(CDAOBJS) $(KRUSK) $(SETOBJS) \ #$(OKRUSH) $(NKRUSK)  \
	bst gst da cda queue rbt SET kruskal
