/**
 * Autogenerated by Thrift Compiler (0.9.3)
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

void DoRpcResponse::__set_found(const bool val) {
  this->found = val;
}

void DoRpcResponse::__set_intResp1(const int64_t val) {
  this->intResp1 = val;
}

void DoRpcResponse::__set_version(const int64_t val) {
  this->version = val;
}

void DoRpcResponse::__set_key(const int64_t val) {
  this->key = val;
}

void DoRpcResponse::__set_intResp4(const int64_t val) {
  this->intResp4 = val;
}

void DoRpcResponse::__set_value(const std::string& val) {
  this->value = val;
}

void DoRpcResponse::__set_intListResp1(const std::vector<int64_t> & val) {
  this->intListResp1 = val;
}

void DoRpcResponse::__set_intListResp2(const std::vector<int64_t> & val) {
  this->intListResp2 = val;
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
  bool isset_found = false;
  bool isset_intResp1 = false;
  bool isset_version = false;
  bool isset_key = false;
  bool isset_intResp4 = false;
  bool isset_value = false;
  bool isset_intListResp1 = false;
  bool isset_intListResp2 = false;

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
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool(this->found);
          isset_found = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->intResp1);
          isset_intResp1 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->version);
          isset_version = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->key);
          isset_key = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->intResp4);
          isset_intResp4 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readBinary(this->value);
          isset_value = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 8:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->intListResp1.clear();
            uint32_t _size0;
            ::apache::thrift::protocol::TType _etype3;
            xfer += iprot->readListBegin(_etype3, _size0);
            this->intListResp1.resize(_size0);
            uint32_t _i4;
            for (_i4 = 0; _i4 < _size0; ++_i4)
            {
              xfer += iprot->readI64(this->intListResp1[_i4]);
            }
            xfer += iprot->readListEnd();
          }
          isset_intListResp1 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 9:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->intListResp2.clear();
            uint32_t _size5;
            ::apache::thrift::protocol::TType _etype8;
            xfer += iprot->readListBegin(_etype8, _size5);
            this->intListResp2.resize(_size5);
            uint32_t _i9;
            for (_i9 = 0; _i9 < _size5; ++_i9)
            {
              xfer += iprot->readI64(this->intListResp2[_i9]);
            }
            xfer += iprot->readListEnd();
          }
          isset_intListResp2 = true;
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
  if (!isset_found)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_intResp1)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_version)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_key)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_intResp4)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_value)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_intListResp1)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_intListResp2)
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

  xfer += oprot->writeFieldBegin("found", ::apache::thrift::protocol::T_BOOL, 2);
  xfer += oprot->writeBool(this->found);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("intResp1", ::apache::thrift::protocol::T_I64, 3);
  xfer += oprot->writeI64(this->intResp1);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("version", ::apache::thrift::protocol::T_I64, 4);
  xfer += oprot->writeI64(this->version);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("key", ::apache::thrift::protocol::T_I64, 5);
  xfer += oprot->writeI64(this->key);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("intResp4", ::apache::thrift::protocol::T_I64, 6);
  xfer += oprot->writeI64(this->intResp4);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("value", ::apache::thrift::protocol::T_STRING, 7);
  xfer += oprot->writeBinary(this->value);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("intListResp1", ::apache::thrift::protocol::T_LIST, 8);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_I64, static_cast<uint32_t>(this->intListResp1.size()));
    std::vector<int64_t> ::const_iterator _iter10;
    for (_iter10 = this->intListResp1.begin(); _iter10 != this->intListResp1.end(); ++_iter10)
    {
      xfer += oprot->writeI64((*_iter10));
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("intListResp2", ::apache::thrift::protocol::T_LIST, 9);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_I64, static_cast<uint32_t>(this->intListResp2.size()));
    std::vector<int64_t> ::const_iterator _iter11;
    for (_iter11 = this->intListResp2.begin(); _iter11 != this->intListResp2.end(); ++_iter11)
    {
      xfer += oprot->writeI64((*_iter11));
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(DoRpcResponse &a, DoRpcResponse &b) {
  using ::std::swap;
  swap(a.success, b.success);
  swap(a.found, b.found);
  swap(a.intResp1, b.intResp1);
  swap(a.version, b.version);
  swap(a.key, b.key);
  swap(a.intResp4, b.intResp4);
  swap(a.value, b.value);
  swap(a.intListResp1, b.intListResp1);
  swap(a.intListResp2, b.intListResp2);
}

DoRpcResponse::DoRpcResponse(const DoRpcResponse& other12) {
  success = other12.success;
  found = other12.found;
  intResp1 = other12.intResp1;
  version = other12.version;
  key = other12.key;
  intResp4 = other12.intResp4;
  value = other12.value;
  intListResp1 = other12.intListResp1;
  intListResp2 = other12.intListResp2;
}
DoRpcResponse& DoRpcResponse::operator=(const DoRpcResponse& other13) {
  success = other13.success;
  found = other13.found;
  intResp1 = other13.intResp1;
  version = other13.version;
  key = other13.key;
  intResp4 = other13.intResp4;
  value = other13.value;
  intListResp1 = other13.intListResp1;
  intListResp2 = other13.intListResp2;
  return *this;
}
void DoRpcResponse::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "DoRpcResponse(";
  out << "success=" << to_string(success);
  out << ", " << "found=" << to_string(found);
  out << ", " << "intResp1=" << to_string(intResp1);
  out << ", " << "version=" << to_string(version);
  out << ", " << "key=" << to_string(key);
  out << ", " << "intResp4=" << to_string(intResp4);
  out << ", " << "value=" << to_string(value);
  out << ", " << "intListResp1=" << to_string(intListResp1);
  out << ", " << "intListResp2=" << to_string(intListResp2);
  out << ")";
}


DoRpcArgs::~DoRpcArgs() throw() {
}


void DoRpcArgs::__set_objid(const int64_t val) {
  this->objid = val;
}

void DoRpcArgs::__set_op(const int64_t val) {
  this->op = val;
}

void DoRpcArgs::__set_strArg1(const std::string& val) {
  this->strArg1 = val;
}

void DoRpcArgs::__set_strArg2(const std::string& val) {
  this->strArg2 = val;
}

void DoRpcArgs::__set_strArg3(const std::string& val) {
  this->strArg3 = val;
}

void DoRpcArgs::__set_boolArg1(const bool val) {
  this->boolArg1 = val;
}

void DoRpcArgs::__set_boolArg2(const bool val) {
  this->boolArg2 = val;
}

void DoRpcArgs::__set_intArg1(const int64_t val) {
  this->intArg1 = val;
}

uint32_t DoRpcArgs::read(::apache::thrift::protocol::TProtocol* iprot) {

  apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_objid = false;
  bool isset_op = false;
  bool isset_strArg1 = false;
  bool isset_strArg2 = false;
  bool isset_strArg3 = false;
  bool isset_boolArg1 = false;
  bool isset_boolArg2 = false;
  bool isset_intArg1 = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->objid);
          isset_objid = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->op);
          isset_op = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readBinary(this->strArg1);
          isset_strArg1 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readBinary(this->strArg2);
          isset_strArg2 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readBinary(this->strArg3);
          isset_strArg3 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool(this->boolArg1);
          isset_boolArg1 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool(this->boolArg2);
          isset_boolArg2 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 8:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->intArg1);
          isset_intArg1 = true;
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

  if (!isset_objid)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_op)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_strArg1)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_strArg2)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_strArg3)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_boolArg1)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_boolArg2)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_intArg1)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t DoRpcArgs::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("DoRpcArgs");

  xfer += oprot->writeFieldBegin("objid", ::apache::thrift::protocol::T_I64, 1);
  xfer += oprot->writeI64(this->objid);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("op", ::apache::thrift::protocol::T_I64, 2);
  xfer += oprot->writeI64(this->op);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("strArg1", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeBinary(this->strArg1);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("strArg2", ::apache::thrift::protocol::T_STRING, 4);
  xfer += oprot->writeBinary(this->strArg2);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("strArg3", ::apache::thrift::protocol::T_STRING, 5);
  xfer += oprot->writeBinary(this->strArg3);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("boolArg1", ::apache::thrift::protocol::T_BOOL, 6);
  xfer += oprot->writeBool(this->boolArg1);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("boolArg2", ::apache::thrift::protocol::T_BOOL, 7);
  xfer += oprot->writeBool(this->boolArg2);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("intArg1", ::apache::thrift::protocol::T_I64, 8);
  xfer += oprot->writeI64(this->intArg1);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(DoRpcArgs &a, DoRpcArgs &b) {
  using ::std::swap;
  swap(a.objid, b.objid);
  swap(a.op, b.op);
  swap(a.strArg1, b.strArg1);
  swap(a.strArg2, b.strArg2);
  swap(a.strArg3, b.strArg3);
  swap(a.boolArg1, b.boolArg1);
  swap(a.boolArg2, b.boolArg2);
  swap(a.intArg1, b.intArg1);
}

DoRpcArgs::DoRpcArgs(const DoRpcArgs& other14) {
  objid = other14.objid;
  op = other14.op;
  strArg1 = other14.strArg1;
  strArg2 = other14.strArg2;
  strArg3 = other14.strArg3;
  boolArg1 = other14.boolArg1;
  boolArg2 = other14.boolArg2;
  intArg1 = other14.intArg1;
}
DoRpcArgs& DoRpcArgs::operator=(const DoRpcArgs& other15) {
  objid = other15.objid;
  op = other15.op;
  strArg1 = other15.strArg1;
  strArg2 = other15.strArg2;
  strArg3 = other15.strArg3;
  boolArg1 = other15.boolArg1;
  boolArg2 = other15.boolArg2;
  intArg1 = other15.intArg1;
  return *this;
}
void DoRpcArgs::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "DoRpcArgs(";
  out << "objid=" << to_string(objid);
  out << ", " << "op=" << to_string(op);
  out << ", " << "strArg1=" << to_string(strArg1);
  out << ", " << "strArg2=" << to_string(strArg2);
  out << ", " << "strArg3=" << to_string(strArg3);
  out << ", " << "boolArg1=" << to_string(boolArg1);
  out << ", " << "boolArg2=" << to_string(boolArg2);
  out << ", " << "intArg1=" << to_string(intArg1);
  out << ")";
}


