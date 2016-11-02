/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef DistSTO_H
#define DistSTO_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "DistSTO_types.h"



#ifdef _WIN32
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class DistSTOIf {
 public:
  virtual ~DistSTOIf() {}
  virtual void read(std::string& _return, const int64_t objid) = 0;
  virtual int64_t lock(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<bool> & has_read) = 0;
  virtual bool check(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<int64_t> & old_versions, const bool may_duplicate_items_, const std::vector<bool> & preceding_duplicate_read_) = 0;
  virtual void install(const int32_t tuid, const int64_t tid, const std::vector<int64_t> & objids, const std::vector<int64_t> & version_ptrs, const std::vector<std::string> & written_values) = 0;
  virtual void abort(const int32_t tuid, const std::vector<int64_t> & unlock_objids) = 0;
};

class DistSTOIfFactory {
 public:
  typedef DistSTOIf Handler;

  virtual ~DistSTOIfFactory() {}

  virtual DistSTOIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(DistSTOIf* /* handler */) = 0;
};

class DistSTOIfSingletonFactory : virtual public DistSTOIfFactory {
 public:
  DistSTOIfSingletonFactory(const boost::shared_ptr<DistSTOIf>& iface) : iface_(iface) {}
  virtual ~DistSTOIfSingletonFactory() {}

  virtual DistSTOIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(DistSTOIf* /* handler */) {}

 protected:
  boost::shared_ptr<DistSTOIf> iface_;
};

class DistSTONull : virtual public DistSTOIf {
 public:
  virtual ~DistSTONull() {}
  void read(std::string& /* _return */, const int64_t /* objid */) {
    return;
  }
  int64_t lock(const int32_t /* tuid */, const std::vector<int64_t> & /* version_ptrs */, const std::vector<bool> & /* has_read */) {
    int64_t _return = 0;
    return _return;
  }
  bool check(const int32_t /* tuid */, const std::vector<int64_t> & /* version_ptrs */, const std::vector<int64_t> & /* old_versions */, const bool /* may_duplicate_items_ */, const std::vector<bool> & /* preceding_duplicate_read_ */) {
    bool _return = false;
    return _return;
  }
  void install(const int32_t /* tuid */, const int64_t /* tid */, const std::vector<int64_t> & /* objids */, const std::vector<int64_t> & /* version_ptrs */, const std::vector<std::string> & /* written_values */) {
    return;
  }
  void abort(const int32_t /* tuid */, const std::vector<int64_t> & /* unlock_objids */) {
    return;
  }
};

typedef struct _DistSTO_read_args__isset {
  _DistSTO_read_args__isset() : objid(false) {}
  bool objid :1;
} _DistSTO_read_args__isset;

class DistSTO_read_args {
 public:

  DistSTO_read_args(const DistSTO_read_args&);
  DistSTO_read_args& operator=(const DistSTO_read_args&);
  DistSTO_read_args() : objid(0) {
  }

  virtual ~DistSTO_read_args() throw();
  int64_t objid;

  _DistSTO_read_args__isset __isset;

  void __set_objid(const int64_t val);

