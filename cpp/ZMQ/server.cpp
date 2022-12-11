#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <zmq.hpp>
#include <mutex>
#include "msgs.pb.h"

#include "mex.h"

const int INIT = 0;
const int CLOSE = 1;
const int RECV = 2;
const int SEND = 3;

void recv() {
  MatlabPython::Numpy msg;
  msg.set_num_dims(4);
  msg.set_dims(0, 3);
  msg.set_type(MatlabPython::Type::FLOAT64);
  auto data = "efrgererg";
  msg.set_data(data);

}

//void send(const mxArray *pMxArray) {
//  MatlabPython::Numpy msg;
//  auto num_dims = mxGetNumberOfDimensions(pMxArray);
//  msg.set_num_dims(num_dims);
//  auto dims = mxGetDimensions(pMxArray);
//  for (auto i = 0; i < num_dims; i++) {
//    msg.set_dims(i, dims[i]);
//  }
//  auto id = mxGetClassID(pMxArray);
//  switch (id) {
//    case mxClassID::mxDOUBLE_CLASS: {
//      msg.set_type(MatlabPython::Type::FLOAT64);
//      double *data_ptr = (double *) mxGetData(pMxArray);
//      msg.set_data(reinterpret_cast<const char *>(data_ptr));
//      break;
//    }
//    case mxClassID::mxINT32_CLASS: {
//      msg.set_type(MatlabPython::Type::INT32);
//      int *data_ptr = (int *) mxGetData(pMxArray);
//      msg.set_data(reinterpret_cast<const char *>(data_ptr));
//      break;
//    }
//    default:
//      std::stringstream ss;
//      ss << "unsupported type: " << mxGetClassName(pMxArray);
//      mexErrMsgTxt(ss.str().c_str());
//      break;
//  }
//
//}


class Server {
private:
  zmq::context_t send_context_;
  zmq::socket_t send_socket_;
  zmq::context_t recv_context_;
  zmq::socket_t recv_socket_;

  std::thread t1_;
  std::mutex mutable mutex_;
  bool running_;
  std::chrono::milliseconds rate_;
  std::string topic_;

//  void run() {
//    bool cond = true;
//    while (cond) {
//      std::cout << std::endl << "running server";
//      std::this_thread::sleep_for(rate_);
//      std::lock_guard<std::mutex> lock(mutex_);
//      cond = running_;
//    }
//  }

public:
  MatlabPython::Numpy latest_msg_;

  void start_server(int port, int period, const std::string &topic) {
    // period (ms)

    std::cout << "start server: " << port << " " << period << std::endl;
    // initialize the zmq context with a single IO thread
    send_context_ = zmq::context_t(1);
    send_socket_ = {send_context_, ZMQ_PUB};
    send_socket_.bind("tcp://*:" + std::to_string(port));

    recv_context_ = zmq::context_t(1);
    recv_socket_ = {recv_context_, ZMQ_SUB};
    recv_socket_.bind("tcp://*:" + std::to_string(port + 10));
    recv_socket_.set(zmq::sockopt::subscribe, topic_);

    running_ = true;
    rate_ = std::chrono::milliseconds(period);
    topic_ = topic;
    t1_ = std::thread(&Server::recv, this);

  }

  mxArray *getLatestArr() {
    mxArray *latest_arr;
    mwSize dims[100]; // larger than 100 dims, then it will fail
    mwSize ndim = latest_msg_.num_dims();
    int total_elements = 1 * (ndim > 0);
    for (auto i = 0; i < ndim; i++) {
      dims[i] = latest_msg_.dims(i);
      total_elements *= dims[i];
    }

    switch (latest_msg_.type()) {
      case (MatlabPython::Type::FLOAT64): {
        latest_arr = mxCreateNumericArray(ndim, dims, mxClassID::mxDOUBLE_CLASS, mxREAL);
        double *ptr = mxGetPr(latest_arr);
        memcpy(ptr, latest_msg_.data().data(), total_elements * sizeof(double));
        break;
      }
      case (MatlabPython::Type::INT32): {
        latest_arr = mxCreateNumericArray(ndim, dims, mxClassID::mxINT32_CLASS, mxREAL);
        double *ptr = mxGetPr(latest_arr);
        memcpy(ptr, latest_msg_.data().data(), total_elements * sizeof(int));
        break;
      }
      default: {
        std::cout << "MatlabPython::Type" << std::endl;
        break;
      }
    }

    return latest_arr;
  }

