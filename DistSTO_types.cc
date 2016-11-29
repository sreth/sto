/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "DistSTO_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>




DoRpcResponse::~DoRpcResponse() throw() {
}


void DoRpcResponse::__set_success(const bool val) {
  this->success = val;
}

void DoRpcResponse::__set_version(const int64_t val) {
  this->version = val;
}

void DoRpcResponse::__set_value(const std::string& val) {
  this->value = val;
}

uint32_t DoRpcResponse::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_success = false;
  bool isset_version = false;
  bool isset_value = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool(this->success);
          isset_success = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->version);
          isset_version = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readBinary(this->value);
          isset_value = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_success)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_version)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_value)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t DoRpcResponse::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("DoRpcResponse");

  xfer += oprot->writeFieldBegin("success", ::apache::thrift::protocol::T_BOOL, 1);
  xfer += oprot->writeBool(this->success);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("version", ::apache::thrift::protocol::T_I64, 2);
  xfer += oprot->writeI64(this->version);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("value", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeBinary(this->value);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(DoRpcResponse &a, DoRpcResponse &b) {
  using ::std::swap;
  swap(a.success, b.success);
  swap(a.version, b.version);
  swap(a.value, b.value);
}

DoRpcResponse::DoRpcResponse(const DoRpcResponse& other0) {
  success = other0.success;
  version = other0.version;
  value = other0.value;
}
DoRpcResponse& DoRpcResponse::operator=(const DoRpcResponse& other1) {
  success = other1.success;
  version = other1.version;
  value = other1.value;
  return *this;
}
void DoRpcResponse::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "DoRpcResponse(";
  out << "success=" << to_string(success);
  out << ", " << "version=" << to_string(version);
  out << ", " << "value=" << to_string(value);
  out << ")";
}


