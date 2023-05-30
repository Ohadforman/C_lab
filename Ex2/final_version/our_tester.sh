#! /usr/bin/env tcsh

./my_grep href test.txt > output1.txt
grep href test.txt | diff output1.txt -

./my_grep .pdf test.txt > output1.txt
grep .pdf test.txt | diff output1.txt -

./my_grep .pdf -A 3 test.txt > output1.txt
grep .pdf -A 3 test.txt | diff output1.txt -

./my_grep href -b test.txt > output1.txt
grep href -b test.txt | diff output1.txt -

./my_grep li -c test.txt > output1.txt
grep li -c test.txt | diff output1.txt -

./my_grep li -A 1 test.txt > output1.txt
grep li -A 1 test.txt | diff output1.txt -

./my_grep a -i test.txt > output1.txt
grep a -i test.txt | diff output1.txt -

./my_grep li -n test.txt > output1.txt
grep li -n test.txt | diff output1.txt -

./my_grep li -b -n test.txt > output1.txt
grep li -b -n test.txt | diff output1.txt -

./my_grep li -b -n -c test.txt > output1.txt
grep li -b -n -c test.txt | diff output1.txt -

./my_grep href -v test.txt > output1.txt
grep href -v test.txt | diff output1.txt -

./my_grep asds -x test.txt > output1.txt
grep asds -x test.txt | diff output1.txt -

./my_grep random -x test.txt > output1.txt
grep random -x test.txt | diff output1.txt -

./my_grep -E 'h[0-9]' test.txt > output1.txt
grep -E "h[0-9]" test.txt | diff output1.txt -

./my_grep -E 'H[0-9]' test.txt > output1.txt
grep -E "H[0-9]" test.txt | diff output1.txt -

./my_grep -i -E 'H[0-9]' test.txt > output1.txt
grep -i -E "H[0-9]" test.txt | diff output1.txt -

./my_grep -E '(str1|str2)[0-9](str1|str2)' test.txt > output1.txt
grep -E "(str1|str2)[0-9](str1|str2)" test.txt | diff output1.txt -

./my_grep -E '(str1|str2).(str1|str2)' test.txt > output1.txt
grep -E "(str1|str2).(str1|str2)" test.txt | diff output1.txt -

./my_grep -E '(str1|str2)..(str1|str2)' test.txt > output1.txt
grep -E "(str1|str2)..(str1|str2)" test.txt | diff output1.txt -

./my_grep -E '\.\.\.' test.txt > output1.txt
grep -E "\.\.\." test.txt | diff output1.txt -

rm output1.txt