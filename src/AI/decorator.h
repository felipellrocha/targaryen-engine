#ifndef COMPOSITE_H
#define COMPOSITE_H

#include "node.h"

class Decorator : public Node {
public:
  Node* child = nullptr;

  void setChild(Node* _child) { child = _child; }
  bool hasNoChild() const { return child == nullptr; }

  ~Decorator() {
    delete child;
  }
};

class Failer : public Decorator {
public:
  Status update() override {
    child->tick();
    return Status::FAILURE;
  }
};

class Succeeder : public Decorator {
public:
  Status update() override {
    child->tick();
    return Status::SUCCESS;
  }
};

class Inverter : public Decorator {
public:
  Status update() override {
    auto status = child->tick();

    if (status == Status::SUCCESS) return Status::FAILURE;
    else if (status == Status::FAILURE) return Status::SUCCESS;

    return status;
  }
};

class Repeater : public Decorator {
public:
  int limit = 0;
  int counter = 0;

  Repeater(int _limit) : limit(_limit) { };

  void initialize() override { counter = 0; }

  Status update() override {
    while (true) {
      auto status = child->tick();

      if (status == Status::RUNNING) return Status::RUNNING;
      if (status == Status::FAILURE) return Status::FAILURE;
      if (limit > 0 && ++counter == limit) return Status::SUCCESS;

      child->reset();
    }
  }
};

class Limiter : public Decorator {
public:
  int limit = 0;
  int counter = 0;

  Limiter(int _limit) : limit(_limit) { };

  Status update() override {
    if (limit > 0 && counter >= limit) return Status::SUCCESS;

    auto status = child->tick();
    
    if (status == Status::RUNNING) return Status::RUNNING;
    if (status == Status::FAILURE) return Status::FAILURE;

    counter += 1;
    return Status::RUNNING;
  }
};

class untilFail : public Decorator {
public:
  Status update() override {
    while (true) {
      auto status = child->tick();

      if (status == Status::FAILURE) return Status::SUCCESS;
    }
  }
};

class untilSuccess : public Decorator {
public:
  Status update() override {
    while (true) {
      auto status = child->tick();

      if (status == Status::SUCCESS) return Status::SUCCESS;
    }
  }
};

#endif
