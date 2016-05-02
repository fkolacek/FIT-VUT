#!/bin/bash
bash ./reset.sh
bash ./runtests.sh ./tree >/dev/null; printf "EC = %d\n" $?
bash ./runtests.sh -v tree; printf "EC = %d\n" $?
bash ./runtests.sh -v tree trivial; printf "EC = %d\n" $?
bash ./runtests.sh -st ./tree/tests sort 2>/dev/null; printf "EC = %d\n" $?
(bash ./runtests.sh -t ./tree/tests 'sort|trivial/[^k]'; printf "EC = %d\n" $?) | cat
bash ./runtests.sh -crtv ./tree/tests sort >/dev/null; printf "EC = %d\n" $?
(bash ./runtests.sh -r -t ./tree/tests; printf "EC = %d\n" $?) | cat
