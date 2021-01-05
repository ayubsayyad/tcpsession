#pragma once

enum MessageType{
    TEST_MESSAGE = 0,
    TEST_MESSAGE2 = 2,
    DATA_MESSAGE = 3,
};

struct MessageHeader{
    uint32_t message_type_;
    uint32_t message_length_;
};


struct TestMessage{
    MessageHeader header_;
    char RawData[1024];
};


struct TestMessage2{
    MessageHeader header_;
    char RawData[2048];
};

struct DataMessage{
    MessageHeader header_;
    uint32_t BufferLength_;
    char RawData_[2048];
};


