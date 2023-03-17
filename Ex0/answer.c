struct Answer {
    float answer;
};

struct Answer WhatIsTheAnswer() {
    struct Answer TheAnswer;
    TheAnswer.answer = 42.0;
    return TheAnswer;
}