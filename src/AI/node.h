#ifndef NODE_H
#define NODE_H

#include <vector>

enum Status {
  INVALID,
  SUCCESS,
  FAILURE,
  RUNNING,
  ERROR,
};

class Node {
public:

  Status status = Status::INVALID;

  virtual ~Node() {};

  virtual Status update() =0;
  virtual void initialize() {}
  virtual void terminate(Status s) {};

  Status tick() {
    if (status != Status::RUNNING) initialize();
    status = update();
    if (status != Status::RUNNING) terminate(status);

    return status;
  }

  bool isSuccess() { return status == Status::SUCCESS; }
  bool isFailure() { return status == Status::FAILURE; }
  bool isRunning() { return status == Status::RUNNING; }
  bool isTerminated() { return status == Status::SUCCESS || status == Status::FAILURE; }

  void reset() { status = Status::INVALID; }
};

typedef vector<Node*> Nodes;

#endif