  bool operator == (const DistSTO_read_args & rhs) const
  {
    if (!(objid == rhs.objid))
      return false;
    return true;
  }
  bool operator != (const DistSTO_read_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DistSTO_read_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DistSTO_read_pargs {
 public:


  virtual ~DistSTO_read_pargs() throw();
  const int64_t* objid;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DistSTO_read_result__isset {
  _DistSTO_read_result__isset() : success(false) {}
  bool success :1;
} _DistSTO_read_result__isset;

class DistSTO_read_result {
 public:

  DistSTO_read_result(const DistSTO_read_result&);
  DistSTO_read_result& operator=(const DistSTO_read_result&);
  DistSTO_read_result() : success() {
  }

  virtual ~DistSTO_read_result() throw();
  std::string success;

  _DistSTO_read_result__isset __isset;

  void __set_success(const std::string& val);

  bool operator == (const DistSTO_read_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const DistSTO_read_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DistSTO_read_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DistSTO_read_presult__isset {
  _DistSTO_read_presult__isset() : success(false) {}
  bool success :1;
} _DistSTO_read_presult__isset;

class DistSTO_read_presult {
 public:


  virtual ~DistSTO_read_presult() throw();
  std::string* success;

  _DistSTO_read_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _DistSTO_lock_args__isset {
  _DistSTO_lock_args__isset() : tuid(false), version_ptrs(false), has_read(false) {}
  bool tuid :1;
  bool version_ptrs :1;
  bool has_read :1;
} _DistSTO_lock_args__isset;

class DistSTO_lock_args {
 public:

  DistSTO_lock_args(const DistSTO_lock_args&);
  DistSTO_lock_args& operator=(const DistSTO_lock_args&);
  DistSTO_lock_args() : tuid(0) {
  }

  virtual ~DistSTO_lock_args() throw();
  int32_t tuid;
  std::vector<int64_t>  version_ptrs;
  std::vector<bool>  has_read;

  _DistSTO_lock_args__isset __isset;

  void __set_tuid(const int32_t val);

  void __set_version_ptrs(const std::vector<int64_t> & val);

  void __set_has_read(const std::vector<bool> & val);

  bool operator == (const DistSTO_lock_args & rhs) const
  {
    if (!(tuid == rhs.tuid))
      return false;
    if (!(version_ptrs == rhs.version_ptrs))
      return false;
    if (!(has_read == rhs.has_read))
      return false;
    return true;
  }
  bool operator != (const DistSTO_lock_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DistSTO_lock_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DistSTO_lock_pargs {
 public:


  virtual ~DistSTO_lock_pargs() throw();
  const int32_t* tuid;
  const std::vector<int64_t> * version_ptrs;
  const std::vector<bool> * has_read;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DistSTO_lock_result__isset {
  _DistSTO_lock_result__isset() : success(false) {}
  bool success :1;
} _DistSTO_lock_result__isset;

class DistSTO_lock_result {
 public:

  DistSTO_lock_result(const DistSTO_lock_result&);
  DistSTO_lock_result& operator=(const DistSTO_lock_result&);
  DistSTO_lock_result() : success(0) {
  }

  virtual ~DistSTO_lock_result() throw();
  int64_t success;

  _DistSTO_lock_result__isset __isset;

  void __set_success(const int64_t val);

  bool operator == (const DistSTO_lock_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const DistSTO_lock_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DistSTO_lock_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DistSTO_lock_presult__isset {
  _DistSTO_lock_presult__isset() : success(false) {}
  bool success :1;
} _DistSTO_lock_presult__isset;

class DistSTO_lock_presult {
 public:


  virtual ~DistSTO_lock_presult() throw();
  int64_t* success;

  _DistSTO_lock_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _DistSTO_check_args__isset {
  _DistSTO_check_args__isset() : tuid(false), version_ptrs(false), old_versions(false), may_duplicate_items_(false), preceding_duplicate_read_(false) {}
  bool tuid :1;
  bool version_ptrs :1;
  bool old_versions :1;
  bool may_duplicate_items_ :1;
  bool preceding_duplicate_read_ :1;
} _DistSTO_check_args__isset;

class DistSTO_check_args {
 public:

  DistSTO_check_args(const DistSTO_check_args&);
  DistSTO_check_args& operator=(const DistSTO_check_args&);
  DistSTO_check_args() : tuid(0), may_duplicate_items_(0) {
  }

  virtual ~DistSTO_check_args() throw();
  int32_t tuid;
  std::vector<int64_t>  version_ptrs;
  std::vector<int64_t>  old_versions;
  bool may_duplicate_items_;
  std::vector<bool>  preceding_duplicate_read_;

  _DistSTO_check_args__isset __isset;

  void __set_tuid(const int32_t val);

  void __set_version_ptrs(const std::vector<int64_t> & val);

  void __set_old_versions(const std::vector<int64_t> & val);

  void __set_may_duplicate_items_(const bool val);

  void __set_preceding_duplicate_read_(const std::vector<bool> & val);

  bool operator == (const DistSTO_check_args & rhs) const
  {
    if (!(tuid == rhs.tuid))
      return false;
    if (!(version_ptrs == rhs.version_ptrs))
      return false;
    if (!(old_versions == rhs.old_versions))
      return false;
    if (!(may_duplicate_items_ == rhs.may_duplicate_items_))
      return false;
    if (!(preceding_duplicate_read_ == rhs.preceding_duplicate_read_))
      return false;
    return true;
  }
  bool operator != (const DistSTO_check_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DistSTO_check_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DistSTO_check_pargs {
 public:


  virtual ~DistSTO_check_pargs() throw();
  const int32_t* tuid;
  const std::vector<int64_t> * version_ptrs;
  const std::vector<int64_t> * old_versions;
  const bool* may_duplicate_items_;
  const std::vector<bool> * preceding_duplicate_read_;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DistSTO_check_result__isset {
  _DistSTO_check_result__isset() : success(false) {}
  bool success :1;
} _DistSTO_check_result__isset;

class DistSTO_check_result {
 public:

  DistSTO_check_result(const DistSTO_check_result&);
  DistSTO_check_result& operator=(const DistSTO_check_result&);
  DistSTO_check_result() : success(0) {
  }

  virtual ~DistSTO_check_result() throw();
  bool success;

  _DistSTO_check_result__isset __isset;

  void __set_success(const bool val);

  bool operator == (const DistSTO_check_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const DistSTO_check_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DistSTO_check_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DistSTO_check_presult__isset {
  _DistSTO_check_presult__isset() : success(false) {}
  bool success :1;
} _DistSTO_check_presult__isset;

class DistSTO_check_presult {
 public:


  virtual ~DistSTO_check_presult() throw();
  bool* success;

  _DistSTO_check_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _DistSTO_install_args__isset {
  _DistSTO_install_args__isset() : tuid(false), tid(false), objids(false), version_ptrs(false), written_values(false) {}
  bool tuid :1;
  bool tid :1;
  bool objids :1;
  bool version_ptrs :1;
  bool written_values :1;
} _DistSTO_install_args__isset;

class DistSTO_install_args {
 public:

  DistSTO_install_args(const DistSTO_install_args&);
  DistSTO_install_args& operator=(const DistSTO_install_args&);
  DistSTO_install_args() : tuid(0), tid(0) {
  }

  virtual ~DistSTO_install_args() throw();
  int32_t tuid;
  int64_t tid;
  std::vector<int64_t>  objids;
  std::vector<int64_t>  version_ptrs;
  std::vector<std::string>  written_values;

  _DistSTO_install_args__isset __isset;

  void __set_tuid(const int32_t val);

  void __set_tid(const int64_t val);

  void __set_objids(const std::vector<int64_t> & val);

  void __set_version_ptrs(const std::vector<int64_t> & val);

  void __set_written_values(const std::vector<std::string> & val);

  bool operator == (const DistSTO_install_args & rhs) const
  {
    if (!(tuid == rhs.tuid))
      return false;
    if (!(tid == rhs.tid))
      return false;
    if (!(objids == rhs.objids))
      return false;
    if (!(version_ptrs == rhs.version_ptrs))
      return false;
    if (!(written_values == rhs.written_values))
      return false;
    return true;
  }
  bool operator != (const DistSTO_install_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DistSTO_install_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DistSTO_install_pargs {
 public:


  virtual ~DistSTO_install_pargs() throw();
  const int32_t* tuid;
  const int64_t* tid;
  const std::vector<int64_t> * objids;
  const std::vector<int64_t> * version_ptrs;
  const std::vector<std::string> * written_values;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DistSTO_install_result {
 public:

  DistSTO_install_result(const DistSTO_install_result&);
  DistSTO_install_result& operator=(const DistSTO_install_result&);
  DistSTO_install_result() {
  }

  virtual ~DistSTO_install_result() throw();

  bool operator == (const DistSTO_install_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const DistSTO_install_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DistSTO_install_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DistSTO_install_presult {
 public:


  virtual ~DistSTO_install_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _DistSTO_abort_args__isset {
  _DistSTO_abort_args__isset() : tuid(false), unlock_objids(false) {}
  bool tuid :1;
  bool unlock_objids :1;
} _DistSTO_abort_args__isset;

class DistSTO_abort_args {
 public:

  DistSTO_abort_args(const DistSTO_abort_args&);
  DistSTO_abort_args& operator=(const DistSTO_abort_args&);
  DistSTO_abort_args() : tuid(0) {
  }

  virtual ~DistSTO_abort_args() throw();
  int32_t tuid;
  std::vector<int64_t>  unlock_objids;

  _DistSTO_abort_args__isset __isset;

  void __set_tuid(const int32_t val);

  void __set_unlock_objids(const std::vector<int64_t> & val);

  bool operator == (const DistSTO_abort_args & rhs) const
  {
    if (!(tuid == rhs.tuid))
      return false;
    if (!(unlock_objids == rhs.unlock_objids))
      return false;
    return true;
  }
  bool operator != (const DistSTO_abort_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DistSTO_abort_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DistSTO_abort_pargs {
 public:


  virtual ~DistSTO_abort_pargs() throw();
  const int32_t* tuid;
  const std::vector<int64_t> * unlock_objids;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DistSTO_abort_result {
 public:

  DistSTO_abort_result(const DistSTO_abort_result&);
  DistSTO_abort_result& operator=(const DistSTO_abort_result&);
  DistSTO_abort_result() {
  }

  virtual ~DistSTO_abort_result() throw();

  bool operator == (const DistSTO_abort_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const DistSTO_abort_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DistSTO_abort_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DistSTO_abort_presult {
 public:


  virtual ~DistSTO_abort_presult() throw();

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class DistSTOClient : virtual public DistSTOIf {
 public:
  DistSTOClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  DistSTOClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void read(std::string& _return, const int64_t objid);
  void send_read(const int64_t objid);
  void recv_read(std::string& _return);
  int64_t lock(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<bool> & has_read);
  void send_lock(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<bool> & has_read);
  int64_t recv_lock();
  bool check(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<int64_t> & old_versions, const bool may_duplicate_items_, const std::vector<bool> & preceding_duplicate_read_);
  void send_check(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<int64_t> & old_versions, const bool may_duplicate_items_, const std::vector<bool> & preceding_duplicate_read_);
  bool recv_check();
  void install(const int32_t tuid, const int64_t tid, const std::vector<int64_t> & objids, const std::vector<int64_t> & version_ptrs, const std::vector<std::string> & written_values);
  void send_install(const int32_t tuid, const int64_t tid, const std::vector<int64_t> & objids, const std::vector<int64_t> & version_ptrs, const std::vector<std::string> & written_values);
  void recv_install();
  void abort(const int32_t tuid, const std::vector<int64_t> & unlock_objids);
  void send_abort(const int32_t tuid, const std::vector<int64_t> & unlock_objids);
  void recv_abort();
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class DistSTOProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<DistSTOIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (DistSTOProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_read(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_lock(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_check(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_install(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_abort(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  DistSTOProcessor(boost::shared_ptr<DistSTOIf> iface) :
    iface_(iface) {
    processMap_["read"] = &DistSTOProcessor::process_read;
    processMap_["lock"] = &DistSTOProcessor::process_lock;
    processMap_["check"] = &DistSTOProcessor::process_check;
    processMap_["install"] = &DistSTOProcessor::process_install;
    processMap_["abort"] = &DistSTOProcessor::process_abort;
  }

  virtual ~DistSTOProcessor() {}
};

class DistSTOProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  DistSTOProcessorFactory(const ::boost::shared_ptr< DistSTOIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< DistSTOIfFactory > handlerFactory_;
};

class DistSTOMultiface : virtual public DistSTOIf {
 public:
  DistSTOMultiface(std::vector<boost::shared_ptr<DistSTOIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~DistSTOMultiface() {}
 protected:
  std::vector<boost::shared_ptr<DistSTOIf> > ifaces_;
  DistSTOMultiface() {}
  void add(boost::shared_ptr<DistSTOIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void read(std::string& _return, const int64_t objid) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->read(_return, objid);
    }
    ifaces_[i]->read(_return, objid);
    return;
  }

  int64_t lock(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<bool> & has_read) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->lock(tuid, version_ptrs, has_read);
    }
    return ifaces_[i]->lock(tuid, version_ptrs, has_read);
  }

  bool check(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<int64_t> & old_versions, const bool may_duplicate_items_, const std::vector<bool> & preceding_duplicate_read_) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->check(tuid, version_ptrs, old_versions, may_duplicate_items_, preceding_duplicate_read_);
    }
    return ifaces_[i]->check(tuid, version_ptrs, old_versions, may_duplicate_items_, preceding_duplicate_read_);
  }

  void install(const int32_t tuid, const int64_t tid, const std::vector<int64_t> & objids, const std::vector<int64_t> & version_ptrs, const std::vector<std::string> & written_values) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->install(tuid, tid, objids, version_ptrs, written_values);
    }
    ifaces_[i]->install(tuid, tid, objids, version_ptrs, written_values);
  }

  void abort(const int32_t tuid, const std::vector<int64_t> & unlock_objids) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->abort(tuid, unlock_objids);
    }
    ifaces_[i]->abort(tuid, unlock_objids);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class DistSTOConcurrentClient : virtual public DistSTOIf {
 public:
  DistSTOConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  DistSTOConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void read(std::string& _return, const int64_t objid);
  int32_t send_read(const int64_t objid);
  void recv_read(std::string& _return, const int32_t seqid);
  int64_t lock(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<bool> & has_read);
  int32_t send_lock(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<bool> & has_read);
  int64_t recv_lock(const int32_t seqid);
  bool check(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<int64_t> & old_versions, const bool may_duplicate_items_, const std::vector<bool> & preceding_duplicate_read_);
  int32_t send_check(const int32_t tuid, const std::vector<int64_t> & version_ptrs, const std::vector<int64_t> & old_versions, const bool may_duplicate_items_, const std::vector<bool> & preceding_duplicate_read_);
  bool recv_check(const int32_t seqid);
  void install(const int32_t tuid, const int64_t tid, const std::vector<int64_t> & objids, const std::vector<int64_t> & version_ptrs, const std::vector<std::string> & written_values);
  int32_t send_install(const int32_t tuid, const int64_t tid, const std::vector<int64_t> & objids, const std::vector<int64_t> & version_ptrs, const std::vector<std::string> & written_values);
  void recv_install(const int32_t seqid);
  void abort(const int32_t tuid, const std::vector<int64_t> & unlock_objids);
  int32_t send_abort(const int32_t tuid, const std::vector<int64_t> & unlock_objids);
  void recv_abort(const int32_t seqid);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _WIN32
  #pragma warning( pop )
#endif



#endif
