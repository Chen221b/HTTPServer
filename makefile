is_seqnum_sv is_seqnum_cl:is_seqnum_sv.o is_seqnum_cl.o
	gcc -o is_seqnum_sv is_seqnum_sv.o
	gcc -o is_seqnum_cl is_seqnum_cl.o

is_seqnum_sv.o: is_seqnum.h r_w_func.c

is_seqnum_cl.o: is_seqnum.h r_w_func.c

clean:
	rm *.o