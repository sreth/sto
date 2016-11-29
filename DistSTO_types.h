/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef DistSTO_TYPES_H
#define DistSTO_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/cxxfunctional.h>




class DoRpcResponse;


class DoRpcResponse : public virtual ::apache::thrift::TBase {
 public:

  DoRpcResponse(const DoRpcResponse&);
  DoRpcResponse& operator=(const DoRpcResponse&);
  DoRpcResponse() : success(0), version(0), value() {
  }

  virtual ~DoRpcResponse() throw();
  bool success;
  int64_t version;
  std::string value;

  void __set_success(const bool val);

  void __set_version(const int64_t val);

  void __set_value(const std::string& val);

  bool operator == (const DoRpcResponse & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    if (!(version == rhs.version))
      return false;
    if (!(value == rhs.value))
      return false;
    return true;
  }
  bool operator != (const DoRpcResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DoRpcResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(DoRpcResponse &a, DoRpcResponse &b);

inline std::ostream& operator<<(std::ostream& out, const DoRpcResponse& obj)
{
  obj.printTo(out);
  return out;
}



#endif
