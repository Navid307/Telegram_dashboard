#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

#include "FileManagement.hpp"

/*
  void send_document(std::string caption) {
    auto vec = get_list_of_files("../../images");
    for (const std::string i : vec) {
      messageService->sendDocument("../../images/" + i, caption);
    }
    system("rm ../../images/*"); // TODO: Add a function to FileManagement to do this!
  }
*/
class Command
{
public:
    virtual ~Command() {}
    virtual void Execute() const = 0;
};

class SimpleCommand : public Command
{
private:
    std::string payload_;

public:
    explicit SimpleCommand(std::string payload) : payload_(payload) {}
    void Execute() const override
    {
        std::cout << "SimpleCommand: See, I can do simple things like printing (" << this->payload_ << ")\n";
    }
};

class HelpCommand : public Command {
    public:
    explicit HelpCommand(void) {
  }
  void Execute() const override {
    std::cout << "Yo! This is the help command" << std::endl;
  }
};

class ComplexCommand : public Command {
  /**
   * @var Receiver
   */
 private:
  //Receiver *receiver_; // Yolo5?
  std::string a_;
  std::string b_;
  public:
  //ComplexCommand(Receiver *receiver, std::string a, std::string b) : receiver_(receiver), a_(a), b_(b) {
  ComplexCommand(std::string a, std::string b) : a_(a), b_(b) {
  }
  
  void Execute() const override {
    std::cout << "ComplexCommand: Complex stuff should be done by a receiver object.\n";
    //this->receiver_->DoSomething(this->a_);
    //this->receiver_->DoSomethingElse(this->b_);
  }
};
