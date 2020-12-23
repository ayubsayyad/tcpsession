#include "Buffer.h"
#include "SampleMessages.h"

class Decoder{
    public:
        uint32_t decodeNProcess(){
            int currIdx = 0;
            while ((buffer_.bufferSize_ - currIdx) >= sizeof(MessageHeader)){
                MessageHeader* msg = (MessageHeader*)&buffer_.buffer_[currIdx];
                if(msg->message_type_ == TEST_MESSAGE){
                    if((buffer_.bufferSize_ - currIdx) >= sizeof(TestMessage)){
                        TestMessage* testMessage = (TestMessage*)&buffer_.buffer_[currIdx];
                        currIdx += sizeof(TestMessage);
                        std::cout << "Raise Callback:" << std::endl;
                    }
                    else{
                        break;
                    }
                }
                else if(msg->message_type_ == TEST_MESSAGE2){
                    if((buffer_.bufferSize_ - currIdx) >= sizeof(TestMessage2)){
                        TestMessage2* testMessage = (TestMessage2*)&buffer_.buffer_[currIdx];
                        currIdx += sizeof(TestMessage2);
                        std::cout << "Raise Callback 2:" << std::endl;
                    }
                    else{
                        break;
                    }
                } 
            }
            if((buffer_.bufferSize_ - currIdx)){
                buffer_.moveToStart(currIdx);
            }
            else{
                buffer_.bufferSize_ = 0;
            }
        }

        Buffer& buffer(){
            return buffer_;
        }
    private:
        Buffer buffer_;
};
