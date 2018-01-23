is_seqnum_sv is_seqnum_cl:is_seqnum_sv.o is_seqnum_cl.o
	gcc -o is_seqnum_sv is_seqnum_sv.o
	gcc -o is_seqnum_cl is_seqnum_cl.o

is_seqnum_sv.o: is_seqnum.h

is_seqnum_cl.o: is_seqnum.h