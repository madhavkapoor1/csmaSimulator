./csma tests/Test1/input1.txt

if cmp -s tests/Test1/expectedOutput1.txt output.txt; then
    echo "Test 1: Output files are identical"
else
    echo "Test 1: Output files are different"
fi

./csma tests/Test2/input2.txt

if cmp -s tests/Test2/expectedOutput2.txt output.txt; then
    echo "Test 2: Output files are identical"
else
    echo "Test 2: Output files are different"
fi

./csma tests/Test3/input3.txt

if cmp -s tests/Test3/expectedOutput3.txt output.txt; then
    echo "Test 3: Output files are identical"
else
    echo "Test 3: Output files are different"
fi

./csma tests/Test4/input4.txt

if cmp -s tests/Test4/expectedOutput4.txt output.txt; then
    echo "Test 4: Output files are identical"
else
    echo "Test 4: Output files are different"
fi

./csma tests/Test5/input5.txt

if cmp -s tests/Test5/expectedOutput5.txt output.txt; then
    echo "Test 5: Output files are identical"
else
    echo "Test 5: Output files are different"
fi