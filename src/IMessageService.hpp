#include <string>

class IMessageService {
 public:
  virtual ~IMessageService() = default;
  virtual std::string getUpdate() = 0;
  virtual void sendMessage(std::string msg) = 0;
  virtual void sendDocument(const std::string &documentPath,
                            const std::string &caption = "caption") = 0;
};