  std::string recv() {
    bool cond = true;
    while (cond) {
//      std::cout << std::endl << "waiting for message";

      zmq::message_t request_topic;
      auto nbytes = recv_socket_.recv(request_topic, zmq::recv_flags::dontwait);
      if (nbytes > 0) {
        auto topic = request_topic.str();
//        std::cout << std::endl << "message received";
        // block until data arrives
        zmq::message_t request;
        nbytes = recv_socket_.recv(request, zmq::recv_flags::none);
        auto tmp = request.to_string();
        latest_msg_.ParseFromString(tmp);
      }

      std::this_thread::sleep_for(rate_);
      std::lock_guard<std::mutex> lock(mutex_);
      cond = running_;
    }

  }

  void send(const mxArray *pMxArray) {
    MatlabPython::Numpy msg;
    auto num_dims = mxGetNumberOfDimensions(pMxArray);
    msg.set_num_dims(num_dims);
    auto dims = mxGetDimensions(pMxArray);

    int number_elements = 1;
    for (auto i = 0; i < num_dims; i++) {
      msg.add_dims(dims[i]);
      number_elements *= dims[i];
    }
    auto id = mxGetClassID(pMxArray);
    switch (id) {
      case mxClassID::mxDOUBLE_CLASS: {
        msg.set_type(MatlabPython::Type::FLOAT64);
        double *data_ptr = (double *) mxGetData(pMxArray);
        msg.set_data(reinterpret_cast<void *>(data_ptr), number_elements * sizeof(double));
        break;
      }
      case mxClassID::mxINT32_CLASS: {
        msg.set_type(MatlabPython::Type::INT32);
        int *data_ptr = (int *) mxGetData(pMxArray);
        msg.set_data(reinterpret_cast<void *>(data_ptr), number_elements * sizeof(int));
        break;
      }
      default:
        std::stringstream ss;
        ss << "unsupported type: " << mxGetClassName(pMxArray);
        mexErrMsgTxt(ss.str().c_str());
        return;
    }

    std::string msg_str;
    msg.SerializeToString(&msg_str);
    zmq::message_t topic(topic_.size());
    memcpy(topic.data(), topic_.data(), topic_.size()); // <= Change your topic message here
    // send topic for filtering
    send_socket_.send(topic, zmq::send_flags::sndmore);
    // send message
    send_socket_.send(zmq::buffer(msg_str), zmq::send_flags::none);
  }

  void stop_server() {
    {
      std::lock_guard<std::mutex> lock(mutex_);
      running_ = false;
    }
    t1_.join();
    std::cout << "stop server" << std::endl;
  }
};


Server *getServer(const mxArray *prhs[]) {
  double *pointer0 = mxGetPr(prhs[1]);
  mxAssert(pointer0 != NULL, "input must be a valid robot pointer\n");
  intptr_t pointer1 = (intptr_t) pointer0[0];
  auto server = (Server *) pointer1;
  return server;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
  int command = mxGetScalar(prhs[0]);

  switch (command) {
    case INIT: {
      if (nrhs != 4)
        mexErrMsgTxt("function takes four inputs: [command, port, period, topic]");
      if (nlhs != 1)
        mexErrMsgTxt("init function has one output");
      int port = mxGetScalar(prhs[1]);
      int period = mxGetScalar(prhs[2]);
      auto topic = std::string(mxArrayToString(prhs[3]));
      std::cout << topic << std::endl;
      Server *server = new Server();
      server->start_server(port, period, topic);
      plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
      double *ptr = mxGetPr(plhs[0]);
      ptr[0] = (intptr_t) server;
      break;
    }
    case CLOSE: {
      if (nrhs != 2)
        mexErrMsgTxt("function takes two inputs: [command, ptr]");
      if (nlhs != 0)
        mexErrMsgTxt("close function has no outputs");

      Server *server = getServer(prhs);
      server->stop_server();
      delete server;
      break;
    }
    case RECV: {
      if (nrhs != 2)
        mexErrMsgTxt("function takes two inputs: [command, ptr]");
      if (nlhs != 1)
        mexErrMsgTxt("close function has one output: latest_data");

      Server *server = getServer(prhs);
      plhs[0] = server->getLatestArr();

      break;
    }

    case SEND: {
      if (nrhs != 3)
        mexErrMsgTxt("function takes three inputs: [command, ptr, arr]");
      if (nlhs != 0)
        mexErrMsgTxt("close function has no outputs");

      Server *server = getServer(prhs);
      auto arr = prhs[2];
      server->send(arr);
      break;
    }

    default:
      mexErrMsgTxt("Input command is invalid");
      break;
  }

}


int test() {

}