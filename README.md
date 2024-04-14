# PA3

## csmaSimulator
In this project, we implemented a simulator that evaluates the performance of CSMA protocols in a wired network.

## Tests

In order to test the functionality of our CSMA simulator, we created five tests to confirm correctness of our program. We created a .sh (tests.sh) script that ran these tests, and compared the "output.txt" file to the expected output file for that test. If they were identical, it would print out a passed message and if they were different, it would print out a failed message. This helped us debug our program.

To run our tests, you can run ./tests.sh from the terminal.

### Test 1:
This was the default test given in the Notion file. We used this to confirm that our initial implementation was correct and also that our .sh script was working correctly.

### Test 2:
This was a test to ensure that consecutive packets could be sent on the same node. In the above test there was never a case where consecutive packets were sent, so we wanted to ensure that this functionality was working.

### Test 3:
This was a test to ensure that the collisions were working correctly by incrementing R on a collision.

### Test 4:
This was a test to test the maximum retransmission attempt, M, to ensure that the R value was reset and the packet was dropped if the maximum retransmission attempt M was reached.

### Test 5:
This was a test to make sure that packets could be sent on a singular node for the entire duration T.