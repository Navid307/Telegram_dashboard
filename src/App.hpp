#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "Commands.hpp"
#include "IMessageService.hpp"
#include "Telegram.hpp"
#include "yolo5.hpp"

extern "C"
{
  void hello_from_rust();
}

extern "C"
{
  std::string execute_command(std::string cmd);
}

class App
{
public:
  App() {

  };

  ~App() {

  };

  void startApp()
  {
    this->createLogFile();
    boost::property_tree::read_json("../../config.json", this->config);
    boost::property_tree::read_json("../../last_message.json",
                                    this->lastMessagePt);
    messageService = std::make_shared<Telegram>();

    std::string lastMessage = this->lastMessagePt.get<std::string>("message");
    std::string c;

    this->sendSSHInfo();

#ifndef TESTING
    while (true)
    {
#else
    {
#endif
      c = messageService->getUpdate();
      if (c != lastMessage)
      {
        lastMessage = c;
        // TODO: this->lastMessageTimestamp
        this->lastMessagePt.put("message", c);
        boost::property_tree::write_json("../../last_message.json",
                                         this->lastMessagePt);
        commandHandler(c);
      }

#ifndef TESTING
      std::this_thread::sleep_for(std::chrono::seconds(5));
#else
#endif
    }
  }

private:
  std::shared_ptr<IMessageService> messageService;

  std::string logFile;
  boost::property_tree::ptree config;
  boost::property_tree::ptree lastMessagePt;

  void createLogFile(void)
  {
    time_t now = time(0);
    tm *localTime = localtime(&now);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d_%H-%M-%S_%Z", localTime);
    std::string logFileName = "../../logs/log_" + std::string(buffer) + ".log";
    std::cout << "Log file name: " << logFileName << std::endl;
  }

public:
  void asyncCommandCallback(const std::string &result)
  {
    std::cout << result << std::endl;
    messageService->sendMessage(result);
  }

  void executeAsyncCommand(const std::function<std::string()> &command)
  {
    std::thread async_thread([this, command]()
                             {
      std::string result = command();
      this->asyncCommandCallback(result); });
    async_thread.detach();
  }

  std::string invokeSystemCommand(const std::string &command)
  {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe)
      throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
      result += buffer.data();
    }
    return result;
  }

  void commandHandler(std::string comm)
  {
    if (comm == "Take photo")
    {
      // std::async(take_photo);  // TODO: Async or a different thread?
    }
    else if (comm == "Rust")
    {
      hello_from_rust();
    }
    else if (comm.find("Async ") == 0)
    {
      std::string async_command = comm.substr(6); // Strip "async " prefix
      executeAsyncCommand([async_command, this]()
                          { return this->invokeSystemCommand(async_command); });
    }
    else if (comm == "ComplexCommand")
    {
    }
    else
    {
      messageService->sendMessage("Unknow command");
    }
  }

  void sendSSHInfo()
  {
    std::string wifi_ssid = this->invokeSystemCommand("iwgetid -r");
    wifi_ssid.erase(0, wifi_ssid.find_first_not_of(" \n\r\t"));
    wifi_ssid.erase(wifi_ssid.find_last_not_of(" \n\r\t") + 1);
    messageService->sendMessage("Wi-Fi SID: " + wifi_ssid);

    std::string ip_address = this->invokeSystemCommand("hostname -I");
    ip_address.erase(0, ip_address.find_first_not_of(" \n\r\t"));
    ip_address.erase(ip_address.find_last_not_of(" \n\r\t") + 1);
    messageService->sendMessage("IP ddress: " + ip_address);
  }
};