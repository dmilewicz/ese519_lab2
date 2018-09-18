//
// Created by David Milewicz on 9/18/18.
//

#ifndef LAB2_NOTES_H
#define LAB2_NOTES_H



#define C6_OCR   238
#define D6_OCR   212
#define E6_OCR   189
#define F6_OCR   178
#define G6_OCR   158
#define A6_OCR   141
#define B6_OCR   126
#define C7_OCR   118


#define OCA_LOW    C6_OCR
#define OCA_HIGH   C7_OCR

#define INPUT_HIGH       1023
#define INPUT_LOW        0

#define RATIO       (float)(OCA_HIGH - OCA_LOW) / (float)(INPUT_HIGH - INPUT_LOW)


uint8_t range_convert(unsigned int input) {
    return (RATIO * input) + OCA_LOW;
}




#endif //LAB2_NOTES_H
